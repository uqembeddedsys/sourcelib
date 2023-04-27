/**
  ******************************************************************************
  * @file    radio/rx/main.c
  * @author  MDS
  * @date    21032016
  * @brief   Receive 32 byte packets using the radio FSM driver
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
#define RX_STATE		1
#define WAITING_STATE	2
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t packetbuffer[32];	/* Packet buffer initialised to 32 bytes (max length) */

/* Private function prototypes -----------------------------------------------*/
void HardwareInit();

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void) {

	int i;
	int current_state = IDLE_STATE;		//Current state of FSM
	uint8_t current_channel;

	HAL_Init();
	HardwareInit();
	
	/* Initialise radio FSM */ 
	radio_fsm_init();

	/* set radio FSM state to IDLE */
	radio_fsm_setstate(RADIO_FSM_IDLE_STATE);
	
    while (1) {		
	
		//Receiving FSM
		switch(current_state) {

			case IDLE_STATE:	//Idle state for reading current channel

				/* Get current channel , if radio FSM is in IDLE State */
				if (radio_fsm_getstate() == RADIO_FSM_IDLE_STATE) {

					radio_fsm_register_read(NRF24L01P_RF_CH, &current_channel);	//Read channel
					debug_log("Current Channel %d\n\r", current_channel);

					current_state = RX_STATE;	//Set next state as RX state.

				} else {
	
						/* if error occurs, set state back to IDLE state */
						debug_log("ERROR: Radio FSM not in Idle state\n\r");
						radio_fsm_setstate(RADIO_FSM_IDLE_STATE);
				}

				break;

			case RX_STATE:	//RX state for putting radio transceiver into receive mode.

				/* Put radio FSM in RX state, if radio FSM is in IDLE or in waiting state */
				if ((radio_fsm_getstate() == RADIO_FSM_IDLE_STATE) || (radio_fsm_getstate() == RADIO_FSM_WAIT_STATE)) {

					if (radio_fsm_setstate(RADIO_FSM_RX_STATE) == RADIO_FSM_INVALIDSTATE) {
						debug_log("ERROR: Cannot set Radio FSM RX state\n\r");
						HAL_Delay(100);
					} else {
				
						current_state = WAITING_STATE;		//set next state as Waiting state
					}
				} else {
	
						/* if error occurs, set state back to IDLE state */
						debug_log("ERROR: Radio FSM not in Idle state\n\r");
						radio_fsm_setstate(RADIO_FSM_IDLE_STATE);
				}


				break;

			case WAITING_STATE:	//Waiting state for reading received packet.

				/* Check if radio FSM is in WAITING STATE */
				if (radio_fsm_getstate() == RADIO_FSM_WAIT_STATE) {

					/* Check for received packet and display  */
					if (radio_fsm_read(packetbuffer) == RADIO_FSM_DONE) {

						debug_log("Received: ");
						for (i = 0; i < 32; i++ ) {
							debug_log("%x ", packetbuffer[i]);
						}
						debug_log("\n\r");
					}

					radio_fsm_setstate(RADIO_FSM_IDLE_STATE);	//Set Radio FSM back to IDLE state.

					current_state = IDLE_STATE;
				}	

				break;
		}

		BRD_LEDGreenToggle();
		HAL_Delay(100); //Delay for 100ms.
	}

}


/**
  * @brief Hardware Initialisation Function.
  * @param  None
  * @retval None
  */
void HardwareInit() {

	BRD_debuguart_init();  //Initialise UART for debug log output

	BRD_LEDInit();		//Initialise LED
	/* Turn off LEDs */
	BRD_LEDGreenOff();

}


