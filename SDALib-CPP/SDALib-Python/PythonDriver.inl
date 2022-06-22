/*
 * This program has been developed by students from the bachelor Computer Science at
 * Utrecht University within the Software Project course.
 * © Copyright Utrecht University (Department of Information and Computing Sciences)
 */

#include "portability.h"
#include "PythonDriver.h"
#include "IPCPointerManager.h"

#define CREATE_PYTHON_DRIVER_IMPLEMENTATION(p_type)                                      \
    template PythonDriver<p_type>::PythonDriver();                                       \
    template void PythonDriver<p_type>::SetPythonDriverFileName(std::string p_fileName); \
    template PyObject *PythonDriver<p_type>::GetPythonDriverObject();                    \
    template std::string PythonDriver<p_type>::GetPythonDriverFileName();

/// @brief Constructs PythonDriver and sets up python code
template <typename PointerManager>
PythonDriver<PointerManager>::PythonDriver()
{
    m_sdaTypesConverter = SDATypesConverter();
}

/// @brief Initializes the python AI
template <typename PointerManager>
void PythonDriver<PointerManager>::InitAI()
{
    PyObject *driverModuleName = PyUnicode_DecodeFSDefault(m_pythonDriverFileName.c_str());
    PyObject *driverModule = PyImport_Import(driverModuleName);

    if (!PyModule_Check(driverModule)) return;
    PyObject *driverDict = PyModule_GetDict(driverModule);
    std::string py_class_name = "SDADriver";
    PyObject *driverClass = PyDict_GetItemString(driverDict, py_class_name.c_str());

    if (!PyCallable_Check(driverClass)) return;
    PyObject *py_arg_tuple = PyTuple_New(0);
    m_pythonDriver = PyObject_CallObject(driverClass, py_arg_tuple);
}

/// @brief          Updates the python AI by passing the data to python
/// @param  p_data  The data
/// @return         The result of the AI
template <typename PointerManager>
SDAAction PythonDriver<PointerManager>::UpdateAI(SDAData &p_data)
{
    PyObject *sdaType = m_sdaTypesConverter.GetPythonSDATypeObject(p_data);
    PyObject *updateAIFuncName = PyUnicode_FromString("UpdateAI");
    PyObject *result = PyObject_CallMethodObjArgs(m_pythonDriver, updateAIFuncName, sdaType, NULL);

    return m_sdaTypesConverter.GetCppSDAAction(result);
}

/// @brief sets the python driver file name
/// @param p_fileName the python driver file name
template <typename PointerManager>
void PythonDriver<PointerManager>::SetPythonDriverFileName(std::string p_fileName)
{
    m_pythonDriverFileName = p_fileName;
}

/// @brief gets the python driver file name
/// @return the python driver file name
template <typename PointerManager>
std::string PythonDriver<PointerManager>::GetPythonDriverFileName()
{
    return m_pythonDriverFileName;
}

/// @brief gets the python driver object
/// @return the python driver object
template <typename PointerManager>
PyObject *PythonDriver<PointerManager>::GetPythonDriverObject()
{
    return m_pythonDriver;
}
