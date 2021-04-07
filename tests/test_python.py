# pylint: disable=missing-module-docstring, missing-function-docstring

# FIXME
import python_cpp_example


def test_say_hello():
    assert python_cpp_example.say_hello() == "Hello world! 3"
