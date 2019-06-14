/*****************************************************************************
* Copyright (C) 2019 by Gabe Warriner
*
* Redistribution, modification or use of this software in source or binary
* forms is permitted as long as the files maintain this copyright. Users are
* permitted to modify this and use it to learn about the field of embedded
* software. Jon Warrinerk and the University of Colorado are not liable for
* any misuse of this material.
*
*****************************************************************************/
/**
* @file angles.h
* @brief An abstraction for the angle calculation module
*
* This header file provides an abstraction of the functions to
* calculate angles
*
* @author Jon Warriner
* @date June 12 2019
* @version 1.0
*
*/

#ifndef ANGLES_H_
#define ANGLES_H_

#include <stdint.h>
#include "MMA8451Q.h"

typedef struct _ANGLE_DATA_
{
	int16_t roll;
	int16_t pitch;
	int16_t yaw;
}ANGLE_DATA;

/**
* @brief Calculate angles
*
* Calculate angles from acceleration data.
*
* @return void.
*/
void Calc_angles(MMA8451Q_DATA *acc, ANGLE_DATA *ang);


#endif /* ANGLES_H_ */
