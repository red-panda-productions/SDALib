# This program has been developed by students from the bachelor Computer Science at
# Utrecht University within the Software Project course.
# © Copyright Utrecht University (Department of Information and Computing Sciences)

SET(SDALIB_HEADERS
	SDADriver.hpp
	SDAAction.hpp
	SDAData.hpp
	SDASpeedDreams.hpp
	IPCPointerManager.h
        SDA_portability.h)

SET(SDALIB_INLINES
	SDADriver.inl
)

SET(SDALIB_SOURCES
	SDADriver.cpp
	SDASpeedDreams.cpp
	IPCPointerManager.cpp
	SDAAction.cpp
	SDAData.cpp
)

ADD_LIBRARY (SDALib STATIC ${SDALIB_HEADERS} ${SDALIB_INLINES} ${SDALIB_SOURCES})

include(GenerateExportHeader)
GENERATE_EXPORT_HEADER(SDALib)

TARGET_LINK_LIBRARIES(SDALib ${IPCLIB_LIBRARY} msgpackc-cxx math SpeedDreamsLib simu)

TARGET_INCLUDE_DIRECTORIES(SDALib PUBLIC ${IPCLIB_INCLUDE_DIR} ${Boost_INCLUDE_DIR} ${CMAKE_BINARY_DIR}/SDALib-CPP ${CMAKE_SOURCE_DIR}/SpeedDreams/math ${CMAKE_SOURCE_DIR}/SpeedDreams ${CMAKE_SOURCE_DIR}/SpeedDreams/simuv4) 

set_target_properties(SDALib PROPERTIES LINKER_LANGUAGE CXX)

INSTALL(TARGETS SDALib DESTINATION lib${SDA_LIB_PREFIX_INSTALL_DIR})
INSTALL(FILES ${SDALIB_HEADERS} ${CMAKE_BINARY_DIR}/SDALib-CPP/sdalib_export.h DESTINATION include)

FILE(GLOB SDA_MACROS ${CMAKE_SOURCE_DIR}/cmake/*.cmake)
INSTALL(FILES ${SDA_MACROS} DESTINATION SDAMacros)

INSTALL(DIRECTORY ${IPCLIB_BIN_DIR}/ DESTINATION bin)

INSTALL(DIRECTORY ${IPCLIB_INCLUDE_DIR}/ DESTINATION include)

INSTALL(DIRECTORY ${IPCLIB_LIB_DIR}/ DESTINATION lib)