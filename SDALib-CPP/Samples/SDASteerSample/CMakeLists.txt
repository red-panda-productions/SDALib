project(SDASteerSample)

add_executable( SDASteerSample
        "SteerSampleAction.h"
        "SteerSampleData.h"
        "SteerSampleDriver.h"
        "SDASteerSample.cpp")

target_link_libraries(
        SDASteerSample
        SDALib
)
target_include_directories(SDASteerSample PUBLIC "../../SDALib-CPP")


add_custom_command(TARGET SDASteerSample POST_BUILD
       COMMAND ${CMAKE_COMMAND} -E copy "${IPCLIB_BIN_DIR}/IPCLib.dll" $<TARGET_FILE_DIR:SDASteerSample>
       COMMAND_EXPAND_LISTS
)
