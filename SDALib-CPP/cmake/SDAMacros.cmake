
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

    IF(UNIX)
        SET(Boost_USE_STATIC_LIBS ON)
    ENDIF(UNIX)

	FIND_PACKAGE(Boost COMPONENTS thread chrono)

	IF(Boost_FOUND)
		MESSAGE(STATUS "Looking for library Boost - found")
	ELSE(Boost_FOUND)
		MESSAGE(FATAL_ERROR "Looking for library Boost - NOT found")
	ENDIF(Boost_FOUND)

	FIND_PACKAGE(msgpack)

	IF(msgpack_FOUND)
		MESSAGE(STATUS "Looking for library msgpack - found")
	ELSE(msgpack_FOUND)
		MESSAGE(FATAL_ERROR "Looking for library msgpack - NOT found")
	ENDIF(msgpack_FOUND)

	SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

	FIND_PACKAGE(PLIB REQUIRED)

	IF(WIN32)
		FIND_PACKAGE(SOLID REQUIRED)
	ENDIF(WIN32)

	FIND_PACKAGE(EXPAT REQUIRED)

	

	IF(NOT INTERNAL_SDA_PROJECT)
		FIND_LIBRARY(SD_MATH_LIBRARY NAMES math HINTS ${IPCLIB_LIB_DIR} REQUIRED)
		FIND_LIBRARY(SD_PORTABILITY_LIBRARY NAMES portability HINTS ${IPCLIB_LIB_DIR} REQUIRED)
		FIND_LIBRARY(SD_ROBOTTOOLS_LIBRARY NAMES robottools HINTS ${IPCLIB_LIB_DIR} REQUIRED)
		FIND_LIBRARY(SDALIB_LIBRARY NAMES SDALib HINTS ${IPCLIB_LIB_DIR} REQUIRED)
		FIND_LIBRARY(SD_SIMU_LIBRARY NAMES simu HINTS ${IPCLIB_LIB_DIR} REQUIRED)
		FIND_LIBRARY(SD_SPEEDDREAMS_LIBRARY NAMES SpeedDreamsLib HINTS ${IPCLIB_LIB_DIR} REQUIRED)
		FIND_LIBRARY(SD_TGF_LIBRARY NAMES tgf HINTS ${IPCLIB_LIB_DIR} REQUIRED)
	ENDIF(NOT INTERNAL_SDA_PROJECT)

ENDMACRO(FIND_SDA_PACKAGES)

MACRO(INSTALL_SDA_PACKAGES TARGET_NAME)
	target_include_directories(${TARGET_NAME} PUBLIC ${Boost_INCLUDE_DIR})
	target_include_directories(${TARGET_NAME} PUBLIC ${IPCLIB_INCLUDE_DIR} ${SDALIB_INCLUDE_DIR})

	target_link_libraries(${TARGET_NAME} 
		${IPCLIB_LIBRARY}
		${PLIB_JS_LIBRARY}
		${EXPAT_LIBRARY}
		${PLIB_UL_LIBRARY}
		${PLIB_SL_LIBRARY}
		${PLIB_SG_LIBRARY}
		${PLIB_SSG_LIBRARY}
		${PLIB_SSGAUX_LIBRARY}
		${PLIB_UL_LIBRARY}
		${SOLID_LIBRARY}
	)

	IF(NOT INTERNAL_SDA_PROJECT)
		target_link_libraries(${TARGET_NAME}
			${SD_SIMU_LIBRARY}
			${SD_MATH_LIBRARY}
			${SD_PORTABILITY_LIBRARY}
			${SD_ROBOTTOOLS_LIBRARY}
			${SDALIB_LIBRARY}
			${SD_SPEEDDREAMS_LIBRARY}
			${SD_TGF_LIBRARY}
		)

	ENDIF(NOT INTERNAL_SDA_PROJECT)

	IF(SDA_PACKAGE_INSTALL_DEBUG)
		MESSAGE(STATUS ${IPCLIB_LIBRARY})
		MESSAGE(STATUS ${PLIB_JS_LIBRARY})
		MESSAGE(STATUS ${EXPAT_LIBRARY})
		MESSAGE(STATUS ${PLIB_UL_LIBRARY})
		MESSAGE(STATUS ${PLIB_SL_LIBRARY})
		MESSAGE(STATUS ${PLIB_SSG_LIBRARY})
		MESSAGE(STATUS ${PLIB_SSGAUX_LIBRARY})
		MESSAGE(STATUS ${PLIB_UL_LIBRARY})
		MESSAGE(STATUS ${SOLID_LIBRARY})
		MESSAGE(STATUS ${SD_MATH_LIBRARY})
		MESSAGE(STATUS ${SD_PORTABILITY_LIBRARY})
		MESSAGE(STATUS ${SD_ROBOTTOOLS_LIBRARY})
		MESSAGE(STATUS ${SDALIB_LIBRARY})
		MESSAGE(STATUS ${SD_SIMU_LIBRARY})
		MESSAGE(STATUS ${SD_SPEEDDREAMS_LIBRARY})
		MESSAGE(STATUS ${SD_TGF_LIBRARY})
	ENDIF(SDA_PACKAGE_INSTALL_DEBUG)

	IF(WIN32)
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
	ENDIF(WIN32)
ENDMACRO(INSTALL_SDA_PACKAGES TARGET_NAME)

MACRO(INSTALL_PYTHON_FILES TARGET_NAME)
	add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/SDALib-Python/Driver.py" $<TARGET_FILE_DIR:${TARGET_NAME}>
			COMMAND_EXPAND_LISTS
			)

	add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
			COMMAND echo $<TARGET_FILE_DIR:${TARGET_NAME}>
			COMMAND_EXPAND_LISTS
			)

	add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/SDALib-Python/SDATypes.py" $<TARGET_FILE_DIR:${TARGET_NAME}>
			COMMAND_EXPAND_LISTS
			)

	IF(WIN32)
		FILE(GLOB PYTHON_FILES "${Python3_RUNTIME_LIBRARY_DIRS}/*.dll")
		FOREACH(FILE ${PYTHON_FILES})
			add_custom_command(TARGET ${TARGET_NAME} PRE_LINK
					COMMAND ${CMAKE_COMMAND} -E copy "${FILE}" $<TARGET_FILE_DIR:${TARGET_NAME}>
					COMMAND_EXPAND_LISTS
					)
		ENDFOREACH(FILE ${PYTHON_FILES})
	ENDIF(WIN32)
ENDMACRO(INSTALL_PYTHON_FILES TARGET_NAME)