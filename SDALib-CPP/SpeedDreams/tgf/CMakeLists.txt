SET(TGF_HEADERS os.h tgf.h modinfo.h osspec.h memmanager.h)
SET(TGF_SOURCES  directory.cpp file.cpp filesetup.cpp 
                formula.cpp hash.cpp  legacymodule.cpp  os.cpp
                params.cpp profiler.cpp schedulespy.cpp tgf.cpp trace.cpp memmanager.cpp)
SET(TGF_OTHER_SOURCES params.dtd)

IF(WIN32)
    SET(TGF_HEADERS ${TGF_HEADERS} windowsspec.h)
    SET(TGF_SOURCES ${TGF_SOURCES} windowsspec.cpp)
ELSE(WIN32)
    SET(TGF_HEADERS ${TGF_HEADERS} linuxspec.h)
    SET(TGF_SOURCES ${TGF_SOURCES} linuxspec.cpp)
ENDIF(WIN32)


ADD_LIBRARY(tgf ${TGF_HEADERS} ${TGF_SOURCES} ${TGF_OTHER_SOURCES})

TARGET_LINK_LIBRARIES(tgf portability ${SOLID_LIBRARY} ${EXPAT_LIBRARY})

TARGET_INCLUDE_DIRECTORIES(tgf PUBLIC ${CMAKE_SOURCE_DIR}/SpeedDreams/portability ${EXPAT_INCLUDE_DIR})