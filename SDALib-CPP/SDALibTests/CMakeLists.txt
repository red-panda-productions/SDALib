project(SDATests)

set(INSTALL_GTEST OFF)
FIND_PACKAGE(Python3 COMPONENTS Interpreter Development REQUIRED)
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
INCLUDE(../cmake/SDAMacros.cmake)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(
  SDATests "SDAActionTest.cpp" "Utils.h" "SDADriverTest.cpp" "PythonDriverTest.cpp" "Mocks/SDADriverMock.h")

IF(WIN32)
    SET(SDA_PYTHON_EXE ${CMAKE_BINARYDIR}/SDALib-Python/SDALib-Python.exe)
ELSE(WIN32)
    SET(SDA_PYTHON_EXE ${CMAKE_BINARYDIR}/SDALib-Python/SDALib-Python)
ENDIF(WIN32)

target_link_libraries(
  SDATests
  SDALib
  SDALib-Pythonlib
  Python3::Python
  gtest_main
  ${SOLID_LIBRARY}
)

target_include_directories(SDATests PUBLIC "../SDALib-CPP")
target_include_directories(SDATests PUBLIC "../SDALib-Python")
target_include_directories(SDATests PUBLIC ${IPCLIB_INCLUDE_DIR} ${Python3_INCLUDE_DIRS})
#add_custom_command(TARGET SDATests POST_BUILD
#       COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:SDATests> $<TARGET_FILE_DIR:SDATests>
#       COMMAND_EXPAND_LISTS
#)

INSTALL_SDA_PACKAGES(SDATests)

include(GoogleTest)
gtest_discover_tests(SDATests DISCOVERY_TIMEOUT 600)

SET(COVERAGE_OUTPUT_FOLDER ${CMAKE_SOURCE_DIR} CACHE STRING "The output folder of the coverage bat files")

FIND_PROGRAM(OPEN_CPP_COVERAGE "OpenCppCoverage")

IF(OPEN_CPP_COVERAGE)
	STRING(REGEX REPLACE "/" "\\\\" SOURCEDIR ${CMAKE_SOURCE_DIR})
    STRING(REGEX REPLACE "/" "\\\\" CURRENTSOURCEDIR ${CMAKE_SOURCE_DIR}/SDALib-CPP)
    STRING(REGEX REPLACE "/" "\\\\" CURRENTBINARYDIR ${CMAKE_CURRENT_BINARY_DIR})
    STRING(REGEX REPLACE "/" "\\\\" BINARYDIR ${CMAKE_BINARY_DIR})
    STRING(REGEX REPLACE "/" "\\\\" EXEPATH ${CMAKE_CURRENT_BINARY_DIR}/SDATests.exe)

    IF(OPTION_GITHUB_ACTIONS)
        MESSAGE(STATUS "Creating Github actions coverage file")
        STRING(CONCAT CURRENTBINARYDIR "${CURRENTBINARYDIR}" "\\Debug")
        SET(EXEPATH "${CURRENTBINARYDIR}\\SDATests.exe")
        SET(COV_EXPORT_TYPE "cobertura:${SOURCEDIR}\\coverage.xml")
        SET(COV_EXTRA_COMMANDS "")
    ELSE(OPTION_GITHUB_ACTIONS)
        SET(COV_EXPORT_TYPE "html:${CURRENTBINARYDIR}\\coverage")
        SET(COV_EXTRA_COMMANDS "@start ${CURRENTBINARYDIR}\\coverage\\index.html")
    ENDIF(OPTION_GITHUB_ACTIONS)

    FILE(GENERATE OUTPUT ${COVERAGE_OUTPUT_FOLDER}/coverage.bat
        CONTENT 
            "@echo off
            cd /d ${CURRENTBINARYDIR}
            \"${OPEN_CPP_COVERAGE}\" --sources=${CURRENTSOURCEDIR} --export_type=${COV_EXPORT_TYPE} --excluded_sources  --excluded_line_regex  -- ${EXEPATH}
            ${COV_EXTRA_COMMANDS}")

    MESSAGE(STATUS "Created SDACoverage.bat, run with ./coverage.bat in ${COVERAGE_OUTPUT_FOLDER}")
ENDIF(OPEN_CPP_COVERAGE)