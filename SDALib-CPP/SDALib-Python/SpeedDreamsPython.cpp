#pragma once

#include <stdio.h>
#include "IPCPointerManager.h"
#include "SDATypesConverter.h"
#ifdef _DEBUG
#undef _DEBUG
#include <Python.h>
#define _DEBUG
#else
#include <Python.h>
#endif
#include "SDASpeedDreams.hpp"

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
    {"call", CallSpeedDreams, METH_VARARGS, ""},
    {0}};

static struct PyModuleDef speedDreamsPythonMod =
    {
        PyModuleDef_HEAD_INIT,
        "SpeedDreamsPython",
        "",
        -1,
        speedDreams_methods};

PyMODINIT_FUNC PyInit_speedDreamsMod(void)
{
    return PyModule_Create(&speedDreamsPythonMod);
}