
SET(_SOURCES 
	SDADriver.cpp)

SET(_HEADERS
	SDADriver.h)


add_library (SDALib "SDADriver.h" "SDADriver.cpp" "AIData.h")


find_package(IPCLIB HINTS "/libraries")
find_library(IPCLIB_LIBRARY HINTS ${IPCLIB_LIB_DIR} NAMES IPCLib)

IF(IPCLIB_FOUND)
	SET(HAVE_IPCLIB 1)
	MESSAGE(STATUS "Looking for library IPCLib - found")
ELSE(IPCLIB_FOUND)
	MESSAGE(STATUS "Looking for library IPCLib - NOT found")
ENDIF(IPCLIB_FOUND)


IF(IPCLIB_FOUND)
	TARGET_LINK_LIBRARIES(SDALib ${IPCLIB_LIBRARY})
ELSE(IPCLIB_FOUND)
	MESSAGE(FATAL_ERROR "Cannot find IPCLib libraries")
ENDIF(IPCLIB_FOUND)

target_link_libraries(SDALib IPCLib)
target_include_directories(SDALib PUBLIC ${IPCLIB_INCLUDE_DIR}) 
