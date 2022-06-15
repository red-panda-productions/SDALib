#pragma once

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
static PyObject* CallSpeedDreams(PyObject* p_data, PyObject* p_action)
{
    SDATypesConverter sdaTypesConverter = SDATypesConverter();

    SDAData oldData = sdaTypesConverter.GetCppSDAData(p_data);
    SDAAction action = sdaTypesConverter.GetCppSDAAction(p_action);

    SDAData newData = SDASpeedDreams(oldData, action);

    return sdaTypesConverter.GetPythonSDATypeObject(newData);
}

static PyMethodDef speedDreams_methods[] = {
    {"update", CallSpeedDreams, METH_VARARGS, ""},
    {0}};

static struct PyModuleDef simulator =
    {
        PyModuleDef_HEAD_INIT,
        "simulator",
        "",
        -1,
        speedDreams_methods};

PyMODINIT_FUNC PyInit_simulator(void)
{
    return PyModule_Create(&simulator);
}