import pytest
import numpy as np
from MeasCompress.bindings import Compressor, Dependency


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
    assert np.allclose(compress.GetTimeOrigin(), t)
    assert np.allclose(compress.GetTimeFit(), [3, 6])
    assert np.allclose(compress.Transform(y), [7, 10])
    assert np.allclose(compress.TransformNoFit(y), [7, 10])
    assert np.allclose(compress.TransformNoFit(t), compress.GetTimeFit())

    t = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
    y = [0, 1, 2, 3, 3, 3, 3, 2, 1, 0]
    dep = Dependency(y, 0.1)
    compress = Compressor().Fit(t, [dep])
    assert np.allclose(compress.GetTimeOrigin(), t)
    assert np.allclose(compress.GetTimeFit(), [0, 3, 6, 9])
    assert np.allclose(compress.Transform(y), [0, 3, 3, 0])
    assert np.allclose(compress.TransformNoFit(y), [0, 3, 3, 0])
    assert np.allclose(compress.TransformNoFit(t), compress.GetTimeFit())

    t = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
    y = [0, 1.1, 2.1, 3, 3, 3, 3, 1.9, 0.9, 0]
    dep = Dependency(y, 0.1)
    compress = Compressor().Fit(t, [dep])
    assert np.allclose(compress.GetTimeOrigin(), t)
    assert np.allclose(compress.GetTimeFit(), [0, 3, 6, 9])
    assert np.allclose(compress.Transform(y), [0.05, 3.025, 2.975, -0.05])
    assert np.allclose(compress.TransformNoFit(y), [0, 3, 3, 0])
    assert np.allclose(compress.TransformNoFit(t), compress.GetTimeFit())
