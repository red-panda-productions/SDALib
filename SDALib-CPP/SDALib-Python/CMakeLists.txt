project(SDALib-Python)
include(../cmake/SDAMacros.cmake)

FIND_PACKAGE(Python3 COMPONENTS Interpreter Development)

add_executable(SDALib-Python "PythonDriver.cpp" "PythonDriver.h")

MESSAGE(STATUS ${Python3_LIBRARY_DIRS})

target_link_libraries(
        SDALib-Python
        SDALib
        ${Python3_LIBRARY_DIRS}/python310.lib
)

IF(CMAKE_BUILD_TYPE MATCHES Debug)
    target_link_libraries(
        SDALib-Python
        ${IPCLIB_LIB_DIR}/boost_python310-vc143-mt-gd-x32-1_78.lib
    )

    add_custom_command(TARGET SDALib-Python POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${IPCLIB_BIN_DIR}/boost_python310-vc143-mt-gd-x32-1_78.dll" $<TARGET_FILE_DIR:SDALib-Python>
        COMMAND_EXPAND_LISTS
    )
ELSE(CMAKE_BUILD_TYPE MATCHES Debug)
    target_link_libraries(
        SDALib-Python
        ${IPCLIB_LIB_DIR}/boost_python310-vc143-mt-x32-1_78.lib
    )

    add_custom_command(TARGET SDALib-Python POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${IPCLIB_BIN_DIR}/boost_python310-vc143-mt-x32-1_78.dll" $<TARGET_FILE_DIR:SDALib-Python>
        COMMAND_EXPAND_LISTS
    )
ENDIF(CMAKE_BUILD_TYPE MATCHES Debug)


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