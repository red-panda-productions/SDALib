

ADD_LIBRARY(SpeedDreamsLib "SDStructs.hpp" "SDStructs.cpp" "SDConstants.hpp")

add_subdirectory ("math")
add_subdirectory ("portability")
add_subdirectory ("tgf")
add_subdirectory ("simuv4")
add_subdirectory ("robottools")

TARGET_LINK_LIBRARIES(SpeedDreamsLib tgf math)
TARGET_INCLUDE_DIRECTORIES(SpeedDreamsLib PUBLIC ${CMAKE_SOURCE_DIR}/SpeedDreams/tgf)
TARGET_INCLUDE_DIRECTORIES(SpeedDreamsLib PUBLIC ${CMAKE_SOURCE_DIR}/SpeedDreams/math)