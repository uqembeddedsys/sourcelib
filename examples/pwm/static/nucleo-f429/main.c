/**
  ******************************************************************************
  * @file    pwm/staic/main.c
  * @author  MDS
  * @date    03032024
  * @brief   Enable a static (not changing) PWM output on Board Pin D5 (PE11).
  *			 See Section 18 (TIM3), P592 of the STM32F4xx Reference Manual.
  *			 Multi-Functional Shield (MFS) Pin PWM signal (~5)
  ******************************************************************************
  *
  */


#include "board.h"
#include "processor_hal.h"

#define TIMER_COUNTER_FREQ  		25000           							//Frequency (in Hz)
#define TIMER_1SECOND_PERIOD_TICKS		TIMER_COUNTER_FREQ*2 					//2*(1/(1/TIMER_COUNTER_FREQ)) is the Period in timer ticks. 
																				//Note a period consists of two zero crossings.
#define TIMER_50PERCENT_DUTYCYLE_TICKS	50*TIMER_1SECOND_PERIOD_TICKS/100		//Duty Cycle on time in timer ticks

void hardware_init(void);


int main(void) {
	
	HAL_Init();
	hardware_init();

	// Main Processing Loop	
	while (1) {

		HAL_Delay(2000);
		BRD_LEDGreenToggle();
	}

	return 0;
}

/*
 * Initialise Hardware
 */
void hardware_init(void) {

	BRD_LEDInit();		//Initialise LEDS

	// Turn off LEDs
	BRD_LEDGreenOff();

	// Enable GPIO Clock
	__GPIOE_CLK_ENABLE();

    GPIOE->OSPEEDR |= (GPIO_SPEED_FAST << (11 * 2));		//Set fast speed.
	GPIOE->PUPDR &= ~(0x03 << (11 * 2));				//Clear bits for no push/pull
	GPIOE->MODER &= ~(0x03 << (11 * 2));				//Clear bits
	GPIOE->MODER |= (GPIO_MODE_AF_PP << (11 * 2));  	//Set Alternate Function Push Pull Mode
	
	GPIOE->AFR[1] &= ~((0x0F) << ((11-8) * 4));			//Clear Alternate Function for pin (lower ARF register)
	GPIOE->AFR[1] |= (GPIO_AF1_TIM1 << ((11-8) * 4));	//Set Alternate Function for pin (lower ARF register) 

	// Timer 1 clock enable
	__TIM1_CLK_ENABLE();

	/* Compute the prescaler value
	   Set the clock prescaler to 50kHz
	   SystemCoreClock is the system clock frequency */
	TIM1->PSC = ((SystemCoreClock / 2) / TIMER_COUNTER_FREQ) - 1;

	// Counting direction: 0 = up-counting, 1 = down-counting (Timer Control Register 1)
	TIM1->CR1 &= ~TIM_CR1_DIR; 

	TIM1->ARR = TIMER_1SECOND_PERIOD_TICKS; 		//Set pulse period to ~1s 
	TIM1->CCR2 = TIMER_50PERCENT_DUTYCYLE_TICKS;	//Set pulse width to 50% duty cycle.

	TIM1->CCMR1 &= ~(TIM_CCMR1_OC2M); 	// Clear OC2M (Channel 2) 
	TIM1->CCMR1 |= (0x6 << 12); 		// Enable PWM Mode 1, upcounting, on Channel 2 
	TIM1->CCMR1 |= (TIM_CCMR1_OC2PE); 	// Enable output preload bit for channel 2 
	
	TIM1->CR1  |= (TIM_CR1_ARPE); 	// Set Auto-Reload Preload Enable 
	TIM1->CCER |= TIM_CCER_CC2E; 	// Set CC2E Bit 
	TIM1->CCER &= ~TIM_CCER_CC2NE; 	// Clear CC2NE Bit for active high output 
		
	/* Set Main Output Enable (MOE) bit
	   Set Off-State Selection for Run mode (OSSR) bit
	   Set Off-State Selection for Idle mode (OSSI) bit */
	TIM1->BDTR |= TIM_BDTR_MOE | TIM_BDTR_OSSR | TIM_BDTR_OSSI; 

	TIM1->CR1 |= TIM_CR1_CEN;	// Enable the counter
}