# pybind11_cpp_module_template

## Initialize

> create a new repo from this template and clone it recursively:

```bash
git clone XXX
git submodule update --init --recursive
```

or

> copy the files and add `pybind11` and optional `catch2` as submodule:

```bash
# pybind11
git submodule add https://github.com/pybind/pybind11.git ./extern/pybind11
cd extern/pybind11
git fetch --tags
git checkout v2.6.2
cd ../..

# catch
git submodule add https://github.com/catchorg/Catch2.git ./extern/catch2
cd extern/catch2
git fetch --tags
git checkout v2.13.4
cd ../..
```

## Installation

To build and install run

```bash
python3 ./setup.py install
```

or

```bash
# delete ./build dictionary before!!
pip3 install .
```

## Tests

To execute all unit tests, run the following command:

```bash
python ./setup.py pytest
```

add to `./vscode/settings.json`

```json
    "python.testing.unittestEnabled": true
```
