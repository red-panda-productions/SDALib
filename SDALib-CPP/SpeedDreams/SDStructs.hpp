/*
 * This program has been developed by students from the bachelor Computer Science at
 * Utrecht University within the Software Project course.
 * © Copyright Utrecht University (Department of Information and Computing Sciences)
 */

#pragma once

/***************************************************************************

    file                 : car.h
    created              : Sun Jan 30 12:00:15 CET 2000
    copyright            : (C) 2000 by Eric Espie
    email                : torcs@free.fr
    version              : $Id: car.h 7166 2020-08-09 23:56:00Z kakukri $

 ***************************************************************************/

 /***************************************************************************
  *                                                                         *
  *   This program is free software; you can redistribute it and/or modify  *
  *   it under the terms of the GNU General Public License as published by  *
  *   the Free Software Foundation; either version 2 of the License, or     *
  *   (at your option) any later version.                                   *
  *                                                                         *
  ***************************************************************************/

  /** @file
                  This is the car structure.
      @author	<a href=mailto:torcs@free.fr>Eric Espie</a>
      @version	$Id: car.h 7166 2020-08-09 23:56:00Z kakukri $
      @ingroup	carstruct
      @note	Short cuts are to be used with the carElt structure.
  */

  /***************************************************************************

      file                 : track.h
      created              : Sun Jan 30 23:00:06 CET 2000
      copyright            : (C) 2000 by Eric Espie
      email                : torcs@free.fr
      version              : $Id: track.h 7165 2020-08-06 17:12:38Z torcs-ng $

   ***************************************************************************/

   /***************************************************************************
    *                                                                         *
    *   This program is free software; you can redistribute it and/or modify  *
    *   it under the terms of the GNU General Public License as published by  *
    *   the Free Software Foundation; either version 2 of the License, or     *
    *   (at your option) any later version.                                   *
    *                                                                         *
    ***************************************************************************/

    /** @file
                This is the track structure.
        @author	<a href=mailto:torcs@free.fr>Eric Espie</a>
        @version	$Id: track.h 7165 2020-08-06 17:12:38Z torcs-ng $
        @ingroup	trackstruct
    */

#include "tgf.h"

#define GF_TAILQ_HEAD(name, type)					\
typedef struct name {							\
	type *tqh_first;	/* first element */			\
	type **tqh_last;	/* addr of last next element */		\
} t ## name

/** Entry in structure
    @ingroup tailq */
#define GF_TAILQ_ENTRY(type)						\
struct {								\
	type *tqe_next;	/* next element */				\
	type **tqe_prev;	/* address of previous next element */	\
}

/***************************************************************************

    file                 : raceman.h
    created              : Sun Jan 30 22:59:17 CET 2000
    copyright            : (C) 2000,2002 by Eric Espie
    email                : torcs@free.fr
    version              : $Id: raceman.h 6839 2020-02-19 22:05:37Z torcs-ng $

 ***************************************************************************/

 /***************************************************************************
  *                                                                         *
  *   This program is free software; you can redistribute it and/or modify  *
  *   it under the terms of the GNU General Public License as published by  *
  *   the Free Software Foundation; either version 2 of the License, or     *
  *   (at your option) any later version.                                   *
  *                                                                         *
  ***************************************************************************/

  /** @file
                  This is the race information structures.
      @author	<a href=mailto:torcs@free.fr>Eric Espie</a>
      @version	$Id: raceman.h 6839 2020-02-19 22:05:37Z torcs-ng $
      @ingroup	raceinfo
  */

#ifndef _TRACKV1_H_
#define _TRACKV1_H_

#include <linalg_t.h>


#define TRK_IDENT	0	/* from 0x01 to 0xFF */

    /* Parameters strings for track files */

#define TRK_SECT_HDR                "Header"

#define TRK_ATT_WIDTH               "width"
#define TRK_ATT_SURF                "surface"
#define TRK_ATT_NAME                "name"
#define TRK_ATT_VERSION             "version"
#define TRK_ATT_AUTHOR              "author"
#define TRK_ATT_DESCR               "description"
#define TRK_ATT_CAT                 "category"
#define TRK_ATT_SUBCAT              "subcategory"
// SIMULATED DRIVING ASSISTANCE: added the speed limit parameter
#define TRK_ATT_AVERAGE_SPEEDLIMIT  "average speed limit"

#define TRK_ATT_NONE                "none"
#define TRK_ATT_SHORT               "short"
#define TRK_ATT_LONG                "long"

#define TRK_SECT_LOCAL              "Local Info"

#define TRK_ATT_STATION             "station"
#define TRK_ATT_TIMEZONE            "timezone"
#define TRK_ATT_ANYRAINLKHD         "overall rain likelyhood"
#define TRK_ATT_LITTLERAINLKHD      "little rain likelyhood"
#define TRK_ATT_MEDIUMRAINLKHD      "medium rain likelyhood"
#define TRK_ATT_TIMEOFDAY           "time of day"
#define TRK_ATT_SUN_ASCENSION       "sun ascension"
#define TRK_ATT_ALTITUDE            "altitude"

#define TRK_SECT_GRAPH              "Graphic"

#define TRK_ATT_3DDESC              "3d description"

#define TRK_ATT_BKGRND              "background image"
#define TRK_ATT_BGTYPE              "background type"
#define TRK_ATT_BGCLR_R             "background color R"
#define TRK_ATT_BGCLR_G             "background color G"
#define TRK_ATT_BGCLR_B             "background color B"

#define TRK_SECT_TRACKLIGHTS        "Track Lights"
#define TRK_ATT_ROLE                "role"
#define TRK_SECT_TOPLEFT            "topleft"
#define TRK_SECT_BOTTOMRIGHT        "bottomright"
#define TRK_ATT_X                   "x"
#define TRK_ATT_Y                   "y"
#define TRK_ATT_Z                   "z"
#define TRK_ATT_TEXTURE_ON          "texture on"
#define TRK_ATT_TEXTURE_OFF         "texture off"
#define TRK_ATT_INDEX               "index"
#define TRK_ATT_RED                 "red"
#define TRK_ATT_GREEN               "green"
#define TRK_ATT_BLUE                "blue"

#define TRK_LST_ENV                 "Environment Mapping"
#define TRK_ATT_ENVNAME             "env map image"

#define TRK_ATT_SPEC_R              "specular color R"
#define TRK_ATT_SPEC_G              "specular color G"
#define TRK_ATT_SPEC_B              "specular color B"

#define TRK_ATT_AMBIENT_R           "ambient color R"
#define TRK_ATT_AMBIENT_G           "ambient color G"
#define TRK_ATT_AMBIENT_B           "ambient color B"

#define TRK_ATT_DIFFUSE_R           "diffuse color R"
#define TRK_ATT_DIFFUSE_G           "diffuse color G"
#define TRK_ATT_DIFFUSE_B           "diffuse color B"

#define TRK_ATT_SHIN                "shininess"

#define TRK_ATT_LIPOS_X             "light position x"
#define TRK_ATT_LIPOS_Y             "light position y"
#define TRK_ATT_LIPOS_Z             "light position z"

#define TRK_ATT_FOVFACT             "fov factor"

#define TRK_SECT_TURNMARKS          "Graphic/Turn Marks"
#define TRK_ATT_HSPACE              "horizontal space"
#define TRK_ATT_VSPACE              "vertical space"

#define TRK_SECT_TERRAIN            "Graphic/Terrain Generation"

#define TRK_ATT_TSTEP               "track step"
#define TRK_ATT_BMARGIN             "border margin"
#define TRK_ATT_BSTEP               "border step"
#define TRK_ATT_ORIENT              "orientation"
#define TRK_VAL_CLOCK               "clockwise"
#define TRK_VAL_CCLOCK              "counter-clockwise"
#define TRK_ATT_SURFRAND            "texture randomness"
#define TRK_ATT_RELIEF              "relief file"
#define TRK_ATT_ELEVATION           "elevation map"
#define TRK_ATT_ALT_MAX             "maximum altitude"
#define TRK_ATT_ALT_MIN             "minimum altitude"
#define TRK_ATT_GRPSZ               "group size"
#define TRK_ATT_OBJMAP              "object map"
#define TRK_ATT_COLOR               "color"
#define TRK_ATT_OBJECT              "object"
#define TRK_ATT_SEED                "random seed"
#define TRK_ATT_ORIENTATION_TYPE    "orientation type"
#define TRK_ATT_ORIENTATION         "orientation"
#define TRK_ATT_DH                  "delta height"
#define TRK_ATT_DV                  "delta vert"
#define TRK_SECT_OBJMAP             "Object Maps"
#define TRK_SECT_SURFACES           "Surfaces"
#define TRK_SECT_OBJECTS            "Objects"

#define TRK_ATT_FRICTION            "friction"
#define TRK_ATT_ROLLRES             "rolling resistance"
#define TRK_ATT_ROUGHT              "roughness"
#define TRK_ATT_ROUGHTWL            "roughness wavelength"
#define TRK_ATT_DAMMAGE             "dammage"
#define TRK_ATT_REBOUND             "rebound"
#define TRK_ATT_TEXTURE             "texture name"
#define TRK_ATT_BUMPNAME            "bump name"
#define TRK_ATT_TEXTYPE             "texture type"
#define TRK_ATT_TEXLINK             "texture link with previous"
#define TRK_ATT_TEXSIZE             "texture size"
#define TRK_ATT_BUMPSIZE            "bump size"
#define TRK_ATT_TEXSTARTBOUNDARY    "texture start on boundary"
#define TRK_ATT_TEXMIPMAP           "texture mipmap"

#define TRK_ATT_R1                  "color R1"
#define TRK_ATT_G1                  "color G1"
#define TRK_ATT_B1                  "color B1"

#define TRK_ATT_R2                  "color R2"
#define TRK_ATT_G2                  "color G2"
#define TRK_ATT_B2                  "color B2"

#define TRK_ATT_STEPLEN             "steps length"

#define TRK_VAL_YES                 "yes"
#define TRK_VAL_NO                  "no"

#define TRK_SECT_MAIN               "Main Track"
#define TRK_LST_SEGMENTS            "Track Segments"
#define TRK_ATT_TYPE                "type"

#define TRK_VAL_STR                 "str"
#define TRK_VAL_LFT                 "lft"
#define TRK_VAL_RGT                 "rgt"

#define TRK_ATT_LG                  "lg"
#define TRK_ATT_RADIUS              "radius"
#define TRK_ATT_RADIUSEND           "end radius"
#define TRK_ATT_ARC                 "arc"
#define TRK_ATT_ZS                  "z start"
#define TRK_ATT_ZE                  "z end"
#define TRK_ATT_ZSL                 "z start left"
#define TRK_ATT_ZSR                 "z start right"
#define TRK_ATT_ZEL                 "z end left"
#define TRK_ATT_ZER                 "z end right"
#define TRK_ATT_GRADE               "grade"
#define TRK_ATT_BKS                 "banking start"
#define TRK_ATT_BKE                 "banking end"
#define TRK_ATT_ID                  "id"
#define TRK_ATT_PROFIL              "profil"
#define TRK_ATT_PROFSTEPS           "profil steps"
#define TRK_ATT_PROFSTEPSLEN        "profil steps length"
#define TRK_ATT_PROFTGTS            "profil start tangent"
#define TRK_ATT_PROFTGTE            "profil end tangent"
#define TRK_ATT_PROFTGTSL           "profil start tangent left"
#define TRK_ATT_PROFTGTEL           "profil end tangent left"
#define TRK_ATT_PROFTGTSR           "profil start tangent right"
#define TRK_ATT_PROFTGTER           "profil end tangent right"
#define TRK_ATT_MARKS               "marks"
#define TRK_ATT_ENVMAP              "env map"
#define TRK_ATT_DOVFACTOR           "DoV factor"

#define TRK_SECT_PITS               "Pits"
#define TRK_ATT_PIT_INDICATOR       "pit indicator"
#define TRK_ATT_MAX_PITS            "max pits"
#define TRK_ATT_BUILDINGS_START     "start buildings"
#define TRK_ATT_BUILDINGS_STOP      "stop buildings"
#define TRK_ATT_PIT_STYLE           "pit style"
#define TRK_ATT_ENTRY               "entry"
#define TRK_ATT_EXIT                "exit"
#define TRK_ATT_START               "start"
#define TRK_ATT_END                 "end"
#define TRK_ATT_SIDE                "side"
#define TRK_ATT_LEN                 "length"
#define TRK_ATT_SPD_LIM             "speed limit"

#define TRK_SECT_LSIDE              "Left Side"
#define TRK_SECT_RSIDE              "Right Side"
#define TRK_SECT_LBORDER            "Left Border"
#define TRK_SECT_RBORDER            "Right Border"
#define TRK_SECT_LBARRIER           "Left Barrier"
#define TRK_SECT_RBARRIER           "Right Barrier"

#define TRK_ATT_SWIDTH              "start width"
#define TRK_ATT_EWIDTH              "end width"
#define TRK_ATT_STYLE               "style"
#define TRK_ATT_HEIGHT              "height"
#define TRK_ATT_BANKTYPE            "banking type"

#define TRK_VAL_PLAN                "plan"
#define TRK_VAL_WALL                "wall"
#define TRK_VAL_CURB                "curb"
#define TRK_VAL_FENCE               "fence"


