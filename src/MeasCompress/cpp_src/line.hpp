#ifndef MEASCOMPRESS_LINE_HPP
#define MEASCOMPRESS_LINE_HPP

#include <span>
#include <algorithm>
#include <functional>
#include <execution>

#include <string>
#include <exception>

namespace measCompress
{

    /**
     * @brief line in the 2 dimensional space
     * 
     * y(t) = (t - t0) * m + y0
     * 
     * @tparam T double (default)
     */
    template <typename T = double>
    class Line
    {
    private:
        static constexpr auto execution_policy = std::execution::seq;

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
         * @brief Construct a new Line object
         */
        Line() = default;

        /**
         * @brief Construct a new Line object
         * 
         * @param m slope of the line
         * @param t0 offset x-axis
         * @param y0 offset y-axis
         */
        Line(T m, T t0, T y0)
            : m(std::move(m)),
              t0(std::move(t0)),
              y0(std::move(y0)) {}

        /**
         * @brief fit a line in a set of points
         * 
         * @param t x coordinates of the points
         * @param y y coordinates of the points
         * @return Line fitted line
         */
        static Line Fit(std::span<const T> t, std::span<const T> y)
        {
            if (t.size() != y.size())
            {
                throw DifferentSize();
            }

            const auto t0 = t[0];

            // sum (dt_i)
            auto dt_sum = std::reduce(execution_policy,
                                      t.begin(), t.end(),
                                      T(0),
                                      [&t0](const T &re, const T &ti) {
                                          return (re + (ti - t0));
                                      });
            // sum (y_i)
            auto y_sum = std::reduce(execution_policy,
                                     y.begin(), y.end(),
                                     T(0),
                                     std::plus<T>());
            // sum (dt_i * dt_i)
            auto dtdt_sum = std::reduce(execution_policy,
                                        t.begin(), t.end(),
                                        T(0),
                                        [&t0](const T &re, const T &ti) {
                                            return (re + (ti - t0) * (ti - t0));
                                        });
            // sum (dt_i * y_i)
            auto dty_sum = std::transform_reduce(execution_policy,
                                                 t.begin(), t.end(),
                                                 y.begin(),
                                                 T(0),
                                                 std::plus<T>(),
                                                 [&t0](const T &ti, const T &yi) {
                                                     return (yi * (ti - t0));
                                                 });
            auto n = y.size();
            T m = (y_sum * dt_sum - n * dty_sum) /
                  (dt_sum * dt_sum - n * dtdt_sum);
            T y0 = (y_sum - m * dt_sum) / n;
            return Line(m, t0, y0);
        }

        /**
         * @brief evaluate the point on the line
         * 
         * @param t x_coordinate
         * @return T y coordinate
         */
        T GetY(const T &t) const noexcept
        {
            return ((t - t0) * m + y0);
        }

        /**
         * @brief Get the Max Error between the line a and point set 
         * 
         * @param t x coordinates of the points
         * @param y y coordinates of the points
         * @return T max error (infinity norm)
         */
        T GetMaxError(std::span<const T> t, std::span<const T> y)
        {
            if (t.size() != y.size())
            {
                throw DifferentSize();
            }

            auto error = std::transform_reduce(
                execution_policy,
                t.begin(), t.end(),
                y.begin(),
                T(0),
                [](const T &err0, const T &erri) {
                    return std::max(err0, erri);
                },
                [this](const T &ti, const T &yi) {
                    return std::abs(yi - GetY(ti));
                });
            return error;
        }

    private:
        T m;
        T t0;
        T y0;
    };

} // namespace measCompress

#endif