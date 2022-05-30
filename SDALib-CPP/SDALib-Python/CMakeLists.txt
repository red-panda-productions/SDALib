project(SDALib-Python)
include(../cmake/SDAMacros.cmake)

FIND_PACKAGE(Python3 COMPONENTS Interpreter Development REQUIRED)

add_executable(SDALib-Python "PythonDriver.cpp" "PythonDriver.h")

add_library(SDALib-Pythonlib "PythonDriver.cpp" "PythonDriver.h")

target_compile_definitions(SDALib-Pythonlib PUBLIC "SDA_LIBRARY")

target_link_libraries(
        SDALib-Python
        SDALib
        Python3::Python
)
target_link_libraries(
        SDALib-Pythonlib
        SDALib
        Python3::Python
)

INSTALL_SDA_PACKAGES(SDALib-Python)
INSTALL_SDA_PACKAGES(SDALib-Pythonlib)

target_include_directories(SDALib-Python PUBLIC "../SDALib-CPP")
target_include_directories(SDALib-Pythonlib PUBLIC "../SDALib-CPP")


target_include_directories(SDALib-Python PUBLIC ${IPCLIB_INCLUDE_DIR} ${Python3_INCLUDE_DIRS})
target_include_directories(SDALib-Pythonlib PUBLIC ${IPCLIB_INCLUDE_DIR} ${Python3_INCLUDE_DIRS})

INSTALL_PYTHON_FILES(SDALib-Python)
INSTALL_PYTHON_FILES(SDALib-Pythonlib)