# This program has been developed by students from the bachelor Computer Science at
# Utrecht University within the Software Project course.
# © Copyright Utrecht University (Department of Information and Computing Sciences)

FIND_LIBRARY(MYSQL_LIBSSL_LIBRARY
        NAMES libssl ssl
        HINTS ENV MYSQL_DIR
        PATH_SUFFIXES lib64 lib libs64 libs libs/Win32 libs/Win64
        PATHS /usr /usr/local ${SDEXT_CUSTOM_3DPARTY_DIR})

FIND_LIBRARY(MYSQL_LIBCRYPTO_LIBRARY
        NAMES libcrypto crypto
        HINTS ENV MYSQL_DIR
        PATH_SUFFIXES lib64 lib libs64 libs libs/Win32 libs/Win64
        PATHS /usr /usr/local ${SDEXT_CUSTOM_3DPARTY_DIR})

IF(WIN32)
    FIND_PATH(MYSQL_INCLUDE_DIR mysql/jdbc.h
            HINTS ENV MYSQL_DIR
            PATH_SUFFIXES
            Headers include
            PATHS
            /usr/local/mysql/*/
            /usr/local
            /usr
            /sw # Fink
            /opt/local # DarwinPorts
            /opt/csw # Blastwave
            /opt
            /..
            ${SDEXT_CUSTOM_3DPARTY_DIR})
ELSEIF(UNIX)
    FIND_PATH(MYSQL_INCLUDE_DIR mysql_connection.h
            HINTS ENV MYSQL_DIR
            PATH_SUFFIXES
            Headers include
            PATHS
            /usr/local/mysql/*/
            /usr/local
            /usr
            /sw # Fink
            /opt/local # DarwinPorts
            /opt/csw # Blastwave
            /opt
            /..
            ${SDEXT_CUSTOM_3DPARTY_DIR})
ENDIF(WIN32)




SET(MYSQL_SUBDIR "")
IF(CMAKE_BUILD_TYPE MATCHES "Debug")
    SET(MYSQL_SUBDIR "/debug")
ENDIF(CMAKE_BUILD_TYPE MATCHES "Debug")

FIND_LIBRARY(MYSQL_CPPCONN_LIBRARY
        NAMES mysqlcppconn
        HINTS ENV MYSQL_DIR
        PATH_SUFFIXES "lib${MYSQL_SUBDIR}" "lib64${MYSQL_SUBDIR}" "libs64${MYSQL_SUBDIR}" "libs${MYSQL_SUBDIR}" "libs/Win32${MYSQL_SUBDIR}" "libs/Win64${MYSQL_SUBDIR}" "lib/*${MYSQL_SUBDIR}"
        PATHS /usr /usr/local /usr/local/mysql/*/ ${SDEXT_CUSTOM_3DPARTY_DIR})

FIND_LIBRARY(MYSQL_CPPCONN8_LIBRARY
        NAMES mysqlcppconn8
        HINTS ENV MYSQL_DIR
        PATH_SUFFIXES "lib64${MYSQL_SUBDIR}" "lib${MYSQL_SUBDIR}" "libs64${MYSQL_SUBDIR}" "libs${MYSQL_SUBDIR}" "libs/Win32${MYSQL_SUBDIR}" "libs/Win64${MYSQL_SUBDIR}" "lib/*${MYSQL_SUBDIR}"
        PATHS /usr /usr/local /usr/local/mysql/*/ ${SDEXT_CUSTOM_3DPARTY_DIR})

FIND_LIBRARY(MYSQL_CPPCONN_STATIC_LIBRARY
        NAMES mysqlcppconn-static
        HINTS ENV MYSQL_DIR
        PATH_SUFFIXES "lib64${MYSQL_SUBDIR}" "lib${MYSQL_SUBDIR}" "libs64${MYSQL_SUBDIR}" "libs${MYSQL_SUBDIR}" "libs/Win32${MYSQL_SUBDIR}" "libs/Win64${MYSQL_SUBDIR}" "lib/*${MYSQL_SUBDIR}"
        PATHS /usr /usr/local /usr/local/mysql/*/ ${SDEXT_CUSTOM_3DPARTY_DIR})

FIND_LIBRARY(MYSQL_CPPCONN8_STATIC_LIBRARY
        NAMES mysqlcppconn8-static
        HINTS ENV MYSQL_DIR
        PATH_SUFFIXES "lib64${MYSQL_SUBDIR}" "lib${MYSQL_SUBDIR}" "libs64${MYSQL_SUBDIR}" "libs${MYSQL_SUBDIR}" "libs/Win32${MYSQL_SUBDIR}" "libs/Win64${MYSQL_SUBDIR}" "lib/*${MYSQL_SUBDIR}"
        PATHS /usr /usr/local /usr/local/mysql/*/ ${SDEXT_CUSTOM_3DPARTY_DIR})

