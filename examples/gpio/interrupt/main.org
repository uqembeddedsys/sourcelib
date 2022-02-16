/**
 ******************************************************************************
 * @file    gpio/gpio_interrupt.c
 * @author  MDS
 * @date    02022015, 05122017
 * @brief   Enable external interrupt on pin PC13 (USER button).
 ******************************************************************************
 *
 */

#include "board.h"
#include "processor_hal.h"
#include "main.h"

void Hardware_init(void);

int main(void) {
	/* STM32F4xx HAL library initialisation:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user
         can eventually implement his proper time base source (a general purpose
         timer for example or other time source), keeping in mind that Time base
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialisation
	 */
	HAL_Init();
	Hardware_init();

	/* Infinite loop */
	while (1) {
	}

	return 0;
}

/*
 * Initialise Hardware
 */
void Hardware_init(void) {

	BRD_LEDInit();		//Initialise LEDS

	// Turn off LEDs
	BRD_LEDRedOff();
	BRD_LEDGreenOff();
	BRD_LEDBlueOff();

	// Enable GPIOC Clock
	__GPIOC_CLK_ENABLE();

    GPIOC->OSPEEDR |= (GPIO_SPEED_FAST << 13);	//Set fast speed.
	GPIOC->PUPDR &= ~(0x03 << (13 * 2));			//Clear bits for no push/pull
	GPIOC->MODER &= ~(0x03 << (13 * 2));			//Clear bits for input mode

	// Enable EXTI clock
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	//select trigger source (port c, pin 13) on EXTICR4.
	SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13;
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;
	SYSCFG->EXTICR[3] &= ~(0x000F);

	EXTI->RTSR |= EXTI_RTSR_TR13;	//enable rising dedge
	EXTI->FTSR &= ~EXTI_FTSR_TR13;	//disable falling edge
	EXTI->IMR |= EXTI_IMR_IM13;		//Enable external interrupt

	//Enable priority (10) and interrupt callback. Do not set a priority lower than 5.
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 10, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/*
 * Push Button callback
 */
void Pb_callback(uint16_t GPIO_Pin) {

	// Check if the pushbutton pin was pressed.
	if (GPIO_Pin == 13) {
		
		BRD_LEDBlueToggle(); 		//Toggle Blue LED

		EXTI->PR |= EXTI_PR_PR13;	//Clear interrupt flag.
	}
}