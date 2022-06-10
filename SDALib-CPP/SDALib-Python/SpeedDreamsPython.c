#include <Python.h>
#include <stdio.h>
#include "IPCPointerManager.h"
#include "PythonDriver.h"
#include "PythonDriver.inl"
#include "SDASpeedDreams.hpp"

CREATE_PYTHON_DRIVER_IMPLEMENTATION(IPCPointerManager)

/// @brief           Calls speed dream functions from Python
/// @param  p_data   The data
/// @param  p_action The action to be performed
/// @return The updated SDAData
static PyObject* CallSpeedDreams(PyObject* p_data, PyObject* p_action)
{
    PythonDriver<IPCPointerManager> driver;

    SDAData oldData = driver.GetCppSDAData(p_data);
    SDAAction action = driver.GetCppSDAAction(p_action);

    //SDAData newData = SDASpeedDreams(oldData, action);

    PyObject* pyObject;
    return pyObject;
    // return driver.GetPythonSDATypeObject(newData);
}

static PyMethodDef speedDreams_methods[] = {
    {"callSpeedDreams", CallSpeedDreams, METH_VARARGS, ""},
    {0}
};

PyMODINIT_FUNC initmorseCodeTrans(void)
{
    Py_InitModule("SpeedDreams", speedDreams_methods);
}