#ifndef MEASCOMPRESS_DEPENDENCY_HPP
#define MEASCOMPRESS_DEPENDENCY_HPP

#include "./line.hpp"

#include <string>
#include <exception>

namespace measCompress
{
    /**
     * @brief Describs a dependency for a single timeseries of a measurement
     * 
     * @tparam T double (default)
     */
    template <typename T = double>
    class Dependency
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
            InvalidSize() : Exception("invalid size") {}
        };

        /**
         * @brief Invalid tolerance exception
         * 
         * e.g. tolerance < 0
         */
        class InvalidTolerance : public Exception
        {
        public:
            InvalidTolerance() : Exception("tolerance must be >= 0") {}
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

        /**
         * @brief Index out of bounds exception
         */
        class IndexOutOfBounds : public Exception
        {
        public:
            IndexOutOfBounds() : Exception("index out of bounds") {}
        };

    public:
        /**
         * @brief Construct a new Dependency object
         * 
         * @param y data of a timeseries
         * @param tol allowed approximation tolerance/error
         */
        Dependency(std::vector<T> y, T tol)
            : y(std::move(y)),
              tol(std::move(tol))
        {
            if (y.size() < 2)
            {
                throw InvalidSize();
            }
            if (tol < T(0))
            {
                throw InvalidTolerance();
            }
        }

        /**
         * @brief Check if a give intervall can approximate with a line
         * 
         * @param t time vector of the hole timeseries
         * @param i0 begin of the considered intervall
         * @param i1 end of the considered intervall
         * @return true, if the intervall can be approximated with a line
         * @return false, else
         */
        bool Check(const std::vector<T> &t,
                   std::size_t i0,
                   std::size_t i1) const
        {
            if (t.size() != y.size())
            {
                throw DifferentSize();
            }
            if (i1 > y.size() || i0 >= i1)
            {
                throw IndexOutOfBounds();
            }
            if (i1 - i0 < 3)
            {
                return true;
            }

            auto t_ = std::span(t.begin() + i0, t.begin() + i1);
            auto y_ = std::span(y.begin() + i0, y.begin() + i1);

            auto line = Line<T>::Fit(t_, y_);
            auto error = line.GetMaxError(t_, y_);
            return error < tol;
        }

        /**
         * @brief Get the Size of the timeseries
         * 
         * @return std::size_t 
         */
        std::size_t GetSize() const noexcept { return y.size(); }

    private:
        std::vector<T> y;
        T tol;
    };

} // namespace measCompress

#endif
