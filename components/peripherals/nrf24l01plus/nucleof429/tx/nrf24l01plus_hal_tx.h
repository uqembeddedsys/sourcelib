/* -----------------------------------------------------------------------------
# Company name: CSSE3010 - Embedded Systems Design & Interfacing
# Project name: nRF24l01+ driver
# Module name : nrf24l01plus
# Functionality: Provides low level radio calls used to initialise and interface
#        with the nrf24l01+.
#
# Author name: Chris Rieger modified by MDS
# Creation date: 010712, 
# Revision date (name): 210316, 240925 MDS
# Changes implemented (date): 210316 Renamed functions and modified for new spec
#(Comments):
------------------------------------------------------------------------------*/

#ifndef _NRF24l01PLUS_HAL_H
#define _NRF24l01PLUS_HAL_H

#ifdef HAVE_PLATFORMCONF_H
#include "platform-conf.h"
#endif

#include "nrf24l01plus_reg.h"
#include "nrf24l01plus_hal_pin.h"


void nrf24l01plus_hal_send(uint8_t *tx_buf);
int nrf24l01plus_hal_txFifoEmpty(void);


#endif
