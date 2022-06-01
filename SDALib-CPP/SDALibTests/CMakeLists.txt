project(SDATests)

set(INSTALL_GTEST OFF)

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
  SDATests "GeneratorUtils.h" "GeneratorUtils.cpp" "Random.hpp" "SDAActionTest.cpp" "Utils.h" "SDADriverTest.cpp" "PythonDriverTest.cpp" "Mocks/SDADriverMock.h")

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

IF(GCOVR)
    SET(COV_SPECIFIC_REGEX "|(.*@NOLINUXCOVERAGE.*)")
ENDIF(GCOVR)

IF(OPEN_CPP_COVERAGE)
    SET(COV_SPECIFIC_REGEX "|(.*@NOWINDOWSCOVERAGE.*)")
ENDIF(OPEN_CPP_COVERAGE)

IF(OPEN_CPP_COVERAGE OR GCOVR)
    SET(COV_EXCLUDE_REGEX "\"((.*@NOCOVERAGE.*)${COV_SPECIFIC_REGEX})\"")
ENDIF(OPEN_CPP_COVERAGE OR GCOVR)

INSTALL_PYTHON_FILES(SDATests)

IF(OPEN_CPP_COVERAGE)
	STRING(REGEX REPLACE "/" "\\\\" SOURCEDIR ${CMAKE_SOURCE_DIR})
    STRING(REGEX REPLACE "/" "\\\\" SDADIR ${CMAKE_SOURCE_DIR}/SDALib-CPP)
    STRING(REGEX REPLACE "/" "\\\\" SDAPYTHONDIR ${CMAKE_SOURCE_DIR}/SDALib-Python)
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
            \"${OPEN_CPP_COVERAGE}\" --sources=${SDADIR} --sources=${SDAPYTHONDIR} --export_type=${COV_EXPORT_TYPE} --excluded_sources  --excluded_line_regex=${COV_EXCLUDE_REGEX}  -- ${EXEPATH}
            ${COV_EXTRA_COMMANDS}")

    MESSAGE(STATUS "Created SDACoverage.bat, run with ./coverage.bat in ${COVERAGE_OUTPUT_FOLDER}")
ENDIF(OPEN_CPP_COVERAGE)

IF(GCOVR)

    IF(OPTION_GITHUB_ACTIONS)
        MESSAGE(STATUS "Creating Github actions coverage file")
        SET(COV_EXPORT_TYPE "--cobertura-pretty")
        SET(COV_EXTRA_COMMANDS "sed -i 's/branches-covered=\"[0-9]*\"/branches-covered=\"0\"/g' coverage.xml
                                sed -i 's/branches-valid=\"[0-9]*\"/branches-valid=\"0\"/g' coverage.xml
                                sed -i 's/branch-rate=\"[0-9|.]*\"/branch-rate=\"0\"/g' coverage.xml
                                sed -i 's/branch=\"true\"//g' coverage.xml
                                sed -i 's/branch=\"false\"//g' coverage.xml")
        SET(COV_OUTPUT "${CMAKE_SOURCE_DIR}/coverage.xml")
    ELSE(OPTION_GITHUB_ACTIONS)
        SET(COV_EXPORT_TYPE "--html --html-details")
        SET(COV_EXTRA_COMMANDS "bash -c \"xdg-open ${CMAKE_BINARY_DIR}/coverage.html &\" 2> /dev/null")
        SET(COV_OUTPUT "${CMAKE_BINARY_DIR}/coverage.html")
    ENDIF(OPTION_GITHUB_ACTIONS)

    FILE(GENERATE OUTPUT ${CMAKE_SOURCE_DIR}/coverage.sh
            CONTENT
            " cd ${CMAKE_CURRENT_BINARY_DIR}
            ${CMAKE_CURRENT_BINARY_DIR}/SDATests
            cd ${CMAKE_SOURCE_DIR}
            gcovr --gcov-ignore-parse-errors --exclude=\"(.*IPCPointerManager.cpp.*|.*SDAReplay.*|.*Samples.*|.*SpeedDreams.*|.*googletest.*|.*CMakeFiles.*)\" --exclude-lines-by-pattern=${COV_EXCLUDE_REGEX} ${COV_EXPORT_TYPE} -o ${COV_OUTPUT}
            ${COV_EXTRA_COMMANDS}"
            FILE_PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_WRITE GROUP_READ WORLD_EXECUTE WORLD_WRITE WORLD_READ)

    MESSAGE(STATUS "Created coverage.sh at ${CMAKE_SOURCE_DIR}/coverage.sh")
ENDIF(GCOVR)