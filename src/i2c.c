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
* @file i2c.c
* @brief i2c driver
*
* This source file implements a i2c driver
*
* @author Jon Warriner
* @date May 9, 2019
* @version 1.0
*
*/

#include "MKL25Z4.h"
#include "i2c.h"

void I2C_init(I2C_Packet *packet)
{

    //I2C0 = 1 - I2C0 clock enabled
    //Don't mess with other bits
    SIM->SCGC4 |= SIM_SCGC4_I2C0(1);

    //PORTE = 1 - PORTE clock enabled
    SIM->SCGC5 |= SIM_SCGC5_PORTE(1);

    //Configure I2C0 pins
    //MUX = 101 - PTE24 configured as ALT5 I2C0_SCL
    PORTE->PCR[24] = PORT_PCR_MUX(5);
    //MUX = 101 - PTE25 configured as ALT5 I2C0_SDA
    PORTE->PCR[25] = PORT_PCR_MUX(5);

    // Make sure I2C is disabled
	I2C0->C1 &= ~I2C_C1_IICEN(1);

	// I2C Frequency Divider Register
	// Multiplier Factor(mul) = 1
	// Clock Rate = 24MHz/(64*1) = 375 kbps
	// SDA = (1/24MHz)*1*13 =  0.5417 us
	// SCL Start = (1/24MHz)*1*26 =  1.083 us
	// SCL Stop = (1/24MHz)*1*33 =  1.375 us
	I2C0->F = I2C_F_MULT(0) | I2C_F_ICR(0x12);

	// I2C Control Register 1
	// I2C Enabled
	// I2C Interrupts Enabled
	// Master Mode
	// Transmit Mode = Transmit
	// Transmit Acknowledgement Enabled
	// Repeat Off
	// Normal Operation
	// DMA Disabled (Could be used in the future)
	I2C0->C1 &= ~I2C_C1_MST(1);		//clear master mode
	I2C0->C1 = I2C_C1_IICEN(1) | I2C_C1_IICIE(1);

	// I2C Control Register 2
	// General Call Address Disabled
	// 7-Bit Address Scheme
	// High Drive Mode
	// Slave Baud Rate Follows Master Baud Rate
	// Range Mode Disabled (For Slave Only)
	// Slave Address, Upper 3 Bits = 0b000 (For Slave Only)
	I2C0->C2 = I2C_C2_HDRS(1);

	// Input Glitch Filter Register
	// Stop Hold Enabled
	// Stop Detection Interrupt Disabled
	// No Filter/Bypass
	I2C0->FLT = I2C_FLT_SHEN(1);

	// Range Address Register (For Slave Only)
	I2C0->RA = I2C_RA_RAD(0x00);

	// SMBus Control and Status Register (Disabled)
	I2C0->SMB = 0x00;

	// if the bus is busy, clear MST
	if(I2C0->S & I2C_S_BUSY_MASK)
		I2C0->C1 &= ~I2C_C1_MST(1);

	// clear ARBL
	if(I2C0->S & I2C_S_ARBL_MASK)
		I2C0->S &= I2C_S_ARBL_MASK;

	packet->state = WR_ADDRESS;
	packet->i2c_callback = 0;
}

uint8_t I2C_Read(I2C_Packet *packet)
{
	volatile uint8_t temp;

	// I2C Transfer done?
	temp = I2C_Transfer_Complete();

	if(temp > 0)
	{
		__asm("bkpt");
		return temp;
	}

	// Ensure the bus isn't busy
	temp = I2C_Check_Busy();

	if(temp > 0)
	{
		__asm("bkpt");
		return temp;
	}

	// MST bit should be clear by the time we want to start another message
	temp =  I2C_Check_MST();

	if(temp > 0)
	{
		__asm("bkpt");
		return temp;
	}

	// Return to Transmit Mode
	I2C0->C1 |= I2C_C1_TX_MASK;

	// Start-bit generation
	I2C0->C1 |= I2C_C1_MST_MASK;

	// Transmit data first byte which is slave address
	// This kicks off the I2C transaction which will be
	// further handled by a state machine running in
	// the background loop or an ISR
	packet->read_write_n = 1;
	packet->state = WR_COMMAND;
	I2C0->D = (packet->slaveAddress << 1);

	return 0;
}

uint8_t I2C_Write(I2C_Packet *packet)
{
	volatile uint8_t temp;

	// Any previous I2C Transfers done?
	temp = I2C_Transfer_Complete();

	if(temp > 0)
	{
		__asm("bkpt");
		return temp;
	}

	// Ensure the bus isn't busy
	temp = I2C_Check_Busy();

	if(temp > 0)
	{
		__asm("bkpt");
		return temp;
	}

	// MST bit should be clear by the time we want to start another message
	temp =  I2C_Check_MST();

	if(temp > 0)
	{
		__asm("bkpt");
		return temp;
	}

//#ifndef I2C_LOG
//	// calculate the PEC
//	packet->PEC = I2C_Calc_PEC(packet, 0);
//#endif

	// Return to Transmit Mode
	I2C0->C1 |= I2C_C1_TX_MASK;

	// Start-bit generation
	I2C0->C1 |= I2C_C1_MST_MASK;

	// Transmit data first byte which is slave address
	// This kicks off the I2C transaction which will be
	// further handled by a state machine running in
	// the background loop or an ISR
	packet->read_write_n = 0;
	packet->state = WR_COMMAND;
	I2C0->D = (packet->slaveAddress << 1);

	return 0;
}

