# pylint: disable=missing-module-docstring, missing-function-docstring

# FIXME
import python_cpp_example


def test_add():
    assert python_cpp_example.add(1, 1) == 2


def test_subtract():
    assert python_cpp_example.subtract(1, 1) == 0