#define TRK_SECT_CAM                "Cameras"

#define TRK_ATT_SEGMENT             "segment"
#define TRK_ATT_TORIGHT             "to right"
#define TRK_ATT_TOSTART             "to start"
#define TRK_ATT_HEIGHT              "height"
#define TRK_ATT_CAM_FOV             "fov start"
#define TRK_ATT_CAM_FOVE            "fov end"

#define TRK_VAL_ASPHALT             "asphalt"
#define TRK_VAL_CONCRETE            "concrete"
#define TRK_VAL_GRASS               "grass"
#define TRK_VAL_SAND                "sand"
#define TRK_VAL_DIRT                "dirt"
#define TRK_VAL_MUD                 "mud"
#define TRK_VAL_ICE                 "ice"
#define TRK_VAL_SNOW                "snow"
#define TRK_VAL_BARRIER             "barrier"

#define TRK_VAL_LINEAR              "linear"
#define TRK_VAL_SPLINE              "spline"

#define TRK_VAL_TANGENT             "tangent"
#define TRK_VAL_LEVEL               "level"

#define TRK_SECT_SECTORS            "Sectors"
#define TRK_ATT_SECTOR_DFS          "distance from start"


/* Parameters strings for track physics constants */

#define TRK_PHYSICS_FILE            "data/tracks/physics.xml"

#define TRKP_SECT_SURFACES          "Surfaces"
#define TRKP_VAL_FRICTIONWDRATIO    "friction wet dry ratio"


/** road camera */
typedef struct RoadCam
{
    char* name;
    t3Dd		pos;
    struct RoadCam* next;
} tRoadCam;

/** Extended track segment */
typedef struct SegExt
{
    int		nbMarks;	/**< turn marks */
    int* marks;		/**< marks array */
} tSegExt;


/** Surface */
typedef struct trackSurface
{
    struct trackSurface* next;	/**< Next surface in list */

    const char* material;       /**< Type of material used */

    float kFriction;            /**< Actual mu = coefficient of friction (set at race start) */
    float kFrictionDry;         /**< Coefficient of friction when dry */
    float kRebound;             /**< Coefficient of energy restitution */
    float kRollRes;             /**< Rolling resistance */
    float kRoughness;           /**< Roughtness in m of the surface (wave height) */
    float kRoughWaveLen;        /**< Wave length in m of the surface */
    float kDammage;             /**< Dammages in case of collision */

} tTrackSurface;



/** Barrier */
typedef struct trackBarrier
{
    int			style;          /**< Barrier style */
    float		width;          /**< Barrier width */
    float		height;         /**< Barrier height */
    tTrackSurface* surface;   /**< Barrier surface */
    vec2f normal;               // Normal on the vertical track inside pointing towards the track middle.
} tTrackBarrier;


/** Track segment (tTrackSeg)
    The segments can be straights (type TR_STR): (the track goes from the right to the left)
    @image html straight-desc.png
    Or can be turn segments (type TR_RGT or TR_LFT): (the track goes from the left to the right)
    @image html turn-1-desc.png
    The reference angle is the orientation of the first segment of the track.
    @ingroup trackstruct
*/
typedef struct trackSeg
{
    char* name;                 /**< Segment name */
    int	id;                     /**< Segment number */

    int type;                   /**< Geometrical type:
                                        - TR_RGT
                                        - TR_LFT
                                        - TR_STR
                                */
#define TR_RGT	    1           /**< Right curve */
#define TR_LFT	    2           /**< Left curve */
#define TR_STR	    3           /**< Straight */

    int type2;                  /**< Position type:
                                        - TR_MAIN
                                        - TR_LSIDE
                                        - TR_RSIDE
                                        - TR_LBORDER
                                        - TR_RBORDER
                                */
#define TR_MAIN	    1           /**< Main track segment (ie road part) */
#define TR_LSIDE    2           /**< Left side segment (outer segment) */
#define TR_RSIDE    3           /**< Right side segment (outer segment) */
#define TR_LBORDER  4           /**< Left border segment (inner segment) */
#define TR_RBORDER  5           /**< Right border segment (inner segment) */

    int style;                  /**< Border and barrier segments style:
                                        - TR_PLAN
                                        - TR_CURB
                                        - TR_WALL
                                        - TR_FENCE
                                        - TR_PITBUILDING
                                */
#define TR_PLAN		0           /**< Flat (border only) */
#define TR_CURB		1           /**< Curb (border only) */
#define TR_WALL		2           /**< Wall (barrier only) */
#define TR_FENCE	3           /**< Fence (no width) (barrier only) */
#define TR_PITBUILDING	4       /**< Pit building wall (barrier only) */

    float SpeedLimit;           // SIMULATED DRIVING ASSISTANCE: added a speed limit to the track
    float length;               /**< Length in meters of the middle of the track */
    /// SIMULATED DRIVING ASSISTANCE: added tdble Time
    tdble Time;                 /**< Estimated time in seconds */
    float width;                /**< Width of the segment (if constant width) */
    float startWidth;           /**< Width of the beginning of the segment */
    float endWidth;             /**< Width of the end of the segment */
    float lgfromstart;          /**< Length of begining of segment from starting line */
    float radius;               /**< Radius in meters of the middle of the track (>0) */
    float radiusr;              /**< Radius in meters of the right side of the track (>0) */
    float radiusl;              /**< Radius in meters of the left side of the track (>0) */
    float arc;                  /**< Arc in rad of the curve (>0) */
    t3Dd center;                /**< Center of the curve */
    t3Dd vertex[4];             /**< Coord of the 4 corners of the segment.
                                <br>Index in:
                                        - TR_SL
                                        - TR_SL
                                        - TR_EL
                                        - TR_ER
                                */
#define TR_SL	0               /**< Start-Left corner */
#define TR_SR	1               /**< Start-Right corner */
#define TR_EL	2               /**< End-Left corner */
#define TR_ER	3               /**< End_Right corner */

    float angle[7];             /** Rotation angles of the track in rad anti-clockwise:
                                    <br>Index in:
                                        - TR_ZS
                                        - TR_ZE
                                        - TR_YL
                                        - TR_YR
                                        - TR_XS
                                        - TR_XE
                                        - TR_CS
                                */
#define TR_ZS	0               /**< Z Start angle */
#define TR_ZE	1               /**< Z End angle */
#define TR_YL	2               /**< Y Left angle */
#define TR_YR	3               /**< Y Right angle */
#define TR_XS	4               /**< X Start angle */
#define TR_XE	5               /**< X End angle */
#define TR_CS   6               /**< Center start angle */

                                /* Straight segment directions, precalculated at track load time.
                                 * They are used frequently by rt* functions, so this can
                                 * optimize track handling a bit. */
    float sin;                  /**< = sin(seg->angle[TR_ZS]) */
    float cos;                  /**< = cos(seg->angle[TR_ZS]) */

    /* constants used to find the height of a point */
    float Kzl;                  /* long constant */
    float Kzw;                  /* width constant */
    /* constant used to find the width of a segment */
    float	Kyl;                /* find y along x */
    t3Dd	rgtSideNormal;      /* normal to the right side in case of straight segment */
    int		envIndex;           /* Environment mapping image index */

    float	height;             /**< Max height for curbs */

    unsigned int raceInfo;      /**< Type of segment regarding the race:
                                    <br>Mask value in:
                                        - TR_NORMAL
                                        - TR_LAST
                                        - TR_START
                                        - TR_PITLANE
                                        - TR_SPEEDLIMIT
                                        - TR_PITENTRY
                                        - TR_PITEXIT
                                        - TR_PIT
                                */
#define TR_NORMAL	0x00000000  /**< Normal segment */
#define TR_LAST		0x00000001  /**< Segment before start line */
#define TR_START	0x00000002  /**< Segment after start line */
#define TR_PITLANE	0x00000004  /**< Pit lane segment */
#define TR_SPEEDLIMIT	0x00000008 /**< Segment where the speed is limited */
#define TR_PITENTRY	0x00000010  /**< Segment where the pit lane cross the main track */
#define TR_PITEXIT	0x00000020  /**< Segment where the pit lane cross the main track */
#define TR_PIT		0x00000040  /**< Car pit */
#define TR_PITSTART	0x00000080  /**< Car pit Star */
#define TR_PITEND	0x00000100  /**< Car pit End */
#define TR_PITBUILD 0x00000200  /**< Car pit while buildings */

    float DoVfactor;            /* the factor to use in calculating DoV for this Seg */

    /* pointers */

    /* optionnal extensions */
    tSegExt* ext;

    tTrackSurface* surface;   /**< Segment surface */
    tTrackBarrier* barrier[2]; /**< Segment barriers */
    tRoadCam* cam;       /* current camera */
    struct trackSeg* next;      /**< Next segment */
    struct trackSeg* prev;      /**< Previous segment */

    // Union to avoid code duplication for left/right side cases and to
    // keep compatibility of code. The side definition is so ugly to
    // match the one of the barrier[].
#define TR_SIDE_LFT 1
#define TR_SIDE_RGT 0
    union {
        struct { struct trackSeg* rside, * lside; };
        struct trackSeg* side[2];
    };

} tTrackSeg;

/* selection for local position structure */
#define TR_TORIGHT	0
#define TR_TOMIDDLE	1
#define TR_TOLEFT	2

/** Location on the track in local coordinates */
typedef struct TrkLocPos
{
    tTrackSeg* seg;		/**< Track segment */
    int		type;           /**< Type of description:
                                    - TR_LPOS_MAIN
                                    - TR_LPOS_SEGMENT
                                    - TR_LPOS_TRACK
                            */

#define TR_LPOS_MAIN	0	/**< Relative to the main segment */
#define TR_LPOS_SEGMENT	1	/**< If the point is on a side, relative to this side */
#define TR_LPOS_TRACK	2	/**< Local pos includes all the track width */

    float	toStart;        /**< Distance to start of segment (or arc if turn) */
    float	toRight;        /**< Distance to right side of segment (+ to inside of track - to outside) */
    float	toMiddle;       /**< Distance to middle of segment (+ to left - to right) */
    float	toLeft;         /**< Distance to left side of segment (+ to inside of track - to outside) */
} tTrkLocPos;

struct CarElt;

/** Driver's pit */
typedef struct TrackOwnPit
{
    tTrkLocPos pos;         /**< Center of the pit position */
    int pitCarIndex;        /**< Index of the car in the car array below which occupies the pit. If the pit is free the value is TR_PIT_STATE_FREE */
#define TR_PIT_STATE_FREE	-1
#define TR_PIT_MAXCARPERPIT 4		// maximum cars per pit
    float lmin;             /**< Pitting area length min */
    float lmax;             /**< Pitting area length max */
    int freeCarIndex;       // Index of next free car entry (look at the next line).
    struct CarElt* car[TR_PIT_MAXCARPERPIT];	/**< Car links for pit */
} tTrackOwnPit;

/** Pits Info Structure */
typedef struct tTrackPitInfo
{
    int type;		/**< Type of Pit:
                   - TR_PIT_NONE
                   - TR_PIT_ON_TRACK_SIDE
                   - TR_PIT_ON_SEPARATE_PATH
                   - TR_PIT_NO_BUILDING
                 */
#define TR_PIT_NONE		0           /**< No pits for that tracks */
#define TR_PIT_ON_TRACK_SIDE	1   /**< The pits are on the track side */
#define TR_PIT_ON_SEPARATE_PATH 2   /**< The pit is on a separate pitlane */
#define TR_PIT_NO_BUILDING	3       /**< Draw no building, only low wall, NASCAR style */

    int nMaxPits;                   /**< number max of pits */
    int nPitSeg;                    /**< actual number of pits */
    int side;                       /**< Pits side:
                                            - TR_RGT
                                            - TR_LFT
                                    */
    float len;                      /**< Lenght of each pit stop */
    float width;                    /**< Width of each pit stop */
    float speedLimit;               /**< Speed limit between pitStart and pitEnd */
    tTrackSeg* pitEntry;            /**< Pit lane segment */
    tTrackSeg* pitStart;            /**< Pit lane segment */
    tTrackSeg* pitEnd;              /**< Pit lane segment */
    tTrackSeg* pitExit;             /**< Pit lane segment */
    tTrackOwnPit* driversPits;      /**< List of pits by driver */
    int carsPerPit;
    int driversPitsNb;              /**< Number of drivers */
    int pitindicator;              /**< Pit Indicator 0/1 */
} tTrackPitInfo;

typedef struct TurnMarksInfo
{
    float	height;
    float	width;
    float	hSpace;
    float	vSpace;
} tTurnMarksInfo;

typedef struct GraphicLightInfo
{
    t3Dd topleft;
    t3Dd bottomright;
    char* onTexture;
    char* offTexture;
    int index;
#define GR_TRACKLIGHT_START_RED 1
#define GR_TRACKLIGHT_START_GREEN 2
#define GR_TRACKLIGHT_START_GREENSTART 3
#define GR_TRACKLIGHT_START_YELLOW 4
#define GR_TRACKLIGHT_POST_YELLOW 5
#define GR_TRACKLIGHT_POST_GREEN 6
#define GR_TRACKLIGHT_POST_RED 7
#define GR_TRACKLIGHT_POST_BLUE 8
#define GR_TRACKLIGHT_POST_WHITE 9
#define GR_TRACKLIGHT_PIT_RED 10
#define GR_TRACKLIGHT_PIT_GREEN 11
#define GR_TRACKLIGHT_PIT_BLUE 12
    int role;
    float red;
    float green;
    float blue;
} tGraphicLightInfo;

