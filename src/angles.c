/*****************************************************************************
* Copyright (C) 2019 by Gabe Warriner
*
* Redistribution, modification or use of this software in source or binary
* forms is permitted as long as the files maintain this copyright. Users are
* permitted to modify this and use it to learn about the field of embedded
* software. Jon Warriner and the University of Colorado are not liable for
* any misuse of this material.
*
*****************************************************************************/
/**
* @file angles.c
* @brief Calculate angles from MMA8451Q 3-axis accelerometer data
*
* This source file implements angle calculation
*
* @author Gabe Warriner
* @date June 12, 2019
* @version 1.0
*
*/

#include "angles.h"

void Calc_angles(MMA8451Q_DATA *acc, ANGLE_DATA *ang)
{
	//Bogus test code just to illustrate how to access the data structures.
	//Here we are just copying the input data to the outputs.
	ang->pitch = acc->x_data;
	ang->roll = acc->y_data;
	ang->yaw = acc->z_data;
}

