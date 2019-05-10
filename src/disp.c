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
* @file disp.c
* @brief display contents of a buffer
*
* This source file provides support to send the contents of a buffer out the serial port
*
* @author Jon Warriner
* @date April 25, 2019
* @version 1.0

*/

#include <stdio.h>
#include <string.h>
#include "disp.h"

int32_t disp_init(disp_t *d, ring_t *obuf, void (*tx_func)())
{
	//if any of the pointers are not initialized then return an error
	if((d == 0) || (obuf == 0))
	{
		return(-1);
	}

	//make sure other control variables are zero
	d->trig = 0;
	d->i = 0;

	//initialize pointers
	d->obuf = obuf;
	d->transmit_trig = tx_func;

	return(0);
}

/**
* @brief Build the results display to send out the serial port
*
* Display a single value
*
* @return void.
*/
void Display_task(disp_t *d)
{
	size_t i;

	//if pointer isn't initialized return without doing anything
	if(d == 0)
	{
		return;
	}

	//If trig is set then we have a new value to write.
	//However, if the tx buffer isn't empty then we are
	//still sending the last update.  We'll have to wait
	//and check again later.
	if((d->trig) && (entries(d->obuf) == 0))
	{
		sprintf(d->sbuf, "%c[2J%c[HAngle - %d\r\n", (char)27, (char)27, d->val);

		//move the string to the TX buffer
		for(i = 0; i <= strlen(d->sbuf); i++)
		{
			insert(d->obuf, d->sbuf[i]);
		}

		//kick off the transmit by enabling the interrupt
		d->transmit_trig();

		d->trig = 0;
	}
}

