# pylint: disable=missing-module-docstring, missing-function-docstring
import pytest
from MeasCompress import Compressor, Dependency


def allclose(a, b):
    if len(a) != len(b):
        return False
    for e1, e2 in zip(a, b):
        if abs(e1 - e2) > 1e-8:
            return False
    return True


def test_fit():
    t = [3, 4, 5, 6]
    y = [7, 8, 9]
    dep = Dependency(y, 0.1)
    with pytest.raises(RuntimeError):
        Compressor().Fit(t, [dep])

    t = [3]
    y = [7, 8, 9]
    dep = Dependency(y, 0.1)
    with pytest.raises(RuntimeError):
        Compressor().Fit(t, [dep])

    t = [3, 4, 5, 6]
    y = [7, 8, 9, 10]
    dep = Dependency(y, 0.1)
    compress = Compressor().Fit(t, [dep])
    assert allclose(compress.GetTimeOrigin(), t)
    assert allclose(compress.GetTimeFit(), [3, 6])
    assert allclose(compress.Transform(y), [7, 10])
    assert allclose(compress.TransformNoFit(y), [7, 10])
    assert allclose(compress.TransformNoFit(t), compress.GetTimeFit())

    t = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
    y = [0, 1, 2, 3, 3, 3, 3, 2, 1, 0]
    dep = Dependency(y, 0.1)
    compress = Compressor().Fit(t, [dep])
    assert allclose(compress.GetTimeOrigin(), t)
    assert allclose(compress.GetTimeFit(), [0, 3, 6, 9])
    assert allclose(compress.Transform(y), [0, 3, 3, 0])
    assert allclose(compress.TransformNoFit(y), [0, 3, 3, 0])
    assert allclose(compress.TransformNoFit(t), compress.GetTimeFit())

    t = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
    y = [0, 1.1, 2.1, 3, 3, 3, 3, 1.9, 0.9, 0]
    dep = Dependency(y, 0.1)
    compress = Compressor().Fit(t, [dep])
    assert allclose(compress.GetTimeOrigin(), t)
    assert allclose(compress.GetTimeFit(), [0, 3, 6, 9])
    assert allclose(compress.Transform(y), [0.05, 3.025, 2.975, -0.05])
    assert allclose(compress.TransformNoFit(y), [0, 3, 3, 0])
    assert allclose(compress.TransformNoFit(t), compress.GetTimeFit())
