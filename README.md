# MeasCompress

## Initialize

```bash
git submodule update --init --recursive
```

## Installation

To build and install run

```bash
python3 ./setup.py install
```

or

```bash
# delete ./build dictionary before!!
pip3 install -e .
```

## Tests

To execute all unit tests, run the following command:

```bash
python ./setup.py pytest
```

## Usage

```python
import numpy as np
from MeasCompress import Compressor, Dependency

def GenData():
    y = np.array([])
    for i in range(10):
        y = np.r_[y, i + np.random.rand(100) * 0.01]
    t = np.linspace(0, 100, y.size)
    return t, y

t, y = GenData()
dep = Dependency(y, 0.1)
comp = Compressor().Fit(t, [dep])
t_compressed = comp.GetTimeFit()
y_compressed = comp.Transform(y)
```
