/**
  ******************************************************************************
  * @file    gpio/main.c
  * @author  MDS
  * @date    27112017
  * @brief   Nucleo onboard GPIO pin read and write, using Pins G9 (Board D0)
  *          G14 (Board D1).
  *          To view the output, run the debugger to view the GPIOG ODR and IDR.
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

	HAL_Init();			  //Initialise board.

	hardware_init();	//Initialise hardware modules

	// Main processing loop
  while (1) {

    // Toggle pin G9 high or low
    write_value = ~write_value;

    if ((write_value & 0x01) == 0x01){

      GPIOG->ODR |= (0x01 << 9);   //Set the bit in the ODR
    } else {  

      GPIOG->ODR &= ~(0x01 << 9);   //Clear the bit in the ODR
    }

    //Turn Green LED on, if pin G14 is high
    if ((GPIOG->IDR & (0x0001 << 14)) == (0x0001 << 14)){
      
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
  __GPIOG_CLK_ENABLE();

  //Initialise G9 as an output.
  GPIOG->MODER &= ~(0x03 << (9 * 2));  //clear bits
  GPIOG->MODER |= (0x01 << (9 * 2));   //Set for push pull

  GPIOG->OSPEEDR &= ~(0x03<<(9 * 2));
	GPIOG->OSPEEDR |=   0x02<<(9 * 2);  // Set for Fast speed

  GPIOG->OTYPER &= ~(0x01 << 9);       //Clear Bit for Push/Pull utput

  // Activate the Pull-up or Pull down resistor for the current IO
  GPIOG->PUPDR &= ~(0x03 << (9 * 2));   //Clear Bits
  GPIOG->PUPDR |= ((0x01) << (9 * 2));  //Set for Pull down output
  
  // Initalise G14 as an input.
  GPIOG->MODER &= ~(0x03 << (14 * 2));			//Clear bits for input mode

  GPIOG->OSPEEDR &= ~(0x03<<(14 * 2));
	GPIOG->OSPEEDR |=   0x02<<(14 * 2);  // Fast speed

	GPIOG->PUPDR &= ~(0x03 << (14 * 2));			//Clear bits for no push/pull
  GPIOG->PUPDR |= ((0x02) << (14 * 2));  //Set for Pull down output

}