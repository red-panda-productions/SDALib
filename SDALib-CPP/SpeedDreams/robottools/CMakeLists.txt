
ADD_LIBRARY(robottools "robottools.h" "rttrack.cpp" "rtutil.cpp")

IF(MSVC)
	# Ignore some run-time libs to avoid link time warnings and sometimes even crashes.
    SET(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} /NODEFAULTLIB:msvcrt.lib")
ENDIF(MSVC)

TARGET_LINK_LIBRARIES(robottools SpeedDreamsLib)
TARGET_INCLUDE_DIRECTORIES(robottools PUBLIC ${CMAKE_SOURCE_DIR}/SpeedDreams)


INSTALL(TARGETS robottools)
INSTALL(FILES "robottools.h" DESTINATION include)