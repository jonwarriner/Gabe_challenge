/*****************************************************************************
* Copyright (C) 2019 by Jon Warriner
*
* Redistribution, modification or use of this software in source or binary
* forms is permitted as long as the files maintain this copyright. Users are
* permitted to modify this and use it to learn about the field of embedded
* software. Jon Warrinerk and the University of Colorado are not liable for
* any misuse of this material.
*
*****************************************************************************/
/**
* @file MMA8451Q.h
* @brief An abstraction for the MMA8451Q 3-axis accelerometer driver
*
* This header file provides an abstraction of the functions to
* implement the MMA8451Q 3-axis accelerometer driver
*
* @author Jon Warriner
* @date June 4 2019
* @version 1.0
*
*/

#ifndef MMA8451Q_H_
#define MMA8451Q_H_

#include <stdint.h>
#include "i2c.h"

#define MMA8451Q_ADDR	0x1D

#define WHO_AM_I		0x0D
#define XYZ_DATA_CFG	0x0E

void Init_MMA8451Q();

uint8_t I2C_Read_WHO_AM_I(uint8_t slaveAddr, I2C_Packet *packet);
uint8_t I2C_Write_XYZ_DATA_CFG(uint8_t slaveAddr, uint8_t data, I2C_Packet *packet);


#endif /* MMA8451Q_H_ */
