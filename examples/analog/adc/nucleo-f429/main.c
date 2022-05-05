/**
  ******************************************************************************
  * @file    adc/main.c
  * @author  MDS
  * @date    05032021
  * @brief   Enable the ADC1 on Board pin A0 (PA3).
  *			 See Section 13 (ADC), P385 of the STM32F4xx Reference Manual.
  ******************************************************************************
  *
  */
#include "board.h"
#include "processor_hal.h"

ADC_HandleTypeDef AdcHandle;
ADC_ChannelConfTypeDef AdcChanConfig;

void hardware_init(void);

int main(void)
{
	unsigned int adc_value;

	HAL_Init();
	hardware_init();

	// Main Processing Loop
	while (1)
	{

		HAL_ADC_Start(&AdcHandle); // Start ADC conversion

		// Wait for ADC conversion to finish
		while (HAL_ADC_PollForConversion(&AdcHandle, 10) != HAL_OK);

		adc_value = AdcHandle.Instance->DR;		//Read ADC1 Data Register for converted value.

		// If ADC value is higher than 1/2 Reference voltage, turn on Green LED.
		// Else if ADC value is lower than 1/2 Reference voltage, turn off Green LED.
		if (adc_value > 0x7FF) {
			
			BRD_LEDGreenOn();
		} else {

			BRD_LEDGreenOff();
		}

		HAL_Delay(1000);
	}
}

/*
 * Initialise Hardware
 */
void hardware_init(void) {

	BRD_LEDInit();		//Initialise LEDS

	// Turn off LEDs
	BRD_LEDGreenOff();

	// Enable the GPIO Clock
  	__GPIOA_CLK_ENABLE();
  
	// Initalise PA3 as an Analog input.
  	GPIOA->MODER |= (0x03 << (3 * 2));			//Set bits for Analog input mode

  	GPIOA->OSPEEDR &= ~(0x03<<(3 * 2));
	GPIOA->OSPEEDR |= 0x02<<(3 * 2);  			// Fast speed

	GPIOA->PUPDR &= ~(0x03 << (3 * 2));			//Clear bits for no push/pull

	__ADC1_CLK_ENABLE();						//Enable ADC1 clock

	AdcHandle.Instance = (ADC_TypeDef *)(ADC1_BASE);						//Use ADC1 (For PA3)
	AdcHandle.Init.ClockPrescaler        = ADC_CLOCKPRESCALER_PCLK_DIV2;	//Set clock prescaler
	AdcHandle.Init.Resolution            = ADC_RESOLUTION12b;				//Set 12-bit data resolution
	AdcHandle.Init.ScanConvMode          = DISABLE;
	AdcHandle.Init.ContinuousConvMode    = DISABLE;
	AdcHandle.Init.DiscontinuousConvMode = DISABLE;
	AdcHandle.Init.NbrOfDiscConversion   = 0;
	AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;	//No Trigger
	AdcHandle.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;		//No Trigger
	AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;				//Right align data
	AdcHandle.Init.NbrOfConversion       = 1;
	AdcHandle.Init.DMAContinuousRequests = DISABLE;
	AdcHandle.Init.EOCSelection          = DISABLE;

	HAL_ADC_Init(&AdcHandle);		//Initialise ADC

	// Configure ADC Channel
	AdcChanConfig.Channel = ADC_CHANNEL_3;					//PA3 has Analog Channel 3 connected
	AdcChanConfig.Rank         = 1;
	AdcChanConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	AdcChanConfig.Offset       = 0;

	HAL_ADC_ConfigChannel(&AdcHandle, &AdcChanConfig);		//Initialise ADC Channel
}
