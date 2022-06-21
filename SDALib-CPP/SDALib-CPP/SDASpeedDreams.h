#pragma once

#include "SDAData.hpp"
#include "SDAAction.hpp"

#define FLOAT_RELAXATION2(target, prev, rate) 			\
do {								\
    tdble __tmp__;						\
    __tmp__ = target;						\
    target = (prev) + (rate) * ((target) - (prev)) * 0.01f;	\
    prev = __tmp__;						\
} while (0)

typedef struct
{
    tdble	spinVel;
    tdble	Tq;
    tdble	brkTq;
    tdble	I;
} tDynAxis;

typedef struct
{
    /* caracteristics */
    int		type;
#define DIFF_NONE		0
#define DIFF_SPOOL		1
#define DIFF_FREE		2
#define DIFF_LIMITED_SLIP	3
#define DIFF_VISCOUS_COUPLER	4
#define DIFF_15WAY_LSD  5
#define DIFF_ELECTRONIC_LSD 6
    tdble	ratio;
    tdble	I;
    tdble	efficiency;
    tdble	bias;
    tdble	dTqMin;
    tdble	dTqMax;
    tdble	dSlipMax;
    tdble   dCoastSlipMax;
    tdble	lockInputTq;
    tdble	viscosity;
    tdble	viscomax;

    /* dynamic data */
    tDynAxis	in;
    tDynAxis	feedBack;
    tDynAxis	*inAxis[2];
    tDynAxis	*outAxis[2];
} tDifferential;

typedef struct {
    tdble rads;
    tdble a;
    tdble b;
} tEngineCurveElem;

typedef struct {
    tdble		maxTq;
    tdble       maxPw;
    tdble       rpmMaxPw;
    tdble       TqAtMaxPw;
    tdble       rpmMaxTq;
    int			nbPts;
    tEngineCurveElem	*data;
} tEngineCurve;

typedef struct
{
    tEngineCurve	curve;
    tdble		revsLimiter;
    tdble		revsMax;
    tdble		tickover;
    tdble		I;
    tdble		rads; /* revs in rad/s ... */
    tdble		Tq; /* output torque */
    tdble		Tq_response; /* response Tq due to mismatch */
    tdble		I_joint; /* joint inertia */
    tdble		fuelcons;
    tdble		brakeCoeff; /* coefficient for constant engine brake */
    tdble		brakeLinCoeff; /* coefficient for RPM dependent engine brake */
    tdble		pressure;
    tdble		exhaust_pressure;
    tdble		exhaust_refract;
    tdble		timeInLimiter; /* time to still spend with fuel cut, in secundum */

    tdble		TCL;		// Optional TCL
    bool		EnableTCL;	// Enable optional TCL
} tEngine;

typedef struct
{
    int		gear;
    int		gearMin;
    int		gearMax;
    int		gearNext;
    tdble   shiftTime; /* time of shifting in sec */
    tdble   timeToEngage; /* time to engage gearNext, in sec */
} tGearbox;

typedef struct
{
    int		state;
#define CLUTCH_APPLIED	 1
#define CLUTCH_RELEASED  0
#define CLUTCH_RELEASING 2
    int		mode;
#define CLUTCH_AUTO	0
#define CLUTCH_MANUAL	1
    tdble	timeToRelease;	/* remaining time before releasing the clutch pedal */
    tdble	releaseTime;	/* time needed for releasing the clutch pedal */
    tdble	transferValue;	/* 1.0 -> released, 0.0 -> applied */
} tClutch;

typedef struct
{
    tGearbox	gearbox;
    tClutch	clutch;
    int		type;
#define TRANS_RWD	0
#define TRANS_FWD	1
#define TRANS_4WD	2
    tdble	overallRatio[MAX_GEARS];	/* including final drive ratio */
    tdble   gearI[MAX_GEARS];       /* raw gear inertia */
    tdble	driveI[MAX_GEARS];		/* Inertia (including engine) */
    tdble	freeI[MAX_GEARS];		/* Inertia when clutch is applied (wheels side) */
    tdble	gearEff[MAX_GEARS];		/* Gear Efficiency */
    tdble	curOverallRatio;
    tdble	curI;

#define TRANS_FRONT_DIFF	0
#define TRANS_REAR_DIFF		1
#define TRANS_CENTRAL_DIFF	2
    tDifferential	differential[3];
} tTransmission;

typedef struct
{
    tdble	pressure;
    tdble	Tq;
    tdble	coeff;
    tdble	I;
    tdble	radius;
    tdble	temp;

    // Options that can be enabled in simuV4
    // by parameters in the car type setup file

    // Add variables here (tdble)
    tdble	TCL;		// TCL factor
    tdble	ABS;		// ABS factor

    // Add switches here (bool)
    bool	EnableABS;	// Switch to enable ABS
} tBrake;

typedef struct
{
    tdble C1, b1, v1; /* coefs for slow */
    tdble C2, b2;     /* coefs for fast */
} tDamperDef;

