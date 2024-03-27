/**
  ******************************************************************************
  * @file    console/main.c
  * @author  MDS
  * @date    27032022
  * @brief   Nucleo debug log example. Prints to the console, every 2 seconds.
  *          Will detect and print key press to console
  * 
  *          VM: Run serial terminal program script - kermusb
  * 
  *          Windows Native Installation: use Putty (Serial Connection/Port) with baudrate 115200.
  *          Linux/OSX Native Installation: run in a terminal window:
  *          kermusb
  *          Alternate Linux/OSX, run in a terminal windows:
  *          OSX: screen /dev/tty.usXXXXXXXX 115200
  *          LINUX: screen /dev/ttyACMx 115200
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
    if ((recvChar = BRD_debuguart_getc(10)) != '\0') {

      BRD_LEDGreenToggle();

      debug_log("Character: %c - ASCII Table value: %d\n\r", recvChar, recvChar);

      BRD_debuguart_putc(recvChar);
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
