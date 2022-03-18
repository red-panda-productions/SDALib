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