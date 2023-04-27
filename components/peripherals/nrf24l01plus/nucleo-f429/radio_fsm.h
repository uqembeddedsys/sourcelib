/**
  ******************************************************************************
  * @file    radio_fsm.h
  * @author  MDS
  * @date    10-January-2016
  * @brief   nrf9051 SPI operations, initialisation and read/write functions
  ******************************************************************************
  *
  */

#ifndef _RADIOFSM_H
#define _RADIOFSM_H

#ifdef HAVE_PLATFORMCONF_H
#include "platform-conf.h"
#endif


//State definitions
#define	RADIO_FSM_IDLE_STATE	0x00
#define	RADIO_FSM_TX_STATE		0x01
#define	RADIO_FSM_RX_STATE		0x02
#define	RADIO_FSM_WAIT_STATE	0x03

//Return status
#define	RADIO_FSM_OK		0
#define	RADIO_FSM_DONE		1
#define	RADIO_FSM_ERROR		-1
#define	RADIO_FSM_INVALIDSTATE		RADIO_FSM_ERROR	//-2

#define radio_fsm_buffer_write(regaddr, bufvalue, bufferlen)	_radio_fsm_buf(0, regaddr, bufvalue, bufferlen)
#define radio_fsm_buffer_read(regaddr, bufvalue, bufferlen)	_radio_fsm_buf(1, regaddr, bufvalue, bufferlen)

#define radio_fsm_register_write(regaddr, regvalue)	_radio_fsm_reg(0, regaddr, regvalue)
#define radio_fsm_register_read(regaddr, regvalue)	_radio_fsm_reg(1, regaddr, regvalue)

void radio_fsm_init();
int radio_fsm_setstate(int state);
int radio_fsm_getstate();
int radio_fsm_read(uint8_t *rx_buf);
int radio_fsm_write(uint8_t *tx_buf);

int _radio_fsm_reg(uint8_t operation, uint8_t regaddr, uint8_t *regvalue);
int _radio_fsm_buf(uint8_t operation, uint8_t regaddr, uint8_t *bufvalue, int bufferlen);

#endif
