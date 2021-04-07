# pylint: disable=missing-module-docstring, missing-function-docstring

import subprocess
import os


def test_cpp():
    root_tests = os.path.dirname(os.path.relpath(__file__))
    bin_tests = os.path.join(root_tests, 'bin')

    print("\n\nTesting C++ code...")
    for name in os.listdir(bin_tests):
        tmp = os.path.join(bin_tests, name)
        print(f"Run '{name}':")
        subprocess.check_call(tmp)
