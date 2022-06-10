#include "PythonDriver.h"
#include "PythonDriver.inl"
#include "SDASpeedDreams.hpp"

CREATE_PYTHON_DRIVER_IMPLEMENTATION(IPCPointerManager)

#ifndef SDA_LIBRARY
/// @brief  Main entry point of the driver
int main()
{
    Py_Initialize();
    PythonDriver<IPCPointerManager> driver;
    driver.Run();
    Py_Finalize();
    return 0;
}

/// @brief           Calls speed dream functions from Python
/// @param  p_data   The data
/// @param  p_action The action to be performed
void CallSpeedDreams(PyObject* p_data, PyObject* p_action)
{
    PythonDriver<IPCPointerManager> driver;

    SDAData oldData = driver.GetSDADataCpp(p_data);
    SDAAction action = driver.GetSDAActionCpp(p_action);

    //SDAData newData = SDASpeedDreams(oldData, action);

    //PyObject* sdaType = driver.GetSDATypeObject(newData);
    //PyObject* updateSDAFuncName = PyUnicode_FromString("UpdateSDAAction");
    //PyObject_CallMethodObjArgs(driver.GetPythonDriverObject(), updateSDAFuncName, sdaType, NULL);
}
#endif