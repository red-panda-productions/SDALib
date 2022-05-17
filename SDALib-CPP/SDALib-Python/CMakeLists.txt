project(SDALib-Python)
include(../cmake/SDAMacros.cmake)

FIND_PACKAGE(Python3 COMPONENTS Interpreter Development REQUIRED)
FIND_PACKAGE(Boost COMPONENTS python REQUIRED)

add_executable(SDALib-Python "PythonDriver.cpp" "PythonDriver.h")

MESSAGE(STATUS ${Python3_LIBRARIES})

target_link_libraries(
        SDALib-Python
#        SDALib
        ${Python3_LIBRARIES}
#        ${Boost_LIBRARIES}
)

MESSAGE(STATUS Debug: ${Boost_PYTHON_LIBRARY_DEBUG}\nRelease: ${Boost_PYTHON_LIBRARY_RELEASE})

IF(CMAKE_BUILD_TYPE MATCHES Debug)
    TARGET_LINK_LIBRARIES(SDALib-Python
            SDALib
            ${Boost_PYTHON_LIBRARY_DEBUG}
            )
ELSE(CMAKE_BUILD_TYPE MATCHES Debug)
    TARGET_LINK_LIBRARIES(SDALib-Python
            SDALib
            ${Boost_PYTHON_LIBRARY_RELEASE}
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