
MACRO(FIND_SDA_PACKAGES)
	CMAKE_POLICY(SET CMP0012 NEW)
	CMAKE_POLICY(SET CMP0074 NEW)
	CMAKE_POLICY(SET CMP0079 NEW)

	FIND_PACKAGE(IPCLIB)
	FIND_LIBRARY(IPCLIB_LIBRARY HINTS ${IPCLIB_LIB_DIR} NAMES IPCLib)

	IF(IPCLIB_FOUND)
		MESSAGE(STATUS "Looking for library IPCLib - found")
	ELSE(IPCLIB_FOUND)
		MESSAGE(FATAL_ERROR "Looking for library IPCLib - NOT found")
	ENDIF(IPCLIB_FOUND)

	SET(Boost_INCLUDE_DIR "${IPCLIB_INCLUDE_DIR}")
	MESSAGE(STATUS "Looking for library Boost - found")

	FIND_PACKAGE(msgpack)

	IF(msgpack_FOUND)
		MESSAGE(STATUS "Looking for library msgpack - found")
	ELSE(msgpack_FOUND)
		MESSAGE(FATAL_ERROR "Looking for library msgpack - NOT found")
	ENDIF(msgpack_FOUND)

	SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

	FIND_PACKAGE(PLIB REQUIRED)

	FIND_PACKAGE(SOLID REQUIRED)

	FIND_PACKAGE(EXPAT REQUIRED)
ENDMACRO(FIND_SDA_PACKAGES)

MACRO(INSTALL_SDA_PACKAGES TARGET_NAME)
	add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
       COMMAND ${CMAKE_COMMAND} -E copy "${IPCLIB_BIN_DIR}/IPCLib.dll" $<TARGET_FILE_DIR:${TARGET_NAME}>
       COMMAND_EXPAND_LISTS
	)

	add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
		   COMMAND ${CMAKE_COMMAND} -E copy "${IPCLIB_BIN_DIR}/solid2.dll" $<TARGET_FILE_DIR:${TARGET_NAME}>
		   COMMAND_EXPAND_LISTS
	)

	add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
		   COMMAND ${CMAKE_COMMAND} -E copy "${IPCLIB_BIN_DIR}/libexpat.dll" $<TARGET_FILE_DIR:${TARGET_NAME}>
		   COMMAND_EXPAND_LISTS
	)
ENDMACRO(INSTALL_SDA_PACKAGES TARGET_NAME)