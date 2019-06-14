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
* @file disp.h
* @brief An abstraction for the character counting and functions
*
* This header file provides an abstraction of the functions to
* count received characters and build a display.
*
* @author Jon Warriner
* @date April 9 2019
* @version 1.0
*
*/

#ifndef DISP_H_
#define DISP_H_

#include "ring.h"

/**
* define the counter and display structured data type
*/
typedef struct
{
	void (*transmit_trig)();	//pointer to a function to trigger transmission of display data
	ring_t *obuf;				//pointer to output ring buffer. This contains formatted output strings to be transmitted
	char sbuf[80];
	uint8_t trig;				//set to 1 to trigger the display update
	uint16_t i;					//index of sample that is currently being updated
	int16_t val;				//single value to display
}disp_t;

/**
* @brief Initialize the display module
*
* @param d pointer to a display structure
* @param obuf pointer to an output ring buffer
* @param tx_func pointer to a function to trigger transmission of the output buffer
*
* @return pointer to ring_t type or 0 on failure
*/
int32_t disp_init(disp_t *d, ring_t *obuf, void (*tx_func)());

/**
* @brief Build the results display to send out the serial port
*
* Display a single value
*
* @return void.
*/
void Display_task(disp_t *d);

/**
* @brief Trigger the display update
*
*
* @return void.
*/
__attribute__((always_inline)) static inline void Display_Trig(disp_t *d, uint16_t idx)
{
	//if pointer isn't initialized return without doing anything
	if(d == 0)
	{
		return;
	}

	d->trig = 1;
}

/**
* @brief Write a new display value and trigger a display update
*
*
* @return void.
*/
__attribute__((always_inline)) static inline void Display_New_Val(disp_t *d, int32_t val)
{
	//if pointer isn't initialized return without doing anything
	if(d == 0)
	{
		return;
	}

	d->val = val;
	d->trig = 1;
}

#endif /* DISP_H_ */