typedef struct TrackGraphicInfo
{
    const char* model3d;           /**< Name of the track 3D model file (.ac/.acc) */
    const char* background;        /**< Name of the background image file (.png) */
    int			bgtype;
#define TR_BACKGROUND_TYPE_0	0   /**< ??? Anyone who knows what's this */
#define TR_BACKGROUND_TYPE_2	2   /**< ??? Anyone who knows what's this */
#define TR_BACKGROUND_TYPE_4	4   /**< ??? Anyone who knows what's this */
    float		bgColor[3];
    int			envnb;
    const char** env;
    tTurnMarksInfo	turnMarksInfo;
    int			nb_lights;
    tGraphicLightInfo* lights;
} tTrackGraphicInfo;

/** Track local information
    @ingroup trackstruct
*/
typedef struct TrackLocalInfo
{
    /* Constant spec. data, read from <track>.xml */
    const char* station;	/**<Station Weather */
    int	timezone;                   /**< timezone for real timeday with not connection http */
    float anyrainlkhood;            /**< Overall likelyhood of having rain (when random) [0, 1] */
    float littlerainlkhood;         /**< Likelyhood of having little rain when it rains [0, 1] */
    float mediumrainlkhood;         /**< Likelyhood of having medium rain when it rains [0, 1-little] */

    /* Actual data for a race (computed at race start) */
    float timeofday;                /**< Local time of day, in seconds from 0:00 (0 = 0:00, 86400 = 24:00 */
    int   timeofdayindex;
    int   raceday;
    float sunascension;             /**< Local sun "height" (related to the latitude, not to the time of day) */
    float altitude;                 /**< Local altitude track */

    int clouds;	/**< Clouds coverage in the sky  (warning : consistency with RM_VAL_CLOUDS_*) */
#define TR_CLOUDS_NONE   0
#define TR_CLOUDS_CIRRUS 1
#define TR_CLOUDS_FEW    2
#define TR_CLOUDS_MANY   3
#define TR_CLOUDS_CUMULUS 4
#define TR_CLOUDS_SCARCE 5
#define TR_CLOUDS_BROKEN 6
#define TR_CLOUDS_FULL   7
#define TR_CLOUDS_RANDOM 8 // Must not be used in the clouds field. Race engine only.
    double cloud_altitude;

    int clouds2;
    double cloud_altitude2;

    int clouds3;
    double cloud_altitude3;

    int rain;                       /**< Rain strength / strength spec (warning : consistency with RM_VAL_RAIN_*) */
#define TR_RAIN_NONE 0
#define TR_RAIN_LITTLE 1
#define TR_RAIN_MEDIUM 2
#define TR_RAIN_HEAVY  3
#define TR_RAIN_RANDOM 4 // Must not be used in the rain field. Race engine only.

    int water;                      /**< Water "level" on the ground (very simple constant model) */
#define TR_WATER_NONE	0
#define TR_WATER_LITTLE	1
#define TR_WATER_SOME	2
#define TR_WATER_MUCH	3
    int snow;
    int hail;
    float relativehumidity;
    float visibility;
    float airtemperature;
    float airpressure;
    float airdensity;
    float windspeed;
    float dewp;
    float winddir;
    int   config;
} tTrackLocalInfo;

/** Track structure
    @ingroup trackstruct
*/
typedef struct Track
{
    const char* name;        /**< Name of the track */
    const char* descr;       /**< Description of the track */
    const char* authors;     /**< Authors names */
    char* filename;    /**< Filename of the track description */
    void* params;      /**< Parameters handle */
    char* internalname;/**< Internal name of the track */
    const char* category;	/**< Category of the track */
    const char* subcategory; /**< used for the moment for long/short speedway */
    int           nseg;         /**< Number of segments */
    int           version;      /**< Version of the track type */
    float         length;       /**< main track length */
    float         width;        /**< main track width */
    tTrackPitInfo pits;         /**< Pits information */
    tTrackSeg* seg;         /**< Segment list for the main track */
    tTrackSurface* surfaces;	/**< Segment surface list */

    // SIMULATED DRIVING ASSISTANCE: added speedlimit and estimated time members
    tdble SpeedLimit; /**< speed limit of the track */
    tdble Time;       /**< main track time (estimated) */

    t3Dd		min;
    t3Dd		max;
    tTrackGraphicInfo	graphic;

    int		numberOfSectors;	/**< Number of sectors. Every sector is used for calculating split times */
    double* sectors;           /**< sectors[i] is the distance from start where sector i+1 ends and sector i starts */

    tTrackLocalInfo local;      /**< Local info (weather, timeof day ...) */
} tTrack;

/* For Type 3 tracks (now obsolete) */

#define TRK_LST_SURF			"List"
#define TRK_LST_CAM             "list"
#define TRK_ATT_PIT_ENTRY		"pit entry"
#define TRK_ATT_PIT_EXIT		"pit exit"
#define TRK_ATT_PIT_START		"pit start"
#define TRK_ATT_PIT_END			"pit end"
#define TRK_ATT_PIT_TYPE		"pit type"
#define TRK_ATT_PIT_SIDE		"pit side"
#define TRK_ATT_PIT_LEN			"pit length"
#define TRK_ATT_PIT_WIDTH		"pit width"

#define TRK_LST_SEG             "segments"
#define TRK_ATT_ENVIND          "env map index"


#define TRK_ATT_LST             "lside type"
#define TRK_ATT_LSW             "lside width"
#define TRK_ATT_LSWS            "lside start width"
#define TRK_ATT_LSWE            "lside end width"
#define TRK_ATT_LSSURF          "lside surface"
#define TRK_ATT_RST             "rside type"
#define TRK_ATT_RSW             "rside width"
#define TRK_ATT_RSWS            "rside start width"
#define TRK_ATT_RSWE            "rside end width"
#define TRK_ATT_RSSURF          "rside surface"
#define TRK_ATT_LBS             "lborder style"
#define TRK_ATT_LBW             "lborder width"
#define TRK_ATT_LBH             "lborder height"
#define TRK_ATT_LBSURF          "lborder surface"
#define TRK_ATT_RBS             "rborder style"
#define TRK_ATT_RBW             "rborder width"
#define TRK_ATT_RBH             "rborder height"
#define TRK_ATT_RBSURF          "rborder surface"

#define TRK_ATT_BHEIGHT         "border height"

#define TRK_ATT_LBAS            "lbarrier style"
#define TRK_ATT_LBAW            "lbarrier width"
#define TRK_ATT_LBAH            "lbarrier height"
#define TRK_ATT_LBAFRCT         "lbarrier friction"
#define TRK_ATT_LBASURF         "lbarrier surface"
#define TRK_ATT_RBAS            "rbarrier style"
#define TRK_ATT_RBAW            "rbarrier width"
#define TRK_ATT_RBAH            "rbarrier height"
#define TRK_ATT_LBAFRCT         "lbarrier friction"
#define TRK_ATT_RBASURF         "rbarrier surface"

#define TRK_VAL_PIT_TYPE_NONE		"no pit"
#define TRK_VAL_PIT_TYPE_SIDE		"track side"
#define TRK_VAL_PIT_TYPE_SEP_PATH	"seperate path"

#define TRK_ATT_FINISH          "finish segment"

#define TRK_ATT_BORDER_DISTANCE     "border distance"

#endif /* _TRACKV1_H_ */


#ifndef __CARV1_H__
#define __CARV1_H__

#include <plib/sg.h>

#define CAR_IDENT	0

#define MAX_NAME_LEN	32

  /* designation */
#define FRNT_RGT	0	/**< front right */
#define FRNT_LFT	1	/**< front left */
#define REAR_RGT	2	/**< rear right */
#define REAR_LFT	3	/**< rear left */
#define FRNT		0	/**< front */
#define REAR		1	/**< rear */
#define RIGHT		0	/**< right */
#define LEFT		1	/**< left */


/** Wheels Specifications */
typedef struct
{
    float	rimRadius;	        /**< Rim radius */
    float	tireHeight;	        /**< Tire height  */
    float	tireWidth;	        /**< Tire width */
    float	brakeDiskRadius;        /**< Brake disk radius */
    float	wheelRadius;	        /**< Overall wheel radius */
} tWheelSpec;
/* structure access short cuts */
#define _rimRadius(i)		info.wheel[i].rimRadius 	/**< short cut to tWheelSpec#rimRadius */
#define _tireHeight(i)		info.wheel[i].tireHeight	/**< short cut to tWheelSpec#tireHeight */
#define _tireWidth(i)		info.wheel[i].tireWidth		/**< short cut to tWheelSpec#tireWidth */
#define _brakeDiskRadius(i)	info.wheel[i].brakeDiskRadius	/**< short cut to tWheelSpec#brakeDiskRadius */
#define _wheelRadius(i)		info.wheel[i].wheelRadius	/**< short cut to tWheelSpec#wheelRadius */

/** Static visual attributes */
typedef struct {
    int		exhaustNb;      /**< Number of exhaust pipes (max 2) */
    t3Dd	exhaustPos[2];	/**< Position of exhaust pipes */
    float	exhaustPower;	/**< Power of the flames (from 1 to 3) */
} tVisualAttributes;

/** Static Public info */
typedef struct {
    char	name[MAX_NAME_LEN];         /**< Driver's name */
    char	sname[MAX_NAME_LEN];        /**< Driver's short name */
    char	codename[3 + 1];              /**< Driver's 3-letter codename */
    char	teamname[MAX_NAME_LEN];     /**< Team name */
    char	carName[MAX_NAME_LEN];      /**< Car object name */
    char	category[MAX_NAME_LEN];     /**< Car's category */
    int		raceNumber;                 /**< Car's race number */
    int		startRank;                  /**< Car's starting position */
    int		driverType;                 /**< Driver type */
    int		networkplayer;              /**< Network player */
    int		skillLevel;                 /**< Driver's skill level (0=rookie -> 3=pro) */
    float	iconColor[3];               /**< Car color in leaders board */
    t3Dd	dimension;                  /**< Car's mesures */
    t3Dd	drvPos;                     /**< Driver's position */
    t3Dd	bonnetPos;                  /**< Bonnet's position */
    float	tank;                       /**< Fuel tank capa */
    float	steerLock;                  /**< Steer lock angle */
    t3Dd	statGC;                     /**< Static pos of GC (should be the origin of car axis) */
    tWheelSpec	wheel[4];               /**< Wheels specifications */
    tVisualAttributes visualAttr;       /**< Visual attributes */
    char	masterModel[MAX_NAME_LEN];	/**< Master 3D model car name (the exact folder name) */
    char	skinName[MAX_NAME_LEN];     /**< Custom skin name, if any */
    int		skinTargets;                /**< Target objects for the custom skinning
                                   <br>The possible targets are :
                                   - RM_CAR_SKIN_TARGET_WHOLE_LIVERY
                                   - RM_CAR_SKIN_TARGET_3D_WHEELS
                                   - RM_CAR_SKIN_TARGET_INTERIOR
                                   - RM_CAR_SKIN_TARGET_BOARD
                                   - RM_CAR_SKIN_TARGET_DRIVER
                                   - RM_CAR_SKIN_TARGET_PIT_DOOR
*/
#define RM_CAR_SKIN_TARGET_WHOLE_LIVERY	 	0x00000001	/**< The whole car external livery */
#define RM_CAR_SKIN_TARGET_3D_WHEELS        0x00000002	/**< The 3D wheels */
#define RM_CAR_SKIN_TARGET_INTERIOR         0x00000010	/**< The car interior */
#define RM_CAR_SKIN_TARGET_BOARD            0x00000020	/**< The interior instrument board */
#define RM_CAR_SKIN_TARGET_DRIVER           0x00000040	/**< The driver */
#define RM_CAR_SKIN_TARGET_PIT_DOOR         0x00000100	/**< The pit door logo */
} tInitCar;

