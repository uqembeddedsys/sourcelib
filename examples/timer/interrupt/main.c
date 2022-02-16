/**
 ******************************************************************************
 * @file    timer/interrupt/main.c
 * @author  MDS, NPG
 * @date    02022015, 06122017
 * @brief   Enable Timer 2 uupdate interrupt. Use the interrupt to flash
 * 			an LED every second.
 * 			See Section 18 (TIM2), P576 of the STM32F4xx Reference Manual.
 ******************************************************************************
 *
 */

#include "board.h"
#include "processor_hal.h"
#include "main.h"

#define TIMER_RUNNING_FREQ  50000           //Frequency (in Hz)
#define TIMER_1SECOND_PERIOD    50000       //Period for 1s (in count ticks)


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

	// Timer 2 clock enable
	__TIM2_CLK_ENABLE();

	// Compute the prescaler value
	// Set the clock prescaler to 50kHz
	// SystemCoreClock is the system clock frequency
	TIM2->PSC = ((SystemCoreClock / 2) / TIMER_RUNNING_FREQ) - 1;

	// Counting direction: 0 = up-counting, 1 = down-counting (Timer Control Register 1)
	TIM2->CR1 |= TIM_CR1_DIR; 

	TIM2->CNT = TIMER_1SECOND_PERIOD;	//Load counter register with 1s period value
	TIM2->ARR = TIMER_1SECOND_PERIOD;	//Load auto reload register with 1s period value

	// Enable related interrupts
	TIM2->DIER |= TIM_DIER_UIE;			// Enable update interrupt to occur on timer counter overflow or underflow.
	
	//Enable priority (10) and interrupt callback. Do not set a priority lower than 5.
	HAL_NVIC_SetPriority(TIM2_IRQn, 10, 0);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);

	TIM2->CR1 |= TIM_CR1_CEN;				// Enable the counter


}

/*
 * Timer update interrupt callback
 */
void Tui_callback(void) {

	if((TIM2->SR & TIM_SR_UIF) != 0) {	// Check if overflow has taken place 

		BRD_LEDBlueToggle();												// If overflow occurred, Toggle LED 
		TIM2->SR &= ~TIM_SR_UIF; 					// Clear the UIF Flag

	}
}