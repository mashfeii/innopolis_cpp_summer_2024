#include <python3.12/Python.h>

#include <cstring>
#include <iostream>
#include <string>
#include "string_utils.hpp"

/*
==== default json.dumps: 0.965908716 s
==== pip ujson.dumps: 0.652567924 s
==== module cjson.dumps: 0.913523984 s
-----------------------------------------
==== default json.loads: 0.000206379 s
==== pip ujson.loads: 0.000196861 s
==== module cjson.loads: 0.00176672 s
*/

static PyObject* parse_pair(PyObject* dict, std::string& pair) {
  trim(pair);

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

  return dict;
}
static PyObject* construct_dict(PyObject* dict,
                                std::string current_string,
                                size_t start_index = 0) {
  std::string pair;
  size_t n = current_string.find(',');
  // case 1: we got the next json entity
  if (n != std::string::npos) {
    pair = current_string.substr(start_index, n - start_index);
    current_string = current_string.substr(n + 1);

    // case 2: we got the last entity
  } else if (current_string.find('}') != std::string::npos) {
    pair = current_string.substr(0, current_string.size() - 1);
    // case 3: unexpected syntax
  } else {
    std::cerr << "Could not construct the Dict Object, went to infinite loop" << std::endl;
    return nullptr;
  }

  if (!parse_pair(dict, pair)) {
    std::cerr << "Unable to convert JSON entity" << std::endl;
    return nullptr;
  }

  //                         base case : recursive case
  return (n == std::string::npos) ? dict : construct_dict(dict, current_string);
}
static PyObject* create_dict(const std::string& json_string) {
  PyObject* dict = nullptr;
  if (!(dict = PyDict_New())) {
    printf("ERROR: Failed to create Dict Object\n");
    return nullptr;
  }

  return construct_dict(dict, json_string, 1);
}
static PyObject* cjson_loads(PyObject* self, PyObject* args) {
  char* buffer;

  if (!PyArg_ParseTuple(args, "s", &buffer)) {
    std::cerr << "Failed to parse argument" << std::endl;
    return nullptr;
  }

  std::string json_string{buffer};

  return create_dict(json_string);
};

static PyObject* cjson_dumps(PyObject* self, PyObject* args) {
  PyObject* dict;
  std::string res = "{";

  if (!PyArg_ParseTuple(args, "O!", &PyDict_Type, &dict)) {
    std::cerr << "Failed to parse argument" << std::endl;
    return nullptr;
  }

  PyObject* key;
  PyObject* value;
  Py_ssize_t pos = 0;
  bool first = true;

  while (PyDict_Next(dict, &pos, &key, &value)) {
    std::string key_string{PyUnicode_AsUTF8(key)};
    if (key_string.empty()) {
      std::cerr << "Unable to convert key to the string" << std::endl;
      return nullptr;
    }

    trim(key_string);

    if (first) {
      res += '\"' + key_string + "\": ";
      first = false;
    } else {
      res += ", \"" + key_string + "\": ";
    }

    if (PyFloat_Check(value)) {
      res += std::to_string(PyFloat_AsDouble(value));
    } else if (PyLong_Check(value)) {
      res += std::to_string(PyLong_AsLong(value));
    } else if (PyUnicode_AsUTF8String(value)) {
      res += '\"' + std::string{PyUnicode_AsUTF8(value)} + '\"';
    } else {
      std::cerr << "Unable to parse value in item" << std::endl;
      return nullptr;
    }
  }
  res += "}";

  return Py_BuildValue("s", res.c_str());
};

static PyMethodDef cjson_functions[] = {
    {"loads", cjson_loads, METH_VARARGS, ""},
    {"dumps", cjson_dumps, METH_VARARGS, ""},
    {NULL, NULL, 0, NULL},
};

static PyModuleDef module = {PyModuleDef_HEAD_INIT, "cjson",
                             "Module with serveral methods to work with JSON", -1, cjson_functions};

PyMODINIT_FUNC PyInit_cjson() {
  return PyModule_Create(&module);
}
