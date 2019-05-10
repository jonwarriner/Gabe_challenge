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
* @file dma.h
* @brief An abstraction for the DMA driver
*
* This header file provides an abstraction of the functions to
* implement the DMA driver
*
* @author Jon Warriner
* @date April 23 2019
* @version 1.0
*
*/


#ifndef DMA_H_
#define DMA_H_

#include <stdint.h>
#include "MKL25Z4.h"

/**
* @brief Initialize the DMA
*
* Initialize the DMA for the application
*
* @return void.
*/
void DMA_init(uint16_t *sbuf, int32_t *dbuf, uint16_t size);

/**
* @brief Clear the DONE flag
*
* Clear the DONE flag at the end of a set of transfers
*
* @return void.
*/
__attribute__((always_inline)) static inline void DMA_Clear_Done()
{
	DMA0->DMA[0].DSR_BCR = DMA_DSR_BCR_DONE(1);
}

/**
* @brief Reconfigure the DMA
*
* Reconfigure the DMA to start a new set of transfers
*
* @return void.
*/
__attribute__((always_inline)) static inline void DMA_Reconfig(uint16_t *sbuf, int32_t *dbuf, uint16_t size)
{
    DMA0->DMA[0].SAR = (uint32_t)sbuf;
    DMA0->DMA[0].DAR = (uint32_t)dbuf;

    DMA0->DMA[0].DSR_BCR = size;
}

#endif /* DMA_H_ */
