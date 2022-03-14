# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#

cmake_minimum_required (VERSION 3.8)

project("SDALib")

ENABLE_TESTING()

CMAKE_POLICY(SET CMP0012 NEW)
CMAKE_POLICY(SET CMP0074 NEW)
cmake_policy(SET CMP0079 NEW)

FIND_PACKAGE(IPCLIB HINTS "/libraries")
FIND_LIBRARY(IPCLIB_LIBRARY HINTS ${IPCLIB_LIB_DIR} NAMES IPCLib)

IF(IPCLIB_FOUND)
	MESSAGE(STATUS "Looking for library IPCLib - found")
ELSE(IPCLIB_FOUND)
	MESSAGE(FATAL_ERROR "Looking for library IPCLib - NOT found")
ENDIF(IPCLIB_FOUND)

SET(Boost_INCLUDE_DIR "${IPCLIB_INCLUDE_DIR}")
MESSAGE(STATUS "Looking for library Boost - found")

FIND_PACKAGE(msgpack HINTS "/libraries")

IF(msgpack_FOUND)
	MESSAGE(STATUS "Looking for library msgpack - found")
ELSE(msgpack_FOUND)
	MESSAGE(FATAL_ERROR "Looking for library msgpack - NOT found")
ENDIF(msgpack_FOUND)


MESSAGE(STATUS "${CMAKE_CURRENT_LIST_DIR}")
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_LIST_DIR}../../libraries" "${CMAKE_CURRENT_LIST_DIR}../libraries")
# Include sub-projects.
add_subdirectory ("SDALib-CPP")
add_subdirectory ("SDALibTests")
