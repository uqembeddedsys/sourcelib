/**
  ******************************************************************************
  * @file    adc/main.c
  * @author  MDS
  * @date    05032021
  * @brief   Enable the ADC1 on Board pin A0 (PA0).
  *			 See Section ADC of the STM32L4xx Reference Manual.
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
		while (HAL_ADC_PollForConversion(&AdcHandle, 100) != HAL_OK);

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
  
	// Initalise PA0 as an Analog input.
  	GPIOA->MODER |= (0x03 << (0 * 2));			//Set bits for Analog input mode

  	GPIOA->OSPEEDR &= ~(0x03<<(0 * 2));
	GPIOA->OSPEEDR |= 0x02<<(0 * 2);  			// Fast speed

	GPIOA->PUPDR &= ~(0x03 << (0 * 2));			//Clear bits for no push/pull

	GPIOA->ASCR |= GPIO_ASCR_EN_0;				//Enable the analog switch input for A0

	__HAL_RCC_ADC_CLK_ENABLE();					//Enable ADC clock

	AdcHandle.Instance = (ADC_TypeDef *)(ADC1_BASE);						//Use ADC1 (For PA0)
  	AdcHandle.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV2;
  	AdcHandle.Init.Resolution            = ADC_RESOLUTION_12B;
  	AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
  	AdcHandle.Init.ScanConvMode          = ADC_SCAN_DISABLE;              /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
  	AdcHandle.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
  	AdcHandle.Init.LowPowerAutoWait      = DISABLE;
  	AdcHandle.Init.ContinuousConvMode    = DISABLE;                       /* Continuous mode disabled to have only 1 conversion at each conversion trig */
  	AdcHandle.Init.NbrOfConversion       = 1;                             /* Parameter discarded because sequencer is disabled */
  	AdcHandle.Init.DiscontinuousConvMode = DISABLE;                       /* Parameter discarded because sequencer is disabled */
  	AdcHandle.Init.NbrOfDiscConversion   = 1;                             /* Parameter discarded because sequencer is disabled */
  	AdcHandle.Init.ExternalTrigConv      = ADC_SOFTWARE_START;            /* Software start to trig the 1st conversion manually, without external event */
  	AdcHandle.Init.DMAContinuousRequests = DISABLE;                       /* ADC with DMA transfer: continuous requests to DMA disabled (default state) since DMA is not used in this example. */
  	AdcHandle.Init.Overrun               = ADC_OVR_DATA_OVERWRITTEN;
  
	HAL_ADC_Init(&AdcHandle);		//Initialise ADC

	// Configure ADC Channel
	AdcChanConfig.Channel = ADC_CHANNEL_5;					//PA0 has Analog Channel 5 connected
	AdcChanConfig.Rank         = 1;
	AdcChanConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
	AdcChanConfig.Offset       = 0;

	HAL_ADC_ConfigChannel(&AdcHandle, &AdcChanConfig);		//Initialise ADC Channel
}
