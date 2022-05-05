/**
  ******************************************************************************
  * @file    fsm/main.c
  * @author  MDS
  * @date    27032022
  * @brief   Simple Finite State Machine (FSM) example to Pushbutton sequence detect. 
  *          If pushbutton is pressed for 3s, the LED will turn on for 2 seconds and 
  *          then turn off. The pushbutton must be released. Otherwise. 
  * 
  *          DEBUG Output
  *          Uncomment #define DEBUG to view debug output
  *          Use a serial terminal (e.g. kermusb on the VM) to view the output.
  ******************************************************************************
  */

#include "board.h"
#include "processor_hal.h"

//#define DEBUG     //Uncomment to enable debug messages
#ifdef DEBUG
#include "debug_log.h"
#endif

// States used
#define S0  0
#define S1  1
#define S2  2
#define S3  3
#define S4  4

void hardware_init(void);
int fsm_processing(int current);

/*
 * Main program
 */
int main(void)  {

  uint32_t prev_tick;
  int currentState = S0;

	HAL_Init();			//Initalise Board
	hardware_init();	//Initalise hardware modules
	
  prev_tick = 0;

	// Main processing loop
  while (1) {
        
		// Call FSM processing function, every second.
    if ((HAL_GetTick() - prev_tick) >= 1000) {
		  
      currentState = fsm_processing(currentState);

#ifdef DEBUG
      debug_log("S%d\n\r", currentState);
#endif
      prev_tick =  HAL_GetTick();
    }
	}

  return 0;
}

/*
 * Return current state and process outputs
 */
int fsm_processing(int current) {

  int nextState = S0;   //Hold Next state value

  switch(current) {

    case S0:    //First state

      //Check if pushbutton is pressed.
      if ((GPIOC->IDR & (0x0001 << 13)) == 0){
        nextState = S1;

      } else {  
        nextState = S0;
      }

      BRD_LEDGreenOff();

      break;

    case S1:

      //Check if pushbutton is pressed.
      if ((GPIOC->IDR & (0x0001 << 13)) == 0){
        nextState = S2;

      } else {  
        nextState = S0;
      }

      //Turn LED off
      BRD_LEDGreenOff();

      break;

    case S2:

      //Check if pushbutton is released.
      if ((GPIOC->IDR & (0x0001 << 13)) == (0x0001 << 13)){
        nextState = S3;
        
      } else {  
        nextState = S0;
      }

      //Turn LED on
      BRD_LEDGreenOn();

      break;

    case S3:

      //Go to next state
      nextState = S4;

      break;

    case S4:

      //Go to next state
      nextState = S0;

      //Turn LED off
      BRD_LEDGreenOff();

      break;

  }

  return nextState;
}

/*
 * Initialise Hardware
 */
void hardware_init(void) {

	BRD_LEDInit();		//Initialise LEDS

	// Turn off LEDs
	BRD_LEDGreenOff();

#ifdef DEBUG
  BRD_debuguart_init();  //Initialise UART for debug log output
#endif

  // Enable GPIO Clock
	__GPIOC_CLK_ENABLE();

  //Enable pushbutton input
  GPIOC->OSPEEDR |= (GPIO_SPEED_FAST << 13);	//Set fast speed.
	GPIOC->PUPDR &= ~(0x03 << (13 * 2));			//Clear bits for no push/pull
	GPIOC->MODER &= ~(0x03 << (13 * 2));			//Clear bits for input mode

}
