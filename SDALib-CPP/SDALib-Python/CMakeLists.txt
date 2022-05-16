project(SDALib-Python)
include(../cmake/SDAMacros.cmake)

FIND_PACKAGE(Python3 COMPONENTS Interpreter Development REQUIRED)
FIND_PACKAGE(Boost COMPONENTS python REQUIRED)

add_executable(SDALib-Python "PythonDriver.cpp" "PythonDriver.h")

target_link_libraries(
        SDALib-Python
        SDALib
        Boost::python
)

target_include_directories(SDALib-Python PUBLIC "../SDALib-CPP")

target_include_directories(SDALib-Python PUBLIC ${IPCLIB_INCLUDE_DIR} ${Python3_INCLUDE_DIRS})

INSTALL_SDA_PACKAGES(SDALib-Python)

add_custom_command(TARGET SDALib-Python POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/Driver.py" $<TARGET_FILE_DIR:SDALib-Python>
    COMMAND_EXPAND_LISTS
)

add_custom_command(TARGET SDALib-Python POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/SDATypes.py" $<TARGET_FILE_DIR:SDALib-Python>
    COMMAND_EXPAND_LISTS
)