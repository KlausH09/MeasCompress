#include "catch2/catch.hpp"
#include "compressor.hpp"

#include <vector>

using namespace measCompress;
using T = double;

void equal(const std::vector<T> &a, const std::vector<T> &b)
{
    REQUIRE(a.size() == b.size());
    for (std::size_t i = 0; i < a.size(); ++i)
    {
        REQUIRE(a[i] == Approx(b[i]));
    }
}

TEST_CASE("fit measurement", "[measCompress, compressor]")
{
    {
        std::vector<T> t = {3, 4, 5, 6};
        std::vector<T> y = {7, 8, 9};
        Dependency<T> dep1(y, T(0.1));
        REQUIRE_THROWS_AS(Compressor<T>().Fit(t, {dep1}),
                          Compressor<T>::DifferentSize);
    }
    {
        std::vector<T> t = {3};
        std::vector<T> y = {7, 8, 9};
        Dependency<T> dep1(y, T(0.1));
        REQUIRE_THROWS_AS(Compressor<T>().Fit(t, {dep1}),
                          Compressor<T>::InvalidSize);
    }
    {
        std::vector<T> t = {3, 4, 5, 6};
        std::vector<T> y = {7, 8, 9, 10};
        Dependency<T> dep1(y, T(0.1));
        auto compress = Compressor<T>().Fit(t, {dep1});

        equal(compress.GetTimeOrigin(), t);
        equal(compress.GetTimeFit(), {3, 6});
        equal(compress.Transform(y), {7, 10});
        equal(compress.TransformNoFit(y), {7, 10});
        equal(compress.TransformNoFit(t), compress.GetTimeFit());
    }
    {
        std::vector<T> t = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        std::vector<T> y = {0, 1, 2, 3, 3, 3, 3, 2, 1, 0};
        Dependency<T> dep1(y, T(0.1));
        auto compress = Compressor<T>().Fit(t, {dep1});

        equal(compress.GetTimeOrigin(), t);
        equal(compress.GetTimeFit(), {0, 3, 6, 9});
        equal(compress.Transform(y), {0, 3, 3, 0});
        equal(compress.TransformNoFit(y), {0, 3, 3, 0});
        equal(compress.TransformNoFit(t), compress.GetTimeFit());
    }
    {
        std::vector<T> t = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        std::vector<T> y = {0, 1.1, 2.1, 3, 3, 3, 3, 1.9, 0.9, 0};
        Dependency<T> dep1(y, T(0.1));
        auto compress = Compressor<T>().Fit(t, {dep1});

        equal(compress.GetTimeOrigin(), t);
        equal(compress.GetTimeFit(), {0, 3, 6, 9});
        equal(compress.Transform(y), {0.05, 3.025, 2.975, -0.05});
        equal(compress.TransformNoFit(y), {0, 3, 3, 0});
        equal(compress.TransformNoFit(t), compress.GetTimeFit());
    }
}