/* structure access shortcuts */
#define _name		info.name               /**< short cut to tInitCar#name */
#define _sname		info.sname              /**< short cut to tInitCar#sname */
#define _cname		info.codename           /**< short cut to tInitCar#codename */
#define _teamname	info.teamname           /**< short cut to tInitCar#teamname */
#define _carName	info.carName		/**< short cut to tInitCar#carName */
#define _category	info.category		/**< short cut to tInitCar#category */
#define _driverType	info.driverType		/**< short cut to tInitCar#driverType */
#define _networkPlayer  info.networkplayer      /**< short cut to tInitCar#networkplayer*/
#define _skillLevel	info.skillLevel		/**< short cut to tInitCar#skillLevel */
#define _raceNumber	info.raceNumber		/**< short cut to tInitCar#raceNumber */
#define _startRank	info.startRank		/**< short cut to tInitCar#startRank */
#define _dimension	info.dimension		/**< short cut to tInitCar#dimension */
#define _dimension_x	info.dimension.x	/**< short cut to tInitCar#dimension.x */
#define _dimension_y	info.dimension.y	/**< short cut to tInitCar#dimension.y */
#define _dimension_z	info.dimension.z	/**< short cut to tInitCar#dimension.z */
#define _drvPos_x	info.drvPos.x		/**< short cut to tInitCar#drvPos.x */
#define _drvPos_y	info.drvPos.y		/**< short cut to tInitCar#drvPos.y */
#define _drvPos_z	info.drvPos.z		/**< short cut to tInitCar#drvPos.z */
#define _bonnetPos_x	info.bonnetPos.x	/**< short cut to tInitCar#bonnetPos.x */
#define _bonnetPos_y	info.bonnetPos.y	/**< short cut to tInitCar#bonnetPos.y */
#define _bonnetPos_z	info.bonnetPos.z	/**< short cut to tInitCar#bonnetPos.z */
#define _statGC		info.statGC             /**< short cut to tInitCar#statGC */
#define _statGC_x	info.statGC.x		/**< short cut to tInitCar#statGC.x */
#define _statGC_y	info.statGC.y		/**< short cut to tInitCar#statGC.y */
#define _statGC_z	info.statGC.z		/**< short cut to tInitCar#statGC.z */
#define _iconColor	info.iconColor		/**< short cut to tInitCar#iconColor */
#define _tank		info.tank               /**< short cut to tInitCar#tank */
#define _steerLock	info.steerLock			/**< short cut to tInitCar#steerLock */
#define _exhaustNb	info.visualAttr.exhaustNb	/**< short cut to tVisualAttributes#exhaustNb */
#define _exhaustPos	info.visualAttr.exhaustPos	/**< short cut to tVisualAttributes#exhaustPos */
#define _exhaustPower	info.visualAttr.exhaustPower	/**< short cut to tVisualAttributes#exhaustPower */
#define _masterModel	info.masterModel	 	/**< short cut to tInitCar#masterModel */
#define _skinName	info.skinName               /**< short cut to tInitCar#skinName */
#define _skinTargets	info.skinTargets		/**< short cut to tInitCar#skinTargets */

#define RM_DRV_HUMAN	1
#define RM_DRV_ROBOT	2

#define RM_PENALTY_DRIVETHROUGH     1
#define RM_PENALTY_STOPANDGO        2
#define RM_PENALTY_10SEC_STOPANDGO  3
#define RM_PENALTY_DISQUALIFIED     4

/** One penalty */
typedef struct CarPenalty
{
    int penalty;	/**< penalty type */
    int lapToClear;	/**< the lap before the penalty has to be cleared */
    GF_TAILQ_ENTRY(struct CarPenalty) link;
} tCarPenalty;

GF_TAILQ_HEAD(CarPenaltyHead, struct CarPenalty);

/** Race Administrative info */
typedef struct
{
    double		bestLapTime;
    bool                commitBestLapTime;  /**< If a rule violation happens (e.g. cutting a corner) the laptime is not commited (false) */
    double* bestSplitTime;
    double		deltaBestLapTime;
    double		curLapTime;
    double* curSplitTime;
    double		lastLapTime;
    double		curTime;
    float		topSpeed;
    float		currentMinSpeedForLap;	// Min speed on current lap, reset on start line crossing
    int			laps;
    int			bestLap;
    int			nbPitStops;
    int			remainingLaps;
    int			pos;
    double		timeBehindLeader;
    int			lapsBehindLeader;
    double		timeBehindPrev;
    double		timeBeforeNext;
    float		distRaced;
    float		distFromStartLine;
    int			currentSector;
    int			nbSectors;
    double		trackLength;
    double		scheduledEventTime;
    tTrackOwnPit* pit;
    int			event;
    tCarPenaltyHead	penaltyList;	/**< List of current penalties */
    float               penaltyTime;
    float		prevFromStartLine;
    double		wrongWayTime;
} tCarRaceInfo;
/* structure access */
#define _bestLapTime	        race.bestLapTime
#define _commitBestLapTime      race.commitBestLapTime
#define _bestSplitTime		race.bestSplitTime
#define _deltaBestLapTime	race.deltaBestLapTime
#define _curLapTime             race.curLapTime
#define _curSplitTime		race.curSplitTime
#define _lastLapTime		race.lastLapTime
#define _curTime                race.curTime
#define _topSpeed               race.topSpeed
#define _currentMinSpeedForLap	race.currentMinSpeedForLap
#define _laps                   race.laps
#define _bestLap                race.bestLap
#define _nbPitStops             race.nbPitStops
#define _remainingLaps		race.remainingLaps
#define _pos                    race.pos
#define _timeBehindLeader	race.timeBehindLeader
#define _lapsBehindLeader	race.lapsBehindLeader
#define _timeBehindPrev		race.timeBehindPrev
#define _timeBeforeNext		race.timeBeforeNext
#define _distRaced              race.distRaced
#define _distFromStartLine	race.distFromStartLine
#define _currentSector		race.currentSector
#define _nbSectors          race.nbSectors
#define _trackLength        race.trackLength
#define _pit                    race.pit
#define _scheduledEventTime	race.scheduledEventTime
#define _event                  race.event
#define _penaltyList		race.penaltyList
#define _penaltyTime	        race.penaltyTime
#define _prevFromStartLine	race.prevFromStartLine
#define _wrongWayTime		race.wrongWayTime

/** Public info on the cars */
typedef struct {
    tDynPt	DynGC;          /**< GC data (car axis) */
    tDynPt	DynGCg;         /**< GC data (world axis) */
    float	speed;		/**< total speed, sqrt(vx*vx + vy*vy + vz*vz) */
    sgMat4	posMat;         /**< position matrix */
    tTrkLocPos	trkPos;		/**< current track position. The segment is the track segment (not sides)*/
    int		state;	    	/**< state of the car.
                                   <br>The states are:
                                   - RM_CAR_STATE_FINISH
                                   - RM_CAR_STATE_PIT
                                   - RM_CAR_STATE_DNF
                                   - RM_CAR_STATE_PULLUP
                                   - RM_CAR_STATE_PULLSIDE
                                   - RM_CAR_STATE_PULLDN
                                   - RM_CAR_STATE_OUT
                                   - RM_CAR_STATE_NO_SIMU
                                   - RM_CAR_STATE_BROKEN
                                   - RM_CAR_STATE_OUTOFGAS
                                   - RM_CAR_STATE_ELIMINATED
                                   - RM_CAR_STATE_ENDRACE_CALLED
                                   - RM_CAR_STATE_SIMU_NO_MOVE
*/
#define RM_CAR_STATE_FINISH	 	0x00000100				/**< Car having passed the finish line */
#define RM_CAR_STATE_PIT	 	0x00000001				/**< Car currently stopped in pits */
#define RM_CAR_STATE_DNF	 	0x00000002				/**< Car did not finish */
#define RM_CAR_STATE_PULLUP	 	0x00000004				/**< Car pulled out in the air */
#define RM_CAR_STATE_PULLSIDE	        0x00000008				/**< Car pulled out in the air */
#define RM_CAR_STATE_PULLDN	 	0x00000010				/**< Car pulled out in the air */
#define RM_CAR_STATE_OUT		(RM_CAR_STATE_DNF | RM_CAR_STATE_FINISH)/**< Car out of race */
#define RM_CAR_STATE_NO_SIMU	        0x000000FF				/**< Do not simulate the car */
#define RM_CAR_STATE_BROKEN	 	0x00000200				/**< Engine no more working */
#define RM_CAR_STATE_OUTOFGAS	 	0x00000400				/**< Out of Gas */
#define RM_CAR_STATE_ELIMINATED	 	0x00000800				/**< Eliminated due to rules infringement */
#define RM_CAR_STATE_ENDRACE_CALLED	0x00001000				/**< Endrace called so robot freed a part of its data */
#define RM_CAR_STATE_SIMU_NO_MOVE	0x00010000 				/**< Simulation without car move (i.e. clutch applied and no wheel move)  */
    tPosd	corner[4];
    float       glance;         /* Glance angle degrees, left -ve, right +ve */
    float       oldglance;
} tPublicCar;
/* structure access */
#define _DynGC		pub.DynGC
#define _pos_X		pub.DynGC.pos.x
#define _pos_Y		pub.DynGC.pos.y
#define _pos_Z		pub.DynGC.pos.z
#define _roll		pub.DynGC.pos.ax
#define _pitch		pub.DynGC.pos.ay
#define _yaw		pub.DynGC.pos.az
#define _yaw_rate	pub.DynGC.vel.az
#define _speed_x	pub.DynGC.vel.x
#define _speed_y	pub.DynGC.vel.y
#define _speed_z	pub.DynGC.vel.z
#define _speed_xy	pub.DynGC.vel.xy
#define _accel_x	pub.DynGC.acc.x
#define _accel_y	pub.DynGC.acc.y
#define _accel_z	pub.DynGC.acc.z
#define _state		pub.state
#define _trkPos		pub.trkPos
#define _speed_X	pub.DynGCg.vel.x
#define _speed_Y	pub.DynGCg.vel.y
#define _corner_x(i)	pub.corner[i].ax
#define _corner_y(i)	pub.corner[i].ay
#define _posMat		pub.posMat
#define _glance		pub.glance
#define _oldglance	pub.oldglance

/** Dynamic wheel information */
typedef struct {
    tPosd	      relPos;			/**< position relative to GC */
    float	      spinVel;		/**< spin velocity rad/s */
    float	      brakeTemp;		/**< brake temperature from 0 (cool) to 1.0 (hot) */
    int		      state;			/**< wheel state */
    tTrackSeg*    seg;			/**< Track segment where the wheel is */
    float         rollRes;                /**< rolling resistance, useful for sound */
    float         temp_in, temp_mid, temp_out;    /*tire temperature inside, middle and outside of tread*/
    float         temp_opt;        /*optimal tire temperature*/
    float         condition;      	/**< tire condition, between 0 and 1 */
    float         treadDepth;     	/**< tread depth, between 0 and 1 */
    float         critTreadDepth; 	/**< critical tread depth, when grip falls off suddenly, between 0 and treadDepth */
    float         slipNorm; 		/**< normalized slip, the variable of Magic Formula */
    float         slipOpt;		/**< the value of slipNorm giving maximal grip */
    float         slipSide;
    float         slipAccel;
    float         Fx;
    float         Fy;
    float         Fz;
    float         effectiveMu;
} tWheelState;
/* structure access */
#define _ride(i)                priv.wheel[i].relPos.z
#define _brakeTemp(i)           priv.wheel[i].brakeTemp
#define _wheelSpinVel(i)        priv.wheel[i].spinVel
#define _wheelSeg(i)            priv.wheel[i].seg
#define _wheelSlipNorm(i)       priv.wheel[i].slipNorm
#define _wheelSlipOpt(i)        priv.wheel[i].slipOpt
#define _wheelSlipSide(i)       priv.wheel[i].slipSide
#define _wheelSlipAccel(i)      priv.wheel[i].slipAccel
#define _wheelFx(i)             priv.wheel[i].Fx
#define _wheelFy(i)             priv.wheel[i].Fy
#define _wheelFz(i)             priv.wheel[i].Fz
#define _tyreT_in(i)            priv.wheel[i].temp_in
#define _tyreT_mid(i)           priv.wheel[i].temp_mid
#define _tyreT_out(i)           priv.wheel[i].temp_out
#define _tyreT_opt(i)           priv.wheel[i].temp_opt
#define _tyreCondition(i)       priv.wheel[i].condition
#define _tyreTreadDepth(i)      priv.wheel[i].treadDepth
#define _tyreCritTreadDepth(i)  priv.wheel[i].critTreadDepth
#define _tyreEffMu(i)           priv.wheel[i].effectiveMu

#define MAX_GEARS	10	/* including reverse and neutral */

typedef struct tCollisionState_
{
    int    collision_count;
    sgVec3 pos;
    sgVec3 force;
} tCollisionState;
/* structure access */
#define _collCount	priv.collision_state.collision_count
#define _collPos	priv.collision_state.pos
#define _collForce	priv.collision_state.force

typedef struct MemoryPoolItem tMemoryPoolItem;
typedef tMemoryPoolItem* tMemoryPool;

typedef struct MemPoolCar
{
    tMemoryPool newTrack;
    tMemoryPool newRace;
    tMemoryPool endRace;
    tMemoryPool shutdown;
} tMemPoolCar;

/* structrure to store one parameter of car setup */
typedef struct
{
    float	value;		/* actual value */
    float	min, max;	/* limits for value*/
    float	desired_value;	/* desired new value */
    float	stepsize;	/* value of increment/decrement in one step */
    bool	changed;	/* TRUE if the item has been changed */
} tCarSetupItem;

/* data for a dashboard item */
typedef struct
{
    int		type;		/* type of the item, for possible values see below */
    tCarSetupItem* setup;	/* store the items setup values */
} tDashboardItem;
/* constants for type: */
#define	DI_NONE					-1
/* types for dashboardInstant */
#define DI_BRAKE_REPARTITION			0
#define DI_FRONT_ANTIROLLBAR			1
#define DI_REAR_ANTIROLLBAR			2
#define DI_FRONT_DIFF_MAX_SLIP_BIAS 		3
#define DI_FRONT_DIFF_COAST_MAX_SLIP_BIAS	4
#define DI_REAR_DIFF_MAX_SLIP_BIAS		5
#define DI_REAR_DIFF_COAST_MAX_SLIP_BIAS	6
#define DI_CENTRAL_DIFF_MAX_SLIP_BIAS		7
#define DI_CENTRAL_DIFF_COAST_MAX_SLIP_BIAS	8
/* number of instant types */
#define NR_DI_INSTANT				9
/* types for dashboardRequest */
#define DI_FUEL					32
#define DI_REPAIR				33
#define DI_TYRE_SET				34
#define DI_FRONT_WING_ANGLE			35
#define DI_REAR_WING_ANGLE			36
#define DI_PENALTY				40
/* number of request types */
#define NR_DI_REQUEST				6