IF(WIN32)
    GET_FILENAME_COMPONENT(MYSQL_LIB_DIR "${MYSQL_CPPCONN_LIBRARY}" PATH CACHE)
   
    IF(CMAKE_BUILD_TYPE MATCHES "Debug")
        GET_FILENAME_COMPONENT(MYSQL_BASE_DIR ${MYSQL_LIB_DIR} DIRECTORY CACHE)
        STRING(CONCAT MYSQL_BIN_DIR_STRING "${MYSQL_BASE_DIR}" "/bin/debug")
        SET(MYSQL_OTHER_BIN "${MYSQL_BIN_DIR_STRING}/..")
    ELSE(CMAKE_BUILD_TYPE MATCHES "Debug")
        GET_FILENAME_COMPONENT(MYSQL_BASE_DIR ${MYSQL_LIB_DIR} DIRECTORY CACHE)
        STRING(CONCAT MYSQL_BIN_DIR_STRING "${MYSQL_BASE_DIR}" "/bin")
        SET(MYSQL_OTHER_BIN "${MYSQL_BIN_DIR_STRING}")
    ENDIF(CMAKE_BUILD_TYPE MATCHES "Debug")

    FIND_FILE(MYSQL_LIBSSL_DLL
            NAMES "libssl.dll" "ssl.dll" "libssl-1_1.dll"
            HINTS ENV MYSQL_DIR
            PATHS ${MYSQL_OTHER_BIN})

    FIND_FILE(MYSQL_LIBCRYPTO_DLL
            NAMES libcrypto-1_1.dll
            HINTS ENV MYSQL_DIR
            PATHS ${MYSQL_OTHER_BIN})

    FIND_FILE(MYSQL_CPPCONN_DLL
            NAMES mysqlcppconn-9-vs14.dll mysqlcppconn.dll
            HINTS ENV MYSQL_DIR
            PATHS ${MYSQL_BIN_DIR_STRING})

    FIND_FILE(MYSQL_CPPCONN8_DLL
            NAMES mysqlcppconn8-2-vs14.dll mysqlcppconn8.dll mysqlcppconn8-vs14.dll
            HINTS ENV MYSQL_DIR
            PATHS ${MYSQL_BIN_DIR_STRING})

    SET(MYSQL_DLL_FILES "${MYSQL_LIBSSL_DLL};${MYSQL_LIBCRYPTO_DLL};${MYSQL_CPPCONN_DLL};${MYSQL_CPPCONN8_DLL}")
ENDIF(WIN32)

SET(MYSQL_BIN_DIR ${MYSQL_BIN_DIR_STRING} CACHE STRING "The binary dir for mysql")

SET(MYSQL_FOUND FALSE)
IF(WIN32)
    IF(MYSQL_LIBCRYPTO_LIBRARY
            AND MYSQL_LIBSSL_LIBRARY
            AND MYSQL_CPPCONN_LIBRARY
            AND MYSQL_CPPCONN8_LIBRARY
            AND MYSQL_CPPCONN_STATIC_LIBRARY
            AND MYSQL_CPPCONN8_STATIC_LIBRARY)
        SET(MYSQL_FOUND TRUE)
    ENDIF(MYSQL_LIBCRYPTO_LIBRARY
            AND MYSQL_LIBSSL_LIBRARY
            AND MYSQL_CPPCONN_LIBRARY
            AND MYSQL_CPPCONN8_LIBRARY
            AND MYSQL_CPPCONN_STATIC_LIBRARY
            AND MYSQL_CPPCONN8_STATIC_LIBRARY)
ENDIF(WIN32)

IF(UNIX)
    IF(MYSQL_LIBCRYPTO_LIBRARY
            AND MYSQL_LIBSSL_LIBRARY
            AND MYSQL_CPPCONN_LIBRARY
            AND MYSQL_CPPCONN_STATIC_LIBRARY)
        SET(MYSQL_FOUND TRUE)
    ENDIF(MYSQL_LIBCRYPTO_LIBRARY
            AND MYSQL_LIBSSL_LIBRARY
            AND MYSQL_CPPCONN_LIBRARY
            AND MYSQL_CPPCONN_STATIC_LIBRARY)
ENDIF(UNIX)
