/**
  ******************************************************************************
  * @file    pwm/dynamic/
  * @authors MDS & SMVC
  * @date    08032023
  * @brief   Enable a static (not changing) PWM output on Board Pin D5 (PE11).
  *			 See Section 18 (TIM1), P592 of the STM32F4xx Reference Manual.
  *			 Multi-Functional Shield (MFS) Pin PWM signal (~5)
  ******************************************************************************
  *
  */


#include "board.h"
#include "processor_hal.h"

#define TIMER_COUNTER_FREQ  				5000           				//Frequency (in Hz)
#define PWM_PULSE_WIDTH_SECONDS				2						//2s
#define PWM_PULSE_WIDTH_TICKS				20000 					//2*((PWM_PULSE_WIDTH_SECONDS)/(1/TIMER_COUNTER_FREQ))		//Period in timer ticks
#define PWM_PERCENT2TICKS_DUTYCYCLE(value)	(uint16_t)(value*PWM_PULSE_WIDTH_TICKS/100)				//Duty Cycle on time in timer ticks
#define PWM_DUTYCYCLE_CHANGE(value) 			TIM1->CCR2=(uint16_t)value									//Change PWM dutycyle

void hardware_init(void);


int main(void) {
	
	int dutycycle;

	HAL_Init();
	hardware_init();

	dutycycle = 10;

	// Main Processing Loop
	while (1) {

		PWM_DUTYCYCLE_CHANGE(PWM_PERCENT2TICKS_DUTYCYCLE(dutycycle));	//Set duty cyle

		dutycycle = 10 + (dutycycle + 10)%100;	//Increment by 10% from 10% to 100%
		HAL_Delay(1000);					//Delay for 5s
		BRD_LEDGreenToggle();

	}

	return 0;
}

/*
 * Initialise Hardware
 */
void hardware_init(void) {

	BRD_LEDInit();		//Initialise LEDs

	// Turn off LEDs
	BRD_LEDGreenOff();

	// Enable GPIOE Clock
	__GPIOE_CLK_ENABLE();

    GPIOE->OSPEEDR |= (GPIO_SPEED_FAST << (11 * 2));		//Set fast speed.
	GPIOE->PUPDR &= ~(0x03 << (11 * 2));				//Clear bits for no push/pull
	GPIOE->MODER &= ~(0x03 << (11 * 2));				//Clear bits
	GPIOE->MODER |= (GPIO_MODE_AF_PP << (11 * 2));  	//Set Alternate Function Push Pull Mode
	
	GPIOE->AFR[1] &= ~((0x0F) << ((11-8) * 4));			//Clear Alternate Function for pin (lower ARF register)
	GPIOE->AFR[1] |= (GPIO_AF1_TIM1 << ((11-8) * 4));	//Set Alternate Function for pin (lower ARF register) 

	// Timer 3 clock enable
	__TIM1_CLK_ENABLE();

	/* Compute the prescaler value
	   Set the clock prescaler to 50kHz
	   SystemCoreClock is the system clock frequency */
	TIM1->PSC = ((SystemCoreClock / 2) / TIMER_COUNTER_FREQ) - 1;

	// Counting direction: 0 = up-counting, 1 = down-counting (Timer Control Register 1)
	TIM1->CR1 &= ~TIM_CR1_DIR; 

	TIM1->ARR = PWM_PULSE_WIDTH_TICKS; 		//Set pulse period to ~1s 
	TIM1->CCR2 = PWM_PERCENT2TICKS_DUTYCYCLE(50);	//Set pulse width to 0% duty cycle.

	TIM1->CCMR1 &= ~(TIM_CCMR1_OC2M); 	// Clear OC2M (Channel 2) 
	TIM1->CCMR1 |= (0x6 << 12); 		// Enable PWM Mode 1, upcounting, on Channel 2 
	TIM1->CCMR1 |= (TIM_CCMR1_OC2PE); 	// Enable output preload bit for channel 2 
	
	TIM1->CR1  |= (TIM_CR1_ARPE); 	// Set Auto-Reload Preload Enable 
	TIM1->CCER |= TIM_CCER_CC2E; 	// Set CC2E Bit
	TIM1->CCER &= ~TIM_CCER_CC2NE; 	// Clear CC2NE Bit for active high output

	TIM1->CCER &= ~TIM_CCER_CC2P; 	// Polarity of PWM output waveform is active high. 
									// If set, reverses polarity (e.g. high becomes low).
									// See p576 of the STM32F429 reference manual
	
	/* Set Main Output Enable (MOE) bit
	   Set Off-State Selection for Run mode (OSSR) bit
	   Set Off-State Selection for Idle mode (OSSI) bit */
	TIM1->BDTR |= TIM_BDTR_MOE | TIM_BDTR_OSSR | TIM_BDTR_OSSI; 

	TIM1->CR1 |= TIM_CR1_CEN;	// Enable the counter
}