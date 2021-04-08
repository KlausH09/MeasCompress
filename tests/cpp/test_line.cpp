#include "catch2/catch.hpp"
#include "line.hpp"

#include <vector>

using namespace measCompress;
using T = double;

TEST_CASE("GetY", "[measCompress, line]")
{
    T m = 1;
    T y0 = 47;
    T t0 = 3;
    Line<T> line(m, t0, y0);

    REQUIRE(line.GetY(T(3)) == Approx(T(47)));
    REQUIRE(line.GetY(T(0)) == Approx(T(44)));
    REQUIRE(line.GetY(T(4)) == Approx(T(48)));
}

TEST_CASE("GetMaxError", "[measCompress, line]")
{
    {
        T m = 0;
        T y0 = 47;
        T t0 = 3;
        Line<T> line(m, t0, y0);
        std::vector<T> t = {1, 2, 3, 4};
        std::vector<T> y = {48, 46.7, 47.2, 46};
        REQUIRE(line.GetMaxError(t, y) == Approx(T(1)));
    }
    {
        T m = 1;
        T y0 = 3;
        T t0 = 3;
        Line<T> line(m, t0, y0);
        std::vector<T> t = {3, 4, 5, 6};
        std::vector<T> y = {4, 3, 6, 5};
        REQUIRE(line.GetMaxError(t, y) == Approx(T(1)));
    }
    {
        T m = 1;
        T y0 = 3;
        T t0 = 3;
        Line<T> line(m, t0, y0);
        std::vector<T> t = {3, 4, 5, 6};
        std::vector<T> y = {4, 3, 6};
        REQUIRE_THROWS_AS(line.GetMaxError(t, y), Line<T>::DifferentSize);
    }
}

TEST_CASE("fit line", "[measCompress, line]")
{
    {
        std::vector<T> t = {1, 1, 2, 2};
        std::vector<T> y = {-1, 1, 4, -4};
        auto line = Line<T>::Fit(t, y);
        REQUIRE(line.GetY(T(0)) == Approx(T(0)));
        REQUIRE(line.GetY(T(7)) == Approx(T(0)));
    }
    {
        std::vector<T> t = {1, 1, 2, 2};
        std::vector<T> y = {2, 4, 3, 5};
        auto line = Line<T>::Fit(t, y);
        REQUIRE(line.GetY(T(0)) == Approx(T(2)));
        REQUIRE(line.GetY(T(1)) == Approx(T(3)));
        REQUIRE(line.GetY(T(2)) == Approx(T(4)));
    }
    {
        std::vector<T> t = {1, 1, 2, 2};
        std::vector<T> y = {2, 4, 3};
        REQUIRE_THROWS_AS(Line<T>::Fit(t, y), Line<T>::DifferentSize);
    }
}
