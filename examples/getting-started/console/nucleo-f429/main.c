/**
  ******************************************************************************
  * @file    console/main.c
  * @author  MDS
  * @date    27032022
  * @brief   Nucleo debug log example. Prints to the console, every 2 seconds.
  *          Will detect and print key press to console
  * 
  *          VSCODE: Run serial terminal - Open Terminal -> New Terminal
  *                  1) Click on "SERIAL MONITOR" and select:
  *                       Port: /dev/ttyACM<x> (For WSL or Linux - e.g. /dev/ttyACM0)
  *                       Port: /dev/tty.us<XXXXXXXX> (OSX only - select the port that starts with /dev/tty.us)
  *                  2) "View Mode" must be set to "Text"
  *                  3) Click "Start Monitoring". Type a letter/number and press <enter> in the box at the bottom of the terminal to send the key value.)
  *
  *          ALTERNATIVE TERMINALS: (Use only if VSCODE is unavailable)
  *          WSL or LINUX: screen /dev/ttyACM<x> 115200 (For WSL or Linux - e.g. screen /dev/ttyACM0 115200)
  *          OSX: screen /dev/tty.us<XXXXXXXX> 115200 (OSX only - select the port that starts with /dev/tty.us)
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
