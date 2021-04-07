"""a example python script which will be a part of the installed module
"""

# FIXME
from python_cpp_example import bindings


def say_hello():
    """this functions says hello

    Returns:
        str: says hello to you
    """
    return f"Hello world! {bindings.add(1,2)}"
