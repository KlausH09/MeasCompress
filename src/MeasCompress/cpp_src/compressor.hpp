#ifndef MEASCOMPRESS_COMPRESSOR_HPP
#define MEASCOMPRESS_COMPRESSOR_HPP

#include <vector>
#include <span>

#include <string>
#include <exception>

#include "./line.hpp"
#include "./dependency.hpp"

namespace measCompress
{
    /**
     * @brief Helper class for compressing a measurement
     * 
     * Compress a measurement by reducing the number of measuremnt points. This
     * will be done by removing points which are approximately on a line.
     * 
     * e.g. a measurement with three points  p_1=(1,7); p_2=(2,8.01); p_3=(3,9)
     * can be approximated with g_1=(1,7); g_2=(3,9) because p_2 lies 
     * approximately on the line p_1/p_2 or rather g_1/g_2
     * 
     * @tparam T double (default)
     */
    template <typename T = double>
    class Compressor
    {
    public:
        /**
        * @brief Base exceptions class
        */
        class Exception : public std::exception
        {
        public:
            Exception(std::string data) : data(std::move(data)) {}
            const char *what() const noexcept override
            {
                return data.c_str();
            }

        protected:
            std::string data;
        };

        /**
         * @brief Invalid sizes exception
         * 
         * e.g. time/data vector is empty
         */
        class InvalidSize : public Exception
        {
        public:
            InvalidSize() : Exception("at least one vector has an invalid dimension") {}
        };

        /**
         * @brief Different sizes exception
         * 
         * e.g. size of the data is different to the size of the time vector
         */
        class DifferentSize : public Exception
        {
        public:
            DifferentSize() : Exception("'t' and 'y' must have the same size") {}
        };

    public:
        /**
         * @brief Construct a new Compressor object
         */
        Compressor() = default;

        /**
         * @brief compute the new points of the compressed measurement
         * 
         * This will be done by "removing" points which are approximately on a 
         * line. So only the begin and end points of the approxmated line will 
         * be saved in the compressed measurement.
         * 
         * @param t_ x vector (or time vector) of the original measurement
         * @param deps depencies for compressing the measurement
         * @return Compressor& (reference to this object)
         */
        Compressor &Fit(std::vector<T> t_,
                        const std::vector<Dependency<T>> &deps)
        {
            const auto n = t_.size();
            if (n < 2)
                throw InvalidSize();
            for (const auto &dep : deps)
            {
                if (dep.GetSize() != n)
                    throw DifferentSize();
            }

            t = std::move(t_);

            position.clear();
            position.reserve(static_cast<std::size_t>(n * 0.1));
            position.push_back(0);

            std::size_t last_step = 64;

            while (true)
            {
                const auto i0 = position.back();
                const auto i1 = binary_search(deps, i0, last_step);
                last_step = i1 - i0;
                position.push_back(i1 - 1);
                if (i1 == t.size())
                    break;
            }
            return *this;
        }

        /**
         * @brief Transform a timeseries to the compressed measurement without fitting
         * 
         * The x-vector (time) of the compressed measuremnt contains a subset of
         * x-vector (time) of the original vector. This functions returns only
         * the positions of the compressed measurement in a given timeseries(y)   
         * 
         * @param y timeseries of the original measurement
         * @return std::vector<T> points of y at the positions of the compressed measurement
         */
        std::vector<T> TransformNoFit(const std::vector<T> &y) const
        {
            if (y.size() != t.size())
                throw InvalidSize();

            std::vector<T> result(position.size());
            for (std::size_t i = 0; i < position.size(); ++i)
            {
                result[i] = y[position[i]];
            }
            return result;
        }

        /**
         * @brief Transform a timeseries to the compressed measurement
         * 
         * Fits line in the timerseries at the position of the compressed
         * measurement.
         * 
         * @param y timeseries of the original measurement
         * @return std::vector<T> compressed version of y
         */
        std::vector<T> Transform(const std::vector<T> &y) const
        {
            if (y.size() != t.size())
                throw InvalidSize();

            std::vector<T> result(position.size());
            for (std::size_t i = 0; i < position.size() - 1; ++i)
            {
                const auto i0 = position[i];
                const auto i1 = position[i + 1] + 1;
                const auto t_ = std::span<const T>(t.begin() + i0, t.begin() + i1);
                const auto y_ = std::span<const T>(y.begin() + i0, y.begin() + i1);

                const auto line = Line<T>::Fit(t_, y_);
                const auto y0 = line.GetY(t_.front());
                const auto y1 = line.GetY(t_.back());

                result[i] = i == 0 ? y0 : (result[i] + y0) / 2;
                result[i + 1] = y1;
            }
            return result;
        }

        /**
         * @brief Get the postions of the compressed measurement
         * 
         * @return const std::vector<std::size_t>& 
         */
        const std::vector<std::size_t> &GetPos() const noexcept { return position; }

        /**
         * @brief Get the x-vector (time) of the original measurement
         * 
         * @return const std::vector<T>& 
         */
        const std::vector<T> &GetTimeOrigin() const noexcept { return t; }

        /**
         * @brief Get the x-vector (time) of the compressed measurement
         * 
         * @return std::vector<T> 
         */
        std::vector<T> GetTimeFit() const { return TransformNoFit(t); }

    private:
        std::size_t binary_search(const std::vector<Dependency<T>> &deps,
                                  std::size_t i0, std::size_t last_step)
        {
            auto check = [this, &deps, i0](std::size_t i1)
            {
                for (const auto &dep : deps)
                    if (!dep.Check(t, i0, i1))
                        return false;
                return true;
            };
            // consider: check(a) is always true

            std::size_t a = i0 + 2;
            std::size_t b = std::min(a + last_step, t.size());
            if (a >= t.size())
                return t.size();

            // go with big steps forward until dependency are false
            while (check(b))
            {
                if (b == t.size())
                    return b;
                a = b;

                // increase stepsize with factor 2
                b = std::min(a + (a - i0), t.size());
            }

            // binary search
            while (a + 1 < b)
            {
                auto m = (a + b) / 2;
                if (check(m))
                    a = m;
                else
                    b = m;
            }
            return a;
        }

    private:
        std::vector<std::size_t> position;
        std::vector<T> t;
    };

} // namespace measCompress

#endif
