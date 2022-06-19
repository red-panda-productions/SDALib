#include <stdio.h>
#ifdef _DEBUG
#undef _DEBUG
#include <Python.h>
#define _DEBUG
#else
#include <Python.h>
#endif

#include "SDASpeedDreams.hpp"
#include "SDATypesConverter.h"

/// @brief           Calls speed dream functions from Python
/// @param  p_data   The data
/// @param  p_action The action to be performed
/// @return The updated SDAData
static PyObject* simulator_update(PyObject *p_self, PyObject* p_args)
{

    PyObject* data = PyTuple_GetItem(p_args, 0);
    PyObject* action = PyTuple_GetItem(p_args, 1);

    SDATypesConverter sdaTypesConverter = SDATypesConverter();

    SDAData oldData = sdaTypesConverter.GetCppSDAData(data);
    SDAAction oldAction = sdaTypesConverter.GetCppSDAAction(action);

    SDAData newData = SDASpeedDreams(oldData, oldAction);

    return sdaTypesConverter.GetPythonSDATypeObject(newData);
}

static PyMethodDef simulator_methods[] = {
    {"update", (PyCFunction)simulator_update, METH_NOARGS, NULL },
    { NULL, NULL, 0, NULL }
};

static struct PyModuleDef simulatormodule =
    {
        PyModuleDef_HEAD_INIT,
        "simulator",
        "",
        -1,
        simulator_methods};

PyMODINIT_FUNC PyInit_simulator(void)
{
    return PyModule_Create(&simulatormodule);
}