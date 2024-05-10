#include <Python.h>

static PyObject* foo_cpp(PyObject*, PyObject* o) {
  double x = PyFloat_AsDouble(o);
  return PyFloat_FromDouble(x + 1.0);
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
