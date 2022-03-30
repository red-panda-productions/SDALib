SET(SIMU_SOURCES aero.cpp atmosphere.cpp axle.cpp brake.cpp car.cpp categories.cpp
                 collide.cpp differential.cpp engine.cpp simu.cpp
                 steer.cpp susp.cpp transmission.cpp wheel.cpp)
SET(SIMU_HEADERS aero.h axle.h brake.h carstruct.h differential.h engine.h
                 sim.h steer.h susp.h transmission.h wheel.h
                 )

ADD_LIBRARY(simu ${SIMU_SOURCES} ${SIMU_HEADERS})

TARGET_LINK_LIBRARIES(simu math tgf portability SpeedDreamsLib robottools)

TARGET_INCLUDE_DIRECTORIES(simu PUBLIC ${CMAKE_SOURCE_DIR}/SpeedDreams/math ${CMAKE_SOURCE_DIR}/SpeedDreams/tgf ${CMAKE_SOURCE_DIR}/SpeedDreams/portability ${CMAKE_SOURCE_DIR}/SpeedDreams/robottools ${CMAKE_SOURCE_DIR}/SpeedDreams)