uint8_t I2C_Transfer_Complete()
{
	volatile uint8_t temp;
	volatile uint16_t waitCount;

	waitCount = 0;

	// Wait for any I2C transfer to finish
	while(!(I2C0->S & I2C_S_TCF_MASK))
	{
		waitCount++;

		if(waitCount == I2C_WAIT_COUNT)
		{
			if((I2C0->C1 & I2C_C1_TX_MASK) == I2C_C1_TX_MASK)
				I2C0->D = 0xFF;
			else
				temp = I2C0->D;

			__asm("bkpt");
			return 2;
		}
	}

	return 0;
}

uint8_t I2C_Check_Busy()
{
	volatile uint8_t temp;
	volatile uint16_t waitCount;

	waitCount = 0;

	// Ensure we aren't busy
	while((I2C0->S & I2C_S_BUSY_MASK))
	{
		waitCount++;

		if(waitCount == I2C_WAIT_COUNT)
		{
			if((I2C0->C1 & I2C_C1_TX_MASK) == I2C_C1_TX_MASK)
				I2C0->D = 0xFF;
			else
				temp = I2C0->D;

			__asm("bkpt");
			return 2;
		}
	}

	return 0;
}

uint8_t I2C_Check_MST()
{
	uint8_t error = 0;
	if(I2C0->C1 & I2C_C1_MST_MASK)
	{
		error = 2;
		__asm("bkpt");
	}

	return(error);
}

void Run_I2C_Master(I2C_Packet *packet)
{
	volatile uint8_t dummy;

	// Clear Interrupt flag
	I2C0->S |= I2C_S_IICIF_MASK;

	// TX or RX mode?
	if(I2C0->C1 & I2C_C1_TX_MASK)
	{
		// This is the case for TX mode

		// Did we not receive an
		// ACK from the receiver?  If not, we want to
		// generate a stop condition.
		if(I2C0->S & I2C_S_RXAK_MASK)
		{
			// reset the packet state
			packet->state = WR_ADDRESS;
			// The data is going to be invalid so don't execute the callback.
			packet->i2c_callback = 0;
			// Generate STOP signal
			I2C0->C1 &= ~I2C_C1_MST_MASK;
//			__asm("bkpt");
		}
		else
		{
			switch(packet->state)
			{
			case WR_ADDRESS:
//				__asm("bkpt");
				I2C0->D = packet->slaveAddress;
				packet->state++;
				break;
			case WR_COMMAND:
				I2C0->D = packet->command;
				if(packet->read_write_n)
				{
					packet->state = RD_ADDRESS;
				}
				else
				{
					packet->state++;
				}
				break;
			case WR_DATA1:
				I2C0->D = packet->data[0];
				packet->state++;
				break;
			case WR_DONE:
				// reset the packet state
				packet->state = WR_ADDRESS;
				// Generate STOP signal
				I2C0->C1 &= ~I2C_C1_MST_MASK;
				break;
			case RD_ADDRESS:
				// generate repeated start
				I2C0->C1 |= I2C_C1_RSTA_MASK;
				I2C0->D = (packet->slaveAddress << 1) | 0x1;
				packet->state++;
				break;
			case RD_SWITCH:
				I2C0->C1 &= ~I2C_C1_TX_MASK;		// switch to receive mode
				dummy = I2C0->D;
				packet->state++;
				break;
			default:
				// reset the packet state
				packet->state = WR_ADDRESS;
				// The data is going to be invalid so don't execute the callback.
				packet->i2c_callback = 0;
				// Generate STOP signal
				I2C0->C1 &= ~I2C_C1_MST_MASK;
//				__asm("bkpt");
				break;
			}
		}
	}
	else
	{
		// This is the case for RX mode
//		__asm("bkpt");

		switch(packet->state)
		{
		case RD_DATA:
			packet->state = WR_ADDRESS;
			// Generate STOP signal
			I2C0->C1 &= ~I2C_C1_MST_MASK;
			// switch back to TX mode before we read the data
			I2C0->C1 |= I2C_C1_TX_MASK;
			packet->data[0] = I2C0->D;
			break;
#ifdef JUNK
		case RD_DATA1:
			packet->data[0] = I2C0->D;
			packet->state++;
			break;
		case RD_DATA2:
			I2C0->C1 |= I2C_C1_TXAK_MASK;
			packet->data[1] = I2C0->D;
			packet->state++;
			break;
		case RD_PEC:
			// reset the packet state
			packet->state = WR_ADDRESS;
			// Generate STOP signal
			I2C0->C1 &= ~I2C_C1_MST_MASK;
			// switch back to TX mode before we read the last byte of data
			I2C0->C1 |= I2C_C1_TX_MASK;
			packet->PEC = I2C0->D;
			break;
#endif
		default:
			// reset the packet state
			packet->state = WR_ADDRESS;
			// The data is going to be invalid so don't execute the callback.
			packet->i2c_callback = 0;
			// Generate STOP signal
			I2C0->C1 &= ~I2C_C1_MST_MASK;
			dummy = I2C0->D;
			break;
		}
	}
//	dummy_flag = 0;
}

void I2C_POLL(I2C_Packet *packet)
{
	if(I2C0->S & I2C_S_IICIF_MASK)
	{
		Run_I2C_Master(packet);
	}
}

uint8_t Check_I2C_Callback(I2C_Packet *packet, void *p)
{
	uint8_t error = 0;
	if((packet->i2c_callback != 0) && (packet->state == WR_ADDRESS))
	{
		error = packet->i2c_callback(packet->data[0], p);
		// we only want to run the callback once
		packet->i2c_callback = 0;
		// make sure TXAK is cleared
		I2C0->C1 &= ~I2C_C1_TXAK_MASK;
	}
	return(error);
}

