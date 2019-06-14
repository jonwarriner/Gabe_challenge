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

uint8_t I2C_Write_CTRL_REG1(uint8_t slaveAddr, uint8_t data, I2C_Packet *packet)
{
	packet->command = CTRL_REG1;
	packet->slaveAddress = slaveAddr;

	packet->data[0] = data;

	return I2C_Write(packet);
}

uint8_t I2C_Read_OUT_X_MSB(uint8_t slaveAddr, I2C_Packet *packet)
{
	uint8_t error;

	packet->command = OUT_X_MSB;
	packet->slaveAddress = slaveAddr;
	packet->i2c_callback = I2C_Read_OUT_X_MSB_CB;

	error = I2C_Read(packet);

	return(error);
}

uint8_t I2C_Read_OUT_X_MSB_CB(uint8_t data, void *p)
{
	MMA8451Q *m = (MMA8451Q *)p;

	m->data.sdata.x_data_msb = data;

	return 0;
}

uint8_t I2C_Read_OUT_X_LSB(uint8_t slaveAddr, I2C_Packet *packet)
{
	uint8_t error;

	packet->command = OUT_X_LSB;
	packet->slaveAddress = slaveAddr;
	packet->i2c_callback = I2C_Read_OUT_X_LSB_CB;

	error = I2C_Read(packet);

	return(error);
}

uint8_t I2C_Read_OUT_X_LSB_CB(uint8_t data, void *p)
{
	MMA8451Q *m = (MMA8451Q *)p;

	m->data.sdata.x_data_lsb = data;

	return 0;
}

uint8_t I2C_Read_OUT_Y_MSB(uint8_t slaveAddr, I2C_Packet *packet)
{
	uint8_t error;

	packet->command = OUT_Y_MSB;
	packet->slaveAddress = slaveAddr;
	packet->i2c_callback = I2C_Read_OUT_Y_MSB_CB;

	error = I2C_Read(packet);

	return(error);
}

uint8_t I2C_Read_OUT_Y_MSB_CB(uint8_t data, void *p)
{
	MMA8451Q *m = (MMA8451Q *)p;

	m->data.sdata.y_data_msb = data;

	return 0;
}

uint8_t I2C_Read_OUT_Y_LSB(uint8_t slaveAddr, I2C_Packet *packet)
{
	uint8_t error;

	packet->command = OUT_Y_LSB;
	packet->slaveAddress = slaveAddr;
	packet->i2c_callback = I2C_Read_OUT_Y_LSB_CB;

	error = I2C_Read(packet);

	return(error);
}

uint8_t I2C_Read_OUT_Y_LSB_CB(uint8_t data, void *p)
{
	MMA8451Q *m = (MMA8451Q *)p;

	m->data.sdata.y_data_lsb = data;

	return 0;
}

uint8_t I2C_Read_OUT_Z_MSB(uint8_t slaveAddr, I2C_Packet *packet)
{
	uint8_t error;

	packet->command = OUT_Z_MSB;
	packet->slaveAddress = slaveAddr;
	packet->i2c_callback = I2C_Read_OUT_Z_MSB_CB;

	error = I2C_Read(packet);

	return(error);
}

uint8_t I2C_Read_OUT_Z_MSB_CB(uint8_t data, void *p)
{
	MMA8451Q *m = (MMA8451Q *)p;

	m->data.sdata.z_data_msb = data;

	return 0;
}

uint8_t I2C_Read_OUT_Z_LSB(uint8_t slaveAddr, I2C_Packet *packet)
{
	uint8_t error;

	packet->command = OUT_Z_LSB;
	packet->slaveAddress = slaveAddr;
	packet->i2c_callback = I2C_Read_OUT_Z_LSB_CB;

	error = I2C_Read(packet);

	return(error);
}

uint8_t I2C_Read_OUT_Z_LSB_CB(uint8_t data, void *p)
{
	MMA8451Q *m = (MMA8451Q *)p;

	m->data.sdata.z_data_lsb = data;

	return 0;
}

void Init_MMA8451Q(MMA8451Q *m, I2C_Packet *packet)
{
	if(packet->state == WR_ADDRESS)
	{
		switch(m->step)
		{
		case 0:
			I2C_Write_CTRL_REG1(MMA8451Q_ADDR, 0x01, packet);		// set to ACTIVE mode
			m->step++;
			break;
		case 1:
			I2C_Write_XYZ_DATA_CFG(MMA8451Q_ADDR, 0x00, packet);	//2g scaling
			m->step++;
			break;
		default:
			// we ran out of init stuff to do, switch to run mode
			m->step = 0;
			m->state = MMA8451Q_RUN;
			break;
		}
	}
}

void Run_MMA8451Q(MMA8451Q *m, I2C_Packet *packet)
{
	if(packet->state == WR_ADDRESS)
	{
		//I2C_Read_WHO_AM_I(MMA8451Q_ADDR, packet);
		switch(m->step)
		{
		case 0:
			I2C_Read_OUT_X_MSB(MMA8451Q_ADDR, packet);
			m->step++;
			break;
		case 1:
			I2C_Read_OUT_X_LSB(MMA8451Q_ADDR, packet);
			m->step++;
			break;
		case 2:
			I2C_Read_OUT_Y_MSB(MMA8451Q_ADDR, packet);
			m->step++;
			break;
		case 3:
			I2C_Read_OUT_Y_LSB(MMA8451Q_ADDR, packet);
			m->step++;
			break;
		case 4:
			I2C_Read_OUT_Z_MSB(MMA8451Q_ADDR, packet);
			m->step++;
			break;
		case 5:
			I2C_Read_OUT_Z_LSB(MMA8451Q_ADDR, packet);
			m->step++;
			break;
		case 6:
			m->step = 0;
			break;
		default:
			m->step = 0;
			break;
		}
	}
}
void Update_MMA8451Q(MMA8451Q *m, I2C_Packet *packet)
{
	if(m->state == MMA8451Q_INIT)
	{
		Init_MMA8451Q(m, packet);
	}
	else
	{
		Run_MMA8451Q(m, packet);
	}

}
