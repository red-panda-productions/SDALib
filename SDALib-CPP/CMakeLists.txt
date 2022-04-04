# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
INCLUDE(cmake/SDAMacros.cmake)
cmake_minimum_required (VERSION 3.8)

project("SDALib")

ENABLE_TESTING()

CMAKE_POLICY(SET CMP0012 NEW)
CMAKE_POLICY(SET CMP0074 NEW)
cmake_policy(SET CMP0079 NEW)

list(APPEND CMAKE_PREFIX_PATH "/libraries" "${CMAKE_CURRENT_LIST_DIR}../../libraries" "${CMAKE_CURRENT_LIST_DIR}../libraries")

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

