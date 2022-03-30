
ADD_LIBRARY(robottools "robottools.h" "rttrack.cpp" "rtutil.cpp")

TARGET_LINK_LIBRARIES(robottools SpeedDreamsLib)
TARGET_INCLUDE_DIRECTORIES(robottools PUBLIC ${CMAKE_SOURCE_DIR}/SpeedDreams)
