project(SDALib-Python)
include(../cmake/SDAMacros.cmake)

set(Boost_USE_STATIC_LIBS ON) 

IF(CMAKE_BUILD_TYPE MATCHES Debug)
    set(Boost_USE_DEBUG_LIBS ON)
    set(Boost_USE_RELEASE_LIBS OFF)
ELSE(CMAKE_BUILD_TYPE MATCHES Debug)
    set(Boost_USE_DEBUG_LIBS OFF)
    set(Boost_USE_RELEASE_LIBS ON)
ENDIF(CMAKE_BUILD_TYPE MATCHES Debug)

FIND_PACKAGE(Python3 COMPONENTS Interpreter Development REQUIRED)
FIND_PACKAGE(Boost COMPONENTS python310 REQUIRED)

add_executable(SDALib-Python "PythonDriver.cpp" "PythonDriver.h")

TARGET_LINK_LIBRARIES(SDALib-Python Boost::python310)

target_link_libraries(
        SDALib-Python
        SDALib
        ${Python3_LIBRARY_DIRS}/python310.lib
)

INSTALL_SDA_PACKAGES(SDALib-Python)

target_include_directories(SDALib-Python PUBLIC "../SDALib-CPP")

target_include_directories(SDALib-Python PUBLIC ${IPCLIB_INCLUDE_DIR} ${Python3_INCLUDE_DIRS})

add_custom_command(TARGET SDALib-Python POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/Driver.py" $<TARGET_FILE_DIR:SDALib-Python>
    COMMAND_EXPAND_LISTS
)

add_custom_command(TARGET SDALib-Python POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/SDATypes.py" $<TARGET_FILE_DIR:SDALib-Python>
    COMMAND_EXPAND_LISTS
)