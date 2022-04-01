project(SDABrakeSample)

add_executable( SDABrakeSample
        "BrakeSampleAction.h"
        "BrakeSampleData.h"
        "BrakeSampleDriver.h"
        "SDABrakeSample.cpp")

target_link_libraries(
        SDABrakeSample
        SDALib
)
target_include_directories(SDABrakeSample PUBLIC "../../SDALib-CPP")


add_custom_command(TARGET SDABrakeSample POST_BUILD
       COMMAND ${CMAKE_COMMAND} -E copy "${IPCLIB_BIN_DIR}/IPCLib.dll" $<TARGET_FILE_DIR:SDABrakeSample>
       COMMAND_EXPAND_LISTS
)
