#pragma once

#include "ipclib_portability.h"

#ifdef WIN32
#define SDA_IP_TYPE PCWSTR
#define LOCALHOST L"127.0.0.1"

#define SDA_THROW_EXCEPTION(p_msg) \
    std::cerr << (p_msg) << std::endl; \
    throw std::exception(p_msg)
#endif

#ifdef __linux__
#define LOCALHOST "127.0.0.1"
#define SDA_IP_TYPE const char*
#define SDA_THROW_EXCEPTION(p_msg) \
    std::cerr << (p_msg) << std::endl; \
    throw std::exception();

#endif

/// @brief		   Checks if the action was reported successfully from IPCLib
/// @param  p_stmt The action that needs to be checked
/// @param  p_msg  The message that is pushed to the standard error output when the action has failed
#define IPC_OK(p_stmt, p_msg)              \
    if ((p_stmt) != IPCLIB_SUCCEED)        \
    {                                      \
        SDA_THROW_EXCEPTION(p_msg);        \
    }
