/**
  ******************************************************************************
  * @file    radio_fsm.c
  * @author  MDS
  * @date    10-January-2016
  * @brief   nrf9051 SPI operations, initialisation and read/write functions
  ******************************************************************************
  *
  */

#include "board.h"

#include "processor_hal.h"

#include "nrf24l01plus.h"
#include "radio_fsm.h"

static int radio_fsm_current_state;

/**
  * @brief  Initialise radio FSM.
  * @param  None
  * @retval None
  */
void radio_fsm_init() {

	radio_fsm_current_state = RADIO_FSM_IDLE_STATE;
	nrf24l01plus_init();	// Set PWR_UP bit, enable CRC(2 unsigned chars) & Prim:TX. MAX_RT & TX_DS enabled..
}

/**
  * @brief  Radio FSM set state.
  * @param  None
  * @retval None
  */
int radio_fsm_setstate(int state) {

	int status;

	//Check conditions before setting state

	switch(state) {

		case RADIO_FSM_IDLE_STATE:	//IDLE state

			//Return to IDLE state if in TX or Wait state.
			if ((radio_fsm_current_state == RADIO_FSM_TX_STATE) || (radio_fsm_current_state == RADIO_FSM_WAIT_STATE) || (radio_fsm_current_state == RADIO_FSM_IDLE_STATE)) {
				radio_fsm_current_state = state;
				status = RADIO_FSM_OK;
			} else {
				status = RADIO_FSM_INVALIDSTATE;
			}			
			break;

		case RADIO_FSM_TX_STATE:		//Set to TX state
			if (radio_fsm_current_state == RADIO_FSM_IDLE_STATE) {	//put in transmiting mode, if in IDLE state
				radio_fsm_current_state = state;
				status = RADIO_FSM_OK;
			} else {
				status = RADIO_FSM_INVALIDSTATE;
			}			
			break;

		
		case RADIO_FSM_RX_STATE:		//Set to RX state

			if (radio_fsm_current_state == RADIO_FSM_IDLE_STATE) {	//put in receiving mode, if in IDLE state
				radio_fsm_current_state = RADIO_FSM_RX_STATE;	//got into waiting state
				
				HAL_Delayus(10);
				nrf24l01plus_ce();		//Set into RX mode

				radio_fsm_current_state = RADIO_FSM_WAIT_STATE;	//got into waiting state
				status = RADIO_FSM_OK;
			} else {
				status = RADIO_FSM_INVALIDSTATE;
			}			
			break;

		default:
			status = RADIO_FSM_INVALIDSTATE;
	}

	return status;

}

/**
  * @brief  Radio FSM get state.
  * @param  None
  * @retval None
  */
int radio_fsm_getstate() {

	return radio_fsm_current_state;
}


/**
  * @brief  Radio FSM read packet Function.
  *			Attempts to receive a packet and puts it into rx_buf. Returns 1 on a receive.
  * @param  RX buffer
  * @retval returns -1 if not in the RX state else returns 0
  */
int radio_fsm_read(uint8_t *rx_buf) {

	//Receive packet, if in RX state
	if (radio_fsm_current_state == RADIO_FSM_WAIT_STATE) {

		if (nrf24l01plus_recv(rx_buf) == 1) {		//If packet received, place in rx buffer and go to IDLE state

			radio_fsm_current_state = RADIO_FSM_IDLE_STATE;
			return RADIO_FSM_DONE;
		}

		return RADIO_FSM_OK;

	} else {

		//Return error
		return RADIO_FSM_ERROR;
	}
}

/**
  * @brief  NRF24l01Plus write Function.
  * @param  TX buffer
  * @retval returns -1 if not in the TX state else returns 0
  */
int radio_fsm_write(uint8_t *tx_buf) {

	//Transmit packet, if in TX state
	if (radio_fsm_current_state == RADIO_FSM_TX_STATE) {

		nrf24l01plus_send(tx_buf);		//Transmit packet
		
		radio_fsm_current_state = RADIO_FSM_IDLE_STATE;	//Move to idle state

		return RADIO_FSM_OK;	//return ok

	} else {

		return RADIO_FSM_ERROR;	//Return error
	}
}


/**
  * @brief  FSM register operation.
  * @param  None
  * @retval Returns 1 if empty else return 0
  */
int _radio_fsm_reg(uint8_t operation, uint8_t regaddr, uint8_t *regvalue) {
    
	if (radio_fsm_current_state == RADIO_FSM_IDLE_STATE) {

		if (operation) {
			*regvalue = nrf24l01plus_rr(regaddr);		//read register
		} else {
			nrf24l01plus_wr(regaddr, *regvalue);	//write register
		}
		
		return RADIO_FSM_OK;

	} else {

		//Return error
		return RADIO_FSM_ERROR;
	}
}

/**
  * @brief  FSM buffer operation.
  * @param  None
  * @retval Returns 1 if empty else return 0
  */
int _radio_fsm_buf(uint8_t operation, uint8_t regaddr, uint8_t *bufvalue, int bufferlen) {
    
	if (radio_fsm_current_state == RADIO_FSM_IDLE_STATE) {

		if (operation) {
			nrf24l01plus_rb(regaddr, bufvalue, bufferlen);		//read register
		} else {
			nrf24l01plus_wb(regaddr, bufvalue, bufferlen);	//write register
		}
		
		return RADIO_FSM_OK;

	} else {

		//Return error
		return RADIO_FSM_ERROR;
	}
}
