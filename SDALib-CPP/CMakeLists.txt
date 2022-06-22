# This program has been developed by students from the bachelor Computer Science at
# Utrecht University within the Software Project course.
# © Copyright Utrecht University (Department of Information and Computing Sciences)

# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
INCLUDE(cmake/SDAMacros.cmake)
cmake_minimum_required (VERSION 3.8)

set(INSTALL_GTEST OFF)
SET(INTERNAL_SDA_PROJECT TRUE)

project("SDALib")

ENABLE_TESTING()

CMAKE_POLICY(SET CMP0012 NEW)
CMAKE_POLICY(SET CMP0074 NEW)
cmake_policy(SET CMP0079 NEW)

IF(WIN32)
    find_program(OPEN_CPP_COVERAGE "OpenCppCoverage")
    if(OPEN_CPP_COVERAGE)
        MESSAGE(STATUS "OpenCppCoverage -- found")
    else(OPEN_CPP_COVERAGE)
        MESSAGE(STATUS "OpenCppCoverage -- not found")
    endif(OPEN_CPP_COVERAGE)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /fp:strict")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /fp:strict")
    SET(PYTHON_CXX_FLAGS ${CMAKE_CXX_FLAGS})
    SET(PYTHON_C_FLAGS ${CMAKE_CXX_FLAGS})
ELSEIF(UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -pthread -msse2 -mfpmath=sse")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -pthread -msse2 -mfpmath=sse")

    IF(CMAKE_BUILD_TYPE MATCHES "Debug")
        find_program(GCOVR "gcovr")
        MESSAGE(STATUS ${GCOVR})
        IF(GCOVR)
            MESSAGE(STATUS "gcovr -- found")
            SET(PYTHON_CXX_FLAGS ${CMAKE_CXX_FLAGS})
            SET(PYTHON_C_FLAGS ${CMAKE_CXX_FLAGS})
            #SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -g -O0")
            #SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage -g -O0")

        ELSE(GCOVR)
            MESSAGE(STATUS "gcovr -- not found")
        ENDIF(GCOVR)
    ENDIF(CMAKE_BUILD_TYPE MATCHES "Debug")
ELSE(WIN32)
    MESSAGE(ERROR "NOT A SUPPORTED PLATFORM")
ENDIF(WIN32)

IF(CMAKE_BUILD_TYPE MATCHES "Debug")
    SET(SDA_LIB_PREFIX_INSTALL_DIR "/debug")
ENDIF(CMAKE_BUILD_TYPE MATCHES "Debug")

list(APPEND CMAKE_PREFIX_PATH "/libraries" "${CMAKE_CURRENT_LIST_DIR}../../libraries" "${CMAKE_CURRENT_LIST_DIR}../libraries")

SET(ENV{SOLID_DIR} "${CMAKE_SOURCE_DIR}/../libraries")

FIND_SDA_PACKAGES()

FIND_PACKAGE(Python3 COMPONENTS Interpreter Development REQUIRED)
# Include sub-projects.
add_subdirectory ("SDALib-CPP")
add_subdirectory ("SDALib-Python")
add_subdirectory ("Samples")
add_subdirectory ("SpeedDreams")
add_subdirectory ("SDAReplay")
add_subdirectory ("SDALibTests")



