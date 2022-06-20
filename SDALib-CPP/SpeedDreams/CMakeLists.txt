# This program has been developed by students from the bachelor Computer Science at
# Utrecht University within the Software Project course.
# © Copyright Utrecht University (Department of Information and Computing Sciences)

SET(_HEADERS SDStructs.hpp SDConstants.hpp)
SET(_SOURCES SDStructs.cpp)

ADD_LIBRARY(SpeedDreamsLib ${_HEADERS} ${_SOURCES})

add_subdirectory ("math")
add_subdirectory ("portability")
add_subdirectory ("tgf")
add_subdirectory ("simuv4")
add_subdirectory ("robottools")

TARGET_LINK_LIBRARIES(SpeedDreamsLib tgf math)
TARGET_INCLUDE_DIRECTORIES(SpeedDreamsLib PUBLIC ${CMAKE_SOURCE_DIR}/SpeedDreams/tgf)
TARGET_INCLUDE_DIRECTORIES(SpeedDreamsLib PUBLIC ${CMAKE_SOURCE_DIR}/SpeedDreams/math)


INSTALL(TARGETS SpeedDreamsLib DESTINATION lib${SDA_LIB_PREFIX_INSTALL_DIR})
INSTALL(FILES ${_HEADERS} DESTINATION include)