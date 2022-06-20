/*
 * This program has been developed by students from the bachelor Computer Science at
 * Utrecht University within the Software Project course.
 * © Copyright Utrecht University (Department of Information and Computing Sciences)
 */

#pragma once

#define SD_FRNT_RGT	0	/**< front right */
#define SD_FRNT_LFT	1	/**< front left */
#define SD_REAR_RGT	2	/**< rear right */
#define SD_REAR_LFT	3	/**< rear left */
#define SD_FRNT		0	/**< front */
#define SD_REAR		1	/**< rear */
#define SD_RIGHT		0	/**< right */
#define SD_LEFT		1	/**< left */

#define SD_PI 3.14159265358979323846  /**< PI */

#ifndef SD_MIN
#define SD_MIN(x,y) ((x) < (y) ? (x) : (y))
#endif

#ifndef SD_MAX
#define SD_MAX(x,y) ((x) > (y) ? (x) : (y))
#endif

/** Angle normalization between -PI and PI */
#define SD_NORM_PI_PI(x) 				\
do {						\
	while ((x) > SD_PI) { (x) -= 2*SD_PI; }	\
	while ((x) < -SD_PI) { (x) += 2*SD_PI; } 	\
} while (0)

#define SD_TR_SL	0		/**< Start-Left corner */
#define SD_TR_SR	1		/**< Start-Right corner */
#define SD_TR_EL	2		/**< End-Left corner */
#define SD_TR_ER	3		/**< End_Right corner */

#define SD_TR_ZS	0		/**< Z Start angle */
#define SD_TR_ZE	1		/**< Z End angle */
#define SD_TR_YL	2		/**< Y Left angle */
#define SD_TR_YR	3		/**< Y Right angle */
#define SD_TR_XS	4		/**< X Start angle */
#define SD_TR_XE	5		/**< X End angle */
#define SD_TR_CS   6		/**< Center start angle */

#define SD_RCM_MAX_DT_ROBOTS	0.02  //might need to be removed