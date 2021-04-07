#include "catch2/catch.hpp"
#include "dependency.hpp"

#include <vector>

using namespace measCompress;
using T = double;

TEST_CASE("constructor dependency", "[measCompress, dependency]")
{
    {
        std::vector<T> y = {};
        REQUIRE_THROWS_AS(Dependency<T>(y, T(0.2)),
                          Dependency<T>::InvalidSize);
    }
    {
        std::vector<T> y = {1};
        REQUIRE_THROWS_AS(Dependency<T>(y, T(0.2)),
                          Dependency<T>::InvalidSize);
    }
    {
        std::vector<T> y = {1, 2};
        Dependency<T>(y, T(0));
    }
    {
        std::vector<T> y = {1, 2};
        Dependency<T>(y, T(0.1));
    }
    {
        std::vector<T> y = {1, 2};
        REQUIRE_THROWS_AS(Dependency<T>(y, T(-0.1)),
                          Dependency<T>::InvalidTolerance);
    }
}
TEST_CASE("check dependency", "[measCompress, dependency]")
{
    std::vector<T> t = {1, 2, 3, 4, 5, 6};
    std::vector<T> y = {5, 6.1, 6.9, 8, 9.5, 10};

    Dependency<T> dep(y, T(0.2));
    REQUIRE(dep.GetSize() == 6);

    REQUIRE(dep.Check(t, 0, 4));
    REQUIRE(dep.Check(t, 1, 4));
    REQUIRE(dep.Check(t, 2, 4));
    REQUIRE(!dep.Check(t, 0, 5));
    REQUIRE(!dep.Check(t, 0, 6));
    REQUIRE(!dep.Check(t, 1, 6));
    REQUIRE(!dep.Check(t, 2, 6));
    REQUIRE(!dep.Check(t, 3, 6));

    {
        std::vector<T> t_ = {1, 2, 3, 4, 5};
        REQUIRE_THROWS_AS(dep.Check(t_, 0, 3),
                          Dependency<T>::DifferentSize);
    }
    {
        std::vector<T> t_ = {1, 2, 3, 4, 5, 6, 7};
        REQUIRE_THROWS_AS(dep.Check(t_, 0, 3),
                          Dependency<T>::DifferentSize);
    }
    REQUIRE_THROWS_AS(dep.Check(t, 3, 3),
                      Dependency<T>::IndexOutOfBounds);
    REQUIRE_THROWS_AS(dep.Check(t, 4, 3),
                      Dependency<T>::IndexOutOfBounds);
    REQUIRE_THROWS_AS(dep.Check(t, 3, 7),
                      Dependency<T>::IndexOutOfBounds);
}
