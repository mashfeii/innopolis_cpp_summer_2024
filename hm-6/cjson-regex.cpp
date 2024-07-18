#include <python3.10/Python.h>

#include <regex.h>
#include <cmath>
#include <cstring>
#include <iostream>
#include <regex>
#include <string>

/*
==== default json.dumps: 0.995832827 s
==== pip ujson.dumps: 0.65685948 s
==== module cjson.dumps: 0.931668499 s
---------------------------------------
==== default json.loads: 0.000251721 s
==== pip ujson.loads: 0.000202194 s
==== module cjson.loads: 0.001716757 s
*/

static PyObject* cjson_loads(PyObject* self, PyObject* args) {
  char* buffer;

  if (!PyArg_ParseTuple(args, "s", &buffer)) {
    std::cerr << "Failed to parse argument" << std::endl;
    return nullptr;
  }
  std::string json_string{buffer};

  PyObject* dict = nullptr;
  if (!(dict = PyDict_New())) {
    printf("ERROR: Failed to create Dict Object\n");
    return nullptr;
  }

  std::regex expression("\"([A-Za-z]*)\"\\s*:\\s*(\"?\\w*\"?)\\s*[},]?\\s*");
  std::smatch match;

  auto words_begin = std::sregex_iterator(json_string.begin(), json_string.end(), expression);
  auto words_end = std::sregex_iterator();

  for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
    std::smatch groups;
    std::string match_item = (*i).str();

    if (std::regex_match(match_item, groups, expression)) {
      PyObject* key = NULL;
      std::string key_string = groups[1].str();

      if (!(key = Py_BuildValue("s", key_string.c_str()))) {
        printf("ERROR: Failed to build string value\n");
        return NULL;
      }

      PyObject* value = NULL;
      std::string value_string = groups[2].str();
      if (value_string[0] == '"') {
        if (!(value =
                  Py_BuildValue("s", value_string.substr(1, value_string.size() - 2).c_str()))) {
          printf("ERROR: Failed to build integer value\n");
          return NULL;
        }
      } else {
        if (!(value = Py_BuildValue("i", std::stoi(value_string)))) {
          printf("ERROR: Failed to build integer value\n");
          return NULL;
        }
      }

      if (PyDict_SetItem(dict, key, value) < 0) {
        printf("ERROR: Failed to set item\n");
        return NULL;
      }
    }
  }
  return dict;
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