typedef struct
{
    tDamperDef bump;
    tDamperDef rebound;
} tDamper;

typedef struct
{
    tdble K;          /* spring coef */
    tdble F0;         /* initial force */
    tdble x0;         /* initial suspension travel */
    tdble xMax;       /* maxi suspension travel */
    tdble bellcrank;  /* ratio of movement between wheel and suspension */
    tdble packers;     /* packer size (min susp. travel) */
} tSpring;

typedef struct Suspension
{
    tSpring spring;
    tDamper damper;
    tdble inertance;

    tdble x; /* suspension travel */
    tdble v; /* suspension travel speed */
    tdble a; /* suspension travel acceleration */

    tdble force;        /* generated force */
    int    state;        /* indicate the state of the suspension */
    /* Note: susp.state is merged with wheel.state in the code,
     * so SIM_SUSP and SIM_WH states cannot use the same bit */
#define SIM_SUSP_COMP   1  	/* the suspension is fully compressed */
#define SIM_SUSP_EXT    2  	/* the suspension is fully extended */
} tSuspension;


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

typedef struct
{
    tdble	steerLock;	/* in rad */
    tdble	maxSpeed;	/* in rad/s */
    tdble	steer;		/* current steer value */

} tSteer;

typedef struct
{
    tdble xpos;

    tSuspension arbSusp;	/* anti-roll bar */
    tSuspension heaveSusp;  /* heave/3rd/central spring */
    tdble	wheight0;

    /* dynamic */
    tdble	force[2]; /* right and left */

    tdble	I;	/* including differential inertia but not wheels */
} tAxle;

typedef struct
{
    tdble	rep;	/* front/rear repartition */
    tdble	coeff;
    tdble   ebrake_pressure;
} tBrakeSyst;

typedef struct
{
    /* dynamic */
    tdble	drag;		/* drag force along car x axis */
    tdble	lift[2];	/* front & rear lift force along car z axis */

    /* static */
    //tdble	SCx2;       replaced by CdBody as initial Cd
    tdble	Clift[2];	/* front & rear lift due to body not wings */
    tdble	Cd;		    /* for aspiration, value updated depending on wing angles */
    tdble	CdBody;	    /* for aspiration, value without wings, for variable wing angles */
} tAero;

typedef struct
{
    /* dynamic */
    t3Dd	forces;
    tdble	Kx;
    tdble	Kz;
    tdble	Kz_org;
    tdble	angle;

    /* static */
    t3Dd	staticPos;

    //>>> simuV4
    /* static for wings with const angle */
    tdble	AoAatMax;	/* [deg] Angle of Attack at the maximum of coefficient of lift */
    tdble	AoAatZero;	/* [deg] Angle of Attack at coefficient of lift = 0 (-30 < AoAatZero < 0) */
    tdble	AoAatZRad;	/* [rad] Angle of Attack at coefficient of lift = 0 (-30 < AoAatZero < 0) */
    tdble	AoAOffset;	/* [deg] Offset for Angle of Attack */

    tdble	CliftMax;	/* Maximum of coefficient of lift (0 < CliftMax < 4) */
    tdble	CliftZero;	/* Coefficient of lift at Angle of Attack = 0 */
    tdble	CliftAsymp;	/* Asymptotic coefficient of lift at large Angle of Attack */
    tdble	a;			/* [deg] Scaled angle at decreasing */
    tdble	b;			/* Delay of decreasing */
    tdble	c;			/* Curvature of start of decreasing */
    tdble	d;			/* Scale at AoA = 0 */
    tdble	f;			/* Scale factor */

    /* parameters for THIN wing model */
    tdble   AoStall;    /* angle of stall =15 deg (1 < AoStall < 45 in degrees) */
    tdble   Stallw;     /* stall width =2 deg, (1 < Stallw < AoStall) */
    tdble   AR;         /* effective aspect ratio of wing, 0 means infinite, must be positive */
    tdble   Kx1, Kx2, Kx3, Kx4;
    tdble   Kz1, Kz2, Kz3;

    int		WingType;	/* -1=None, 0=FLAT, 1=PROFILE, 2=THIN... */
    //<<< simuV4
} tWing;

