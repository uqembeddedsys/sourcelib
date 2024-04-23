/**
 ******************************************************************************
 * @file    exam0les/peripherals/nrf24l01plus/main.c
 * @author  MDS, NPG
 * @date    02022015, 07122017
 * @brief   Write to and read from the nrf24l01plus channel register and 
 *          read the nrf24L01plus status (0x07) register
 * 			NOTE: This example does not send or receive with the nrf24L01+.
 * 			CONNECTION: nrf24l01plus MUST be connected to the Side Board RA connector.
 * 			REFERENCE: nrf24L01plus datasheet
 * 			pins: CE - D9 (If MFS connected, pin is D9 but labeled ~9 on the PWM out pins)
 * 			MISO, MOSI, SCK and CSN - use side board RA connector (See lab 1.4)
 * 
 * 			IMPORTANT NOTE: You must power-cycle your board before it will work. 
 *                 			Disconnect the USB cable from your computer
 * 							Then reconnect the USB cable to your computer.
 ******************************************************************************
 *
 */

#include "board.h"
#include "processor_hal.h"
#include "debug_log.h"
#include "nrf24l01plus.h"

void hardware_init(void);

int main(void) {

	uint8_t status;

	HAL_Init(); // Initialise Board
	hardware_init(); // Initialise hardware peripherals

	/* Cyclic Executive loop */
	while (1) {

		
		nrf24l01plus_wr(NRF24L01P_RF_CH, 40);			//Write 40 to the radio channel register
		debug_log("nrf24L01plus Channel register value: %#.2x\r\n", nrf24l01plus_rr(NRF24L01P_RF_CH));	// See page 59 of nrf24L01+ datasheet for status register definition

		status = nrf24l01plus_rr(NRF24L01P_STATUS);							//read the status register
		debug_log("nrf24L01plus Status register value: %#.2x\r\n", status);	// See page 59 of nrf24L01+ datasheet for status register definition
																			// NOTE: default value of the status register is 0x0E

		BRD_LEDGreenToggle();                                             //Toggle Green LED on/off
		HAL_Delay(1000);                                                //Delay for 1s (1000ms)

	}
}

/**
 * Initialise Hardware
 */
void hardware_init(void) {

	BRD_debuguart_init();  //Initialise UART for debug log output

	BRD_LEDInit(); //Initialise LEDs

	/* Turn off LEDs */
	BRD_LEDGreenOff();

	nrf24l01plus_init();		//Initialise NRF24L01plus module (includes CE, CSN, MISO, MOSI & SCK pins)


}

