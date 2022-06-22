# This program has been developed by students from the bachelor Computer Science at
# Utrecht University within the Software Project course.
# © Copyright Utrecht University (Department of Information and Computing Sciences)

project(SDABrakeSample)

include(../../cmake/SDAMacros.cmake)

add_executable( SDABrakeSample
        "BrakeSampleAction.h"
        "BrakeSampleDriver.h"
        "SDABrakeSample.cpp")

target_link_libraries(
        SDABrakeSample
        SDALib
)

IF(UNIX)
    TARGET_LINK_LIBRARIES(SDABrakeSample rt)
ENDIF(UNIX)

target_include_directories(SDABrakeSample PUBLIC "../../SDALib-CPP")

INSTALL_SDA_PACKAGES(SDABrakeSample)
INSTALL_SDA_DLLS(SDABrakeSample)

