/*
 * This program has been developed by students from the bachelor Computer Science at
 * Utrecht University within the Software Project course.
 * © Copyright Utrecht University (Department of Information and Computing Sciences)
 */

#pragma once

#include "SDAAction.hpp"
#include "SDADriver.hpp"
#include "SDAData.hpp"
#include "SDATypesConverter.h"
#ifdef _DEBUG
#undef _DEBUG
#include <Python.h>
#define _DEBUG
#else
#include <Python.h>
#endif
#include <stdlib.h>
#include <iostream>
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING 1
#include <experimental/filesystem>

namespace filesystem = std::experimental::filesystem;

///@brief class for connecting SDALib to a Python decision maker
template <typename PointerManager>
class PythonDriver : public AIInterface<PointerManager>
{
public:
    PythonDriver();

    void SetPythonDriverFileName(std::string p_fileName);
    std::string GetPythonDriverFileName();

    PyObject* m_pythonDriver;
    std::string m_pythonDriverFileName = "Driver";

#ifdef TEST
public:
#else
protected:
#endif
    void InitAI() override;

    SDAAction UpdateAI(SDAData& p_data) override;

    SDATypesConverter m_sdaTypesConverter;
};