/** Data known only by the driver */
typedef struct
{
    void* paramsHandle;          /**< accessible parameters for modules */
    void* carHandle;             /**< parameters for car caracteristics */
    int		driverIndex;            /**< index when multiple drivers are in the same dll */
    int     moduleIndex;            /**< index which is the same as the one in the files */
    char	modName[MAX_NAME_LEN];	/**< dll name */
    tWheelState	wheel[4];
    tPosd	corner[4];              /**< car's corners position */
    int		gear;                   /**< current gear */
    int		gearNext;               /**< next gear while shiting */
    float	fuel;                   /**< remaining fuel (liters) */
    float   fuel_consumption_total; // l
    float   fuel_consumption_instant; // l/100km (>100 means infinity)
    float	enginerpm;
    float	enginerpmRedLine;
    float	enginerpmMax;
    float	enginerpmMaxTq;
    float	enginerpmMaxPw;
    float	engineMaxTq;
    float	engineMaxPw;
    float	gearRatio[MAX_GEARS];	/**< including final drive */
    int		gearNb;                 /**< incl reverse and neutral */
    int		gearOffset;             /**< gearRatio[gear + gearOffset] is the ratio for gear */
    float	skid[4];                /**< skid intensity */
    float	reaction[4];            /**< reaction on wheels */
    int		collision;
    int		simcollision;           /**< For rules etc. reflects the collision state from simu */
    float   smoke;
    t3Dd	normal;
    t3Dd	collpos;                /**< Collision position, useful for sound ; Simu V2 only */
    int		dammage;
    int		debug;
    tCollisionState collision_state;    /**< collision state ; Simu V3 only  */
    float   localPressure;	// Environment pressure at cars location
    tMemPoolCar	memoryPool;
    float       driveSkill;             /**< Skill level for robots: 0.0 means as fast as possible; 10.0 means at a slower speed so players can easier win */
    float       steerTqCenter;          /**< torques on steering wheel for force feedback, this is the centering torque, linear with steering angle */
    float	steerTqAlign;		/**< force feedback torque: tire aligning torque from magic formula */
    tDashboardItem dashboardInstant[NR_DI_INSTANT];
    int		dashboardInstantNb;	/**< number and list of immediately changing items in dashboard */
    tDashboardItem dashboardRequest[NR_DI_REQUEST];
    int		dashboardRequestNb;	/**< number and list of items requested to change during next pit stop */
    int		dashboardActiveItem;	/**< active item in dashboard, 0 .. dashboardInstantNb+dashboardRequestNb-1 */
} tPrivCar;
/* structure access */
#define _fuelTotal      priv.fuel_consumption_total
#define _fuelInstant    priv.fuel_consumption_instant
#define _driverIndex	priv.driverIndex
#define _moduleIndex	priv.moduleIndex
#define _paramsHandle	priv.paramsHandle
#define _carHandle      priv.carHandle
#define _modName        priv.modName
#define _enginerpm      priv.enginerpm
#define _enginerpmRedLine	priv.enginerpmRedLine
#define _enginerpmMax	priv.enginerpmMax
#define _enginerpmMaxTq	priv.enginerpmMaxTq
#define _enginerpmMaxPw	priv.enginerpmMaxPw
#define _engineMaxTq	priv.engineMaxTq
#define _engineMaxPw	priv.engineMaxPw
#define _gearRatio      priv.gearRatio
#define _gearNb         priv.gearNb
#define _gearOffset     priv.gearOffset
#define _fuel           priv.fuel
#define _gear           priv.gear
#define _gearNext       priv.gearNext
#define _debug          priv.debug
#define _skid           priv.skid
#define _reaction       priv.reaction
#define _dammage        priv.dammage
#define _driveSkill     priv.driveSkill
#define _collision      priv.collision

#define _smoke          priv.smoke
#define _normal         priv.normal
#define _coll2Pos       priv.collpos

#define _steerTqCenter		priv.steerTqCenter
#define _steerTqAlign		priv.steerTqAlign

#define _newTrackMemPool	priv.memoryPool.newTrack
#define _newRaceMemPool		priv.memoryPool.newRace
#define _endRaceMemPool		priv.memoryPool.endRace
#define _shutdownMemPool	priv.memoryPool.shutdown

#define	_dashboardInstant	priv.dashboardInstant
#define _dashboardInstantNb	priv.dashboardInstantNb
#define _dashboardRequest	priv.dashboardRequest
#define _dashboardRequestNb	priv.dashboardRequestNb
#define _dashboardActiveItem	priv.dashboardActiveItem

/** Info returned by driver during the race */
/** New order to get better alignment, additional parameters for new features */
typedef struct
{
    float	steer;	            /**< Steer command       [-1.0, 1.0]  */
    float	accelCmd;           /**< Accelerator command [ 0.0, 1.0] */
    float	brakeCmd;           /**< Brake command       [ 0.0, 1.0] */
    float	clutchCmd;          /**< Clutch command      [ 0.0, 1.0] */

    // New commands for single wheel braking
    float	brakeFrontLeftCmd;  /**< Brake front left command  [0.0, 1.0] 0: no brake; 1: max brake */
    float	brakeFrontRightCmd; /**< Brake front right command [0.0, 1.0] 0: no brake; 1: max brake */
    float	brakeRearLeftCmd;   /**< Brake rear left command   [0.0, 1.0] 0: no brake; 1: max brake */
    float	brakeRearRightCmd;  /**< Brake rear right command  [0.0, 1.0] 0: no brake; 1: max brake */

    // New commands for variable wings (incl. airbrake)
    float	wingFrontCmd;       /**< Wing angle of attack front [0, PI/4: angle in rad] */
    float	wingRearCmd;        /**< Wing angle of attack rear  [0, PI/4: angle in rad] */
    float	reserved1;          /**< reserved for future use */
    float	reserved2;          /**< reserved for future use */

    int		gear;  	            /**< [-1,MAX_GEARS - 2] for gear selection */
    int		raceCmd;            /**< command issued by the driver */
#define RM_CMD_NONE		0	    /**< No race command */
#define RM_CMD_PIT_ASKED	1	/**< Race command: Pit asked */
#define RM_CMD_MAX_MSG_SIZE 32

    int	lightCmd;           /**< Lights command */
#define RM_LIGHT_HEAD1		0x00000001	/**< head light 1 */
#define RM_LIGHT_HEAD2		0x00000002	/**< head light 2 */

    int	ebrakeCmd;          /**< Parking/Emergency Brake command */

    int	wingControlMode;        /**< 0: No wing control; 1 fixed wing angles; 2 variable wing angles */
    int	singleWheelBrakeMode;   /**< 0: No single wheel brake; 1 single wheel brake mode */
    int	switch3;                /**< reserved for future use */
    int	telemetryMode;          /**< Define mode for telemetry output: 0: no output; 1: full output; ... see car.cpp SimCarUpdate2 */

    char	msg[4][RM_CMD_MAX_MSG_SIZE];     /**< 4 lines of 31 characters : 0-1 from car, 2-3 from race engine */
#define RM_MSG_LEN	31

    float	msgColor[4]; /**< RGBA of text */
    tDashboardItem* setupChangeCmd;	/* setup item changed in dashboard */
} tCarCtrl;
#define _steerCmd	ctrl.steer
#define _accelCmd	ctrl.accelCmd
#define _brakeCmd	ctrl.brakeCmd
#define _clutchCmd	ctrl.clutchCmd

#define _gearCmd	ctrl.gear
#define _raceCmd	ctrl.raceCmd
#define _lightCmd	ctrl.lightCmd
#define _ebrakeCmd	ctrl.ebrakeCmd

#define _brakeFLCmd	ctrl.brakeFrontLeftCmd
#define _brakeFRCmd	ctrl.brakeFrontRightCmd
#define _brakeRLCmd	ctrl.brakeRearLeftCmd
#define _brakeRRCmd	ctrl.brakeRearRightCmd

#define _wingFCmd	ctrl.wingFrontCmd
#define _wingRCmd	ctrl.wingRearCmd

#define _msgCmd		ctrl.msg
#define _msgColorCmd	ctrl.msgColor

#define _telemetryMode ctrl.telemetryMode
#define _singleWheelBrakeMode ctrl.singleWheelBrakeMode

struct RobotItf;

/* car setup parameters */
typedef struct
{
    tCarSetupItem FRWeightRep, FRLWeightRep, RRLWeightRep;
    tCarSetupItem fuel;
    tCarSetupItem wingAngle[2];
    tCarSetupItem revsLimiter;
    tCarSetupItem gearRatio[MAX_GEARS];
    int           differentialType[3];
    tCarSetupItem differentialRatio[3];
    tCarSetupItem differentialMinTqBias[3], differentialMaxTqBias[3], differentialViscosity[3];
    tCarSetupItem differentialLockingTq[3], differentialMaxSlipBias[3], differentialCoastMaxSlipBias[3];
    tCarSetupItem steerLock;
    tCarSetupItem brakeRepartition, brakePressure;
    tCarSetupItem rideHeight[4], toe[4], camber[4];
    tCarSetupItem tirePressure[4], tireOpLoad[4]; //note: pressure is unused now
    tCarSetupItem arbSpring[2], arbBellcrank[2];
    tCarSetupItem heaveSpring[2], heaveBellcrank[2], heaveInertance[2];
    tCarSetupItem heaveFastBump[2], heaveSlowBump[2], heaveBumpLvel[2];
    tCarSetupItem heaveFastRebound[2], heaveSlowRebound[2], heaveReboundLvel[2];
    tCarSetupItem suspSpring[4], suspBellcrank[4], suspInertance[4];
    tCarSetupItem suspCourse[4], suspPacker[4];
    tCarSetupItem suspFastBump[4], suspSlowBump[4], suspBumpLvel[4];
    tCarSetupItem suspFastRebound[4], suspSlowRebound[4], suspReboundLvel[4];
    tCarSetupItem reqRepair, reqTireset, reqPenalty; //used for pit stop repair, tire set and stop & go request
} tCarSetup;

/** Command issued by the car during pit stop */
typedef struct CarPitCmd
{
    float		fuel;
    int			repair;
#define RM_PIT_REPAIR		0
#define RM_PIT_STOPANDGO	1
    int			stopType;
    bool		setupChanged; /* TRUE if setup has changed during the pit stop */

    enum TireChange { NONE = 0, ALL = 1 };
    TireChange tireChange;
} tCarPitCmd;
/* structure access */
#define _pitFuel	pitcmd.fuel
#define _pitRepair	pitcmd.repair
#define _pitStopType	pitcmd.stopType


/** Car structure (tCarElt).
    This is the main car structure, used everywhere in the code.
*/
typedef struct CarElt
{
    int             index;	/**< car index */
    tInitCar		info;	/**< public */
    tPublicCar		pub;	/**< public */
    tCarRaceInfo	race;	/**< public */
    tPrivCar		priv;	/**< private */
    tCarCtrl		ctrl;	/**< private */
    tCarSetup		setup;	/**< private */
    tCarPitCmd		pitcmd;	/**< private */
    struct RobotItf* robot;	/**< private */
    struct CarElt* next;
} tCarElt;

/* Sections in XML description files */

#define SECT_SIMU_SETTINGS			"Simulation Options"
#define SECT_CAR					"Car"
#define SECT_FRNT					"Front"
#define SECT_FRNTWING				"Front Wing"
#define SECT_FRNTWINGSHORT          "Front Wing Short"
#define SECT_FRNTWINGLONG           "Front Wing Long"
#define SECT_FRNTWINGROAD           "Front Wing Road"
#define SECT_FRNTAXLE				"Front Axle"
#define SECT_FRNTARB				"Front Anti-Roll Bar"
#define SECT_FRNTHEAVE				"Front Heave Spring"
#define SECT_FRNTRGTWHEEL			"Front Right Wheel"
#define SECT_FRNTLFTWHEEL			"Front Left Wheel"
#define SECT_FRNTRGTSUSP			"Front Right Suspension"
#define SECT_FRNTLFTSUSP			"Front Left Suspension"
#define SECT_FRNTRGTBRAKE			"Front Right Brake"
#define SECT_FRNTLFTBRAKE			"Front Left Brake"
#define SECT_FRNTDIFFERENTIAL		"Front Differential"
#define SECT_REAR					"Rear"
#define SECT_REARWING				"Rear Wing"
#define SECT_REARWINGSHORT          "Rear Wing Short"
#define SECT_REARWINGLONG           "Rear Wing Long"
#define SECT_REARWINGROAD           "Rear Wing Road"
#define SECT_REARAXLE				"Rear Axle"
#define SECT_REARARB				"Rear Anti-Roll Bar"
#define SECT_REARHEAVE				"Rear Heave Spring"
#define SECT_REARRGTWHEEL			"Rear Right Wheel"
#define SECT_REARLFTWHEEL			"Rear Left Wheel"
#define SECT_REARRGTSUSP			"Rear Right Suspension"
#define SECT_REARLFTSUSP			"Rear Left Suspension"
#define SECT_REARRGTBRAKE			"Rear Right Brake"
#define SECT_REARLFTBRAKE			"Rear Left Brake"
#define SECT_REARDIFFERENTIAL		"Rear Differential"
#define SECT_CENTRALDIFFERENTIAL	"Central Differential"
#define SECT_STEER                  "Steer"
#define SECT_BRKSYST                "Brake System"
#define SECT_AERODYNAMICS           "Aerodynamics"
#define SECT_ENGINE                 "Engine"
#define SECT_CLUTCH                 "Clutch"
#define SECT_DRIVETRAIN             "Drivetrain"
#define SECT_GEARBOX                "Gearbox"
#define SECT_DRIVER                 "Driver"
#define SECT_BONNET                 "Bonnet"
#define SECT_GROBJECTS              "Graphic Objects"
#define SECT_EXHAUST                "Exhaust"
#define SECT_LIGHT                  "Light"
#define SECT_FEATURES               "Features"

