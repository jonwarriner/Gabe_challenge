/*****************************************************************************
* Copyright (C) 2019 by Jon Warriner
*
* Redistribution, modification or use of this software in source or binary
* forms is permitted as long as the files maintain this copyright. Users are
* permitted to modify this and use it to learn about the field of embedded
* software. Jon Warriner and the University of Colorado are not liable for
* any misuse of this material.
*
*****************************************************************************/
/**
* @file MMA8451Q.c
* @brief MMA8451Q 3-axis accelerometer driver
*
* This source file implements a MMA8451Q driver
*
* @author Jon Warriner
* @date June 4, 2019
* @version 1.0
*
*/

#include "MMA8451Q.h"


uint8_t I2C_Read_WHO_AM_I(uint8_t slaveAddr, I2C_Packet *packet)
{
	uint8_t error;

	packet->command = WHO_AM_I;
	packet->slaveAddress = slaveAddr;
	//packet->i2c_callback = I2C_Read_UI_Status_CB;

	error = I2C_Read(packet);

	return(error);
}

uint8_t I2C_Write_XYZ_DATA_CFG(uint8_t slaveAddr, uint8_t data, I2C_Packet *packet)
{
	packet->command = XYZ_DATA_CFG;
	packet->slaveAddress = slaveAddr;

	packet->data[0] = data;

	return I2C_Write(packet);
}

void Init_MMA8451Q(I2C_Packet *packet)
{
	//I2C_Write_XYZ_DATA_CFG(MMA8451Q_ADDR, 0x00, packet);
	I2C_Read_WHO_AM_I(MMA8451Q_ADDR, packet);
}
