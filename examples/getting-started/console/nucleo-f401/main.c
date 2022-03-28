/**
  ******************************************************************************
  * @file    console/main.c
  * @author  MDS
  * @date    27112020
  * @brief   Nucleo debug log example. Prints to the console, every 2 seconds.
  *          Run serial terminal program script - kermusb to view the output
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "processor_hal.h"
#include "debug_log.h"

void hardware_init(void);

/*
 * Main program - flashes onboard LEDs
 */
int main(void)  {

  uint32_t prev_tick;
  char recvChar;

	HAL_Init();			//Initalise Board
	hardware_init();	//Initalise hardware modules
	
  prev_tick = 0;

	// Main processing loop
  while (1) {
        
		// Display system time every 2 seconds
    if ((HAL_GetTick() - prev_tick) >= 2000) {
      prev_tick =  HAL_GetTick();
      debug_log("Sys time %d\n\r", HAL_GetTick());

    }

    //Check for key press (other than null character)
    if ((recvChar = BRD_debuguart_getc()) != '\0') {

      BRD_LEDGreenToggle();

      debug_log("Character: %c - ASCII Table value: %d\n\r", recvChar);
    }

	}

  return 0;
}

/*
 * Initialise Hardware
 */
void hardware_init(void) {

	BRD_LEDInit();		//Initialise LEDS

  BRD_debuguart_init();  //Initialise UART for debug log output

	// Turn off Green LED
	BRD_LEDGreenOff();
}
