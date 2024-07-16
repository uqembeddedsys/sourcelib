/**
  ******************************************************************************
  * @file    sysmon/main.c
  * @author  MDS
  * @date    16072024
  * @brief   Nucleo429ZI SYSMON and onboard LED flashing example. Uses the system time (ms) 
  *          and system delay (ms) to ensure that the LEDs toggle, every 2s. The 
  *          SYSMON channels (0,1,2) are also toggled on pins: CN12-61, CN12-65, CN12-69.
  ******************************************************************************
  */

#include "board.h"
#include "processor_hal.h"

void hardware_init(void);

/*
 * Main program - flashes onboard LEDs
 */
int main(void)  {

  uint32_t prev_tick;

	HAL_Init();			//Initalise Board
	hardware_init();	//Initalise hardware modules
	
  prev_tick = 0;

	// Main processing loop
  while (1) {

    // Toggle all LEDs and SYSMON channel 0, every 2 seconds
    if ((HAL_GetTick() - prev_tick) >= 2000) {
		  BRD_LEDRedToggle();
		  BRD_LEDGreenToggle();
		  BRD_LEDBlueToggle();

      SYSMON_CHAN0_TOGGLE();
      SYSMON_CHAN1_SET();


      prev_tick =  HAL_GetTick();
    }

		HAL_Delay(1000);		//Delay for 1s

    SYSMON_CHAN1_CLR();
    SYSMON_CHAN2_TOGGLE();
	}

  return 0;
}

/*
 * Initialise Hardware
 */
void hardware_init(void) {

	BRD_LEDInit();		//Initialise LEDS

	// Turn off LEDs
	BRD_LEDRedOff();
	BRD_LEDGreenOff();
	BRD_LEDBlueOff();

  BRD_sysmon_init();  //Initialise sysmon

  //Set all sysmon outputs to low.
  SYSMON_CHAN0_CLR();
  SYSMON_CHAN1_CLR();
  SYSMON_CHAN2_CLR();

}
