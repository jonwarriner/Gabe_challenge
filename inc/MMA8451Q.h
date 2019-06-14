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

/**
* enumeration of the MMA8451Q registers
*/
typedef enum
{
	STATUS = 0,
	OUT_X_MSB,
	OUT_X_LSB,
	OUT_Y_MSB,
	OUT_Y_LSB,
	OUT_Z_MSB,
	OUT_Z_LSB,
	Reserved_0x07,
	Reserved_0x08,
	F_SETUP,
	TRIG_CFG,
	SYSMOD,
	INT_SOURCE,
	WHO_AM_I,
	XYZ_DATA_CFG,
	HP_FILTER_CUTOFF,
	PL_STATUS,
	PL_CFG,
	PL_COUNT,
	PL_BF_ZCOMP,
	P_L_THS_REG,
	FF_MT_CFG,
	FF_MT_SRC,
	FF_MT_THS,
	FF_MT_COUNT,
	Reserved_0x19,
	Reserved_0x1A,
	Reserved_0x1B,
	Reserved_0x1C,
	TRANSIENT_CFG,
	TRANSIENT_SCR,
	TRANSIENT_THS,
	TRANSIENT_COUNT,
	PULSE_CFG,
	PULSE_SRC,
	PULSE_THSX,
	PULSE_THSY,
	PULSE_THSZ,
	PULSE_TMLT,
	PULSE_LTCY,
	PULSE_WIND,
	ASLP_COUNT,
	CTRL_REG1,
	CTRL_REG2,
	CTRL_REG3,
	CTRL_REG4,
	CTRL_REG5,
	OFF_X,
	OFF_Y,
	OFF_Z
} MMA8451Q_REG;


/**
* enumeration of the MMA8451Q state
*/
typedef enum
{
	MMA8451Q_INIT = 0,
	MMA8451Q_RUN
} MMA8451Q_STATE;

typedef struct _MMA8451Q_DATA_
{
	int16_t x_data;
	int16_t y_data;
	int16_t z_data;
}MMA8451Q_DATA;

typedef struct _MMA8451Q_DATA_SPLIT_
{
	uint8_t x_data_lsb;
	uint8_t x_data_msb;
	uint8_t y_data_lsb;
	uint8_t y_data_msb;
	uint8_t z_data_lsb;
	uint8_t z_data_msb;
}MMA8451Q_DATA_SPLIT;

typedef union
{
	MMA8451Q_DATA data;
	MMA8451Q_DATA_SPLIT sdata;
}XYZ_DATA;

/**
* define the MMA8451Q structured data type
*/
typedef struct _MMA8451Q_
{
	MMA8451Q_STATE state;
	uint8_t step;
	XYZ_DATA data;
} MMA8451Q;

uint8_t I2C_Read_WHO_AM_I(uint8_t slaveAddr, I2C_Packet *packet);
uint8_t I2C_Write_XYZ_DATA_CFG(uint8_t slaveAddr, uint8_t data, I2C_Packet *packet);
uint8_t I2C_Write_CTRL_REG1(uint8_t slaveAddr, uint8_t data, I2C_Packet *packet);

uint8_t I2C_Read_OUT_X_MSB(uint8_t slaveAddr, I2C_Packet *packet);
uint8_t I2C_Read_OUT_X_MSB_CB(uint8_t data, void *p);
uint8_t I2C_Read_OUT_X_LSB(uint8_t slaveAddr, I2C_Packet *packet);
uint8_t I2C_Read_OUT_X_LSB_CB(uint8_t data, void *p);
uint8_t I2C_Read_OUT_Y_MSB(uint8_t slaveAddr, I2C_Packet *packet);
uint8_t I2C_Read_OUT_Y_MSB_CB(uint8_t data, void *p);
uint8_t I2C_Read_OUT_Y_LSB(uint8_t slaveAddr, I2C_Packet *packet);
uint8_t I2C_Read_OUT_Y_LSB_CB(uint8_t data, void *p);
uint8_t I2C_Read_OUT_Z_MSB(uint8_t slaveAddr, I2C_Packet *packet);
uint8_t I2C_Read_OUT_Z_MSB_CB(uint8_t data, void *p);
uint8_t I2C_Read_OUT_Z_LSB(uint8_t slaveAddr, I2C_Packet *packet);
uint8_t I2C_Read_OUT_Z_LSB_CB(uint8_t data, void *p);


void Init_MMA8451Q(MMA8451Q *m, I2C_Packet *packet);
void Run_MMA8451Q(MMA8451Q *m, I2C_Packet *packet);
void Update_MMA8451Q(MMA8451Q *m, I2C_Packet *packet);



#endif /* MMA8451Q_H_ */
