#include <stdio.h>
#define PY_SSIZE_T_CLEAN

#ifdef _DEBUG
#undef _DEBUG
#include <Python.h>
#define _DEBUG
#else
#include <Python.h>
#endif

#include "SDASpeedDreams.h"
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

    SDASpeedDreams sdaSpeedDreams;
    SDAData newData = sdaSpeedDreams.UpdateSimulator(oldData, oldAction);

    PyTuple_SetItem(p_args, 3, sdaTypesConverter.GetPythonSDATypeObject(newData));

    return PyLong_FromDouble(1);
}

static PyMethodDef simulator_methods[] = {
    {"update", simulator_update, METH_VARARGS, "" },
    { NULL, NULL, 0, NULL }
};

static struct PyModuleDef simulator_module =
    {
        PyModuleDef_HEAD_INIT,
        "simulator",
        "",
        -1,
        simulator_methods};

PyMODINIT_FUNC PyInit_simulator(void)
{
    return PyModule_Create(&simulator_module);
}