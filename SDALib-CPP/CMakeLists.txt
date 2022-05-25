# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
INCLUDE(cmake/SDAMacros.cmake)
cmake_minimum_required (VERSION 3.8)

IF(WIN32)
	find_program(OPEN_CPP_COVERAGE "OpenCppCoverage")
	if(OPEN_CPP_COVERAGE)
		MESSAGE(STATUS "OpenCppCoverage -- found")
	else(OPEN_CPP_COVERAGE)
		MESSAGE(STATUS "OpenCppCoverage -- not found")
	endif(OPEN_CPP_COVERAGE)
ELSEIF(UNIX)
	IF(CMAKE_BUILD_TYPE MATCHES "Debug")
		find_program(GCOVR "gcovr")
		IF(GCOVR)
			MESSAGE(STATUS "gcovr -- found")
			SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -g -O0")
			SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage -g -O0")
			#SET(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)
		ELSE(GCOVR)
			MESSAGE(STATUS "gcovr -- not found")
		ENDIF(GCOVR)
	ENDIF(CMAKE_BUILD_TYPE MATCHES "Debug")
ELSE(WIN32)
	MESSAGE(ERROR "NOT A SUPPORTED PLATFORM")
ENDIF(WIN32)

set(INSTALL_GTEST OFF)
SET(INTERNAL_SDA_PROJECT TRUE)

project("SDALib")

ENABLE_TESTING()

CMAKE_POLICY(SET CMP0012 NEW)
CMAKE_POLICY(SET CMP0074 NEW)
cmake_policy(SET CMP0079 NEW)

list(APPEND CMAKE_PREFIX_PATH "/libraries" "${CMAKE_CURRENT_LIST_DIR}../../libraries" "${CMAKE_CURRENT_LIST_DIR}../libraries")

SET(ENV{SOLID_DIR} "${CMAKE_SOURCE_DIR}/../libraries")

FIND_SDA_PACKAGES()

# Include sub-projects.
add_subdirectory ("SDALib-CPP")
add_subdirectory ("SDALibTests")
add_subdirectory ("Samples")
add_subdirectory ("SpeedDreams")
add_subdirectory ("SDAReplay")

option(OPTION_SDALIB_PYTHON "installs the python package" FALSE)
IF(OPTION_SDALIB_PYTHON)
	add_subdirectory ("SDALib-Python")
ENDIF(OPTION_SDALIB_PYTHON)

