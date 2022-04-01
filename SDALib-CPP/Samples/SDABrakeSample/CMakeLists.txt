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
target_include_directories(SDABrakeSample PUBLIC "../../SDALib-CPP")


INSTALL_SDA_PACKAGES(SDABrakeSample)

