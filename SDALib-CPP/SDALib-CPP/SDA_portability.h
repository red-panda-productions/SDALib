/*
 * This program has been developed by students from the bachelor Computer Science at
 * Utrecht University within the Software Project course.
 * © Copyright Utrecht University (Department of Information and Computing Sciences)
 */

#pragma once

#include "ipclib_portability.h"
#include "portability.h"
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING 1
#include <experimental/filesystem>

#ifdef WIN32
#define SDA_IP_TYPE PCWSTR
#define LOCALHOST   L"127.0.0.1"

#define SDA_THROW_EXCEPTION(p_msg)     \
    std::cerr << (p_msg) << std::endl; \
    throw std::exception(p_msg)

#include "Windows.h"

#define SET_WORKING_DIR()                                                                                \
    char exeDir[260];                                                                                    \
    GetModuleFileName(NULL, exeDir, 260);                                                                \
    const char* workingDir = std::experimental::filesystem::path(exeDir).parent_path().string().c_str(); \
    chdir(workingDir)
#endif

#ifdef __linux__
#define LOCALHOST   "127.0.0.1"
#define SDA_IP_TYPE const char*
#define SDA_THROW_EXCEPTION(p_msg)     \
    std::cerr << (p_msg) << std::endl; \
    throw std::exception();

#define SET_WORKING_DIR()                                                                                \
    const char* exeDir = std::experimental::filesystem::canonical("/proc/self/exe").string().c_str();    \
    const char* workingDir = std::experimental::filesystem::path(exeDir).parent_path().string().c_str(); \
    chdir(workingDir)

#endif

/// @brief		   Checks if the action was reported successfully from IPCLib
/// @param  p_stmt The action that needs to be checked
/// @param  p_msg  The message that is pushed to the standard error output when the action has failed
#define IPC_OK(p_stmt, p_msg)       \
    if ((p_stmt) != IPCLIB_SUCCEED) \
    {                               \
        SDA_THROW_EXCEPTION(p_msg); \
    }
