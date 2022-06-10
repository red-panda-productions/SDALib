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

static PyObject* CallSpeedDreams(PyObject* p_data, PyObject* p_action);