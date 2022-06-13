project(SDALib-Python)
include(../cmake/SDAMacros.cmake)


add_executable(SDALib-Python "SDATypesConverter.cpp" "SDATypesConverter.h" "PythonDriver.cpp"
        "PythonDriver.h" "PythonDriver.inl" "SpeedDreamsPython.cpp")

add_library(SDALib-Pythonlib "SDATypesConverter.cpp" "SDATypesConverter.h"  "PythonDriver.cpp"
        "PythonDriver.h" "PythonDriver.inl" "SpeedDreamsPython.cpp")

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
IF(UNIX)
    TARGET_LINK_LIBRARIES(SDALib-Python rt)
    TARGET_LINK_LIBRARIES(SDALib-Pythonlib rt)
ENDIF(UNIX)

INSTALL_PYTHON_SIMULATION(SDALib-Python)


