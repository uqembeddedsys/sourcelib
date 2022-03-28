/**
  ******************************************************************************
  * @file    pwm/staic/main.c
  * @author  MDS
  * @date    02032018
  * @brief   Enable a static (not changing) PWM output on Board Pin D4 (PB5).
  *			 See Section TIMER of the STM32L4xx Reference Manual.
  ******************************************************************************
  *
  */


#include "board.h"
#include "processor_hal.h"

#define TIMER_COUNTER_FREQ  		50000           							//Frequency (in Hz)
#define TIMER_1SECOND_PERIOD_TICKS		50000 									//1/(1/TIMER_RUNNING_FREQ) is the Period in timer ticks
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

	// Enable GPIOC Clock
	__GPIOB_CLK_ENABLE();

    GPIOB->OSPEEDR |= (GPIO_SPEED_FAST << 2);		//Set fast speed.
	GPIOB->PUPDR &= ~(0x03 << (5 * 2));				//Clear bits for no push/pull
	GPIOB->MODER &= ~(0x03 << (5 * 2));				//Clear bits
	GPIOB->MODER |= (GPIO_MODE_AF_PP << (5 * 2));  	//Set Alternate Function Push Pull Mode
	
	GPIOB->AFR[0] &= ~((0x0F) << (5 * 4));			//Clear Alternate Function for pin (lower ARF register)
	GPIOB->AFR[0] |= (GPIO_AF2_TIM3 << (5 * 4));	//Set Alternate Function for pin (lower ARF register) 

	// Timer 3 clock enable
	__TIM3_CLK_ENABLE();

	/* Compute the prescaler value
	   Set the clock prescaler to 50kHz
	   SystemCoreClock is the system clock frequency */
	TIM3->PSC = ((SystemCoreClock / 2) / TIMER_COUNTER_FREQ) - 1;

	// Counting direction: 0 = up-counting, 1 = down-counting (Timer Control Register 1)
	TIM3->CR1 &= ~TIM_CR1_DIR; 

	TIM3->ARR = TIMER_1SECOND_PERIOD_TICKS; 		//Set pulse period to ~1s 
	TIM3->CCR2 = TIMER_50PERCENT_DUTYCYLE_TICKS;	//Set pulse width to 50% duty cycle.

	TIM3->CCMR1 &= ~(TIM_CCMR1_OC2M); 	// Clear OC2M (Channel 2) 
	TIM3->CCMR1 |= (0x6 << 12); 		// Enable PWM Mode 1, upcounting, on Channel 2 
	TIM3->CCMR1 |= (TIM_CCMR1_OC2PE); 	// Enable output preload bit for channel 2 
	
	TIM3->CR1  |= (TIM_CR1_ARPE); 	// Set Auto-Reload Preload Enable 
	TIM3->CCER |= TIM_CCER_CC2E; 	// Set CC2E Bit 
	TIM3->CCER &= ~TIM_CCER_CC2NE; 	// Clear CC2NE Bit for active high output 
	
	/* Set Main Output Enable (MOE) bit
	   Set Off-State Selection for Run mode (OSSR) bit
	   Set Off-State Selection for Idle mode (OSSI) bit */
	TIM3->BDTR |= TIM_BDTR_MOE | TIM_BDTR_OSSR | TIM_BDTR_OSSI; 

	TIM3->CR1 |= TIM_CR1_CEN;	// Enable the counter
}