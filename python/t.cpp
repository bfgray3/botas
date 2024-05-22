#include <vector>

#include <Python.h>


#include <vector>
#include <utility>


template <typename T>
constexpr std::pair<std::size_t, std::size_t> find_percentile_indices(
  const std::vector<T>& v,  // TODO: more general
  const double confidence_level
) {
  const auto len{static_cast<double>(v.size())};
  return {
    static_cast<std::size_t>((1 - confidence_level) * len),
    static_cast<std::size_t>(confidence_level * len)
  };
}


static PyObject* foo_cpp(PyObject*, PyObject* o) {
  double x = PyFloat_AsDouble(o);
  std::vector<int> v{5, 5, 6};
  return PyFloat_FromDouble(x + 1.0 + static_cast<double>(v.size()));
}

static PyMethodDef methods[] = {
  {"foo", (PyCFunction)foo_cpp, METH_O, "docstring"}, // TODO: need this cast???
  {nullptr, nullptr, 0, nullptr}
};

static PyModuleDef mod = {
  PyModuleDef_HEAD_INIT,
  "botas",
  "docstring",
  0, // TODO: what is this???
  methods
};

PyMODINIT_FUNC PyInit_botas() {
  return PyModule_Create(&mod);
}
