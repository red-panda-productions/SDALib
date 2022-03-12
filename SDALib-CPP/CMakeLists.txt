# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#

cmake_minimum_required (VERSION 3.8)

CMAKE_POLICY(SET CMP0012 NEW)
CMAKE_POLICY(SET CMP0074 NEW)
cmake_policy(SET CMP0079 NEW)

project("SDALib")

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_LIST_DIR}../../libraries")
# Include sub-projects.
add_subdirectory ("SDALib-CPP")
