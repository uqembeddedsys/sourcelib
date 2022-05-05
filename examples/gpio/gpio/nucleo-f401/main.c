/**
  ******************************************************************************
  * @file    gpio/main.c
  * @author  MDS
  * @date    27112017
  * @brief   Nucleo onboard GPIO pin read and write, using Pins B3 (Board D3)
  *          B5 (Board D4).
  *          To view the output, run the debugger to view the GPIOB ODR and IDR.
  ******************************************************************************
  */

#include "board.h"
#include "processor_hal.h"

void hardware_init(void);

/*
 * Main program - flashes on board LEDs
 */
int main(void)  {

	uint16_t write_value = 0;

	HAL_Init();			//Initialise board.

	hardware_init();	//Initialise hardware modules

	// Main processing loop
  while (1) {

    // Toggle pin B3 high or low
    write_value = ~write_value;

    if ((write_value & 0x01) == 0x01){

      GPIOB->ODR |= (0x01 << 3);   //Set the bit in the ODR
    } else {  

      GPIOB->ODR &= ~(0x01 << 3);   //Clear the bit in the ODR
    }

    //Turn Blue LED on, if pin B5 is high
    if ((GPIOB->IDR & (0x0001 << 5)) == (0x0001 << 5)){
      
      BRD_LEDGreenOn(); 
    } else {  

      BRD_LEDGreenOff();
    } 
  }

  return 0;
}

/*
 * Initialise Hardware
 */
void hardware_init(void) {

  BRD_LEDInit();		//Initialise LEDS
	BRD_LEDGreenOff();

	// Enable the GPIO Clock
  __GPIOB_CLK_ENABLE();

  //Initialise B3 as an output.
  GPIOB->MODER &= ~(0x03 << (3 *2));  //clear bits
  GPIOB->MODER |= (0x01 << (3 *2));   //Set for push pull

  GPIOB->OSPEEDR &= ~(0x03<<(3 *2));
	GPIOB->OSPEEDR |=   0x02<<(3 *2);  // Set for Fast speed

  GPIOB->OTYPER &= ~(0x01 << 3);       //Clear Bit for Push/Pull output

  // Activate the Pull-up or Pull down resistor for the current IO
  GPIOB->PUPDR &= ~(0x03 << (3 *2));   //Clear Bits
  GPIOB->PUPDR |= ((0x01) << (3 *2));  //Set for Pull down output
  
  // Initalise B5 as an input.
  GPIOB->MODER &= ~(0x03 << (5 * 2));			//Clear bits for input mode

  GPIOB->OSPEEDR &= ~(0x03<<(5 * 2));
	GPIOB->OSPEEDR |=   0x02<<(5 * 2);  // Fast speed

	GPIOB->PUPDR &= ~(0x03 << (5 * 2));			//Clear bits for no push/pull
  GPIOB->PUPDR |= ((0x02) << (5 * 2));  //Set for Pull down output

}