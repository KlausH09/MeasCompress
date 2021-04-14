#! /usr/bin/env python3

""" standard script for install the c++ and python code as module in a
python environment. e.g.

pip install -e .
# or
python ./setup.py install
"""

import os
import re
import sys
import platform
import subprocess

from setuptools import setup, Extension, find_packages
from setuptools.command.build_ext import build_ext
from distutils.version import LooseVersion
from shutil import copyfile, copymode


class CMakeExtension(Extension):
    """ for add C++ code via CMake to the python module
    """

    def __init__(self, name, sourcedir=''):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)


class CMakeBuild(build_ext):
    """ helper class for building the C++ code with CMake
    """

    def run(self):
        try:
            out = subprocess.check_output(['cmake', '--version'])
        except OSError as error:
            raise RuntimeError(
                "CMake must be installed to build the following extensions: " +
                ", ".join(e.name for e in self.extensions)) from error

        if platform.system() == "Windows":
            cmake_version = LooseVersion(re.search(r'version\s*([\d.]+)',
                                                   out.decode()).group(1))
            if cmake_version < '3.1.0':
                raise RuntimeError("CMake >= 3.1.0 is required on Windows")

        for ext in self.extensions:
            self.build_extension(ext)

    def build_extension(self, ext):
        extdir = os.path.abspath(
            os.path.dirname(self.get_ext_fullpath(ext.name)))

        cfg = 'Debug' if self.debug else 'Release'

        cmake_args = ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=' + extdir,
                      '-DPYTHON_EXECUTABLE=' + sys.executable,
                      '-DCMAKE_BUILD_TYPE=' + cfg]

        build_args = ['--config', cfg]

        if platform.system() == "Windows":
            cmake_args += [
                f"-DCMAKE_LIBRARY_OUTPUT_DIRECTORY_{cfg.upper()}={extdir}",
            ]
            if sys.maxsize > 2**32:
                cmake_args += ['-A', 'x64']
            build_args += ['--', '/m']
        else:
            build_args += ['--', '-j2']

        env = os.environ.copy()
        env['CXXFLAGS'] = '{} -DVERSION_INFO=\\"{}\\"'.format(
            env.get('CXXFLAGS', ''),
            self.distribution.get_version())
        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)
        subprocess.check_call(['cmake', ext.sourcedir] + cmake_args,
                              cwd=self.build_temp, env=env)
        subprocess.check_call(['cmake', '--build', '.'] + build_args,
                              cwd=self.build_temp)

        # Copy *_test file to tests directory (because "pip install" will not
        # build the C++ code in ./build/)
        extension = '.exe' if platform.system() == "Windows" else ''

        for name in os.listdir(self.build_temp):
            if name.endswith('_test' + extension):
                tmp = os.path.join(self.build_temp, name)
                self.copy_test_file(tmp)
        print()  # Add an empty line for cleaner output

    def copy_test_file(self, src_file):
        '''
        Copy ``src_file`` to ``dest_file`` ensuring parent directory exists.
        By default, message like `creating directory /path/to/package` and
        `copying directory /src/path/to/package -> path/to/package` are
        displayed on standard output. Adapted from scikit-build.
        '''
        # Create directory if needed
        dest_dir = os.path.join(os.path.dirname(
            os.path.abspath(__file__)), 'tests', 'bin')
        if dest_dir != "" and not os.path.exists(dest_dir):
            print("creating directory {}".format(dest_dir))
            os.makedirs(dest_dir)

        # Copy file
        dest_file = os.path.join(dest_dir, os.path.basename(src_file))
        print("copying {} -> {}".format(src_file, dest_file))
        copyfile(src_file, dest_file)
        copymode(src_file, dest_file)


MODULE_NAME = 'MeasCompress'
setup(
    name=MODULE_NAME,
    packages=find_packages('src'),
    package_dir={'': 'src'},
    ext_modules=[CMakeExtension(f"{MODULE_NAME}/bindings")],
    cmdclass=dict(build_ext=CMakeBuild),
)