/* Parameter names */
#define PRM_CATEGORY		"category"
#define PRM_LEN				"body length"
#define PRM_WIDTH			"body width"
#define PRM_OVERALLLEN		"overall length"
#define PRM_OVERALLWIDTH	"overall width"
#define PRM_HEIGHT			"body height"
#define PRM_MASS			"mass"
#define PRM_FRWEIGHTREP		"front-rear weight repartition"
#define PRM_FRLWEIGHTREP	"front right-left weight repartition"
#define PRM_RRLWEIGHTREP	"rear right-left weight repartition"
#define PRM_GCHEIGHT		"GC height"
#define PRM_TANK			"fuel tank"
#define PRM_FUEL			"initial fuel"
#define PRM_CENTR			"mass repartition coefficient"
#define PRM_ROLLROTINERTIA		"roll rotational inertia"
#define PRM_PITCHROTINERTIA		"pitch rotational inertia"
#define PRM_YAWROTINERTIA		"yaw rotational inertia"
#define PRM_INERTIA			"inertia"
#define PRM_EFFICIENCY		"efficiency"
#define PRM_TYPE			"type"
#define PRM_SIZE			"size"

/* Tires */
#define PRM_MU				"mu"
#define PRM_RIMDIAM			"rim diameter"
#define PRM_TIREWIDTH			"tire width"
#define PRM_TIREHEIGHT			"tire height"
#define PRM_TIRERATIO			"tire height-width ratio"
#define PRM_RIDEHEIGHT			"ride height"
#define PRM_ROLLINGRESIST		"rolling resistance"
#define PRM_TOE				"toe"
#define PRM_CAMBER			"camber"
#define PRM_CA				"stiffness"
#define PRM_RFACTOR			"dynamic friction"
#define PRM_EFACTOR			"elasticity factor"
#define PRM_PRESSURE			"pressure"
#define PRM_LOADFMAX			"load factor max"
#define PRM_LOADFMIN			"load factor min"
#define PRM_OPLOAD			"operating load"
#define PRM_ALIGNTQFACTOR		"aligning torque factor"
#define PRM_OPTTEMP			"optimal temperature"
#define PRM_INITTEMP			"initial temperature"
#define PRM_COLDMUFACTOR		"cold mu factor"
#define PRM_HEATINGMULT			"heating multiplier"
#define PRM_AIRCOOLINGMULT		"air cooling multiplier"
#define PRM_SPEEDCOOLINGMULT	        "speed cooling multiplier"
#define PRM_WEARRATE			"wear rate multiplier"
#define PRM_FALLOFFTREADDEPTH	        "falloff tread depth"
#define PRM_FALLOFFGRIPMULT		"falloff grip multiplier"
#define PRM_REMAININGGRIPMULT	        "remaining grip multiplier"
#define PRM_IDEALTEMP	                "ideal temperature"
#define PRM_WEAR		        "wear"

#define PRM_SPR				"spring"
#define PRM_SUSPCOURSE		"suspension course"
#define PRM_BELLCRANK		"bellcrank"
#define PRM_PACKERS			"packers"
#define PRM_SLOWBUMP		"slow bump"
#define PRM_SLOWREBOUND		"slow rebound"
#define PRM_FASTBUMP		"fast bump"
#define PRM_FASTREBOUND		"fast rebound"
#define PRM_BUMPLVEL		"bump limit velocity"
#define PRM_REBOUNDLVEL		"rebound limit velocity"

#define PRM_XPOS			"xpos"
#define PRM_YPOS			"ypos"
#define PRM_ZPOS			"zpos"

#define PRM_STEERLOCK		"steer lock"
#define PRM_STEERSPD		"max steer speed"

#define PRM_BRKDIAM			"disk diameter"
#define PRM_BRKAREA			"piston area"
#define PRM_BRKREP			"front-rear brake repartition"
#define PRM_BRKPRESS		"max pressure"
#define PRM_EBRKPRESS		"emergency brake pressure"
#define PRM_BRKCOR_LR		"brake corr lr"
#define PRM_BRKCOR_FR		"brake corr fr"

#define PRM_CX				"Cx"
#define PRM_CL				"Clift"
#define PRM_CLBIAS			"Clift bias"
#define PRM_FCL				"front Clift"
#define PRM_RCL				"rear Clift"
#define PRM_FRNTAREA		"front area"
#define PRM_WINGAREA		"area"
#define PRM_WINGANGLE		"angle"
#define PRM_WINGANGLEVARIABLE "variable"

#define PRM_WINGTYPE		"wing type"
#define PRM_AOAATMAX		"aoa at max"
#define PRM_AOAATZERO		"aoa at zero"
#define PRM_AOAOFFSET		"aoa offset"

#define PRM_CLMAX			"clift max"
#define PRM_CLATZERO		"clift at zero"
#define PRM_CLASYMP			"clift asymptotic"
#define PRM_DELAYDECREASE	"clift delay"
#define PRM_CURVEDECREASE	"clift curve"

#define PRM_ANGLEOFSTALL	"angle of stall"
#define PRM_STALLWIDTH		"stall width"
#define PRM_ASPECTRATIO		"aspect ratio"

/* Engine */
#define PRM_REVSLIM			"revs limiter"
#define PRM_REVSMAX			"revs maxi"
#define PRM_TICKOVER		"tickover"
#define PRM_RPM				"rpm"
#define PRM_TQ				"Tq"
#define ARR_DATAPTS			"data points"
#define PRM_FUELCONS		"fuel cons factor"
#define PRM_ENGBRKCOEFF		"brake coefficient"
#define PRM_ENGBRKLINCOEFF	"brake linear coefficient"
#define PRM_POWER			"power"
#define PRM_TURBO			"turbo"
#define PRM_TURBO_RPM		"turbo rpm"
#define PRM_TURBO_FACTOR	"turbo factor"
#define PRM_TURBO_LAG		"turbo lag"
#define PRM_CAPACITY		"capacity"
#define PRM_CYLINDERS		"cylinders"
#define PRM_ENGSHAPE		"shape"
#define PRM_ENGPOS			"position"

#define PRM_RATIO			"ratio"
#define PRM_BIAS			"bias"

#define ARR_GEARS			"gears"
#define PRM_SHIFTTIME		"shift time"

#define	PRM_ROLLCENTER		"roll center height"

#define LST_RANGES			"Ranges"
#define PRM_THRESHOLD		"threshold"
#define PRM_CAR				"car"
#define PRM_WHEELSON		"wheels"
#define PRM_TEMPLATE		"template"
#define PRM_ENV				"env"
#define PRM_BONNET			"bonnet"
#define PRM_WHEEL_TEXTURE	"wheel texture"
#define PRM_SHADOW_TEXTURE	"shadow texture"

#define SECT_STEERWHEEL	    "Steer Wheel"
#define PRM_SW_MODEL        "model"
#define PRM_SW_MODELHR      "hi res model"
#define PRM_SW_ANGLE        "angle"
#define PRM_SW_MOVT         "movt"

#define LST_DRIVER          "Driver"
#define PRM_DRIVERSTEER     "steer"
#define PRM_DRIVERMODEL     "driver"

#define LST_REARWING        "RearWing"
#define PRM_REARWINGANGLE   "angle"
#define PRM_REARWINGMODEL   "rearwing"

#define SECT_COCKPIT		"cockpit"
#define PRM_MODELCOCKPIT	"model cockpit"

#define SECT_WING_MODEL		"wing model"
#define PRM_WING_1			"model short oval"
#define PRM_WING_2			"model long oval"
#define PRM_WING_3          "model road"

#define PRM_MIN_TQ_BIAS		"min torque bias"
#define PRM_MAX_TQ_BIAS		"max torque bias"
#define PRM_MAX_SLIP_BIAS	"max slip bias"
#define PRM_COAST_MAX_SLIP_BIAS "coast max slip bias"
#define PRM_LOCKING_TQ		"locking input torque"
#define PRM_VISCOSITY_FACTOR	"viscosity factor"


#define VAL_DIFF_NONE           "NONE"
#define VAL_DIFF_SPOOL          "SPOOL"
#define VAL_DIFF_FREE           "FREE"
#define VAL_DIFF_LIMITED_SLIP	"LIMITED SLIP"
#define VAL_DIFF_LOCKING     	"LOCKING"
#define VAL_DIFF_VISCOUS_COUPLER "VISCOUS COUPLER"
#define VAL_DIFF_15WAY_LSD	"1.5 WAY LSD"
#define VAL_DIFF_ELECTRONIC_LSD	"ELECTRONIC LSD"

#define VAL_TRANS_RWD		"RWD"
#define VAL_TRANS_FWD		"FWD"
#define VAL_TRANS_4WD		"4WD"

#define VAL_ENGSHAPE_V		"v"
#define VAL_ENGSHAPE_L		"l"
#define VAL_ENGSHAPE_H		"h"
#define VAL_ENGSHAPE_W		"w"

#define VAL_ENGPOS_FRONT		"front"
#define VAL_ENGPOS_FRONTMID		"front-mid"
#define VAL_ENGPOS_MID          "mid"
#define VAL_ENGPOS_REARMID		"rear-mid"
#define VAL_ENGPOS_REAR         "rear"

/* graphic */
#define PRM_TACHO_TEX           "tachometer texture"
#define PRM_TACHO_XSZ           "tachometer width"
#define PRM_TACHO_YSZ           "tachometer height"
#define PRM_TACHO_XPOS          "tachometer x pos"
#define PRM_TACHO_YPOS          "tachometer y pos"
#define PRM_TACHO_NDLXSZ        "tachometer needle width"
#define PRM_TACHO_NDLYSZ        "tachometer needle height"
#define PRM_TACHO_XCENTER       "tachometer needle x center"
#define PRM_TACHO_YCENTER       "tachometer needle y center"
#define PRM_TACHO_XDIGITCENTER	"tachometer digit x center"
#define PRM_TACHO_YDIGITCENTER	"tachometer digit y center"
#define PRM_TACHO_MINVAL        "tachometer min value"
#define PRM_TACHO_MAXVAL        "tachometer max value"
#define PRM_TACHO_MINANG        "tachometer min angle"
#define PRM_TACHO_MAXANG        "tachometer max angle"

#define PRM_SPEEDO_TEX          "speedometer texture"
#define PRM_SPEEDO_XSZ          "speedometer width"
#define PRM_SPEEDO_YSZ          "speedometer height"
#define PRM_SPEEDO_XPOS         "speedometer x pos"
#define PRM_SPEEDO_YPOS         "speedometer y pos"
#define PRM_SPEEDO_NDLXSZ       "speedometer needle width"
#define PRM_SPEEDO_NDLYSZ       "speedometer needle height"
#define PRM_SPEEDO_XCENTER      "speedometer needle x center"
#define PRM_SPEEDO_YCENTER      "speedometer needle y center"
#define PRM_SPEEDO_XDIGITCENTER	"speedometer digit x center"
#define PRM_SPEEDO_YDIGITCENTER	"speedometer digit y center"
#define PRM_SPEEDO_MINVAL       "speedometer min value"
#define PRM_SPEEDO_MAXVAL       "speedometer max value"
#define PRM_SPEEDO_MINANG       "speedometer min angle"
#define PRM_SPEEDO_MAXANG       "speedometer max angle"
#define PRM_SPEEDO_DIGITAL      "speedometer digital"

#define PRM_NEEDLE_RED			"needle red"
#define PRM_NEEDLE_GREEN		"needle green"
#define PRM_NEEDLE_BLUE			"needle blue"
#define PRM_NEEDLE_ALPHA		"needle alpha"

#define PRM_WHEEL_3D            "3d wheel"
#define PRM_FRONT_WHEEL_3D      "3d front wheel"
#define PRM_REAR_WHEEL_3D       "3d rear wheel"

/* Lights */
#define	VAL_LIGHT_HEAD1		"head1"
#define	VAL_LIGHT_HEAD2		"head2"
#define	VAL_LIGHT_BRAKE		"brake"
#define	VAL_LIGHT_BRAKE2	"brake2"
#define	VAL_LIGHT_REVERSE	"reverse"
#define	VAL_LIGHT_REAR		"rear"

