/**
  ******************************************************************************
  * @file    radio/tx/main.c
  * @author  MDS
  * @date    21042023
  * @brief   Send 32 byte packets using the nrf9051plus radio transciever
  *			 See the nrf9051plus datasheet.
  ******************************************************************************
  *  
  */ 

/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "processor_hal.h"
#include "debug_log.h"
#include "radio_fsm.h"
#include "nrf24l01plus.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define IDLE_STATE		0
#define TX_STATE		1
#define WAITING_STATE	2
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t packetbuffer[32];	/* Packet buffer initialised to 32 bytes (max length) */

/* Private function prototypes -----------------------------------------------*/
void hardware_init();

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void) {

	int i;
	int current_state = IDLE_STATE;		//Current state of FSM
	uint8_t current_channel = 64;
	uint8_t tx_addr[] = {0x90, 0x00, 0x00, 0x47, 0xb4};

	HAL_Init();
	hardware_init();
	
		debug_log("start\n\r");

	/* Initialise radio FSM */ 
	radio_fsm_init();

	 // 	nrf24l01plus_wr(NRF24L01P_RF_CH, current_channel); 	// Select RF channel
	//nrf24l01plus_wb(NRF24L01P_WRITE_REG | NRF24L01P_TX_ADDR, tx_addr, 5);		  // Writes TX_Address to nRF24L01

	// Fill packet with 'dummy' alphabet data 
	//for (i = 0; i < 32; i++) {
	//	packetbuffer[i] = 0x00; //'A'+ (i%26);
	//}

	/* set radio FSM state to IDLE */
	radio_fsm_setstate(RADIO_FSM_IDLE_STATE);
    while (1) {		
	
		//Transmit FSM
		switch(current_state) {

			case IDLE_STATE:	//Idle state for reading current channel

				// Get current channel , if radio FSM is in IDLE State 
				if (radio_fsm_getstate() == RADIO_FSM_IDLE_STATE) {

					radio_fsm_register_read(NRF24L01P_RF_CH, &current_channel);	//Read channel
					debug_log("Current Channel %d\n\r", current_channel);

					current_state = TX_STATE;	//Set next state as TX state.

				} else {
	
						// if error occurs, set state back to IDLE state 
						debug_log("ERROR: Radio FSM not in Idle state\n\r");
						radio_fsm_setstate(RADIO_FSM_IDLE_STATE);
				}

				break;

			case TX_STATE:	//TX state for writing packet to be sent.

				// Put radio FSM in TX state, if radio FSM is in IDLE state 
				if (radio_fsm_getstate() == RADIO_FSM_IDLE_STATE) {

					if (radio_fsm_setstate(RADIO_FSM_TX_STATE) == RADIO_FSM_INVALIDSTATE) {
						debug_log("ERROR: Cannot set Radio FSM RX state\n\r");
						HAL_Delay(100);
					} else {

						// Fill packet with 'dummy' alphabet data 
						for (i = 0; i < 32; i++) {
							packetbuffer[i] = 'A'+ (i%26);
						}

						debug_log("sending...\n\r");

						// Send packet - radio FSM will automatically go to IDLE state, after write completes. 
						radio_fsm_write(&packetbuffer);				

						current_state = IDLE_STATE;		//set next state as Waiting state
					}
				} else {
	
						// if error occurs, set state back to IDLE state 
						debug_log("ERROR: Radio FSM not in Idle state\n\r");
						radio_fsm_setstate(RADIO_FSM_IDLE_STATE);
				}

				break;

		}		

    	HAL_Delay(1000); //Delay for 100ms.
		BRD_LEDGreenToggle();
  	}

}


/**
  * @brief Hardware Initialisation Function.
  * @param  None
  * @retval None
  */
void hardware_init() {

	BRD_debuguart_init();  //Initialise UART for debug log output


	BRD_LEDInit();		//Initialise Blue LED
	BRD_LEDGreenOff();	// Turn off LEDs


}

