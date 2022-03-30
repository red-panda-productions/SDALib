SET(_HEADERS constants.h linalg_t.h straight2_t.h  v2_t.h v3_t.h  v4_t.h
             point3d.h SGCMath.h SGGeoc.h SGGeod.h sg_geodesy.h SGGeodesy.h
             SGLimits.h SGMath.h SGMathFwd.h SGMatrix.h SGMisc.h
             SGQuat.h sg_random.h sg_types.h SGVec3.h SGVec4.h vector.h)

SET(_SOURCES sg_geodesy.cpp SGGeodesy.cpp vector.cpp)

# Fictive = no-output target, for having source files available in IDEs.
add_library(math STATIC ${_SOURCES} ${_HEADERS})

TARGET_INCLUDE_DIRECTORIES(math PUBLIC ${PLIB_INCLUDE_DIR})

TARGET_LINK_LIBRARIES(math ${PLIB_UL_LIBRARY} ${PLIB_SL_LIBRARY} ${PLIB_SG_LIBRARY} ${PLIB_SGG_LIBRARY} ${PLIB_SGGAUX_LIBRARY} ${PLIB_JS_LIBRARY})