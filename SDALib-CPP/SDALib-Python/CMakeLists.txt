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

add_custom_command(TARGET SDALib-Python POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/Driver.py" $<TARGET_FILE_DIR:SDALib-Python>
    COMMAND_EXPAND_LISTS
)

add_custom_command(TARGET SDALib-Python POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/SDATypes.py" $<TARGET_FILE_DIR:SDALib-Python>
    COMMAND_EXPAND_LISTS
)

add_custom_command(TARGET SDALib-Pythonlib POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/Driver.py" $<TARGET_FILE_DIR:SDALib-Python>
    COMMAND_EXPAND_LISTS
)

add_custom_command(TARGET SDALib-Pythonlib POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/SDATypes.py" $<TARGET_FILE_DIR:SDALib-Python>
    COMMAND_EXPAND_LISTS
)