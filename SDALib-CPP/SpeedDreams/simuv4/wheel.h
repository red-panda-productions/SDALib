/***************************************************************************

    file                 : wheel.h
    created              : Sun Mar 19 00:09:18 CET 2000
    copyright            : (C) 2000 by Eric Espie
    email                : torcs@free.fr
    version              : $Id: wheel.h 3087 2010-11-03 23:42:34Z kakukri $

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef _WHEEL_H__
#define _WHEEL_H__

#include "differential.h"

typedef struct
{
    /* internal data */
    tSuspension  susp;		/* associated suspension */
    tBrake       brake;		/* associated brake disk */

    /* dynamic */
    t3Dd	forces;		/* forces acting on car */
    t3Dd	torques;	/* torques acting on car (gyroscopic forces) */
    tdble   torqueAlign;  /* torque for force feedback from magic formula */
    tdble	rollRes;	/* Rolling Resistance (summed over the car) */
    tdble	rideHeight;	/* height of the bottom of the car */
    tdble	zRoad;		/* z of the road */
    t3Dd   	pos;	   	/* world related */
    t3Dd	bodyVel;	/* world related */
    tdble  	driveTq;   	/* engine torque */
    tdble	vt;

    tdble  	spinTq;		/* spin torque feedback */
    tdble  	spinVel;   	/* spin velocity */
    tdble  	prespinVel;   	/* spin velocity */
    int     	state;     	/* wheel state */
    /* 1 and 2 are for suspension state */
#define SIM_WH_SPINNING 4	/* the wheel is spinning */
#define SIM_WH_LOCKED   8	/* the wheel is locked */
#define SIM_WH_INAIR   16   /* the wheel is in the air */
    tdble	axleFz;		/* force from axle (anti-roll bar) */
    tdble   axleFz3rd;  /* force from axle (3rd/heave spring) */
    tTrkLocPos	trkPos;		/* current track position */
    tPosd	relPos;		/* relative pos / GC */
    tdble	sa;		/* slip angle */
    tdble	sx;		/* longitudinal slip value */
    tdble	steer;

    /* static */
    tPosd	staticPos;	/* pos relative to the GC (z is suspension travel at rest) */
                /* and angles are camber (ax), caster (ay) and toe (az) */
    tdble   cosax, sinax;/*cosinus and sinus of relPos.ax*/

    tdble  	weight0;	/* initial weight on this wheel */
    tdble	tireSpringRate;
    tdble  	radius;
    tdble  	mu;
    tdble  	I;       	/* I = inertial moment of the wheel */
    tdble  	curI;       	/* Current inertia for the wheel (including transmission) */
    tdble	mfC;		/* Magic Formula C coeff */
    tdble	mfB;		/* Magic Formula B coeff */
    tdble	mfE;		/* Magic Formula E coeff */
    tdble	lfMax;		/* Load factor */
    tdble	lfMin;		/* Load factor */
    tdble	lfK;		/* Load factor */
    tdble	opLoad;		/* Operating load */
    tdble   AlignTqFactor; /* aligning torque factor */
    tdble	mass;		/* total wheel mass (incl. brake) (unsprung mass) */
    tdble	camber;		/* camber, negative toward exterior on both sides */
    tdble	pressure;	/* tire pressure */

    tdble   Ttire;      /* tire temperature in K */
    tdble   Topt;       /* optimal temperature in K, where mu maximal */
    tdble   Tinit;      /* initial tire temperature, right after pit or at start */
    tdble   muTmult;    /* mu = mumax * (1 - muTmult*(T-Topt)^2) */
    tdble   heatingm;   /* heating multiplier */
    tdble   aircoolm;   /* air cooling multiplier */
    tdble   speedcoolm; /* how aircoolm increases with speed */
    tdble   wearrate; /* degradation multiplier */
    tdble   treadDepth; /* tread depth, between 0 and 1 */
    tdble   critTreadDepth; /* critical tread depth, when grip falls off suddenly */
    tdble   muTDmult[2]; /* mu is multiplied by muTDmult[i]*treadDepth+muTDoffset[i] */
    tdble   muTDoffset[2];

    tDynAxis	in;
    tDynAxis	feedBack;

    tdble	preFn, preFt;
} tWheel;

#endif /* _WHEEL_H__ */
