project(SDAReplay)

FIND_PACKAGE(Boost COMPONENTS program_options)

add_executable( SDAReplay
        "ReplayAction.h"
        "ReplayData.h"
        "ReplayDriver.h"
        "ReplayMain.cpp")

target_link_libraries(
        SDAReplay
        SDALib
        Boost::program_options
)

IF(UNIX)
    TARGET_LINK_LIBRARIES(SDAReplay rt)
ENDIF(UNIX)

target_include_directories(SDAReplay PUBLIC "../SDALib-CPP")

IF(WIN32)
    add_custom_command(TARGET SDAReplay POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy "${IPCLIB_BIN_DIR}/IPCLib.dll" $<TARGET_FILE_DIR:SDAReplay>
            COMMAND_EXPAND_LISTS
            )
ENDIF(WIN32)

FILE(COPY "${CMAKE_SOURCE_DIR}/ReplayData" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")