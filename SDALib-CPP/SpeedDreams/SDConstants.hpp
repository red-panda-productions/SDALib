#pragma once

#define FRNT_RGT	0	/**< front right */
#define FRNT_LFT	1	/**< front left */
#define REAR_RGT	2	/**< rear right */
#define REAR_LFT	3	/**< rear left */
#define FRNT		0	/**< front */
#define REAR		1	/**< rear */
#define RIGHT		0	/**< right */
#define LEFT		1	/**< left */

const double PI = 3.14159265358979323846;  /**< PI */

#ifndef MIN
#define MIN(x,y) ((x) < (y) ? (x) : (y))
#endif

#ifndef MAX
#define MAX(x,y) ((x) > (y) ? (x) : (y))
#endif

/** Angle normalization between -PI and PI */
#define NORM_PI_PI(x) 				\
do {						\
	while ((x) > PI) { (x) -= 2*PI; }	\
	while ((x) < -PI) { (x) += 2*PI; } 	\
} while (0)

#define TR_SL	0		/**< Start-Left corner */
#define TR_SR	1		/**< Start-Right corner */
#define TR_EL	2		/**< End-Left corner */
#define TR_ER	3		/**< End_Right corner */

#define TR_ZS	0		/**< Z Start angle */
#define TR_ZE	1		/**< Z End angle */
#define TR_YL	2		/**< Y Left angle */
#define TR_YR	3		/**< Y Right angle */
#define TR_XS	4		/**< X Start angle */
#define TR_XE	5		/**< X End angle */
#define TR_CS   6		/**< Center start angle */

#define RCM_MAX_DT_ROBOTS	0.02  //might need to be removed