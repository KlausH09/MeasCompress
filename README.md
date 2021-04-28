# MeasCompress

## Initialize

```bash
git submodule update --init --recursive
```

## Installation

To build and install run

```bash
python ./setup.py install
```

or

```bash
# delete ./build dictionary before!!
pip install -e .
```

## Tests

To execute all unit tests, run the following command:

```bash
python ./setup.py pytest
```

## Usage Basic

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

## Usage GUI

```python
import numpy as np
from MeasCompress import MeasCompressGUI

def GenData():
    y1 = np.array([])
    y2 = np.array([])
    for i in range(10):
        y1 = np.r_[y1, (i//2) % 2 + np.random.rand(100) * 0.02]
        y2 = np.r_[y2, (i//3) % 2 + np.random.rand(100) * 0.02]
    t = np.linspace(0, 100, y1.size)
    return t, y1, y2

t, y1, y2 = GenData()

app = MeasCompressGUI(t)
app.add('U', y1, show=True, tol=0.2)
app.add('I', y2, show=True, tol=0.2)
app.run()
```
