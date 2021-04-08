# pylint: disable=missing-module-docstring, missing-function-docstring
import pytest
from MeasCompress.bindings import Dependency


def test_constructor():

    with pytest.raises(RuntimeError):
        Dependency([], 0.2)
    with pytest.raises(RuntimeError):
        Dependency([1], 0.2)
    with pytest.raises(RuntimeError):
        Dependency([1, 2], -0.1)

    Dependency([1, 2], 0)
    Dependency([1, 2], 0.1)
