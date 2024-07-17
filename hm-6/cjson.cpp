#include <python3.10/Python.h>

#include <cstring>
#include <iostream>

PyObject* construct_dict(const std::string& json_string) {
  PyObject* dict = nullptr;
  if (!(dict = PyDict_New())) {
    printf("ERROR: Failed to create Dict Object\n");
    return nullptr;
  }

  size_t prev_position = 1;
  std::string temp = json_string;
  for (;;) {
    std::string pair;
    if (size_t n = temp.find(','); n != std::string::npos) {
      pair = temp.substr(prev_position, n - 1);
      temp = temp.substr(n + 1);
    } else if (temp.find('}') != std::string::npos) {
      pair = temp.substr(prev_position, temp.size() - prev_position - 1);
      temp.clear();
    } else {
      break;
    }

    PyObject* key = nullptr;
    std::string key_string = pair.substr(1, pair.find('"', 1) - 1);
    if (!(key = Py_BuildValue("s", key_string.data()))) {
      printf("ERROR: Failed to build string value\n");
      return nullptr;
    }

    PyObject* value = nullptr;
    std::string value_string = pair.substr(pair.find(':') + 1);

    if (size_t n = value_string.find('"'); n == std::string::npos) {
      int ivalue = std::stoi(value_string);

      if (!(value = Py_BuildValue("i", ivalue))) {
        printf("ERROR: Failed to build integer value\n");
        return nullptr;
      }
    } else {
      value_string = value_string.substr(n + 1, value_string.size() - n - 2);

      if (!(value = Py_BuildValue("s", value_string.data()))) {
        printf("ERROR: Failed to build integer value\n");
        return nullptr;
      }
    }

    if (PyDict_SetItem(dict, key, value) < 0) {
      printf("ERROR: Failed to set item\n");
      return nullptr;
    }
  }

  return dict;
}

PyObject* cjson_loads(PyObject* self, PyObject* args) {
  char* buffer;

  if (!PyArg_ParseTuple(args, "s", &buffer)) {
    std::cerr << "Failed to parse argument" << std::endl;
    return nullptr;
  }

  std::string json_string{buffer};

  return construct_dict(json_string);
};

PyObject* cjson_dumps(PyObject* self, PyObject* args) { return nullptr; };

static PyMethodDef cjson_functions[] = {
    {"loads", cjson_loads, METH_VARARGS, ""},
    {"dumps", cjson_dumps, METH_VARARGS, ""},
    {NULL, NULL, 0, NULL},
};

static PyModuleDef module = {PyModuleDef_HEAD_INIT, "cjson",
                             "Module with serveral methods to work with JSON",
                             -1, cjson_functions};

PyMODINIT_FUNC PyInit_cjson() { return PyModule_Create(&module); }