/* Simulation Options */
#define PRM_DAMAGE_TYRES            "damage/tyres"
#define PRM_DAMAGE_SUSPENSION       "damage/suspension"
#define PRM_DAMAGE_ALIGNMENT        "damage/alignment"
#define PRM_DAMAGE_AERO             "damage/aero"
#define PRM_MODEL_AEROFLOW          "model/aero/flow"
#define PRM_MODEL_AERO_FACTOR       "model/aero/factor"
#define PRM_MODEL_TYRE_TEMPERATURE  "model/tyre/temperature"

// Collision constants.
#define SEM_COLLISION			0x01
#define SEM_COLLISION_XYSCENE	        0x02
#define SEM_COLLISION_CAR		0x04
#define SEM_COLLISION_Z			0x08
#define SEM_COLLISION_Z_CRASH	        0x10

/* features */
#define PRM_AEROTOCG            "shifting aero coordinates"
#define PRM_SLOWGRIP            "fixed low speed grip"
#define PRM_REALGEARCHANGE      "realistic gear change"
#define PRM_REVLIMIT            "realistic rev limiter"
#define PRM_TIRETEMPDEG         "tire temperature and degradation"
#define PRM_FIXEDWHEELFORCE     "fixed wheel force"
#define PRM_TCLINSIMU           "enable tcl"
#define PRM_ABSINSIMU           "enable abs"
#define PRM_ESPINSIMU           "enable esp"
#define PRM_LIMITEDGROUNDEFFECT	"enable cliftlimit"
#define VAL_YES                 "yes"
#define VAL_NO                  "no"
#define FEAT_AEROTOCG               0x01        //shift aero coordinates with the CG
#define FEAT_SLOWGRIP               0x02        //fix magic formula divergence at low speeds
#define FEAT_REALGEARCHANGE         0x04        //neutral between old and new gears without autoclutch
#define FEAT_REVLIMIT               0x08        //rev drop when hitting the limit
#define FEAT_TIRETEMPDEG            0x10        //tire temperature and degradation
#define FEAT_FIXEDWHEELFORCE        0x20        //fixed wheel force in SimWheelUpdateForce
#define FEAT_TCLINSIMU              0x40        //TCL simulation with 500 Hz at single wheels and engine
#define FEAT_ABSINSIMU              0x80        //ABS simulation with 500 Hz at single wheels
#define FEAT_ESPINSIMU              0x100       //ESP simulation with 500 Hz at single wheels
#define FEAT_LIMITEDGROUNDEFFECT    0x200       //Limit for Clift is enabled

#endif /* __CARV1_H__ */


#ifndef _RACEMANV1_H_
#define _RACEMANV1_H_

#define RCM_IDENT 0

#define RACE_ENG_CFG	"config/raceengine.xml"

struct RmInfo;

typedef int (*tfRmRunState) (struct RmInfo*);

#define RM_SYNC			0x00000001
#define RM_ASYNC		0x00000002

#define RM_END_RACE		0x00000010
#define RM_CONTINUE_RACE	0x00000020

#define RM_NEXT_STEP		0x00000100
#define RM_NEXT_RACE		0x00000200
#define RM_NEXT_EVENT		0x00000400

#define RM_QUIT			0x10000000
#define RM_ERROR	        0x2000000


#define RCM_MAX_DT_SIMU		0.006
#define RCM_MAX_DT_ROBOTS	0.02
#define RCM_MAX_DT_FRAME	0.05

/** General info on current race */
typedef struct {
    int			ncars;		/**< number of cars */
    int			totLaps;	/**< total laps */
    int			extraLaps;	/**< number of laps after the time finishes */
    double		totTime;	/**< total time */
    int			state;
#define RM_RACE_RUNNING		0X00000001
#define RM_RACE_FINISHING	0X00000002
#define RM_RACE_ENDED		0X00000004
#define RM_RACE_STARTING	0X00000008
#define RM_RACE_PRESTART	0X00000010
#define RM_RACE_PAUSED		0X40000000
    int			type;		/**< Race session type */
#define RM_TYPE_PRACTICE	0 /* Please keep the order */
#define RM_TYPE_QUALIF		1
#define RM_TYPE_RACE		2
    int                 maxDammage;
    unsigned long	fps;
    int			features;	/**< A list of features a race have (the common set of features all the robots in the race actually support) */
#define RM_FEATURE_PENALTIES	0x01 /**< A robot with this feature implements penalties */
#define RM_FEATURE_TIMEDSESSION 0x02 /**< A robot with this feature implements timed sessions */
#define RM_FEATURE_WETTRACK	    0x04 /**< A robot with this feature can handle wet track conditions */
#define RM_FEATURE_REALWEATHER  0x08 /**< A robot with this feature can handle with dynamic weather */
} tRaceAdmInfo;

#define _ncars		raceInfo.ncars
#define _totLaps	raceInfo.totLaps
#define _extraLaps	raceInfo.extraLaps
#define _totTime	raceInfo.totTime
#define _raceState	raceInfo.state
#define _raceType	raceInfo.type
#define _maxDammage	raceInfo.maxDammage
#define _features	raceInfo.features

/** cars situation used to inform the GUI and the drivers */
typedef struct Situation {
    tRaceAdmInfo	raceInfo;
    double		deltaTime;
    double		currentTime;	/**< current time in sec since the beginning of the simulation */
    double		accelTime;	/**< accelerared time used for 24hr race simulation */
    int			nbPlayers;	/**< number of human player in local (splitted screen) */
    tCarElt** cars;		/**< list of cars */
} tSituation;

/** Race Engine states */
#define RE_STATE_CONFIG			0
#define RE_STATE_EVENT_INIT		1
#define RE_STATE_EVENT_LOOP		2
#define RE_STATE_PRE_RACE		3
#define RE_STATE_RACE_START		4
#define RE_STATE_NETWORK_WAIT   5
#define RE_STATE_RACE			6
#define RE_STATE_RACE_STOP		7
#define RE_STATE_RACE_END		8
#define RE_STATE_POST_RACE		9
#define RE_STATE_EVENT_SHUTDOWN	10
#define RE_STATE_EVOLUTION		11
#define RE_STATE_SHUTDOWN		12
#define RE_STATE_RESULTS		13
#define RE_STATE_CLEANUP		14
#define RE_STATE_WAITFORKEYPRESS 15
#define RE_STATE_ERROR			16
#define RE_STATE_EXIT			17
#define RE_STATE_PRE_RACE_PAUSE	18
#define RE_STATE_RACE_COOLDOWN	19

/** Race Engine Car Information about the race */
typedef struct
{
    tTrkLocPos	prevTrkPos;
    float	sTime;
    int		lapFlag;
    char* raceMsg;
    double	totalPitTime;
    double	startPitTime;
    float	topSpd;
    float	botSpd;
    float	fuel;
} tReCarInfo;

#define	RM_DISP_MODE_NONE    0x00
#define	RM_DISP_MODE_NORMAL    0x01
#define	RM_DISP_MODE_SIMU_SIMU 0x02
#define	RM_DISP_MODE_NUMBER 4 // Possible combinations of the non-undefined modes
#define	RM_DISP_MODE_UNDEFINED 0x04

/** Race Engine Information.
   @image	html raceenginestate.gif
 */
typedef struct
{
    int			state;
    void* param;
    const char* filename;
    const char* name;
    const char* raceName;
    tReCarInfo* carInfo;
    double		curTime; // Explain please.
    double		lastRobTime; // Last time the robots were rbDrive'd.
    double		timeMult;
    int			running;
    unsigned	displayMode; // Bit field (see RM_DISP_MODE_*)
    tCarElt* pitRequester; // The car asking for pit (stopped in the slot).
    char* message;
    double		messageEnd;
    char* bigMessage;
    double		bigMessageEnd;
} tRaceEngineInfo;

#define _reState	raceEngineInfo.state
#define _reParam	raceEngineInfo.param
#define _reSimItf	raceEngineInfo.itf.simItf
#define _reFilename	raceEngineInfo.filename
#define _reName		raceEngineInfo.name
#define _reRaceName	raceEngineInfo.raceName
#define _reCarInfo	raceEngineInfo.carInfo
#define _reCurTime	raceEngineInfo.curTime
#define _reTimeMult	raceEngineInfo.timeMult
#define _reRunning	raceEngineInfo.running
#define _reLastRobTime	raceEngineInfo.lastRobTime
#define _displayMode	raceEngineInfo.displayMode
#define _rePitRequester	raceEngineInfo.pitRequester
#define _reMessage	raceEngineInfo.message
#define _reMessageEnd	raceEngineInfo.messageEnd
#define _reBigMessage	raceEngineInfo.bigMessage
#define _reBigMessageEnd	raceEngineInfo.bigMessageEnd

#define RM_PNST_DRIVETHROUGH	0x00000001
#define RM_PNST_STOPANDGO	    0x00000002
#define RM_PNST_STOPANDGO_OK	0x00000004
#define RM_PNST_OVERSPEED	    0x00000008
#define RM_PNST_SPD		        0x00010000
#define RM_PNST_STNGO		    0x00020000

typedef struct RmRaceRules
{
    enum RmRuleFlags
    {
        CORNER_CUTTING_TIME_INVALIDATE = 1,
        WALL_HIT_TIME_INVALIDATE = 2,
        CORNER_CUTTING_TIME_PENALTY = 4
    };

    int   enabled;
    float fuelFactor;
    float damageFactor;
    float refuelFuelFlow;
    float damageRepairFactor;
    float pitstopBaseTime;
    float tireFactor;
    float allTiresChangeTime;
} tRmRaceRules;


typedef struct RmCarRules
{
    int			ruleState;
} tRmCarRules;


/**
 * Race Manager General Info
 */
typedef struct RmInfo
{
    tCarElt* carList;	/**< List of all the cars racing */
    tSituation* s;		/**< Situation during race */
    tTrack* track;		/**< Current track */
    void* params;	/**< Raceman parameters */
    void* mainParams;    /**< Stays the same even if params change because of more xml-files per raceman */
    void* results;	/**< Race results */
    void* mainResults;   /**< Stays the same even if params change because of more xml-files per raceman */
    //tModList** robModList;	/**< robot modules loaded */
    tRmCarRules* rules;		/**< by car rules */
    tRaceEngineInfo	raceEngineInfo;
    tRmRaceRules    raceRules;
} tRmInfo;

/*
 * Parameters name definitions for Race Managers
 */
#define RM_SECT_HEADER		"Header"
#define RM_SECT_DRIVERS		"Drivers"
#define RM_SECT_STARTINGGRID	"Starting Grid"
#define RM_SECT_RACES		"Races"
#define RM_SECT_TRACKS		"Tracks"
#define RM_SECT_CONF		"Configuration"
#define RM_SECT_OPTIONS		"Options"
#define RM_SECT_POINTS		"Points"
#define RM_SECT_CLASSPOINTS	"Class Points"
#define RM_SECT_RACECARS	"RaceConfig/Cars"


#define RM_SECT_DRIVERS_RACING	"Drivers Start List"

#define RM_SECT_GROUPS		"Groups"
#define RM_SECT_TEAMS		"Teams"
#define RM_SECT_CLASSES		"Classes"
#define RM_SECT_ALLOWEDTRACKS	"Allowed Tracks"

#define RM_ATTR_CUR_CONF	"current configuration"
#define RM_ATTR_START_ORDER	"starting order"
#define RM_ATTR_ALLOW_RESTART	"restart"
#define RM_ATTR_ENABLED		"enabled"
#define RM_ATTR_MUST_COMPLETE	"must complete"
#define RM_ATTR_SPLASH_MENU	"splash menu"
#define RM_ATTR_DISP_START_GRID	"display starting grid"

#define RM_ATTR_MAXNUM		"maximum number"
#define RM_ATTR_MINNUM		"minimum number"
#define RM_ATTR_FOCUSED		"focused module"
#define RM_ATTR_FOCUSEDIDX	"focused idx"
#define RM_ATTR_ACCEPT_TYPES	"accepted types"
#define RM_ATTR_REJECT_TYPES	"rejected types"
#define RM_ATTR_ACCEPT_CATEGORIES	"accepted categories"
#define RM_ATTR_REJECT_CATEGORIES	"rejected categories"

#define RM_ATTR_TOTALNUM	"total number"
#define RM_ATTR_MAX_DRV		"maximum drivers"
#define RM_ATTR_NUMBER		"number"
#define RM_ATTR_CAR		    "car"
#define RM_ATTR_NBGROUPS	"number of groups"
#define RM_ATTR_CAR_CATEGORY    "Car Category"

#define RM_ATTR_PRIO		"priority"
#define RM_ATTR_NAME		"name"
#define RM_ATTR_TYPE		"type"
#define RM_ATTR_SUBTYPE		"subtype"
#define RM_ATTR_FULLNAME	"full name"
#define RM_ATTR_DRVNAME		"driver name"
#define RM_ATTR_CATEGORY	"category"
#define RM_ATTR_DESCR		"description"
#define RM_ATTR_BGIMG		"menu image"
#define RM_ATTR_RUNIMG		"run image"
#define RM_ATTR_STARTIMG	"start image"

