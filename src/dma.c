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
* @file dma.c
* @brief DMA driver
*
* This source file implements a DMA driver
*
* @author Jon Warriner
* @date April 23, 2019
* @version 1.0
*
*/

#include "dma.h"

void DMAMUX_init()
{
    //DMAMUX = 1 - DMAMUX clock enabled
    //Don't mess with other bits
    SIM->SCGC6 |= SIM_SCGC6_DMAMUX(1);

	//ENBL = 1 - DMA channel is enabled
    //TRIG = 0 - Triggering is disabled. (Normal mode)
    //SOURCE = 0x28 - Source is ADC0
    DMAMUX0->CHCFG[0] = DMAMUX_CHCFG_ENBL(1) | DMAMUX_CHCFG_SOURCE(kDmaRequestMux0ADC0);
}

void DMA_init(uint16_t *sbuf, int32_t *dbuf, uint16_t size)
{

	DMAMUX_init();

	//DMA = 1 - DMA clock enabled
    //Don't mess with other bits
    SIM->SCGC7 |= SIM_SCGC7_DMA(1);

    DMA0->DMA[0].SAR = (uint32_t)sbuf;
    DMA0->DMA[0].DAR = (uint32_t)dbuf;

	//DMA Control Register 1
	//EINT = 1 - Interrupt signal is enabled.
	//ERQ = 1 - Enable peripheral request to initiate transfer
	//CS = 1 - Single read/write transfer per request
	//AA = 0 - Auto-align disabled
	//EADREQ = 0 - Asynchronous DMA requests disabled
	//SINC = 0 - No change to SAR after a successful transfer
	//SSIZE = 00 - Source size is 32-bit
	//DINC = 1 - The DAR increments by 1, 2, 4 depending upon the size of the transfer
	//DSIZE = 00 - Destination size is 32-bit
	//START bit not set (we will trigger DMA with the ADC peripheral)
	//No source address buffer
	//No destination address buffer
	//Do not clear ERQ bit
	//No channel-to-channel linking
    DMA0->DMA[0].DCR = DMA_DCR_EINT(1) | DMA_DCR_ERQ(1) | DMA_DCR_CS(1) | DMA_DCR_AA(0) | DMA_DCR_EADREQ(0) | \
    				   DMA_DCR_SINC(0) | DMA_DCR_SSIZE(0) | DMA_DCR_DINC(1) | DMA_DCR_DSIZE(0);

    DMA0->DMA[0].DSR_BCR = size;
}

