#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "compressor.hpp"
#include "dependency.hpp"

namespace py = pybind11;

using T = double;
using Dependency = measCompress::Dependency<T>;
using Compressor = measCompress::Compressor<T>;

PYBIND11_PLUGIN(bindings)
{
  py::module m("bindings", R"doc(
       MeasCompress C++ implementation
        -----------------------
        .. currentmodule:: bindings
        .. autosummary::
    )doc");

  py::class_<Dependency>(m, "Dependency")
      .def(py::init<std::vector<T>, T>()); // TODO docstring

  py::class_<Compressor>(m, "Compressor")
      .def(py::init<>())
      .def("Fit", &Compressor::Fit)
      .def("TransformNoFit", &Compressor::TransformNoFit)
      .def("Transform", &Compressor::Transform)
      .def("GetPos", &Compressor::GetPos)
      .def("GetTimeFit", &Compressor::GetTimeFit)
      .def("GetTimeOrigin", &Compressor::GetTimeOrigin); // TODO docstring

  return m.ptr();
}