#define RM_ATTR_MODULE		"module"
#define RM_ATTR_IDX		    "idx"
#define RM_ATTR_CARNAME	    "car name"
#define RM_ATTR_TEAMNAME	"team"
#define RM_ATTR_SKINNAME	"skin name"
#define RM_ATTR_SKINTARGETS	"skin targets"
#define RM_ATTR_EXTENDED	"extended"
#define RM_ATTR_SKILLLEVEL	"skill level"
#define RM_ATTR_DISPMODE	"display mode"
#define RM_ATTR_DISPRES		"display results"

#define RM_ATTR_TIMESTEP	"time step"

#define RM_ATTR_RACE		"race"
#define RM_ATTR_ROWS		"rows"
#define RM_ATTR_TOSTART		"distance to start"
#define RM_ATTR_COLDIST		"distance between columns"
#define RM_ATTR_COLOFFSET	"offset within a column"
#define RM_ATTR_INITSPEED	"initial speed"
#define RM_ATTR_INITHEIGHT	"initial height"
#define RM_ATTR_SHOW_RACE	"show race"
#define RM_ATTR_MAX_DMG		"maximum damage"
#define RM_ATTR_DISTANCE	"distance"
#define RM_ATTR_LAPS		"laps"
#define RM_ATTR_SESSIONTIME	"sessiontime"
#define RM_ATTR_CLOUDS	    "clouds"
#define RM_ATTR_RAIN        "rain"
#define RM_ATTR_TIME_OF_DAY	"time of day"
#define RM_ATTR_WEATHER     "weather"
#define RM_ATTR_QUAL_LAPS	"Qualification laps"
#define RM_ATTR_POLE		"pole position side"
#define RM_ATTR_CARSPERPIT	"cars per pit"

#define RM_ATTR_INVALIDATE_BEST_LAP_WALL_TOUCH  "invalidate best lap on wall touch"
#define RM_ATTR_INVALIDATE_BEST_LAP_CORNER_CUT  "invalidate best lap on corner cutting"
#define RM_ATTR_CORNER_CUT_TIME_PENALTY         "corner cutting time penalty"
#define RM_ATTR_DAMAGE_FACTOR                   "damage factor"
#define RM_ATTR_FUEL_FACTOR                     "fuel consumption factor"
#define RM_ATTR_TIRE_FACTOR                     "tire factor"
#define RM_ATTR_PIT_SPEED_LIMIT                 "pit speed limit"
#define RM_ATTR_REFUEL_FUEL_FLOW                "refuel fuel flow"
#define RM_ATTR_DAMAGE_REPAIR_FACTOR            "damage repair factor"
#define RM_ATTR_PITSTOP_BASE_TIME               "pitstop base time"
#define RM_ATTR_ALL_TIRES_CHANGE_TIME           "all tires change time"

#define RM_ATTR_POINTS		"points"

#define RM_VAL_TRACKSEL		"track select"
#define RM_VAL_DRVSEL		"drivers select"
#define RM_VAL_RACECONF		"race config"
#define RM_VAL_ANYRACE		"All Sessions"

#ifdef CLIENT_SERVER
#define RM_VAL_CLIENTCONF	"client config"
#define RM_VAL_SERVERCONF	"server config"
#endif

#define RM_VAL_CONFRACELEN	   "race length"
#define RM_VAL_CONFDISPMODE	   "display mode"
#define RM_VAL_CONFTIMEOFDAY   "time of day"
#define RM_VAL_CONFCLOUDCOVER  "cloud cover"
#define RM_VAL_CONFRAINFALL	   "rain fall"
#define RM_VAL_CONFWEATHER     "weather"

#define RM_CONF_RACE_LEN	0x0001
#define RM_CONF_DISP_MODE	0x0002
#define RM_CONF_TIME_OF_DAY	0x0004
#define RM_CONF_CLOUD_COVER	0x0008
#define RM_CONF_RAIN_FALL	0x0010
#define RM_CONF_WEATHER     0x0016

#define RM_VAL_DRV_LIST_ORDER	"drivers list"
#define RM_VAL_LAST_RACE_ORDER	"last race"
#define RM_VAL_LAST_RACE_RORDER	"last race reversed"

#define RM_VAL_RACE		"race"
#define RM_VAL_QUALIF		"qualifications"
#define RM_VAL_PRACTICE		"practice"

#define RM_VAL_YES		"yes"
#define RM_VAL_NO		"no"

#define RM_VAL_VISIBLE		"normal"
#define RM_VAL_INVISIBLE	"results only"
#define RM_VAL_SIMUSIMU		"simulation simulation"

#define RM_VAL_TIME_DAWN	"dawn"
#define RM_VAL_TIME_MORNING	"morning"
#define RM_VAL_TIME_NOON	"noon"
#define RM_VAL_TIME_AFTERNOON	"afternoon"
#define RM_VAL_TIME_DUSK	"dusk"
#define RM_VAL_TIME_NIGHT	"night"
#define RM_VAL_TIME_NOW		"now"
#define RM_VAL_TIME_TRACK	"from track"
#define RM_VAL_TIME_RANDOM	"random"
#define RM_VAL_TIME_24HR	"24 hour"
#define RM_VAL_TIME_REAL	"real"
#define RM_VALS_TIME { RM_VAL_TIME_DAWN, RM_VAL_TIME_MORNING, RM_VAL_TIME_NOON, \
    RM_VAL_TIME_AFTERNOON, RM_VAL_TIME_DUSK, RM_VAL_TIME_NIGHT, \
    RM_VAL_TIME_NOW, RM_VAL_TIME_TRACK, RM_VAL_TIME_RANDOM, \
    RM_VAL_TIME_24HR, RM_VAL_TIME_REAL}

 // Indices for RM_VALS_TIME.
#define RM_IND_TIME_DAWN	0
#define RM_IND_TIME_MORNING	1
#define RM_IND_TIME_NOON	2
#define RM_IND_TIME_AFTERNOON	3
#define RM_IND_TIME_DUSK	4
#define RM_IND_TIME_NIGHT	5
#define RM_IND_TIME_NOW		6
#define RM_IND_TIME_TRACK	7
#define RM_IND_TIME_RANDOM	8
#define RM_IND_TIME_24HR	9
#define RM_IND_TIME_REAL	10

#define RM_VAL_CLOUDS_NONE	    "no cloud"
#define RM_VAL_CLOUDS_CIRRUS    "cirrus"
#define RM_VAL_CLOUDS_FEW	    "few clouds"
#define RM_VAL_CLOUDS_MANY	    "many clouds"
#define RM_VAL_CLOUDS_CUMULUS   "cumulus"
#define RM_VAL_CLOUDS_SCARCE	"scarce clouds"
#define RM_VAL_CLOUDS_BROKEN    "broken"
#define RM_VAL_CLOUDS_FULL	    "full cover"
#define RM_VAL_CLOUDS_RANDOM	"random"
#define RM_VAL_CLOUDS_REAL	    "real"
#define RM_VALS_CLOUDS { RM_VAL_CLOUDS_NONE, RM_VAL_CLOUDS_CIRRUS, RM_VAL_CLOUDS_FEW, RM_VAL_CLOUDS_MANY, RM_VAL_CLOUDS_CUMULUS, RM_VAL_CLOUDS_SCARCE, RM_VAL_CLOUDS_BROKEN, RM_VAL_CLOUDS_FULL, RM_VAL_CLOUDS_RANDOM }

#define RM_VAL_RAIN_NONE	"none"
#define RM_VAL_RAIN_LITTLE	"little"
#define RM_VAL_RAIN_MEDIUM	"medium"
#define RM_VAL_RAIN_HEAVY	"heavy"
#define RM_VAL_RAIN_RANDOM	"random"
#define RM_VAL_RAIN_REAL	"real"
#define RM_VALS_RAIN { RM_VAL_RAIN_NONE, RM_VAL_RAIN_LITTLE, RM_VAL_RAIN_MEDIUM, RM_VAL_RAIN_HEAVY, RM_VAL_RAIN_RANDOM }

#define RM_VAL_WEATHER_CONFIG   "config"
#define RM_VAL_WEATHER_RECORDED "recorded"
#define RM_VAL_WEATHER_REAL     "real"
#define RM_VALS_WEATHER { RM_VAL_WEATHER_CONFIG, RM_VAL_WEATHER_RECORDED, RM_VAL_WEATHER_REAL }

/* Movie capture */

#define RM_SECT_MOVIE_CAPTURE	"Movie Capture"

#define RM_ATT_CAPTURE_ENABLE	"enable capture"
#define RM_ATT_CAPTURE_FPS	"fps"
#define RM_ATT_CAPTURE_OUT_DIR	"output directory"

#define RM_SECT_SUBFILES	"Header/Subfiles"

#define RM_ATTR_HASSUBFILES	"has subfiles"
#define RM_ATTR_FIRSTSUBFILE	"first subfile"
#define RM_ATTR_SUFFIX		"suffix"
#define RM_ATTR_SUBFILE_SUFFIX	"subfile suffix"
#define RM_ATTR_LASTSUBFILE	"islast"
#define RM_ATTR_NEXTSUBFILE	"next subfile"
#define RM_ATTR_PREVSUBFILE	"prev subfile"
#define RM_ATTR_RESULTSUBFILE	"result subfile"
#define RM_ATTR_SUBFILE		"subfile"

#define RM_SECT_ENDOFSEASON	"End-Of-Season"
#define RM_SECT_ENDOFSEASON_CLASSPOINTS	"End-Of-Season/Class Points"

#define RM_SECT_FIRSTNAME	"Names/First Name"
#define RM_SECT_LASTNAME	"Names/Last Name"
#define RM_SECT_DRIVERINFO	"Driver Info"

/* Race Engine modules */

#define RM_SECT_MODULES	"Modules"

#define RM_ATTR_MOD_TRACK		"track"
#define RM_ATTR_MOD_SIMU		"simu"
#define RM_ATTR_MOD_GRAPHIC		"graphic"

#define RM_VAL_MOD_SIMU_V2		"simuv2"
#define RM_VAL_MOD_SIMU_V2_1	"simuv2.1"
#define RM_VAL_MOD_SIMU_V3		"simuv3"
#define RM_VAL_MOD_SIMU_V4		"simuv4"
#define RM_VAL_MOD_SIMU_REPLAY		"simureplay"
#define RM_VAL_MOD_TRACK		"track"
#define RM_VAL_MOD_SSGRAPH		"ssggraph"
#define RM_VAL_MOD_OSGGRAPH		"osggraph"

/* Replay Settings */

#define RM_VAL_REPLAY_OFF		"0"
#define RM_VAL_REPLAY_LOW		"2"
#define RM_VAL_REPLAY_NORMAL		"10"
#define RM_VAL_REPLAY_HIGH		"30"
#define RM_VAL_REPLAY_PERFECT		"100"

/* Race Engine itself */

#define RM_SECT_RACE_ENGINE	"Race Engine"

#define RM_ATTR_MULTI_THREADING		"multi-threading"
#define RM_ATTR_THREAD_AFFINITY		"thread affinity"
#define RM_ATTR_REPLAY_RATE		"replay rate"

#define RM_ATTR_STARTPAUSED		"startpaused"
#define RM_ATTR_COOLDOWN		"cooldown"

#define RM_VAL_AUTO		"auto"
#define RM_VAL_ON		"on"
#define RM_VAL_OFF		"off"

/* Results */

#define RE_SECT_HEADER		"Header"
#define RE_ATTR_DATE		"date"

#define RE_SECT_CURRENT		"Current"
#define RE_ATTR_CUR_RACE	"current race"
#define RE_ATTR_CUR_TRACK	"current track"
#define RE_ATTR_CUR_DRIVER	"current driver"
#define RE_ATTR_CUR_FILE	"current file"
#define RE_ATTR_PREV_FILE	"previous file"
#define RE_ATTR_CUR_SEASON	"current season"

#define RE_SECT_DRIVERS		"Drivers"
#define RE_SECT_DRIVER		"Driver"
#define RE_ATTR_DLL_NAME	"dll name"
#define RE_ATTR_INDEX		"index"

#define RE_SECT_STANDINGS	"Standings"

#define RE_SECT_RESULTS		"Results"
#define RE_SECT_STARTINGGRID	"Starting Grid"

#define RE_SECT_QUALIF		"Qualifications"

#define RE_SECT_FINAL		"Final"

#define RE_SECT_RANK		"Rank"

#define RE_SECT_TEAMINFO	"Team Info"

#define RE_ATTR_NAME		"name"
#define RE_ATTR_SNAME		"short name"
#define RE_ATTR_CAR		"car"
#define RE_ATTR_MODULE		"module"
#define RE_ATTR_IDX		"idx"
#define RE_ATTR_LAPS		"laps"
#define RE_ATTR_BEST_LAP_TIME	"best lap time"
#define RE_ATTR_TIME		"time"
#define RE_ATTR_SESSIONTIME	"session time"
#define RE_ATTR_TOP_SPEED	"top speed"
#define RE_ATTR_BOT_SPEED	"bottom speed"
#define RE_ATTR_DAMMAGES	"damages"
#define RE_ATTR_NB_PIT_STOPS	"pit stops"
#define RE_ATTR_POINTS		"points"
#define RE_ATTR_PENALTYTIME	"penaltytime"

#define RE_SECT_CLASSPOINTS	"Class Points"

#endif /* _RACEMANV1_H_ */

