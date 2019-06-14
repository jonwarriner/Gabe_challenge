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
* @file i2c.h
* @brief An abstraction for the i2c driver
*
* This header file provides an abstraction of the functions to
* implement the i2c driver
*
* @author Jon Warriner
* @date May 9 2019
* @version 1.0
*
*/

#ifndef I2C_H_
#define I2C_H_

#include <stdint.h>

#define I2C_WAIT_COUNT							(10000)

/**
* enumeration of states for the I2C master state-machine
*/
typedef enum
{
	WR_ADDRESS = 0,
	WR_COMMAND,
	WR_DATA1,
	WR_DONE,
	RD_ADDRESS,
	RD_SWITCH,
	RD_DATA
#ifdef JUNK
	RD_DATA1,
	RD_DATA2,
	RD_PEC
#endif
} I2C_STATE;

/**
* define the I2C Packet structured data type
*/
typedef struct _I2C_PACKET_
{
	uint8_t (*i2c_callback)(uint8_t, void *);
	I2C_STATE state;
	uint8_t read_write_n;					// read/write_n
	uint8_t slaveAddress;
	uint8_t command;
	uint8_t byteCount;
#ifdef I2C_LOG
	uint8_t data[14];
	uint8_t idx;
#else
	uint8_t data[4];
#endif
} I2C_Packet;

/**
* @brief Initialize I2C
*
* Initialize I2C for the application
*
* @return void.
*/
void I2C_init(I2C_Packet *packet);

/**
* @brief I2C Read
*
* Read from an I2C device
*
* @return error.
*/
uint8_t I2C_Read(I2C_Packet *packet);

/**
* @brief I2C Write
*
* Write to an I2C device
*
* @return error.
*/
uint8_t I2C_Write(I2C_Packet *packet);

/**
* @brief I2C Transfer Complete
*
* Has an I2C transfer completed?
*
* @return error.
*/
uint8_t I2C_Transfer_Complete();

/**
* @brief I2C Check Busy
*
* Is the I2C bus busy?
*
* @return error.
*/
uint8_t I2C_Check_Busy();

/**
* @brief I2C Check MST
*
* Is the I2C in master mode?
*
* @return error.
*/
uint8_t I2C_Check_MST();

/**
* @brief Run I2C Master
*
* Run the I2C Master state machine
*
* @return error.
*/
void Run_I2C_Master(I2C_Packet *packet);

/**
* @brief I2C Handler
*
* I2C functionality for the background loop
*
* @return void.
*/
void I2C_POLL(I2C_Packet *packet);

/**
* @brief I2C Callback Handler
*
* I2C Callback functionality for the background loop
*
* @return void.
*/
uint8_t Check_I2C_Callback(I2C_Packet *packet, void *p);

#endif /* I2C_H_ */