typedef struct
{
    /* driver's interface */
    tCarCtrl	*ctrl;
    void	*params;
    tCarElt	*carElt;

    tCarCtrl	preCtrl;

    /* components */
    tAxle		axle[2];
    tWheel		wheel[4];
    tSteer		steer;
    tBrakeSyst		brkSyst;
    tAero		aero;
    tWing		wing[2];
    tTransmission	transmission;	/* includes clutch, gearbox and driveshaft */
    tEngine		engine;

    /* static */
    t3Dd	dimension;	/* car's mesures */
    tdble	mass;		/* mass with pilot (without fuel) */
    tdble	Minv;		/* 1 / mass with pilot (without fuel) */
    tdble	tank;		/* fuel tank capa */
    t3Dd	statGC;		/* static pos of GC */
    t3Dd	Iinv;		/* inverse of inertial moment along the car's 3 axis */

    /* dynamic */
    tdble	fuel;		/* current fuel load */
    tdble	fuel_consumption; /* average fuel consumption */
    tdble	fuel_prev;	/* average fuel consumption */
    tdble	fuel_time;	/* average fuel consumption */
    tDynPt	DynGC;		/* GC local data except position */
    tDynPt	DynGCg;		/* GC global data */
    tPosd	VelColl;	/* resulting velocity after collision */
    tDynPt	preDynGC;	/* previous one */
    tTrkLocPos	trkPos;		/* current track position */
    tdble	airSpeed2;	/* current air speed (squared) for aerodynamic forces */

    /* internals */
    tdble	Cosz;
    tdble	Sinz;
    tDynPt	corner[4];	/* x,y,z for static relative pos, ax,ay,az for dyn. world coord */
    int		collision;
    t3Dd	normal;
    t3Dd	collpos;
    tdble	wheelbase;
    tdble	wheeltrack;
    sgMat4	posMat;
    void*	shape;		/* for collision */
    int		blocked;		// Flag to show if the car has had already a collision in the same timestep.
    int		dammage;

    /* enabling features */
    int features;

    tDynPt	restPos;	/* target rest position after the car is broken */

    int		collisionAware;
} tCar;

class SDASpeedDreams
{
public:
    SDAData UpdateSimulator(const SDAData& p_data, SDAAction& p_action);

private:
    tCar CarConstructor(const tCar& p_other, tCarElt* p_carElt);
    tCar *SimCarTable = 0;
    tdble SimDeltaTime;
    tdble simSkidFactor[6]{0.4f, 0.3f, 0.2f, 0.1f, 0.0f, 0.0f};
    tdble simDammageFactor[6]{0.0f, 0.1f, 0.3f, 0.5f, 0.8f, 1.0f};
    void SimUpdate(tSituation *s, double deltaTime);
    tdble SimTimeOfDay = 0;
    double Tair = 273.15;;
    double simDegree = 0.000000023;
    double startDay = 0.0;
    double endDay = 0.0;
    void SimAtmosphereUpdate(tSituation *s);
    void RemoveCar(tCar *car, tSituation *s);
    void SimCollideRemoveCar(tCar *car, int nbcars);
    void RtTrackLocal2Global(tTrkLocPos *p, tdble *X, tdble *Y, int flag);
    tdble RtTrackHeightL(tTrkLocPos *p);
    tdble RtTrackSideTgAngleL(tTrkLocPos *p);
    tdble RtTrackGetWidth(tTrackSeg *seg, tdble toStart);
    void ctrlCheck(tCar *car);
    void SimInstantReConfig(tCar *car);
    void SimBrakeSystemReConfig(tCar *car);
    void SimArbReConfig(tCar *car, int index);
    void SimSteerUpdate(tCar *car);
    void SimGearboxUpdate(tCar *car);
    void SimEngineUpdateTq(tCar *car);
    void SimCarUpdateWheelPos(tCar *car);
    void SimBrakeSystemUpdate(tCar *car);
    void SimAeroUpdate(tCar *car, tSituation *s);
    void SimWingUpdate(tCar *car, int index, tSituation* s);
    void SimWheelUpdateRide(tCar *car, int index);
    tdble CliftFromAoA(tWing* wing);
    tdble F(tWing* wing);
    void RtTrackGlobal2Local(tTrackSeg *segment, tdble X, tdble Y, tTrkLocPos *p, int type);
    void SimSuspCheckIn(tSuspension *susp);
    void SimBrakeUpdate(tCar *car, tWheel *wheel, tBrake *brake);
    void SimAxleUpdate(tCar *car, int index);
    void SimSuspUpdate(tSuspension *susp);
    tdble springForce(tSuspension *susp);
    tdble damperForce(tSuspension *susp);
    void SimWheelUpdateForce(tCar *car, int index);
    void SimTransmissionUpdate(tCar *car);
    void SimDifferentialUpdate(tCar *car, tDifferential *differential, int first);
    void updateSpool(tCar *car, tDifferential *differential, int first);
    tdble SimEngineUpdateRpm(tCar *car, tdble axleRpm);
    float urandom();
    void SimUpdateFreeWheels(tCar *car, int axlenb);
    void SimWheelUpdateRotation(tCar *car);
    void SimCarUpdate(tCar *car, tSituation * /* s */);
    void SimCarUpdateForces(tCar *car);
    void SimCarUpdateSpeed(tCar *car);
    void SimCarUpdatePos(tCar *car);
    const tdble aMax = 1.0f;
    void SimCarUpdateCornerPos(tCar *car);
    void SimCarCollideZ(tCar *car);
    void RtTrackSurfaceNormalL(tTrkLocPos *p, t3Dd *norm);
    void SimCarCollideXYScene(tCar *car);
    void SimCarCollideCars(tSituation *s);
    void SimCarUpdate2(tCar *car, tSituation * /* s */);
    int SimTelemetry = -1;
    void SimTelemetryOut(tCar *car);
};





