/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "processor_hal.h"
#include <stdio.h>
#include <string.h>

/**
  * @brief  Initialise the delay  counter
  * @param  None
  * @retval None
  */
void BRD_delayInit() {

	HAL_Init();

#ifndef NODEBUGUART
	BRD_debuguart_init();
#endif

#ifndef NOSYSMON
	BRD_sysmon_init();
#endif

	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;


}

/**
  * @brief  Configures LED GPIO.
  * @param  Led: Specifies the Led to be configured.
  *   This parameter can be one of following parameters:
  * @retval None
  */
void BRD_LEDInit() {

  	// Enable the GPIO_LED Clock
 	__BRD_ALL_LED_GPIO_CLK();

  	//Initialise LED pins as an output.
	MODIFY_REG(__BRD_ALL_LED_GPIO->MODER, ( 0x03 << (BRD_RED_LED_PIN*2)) | ( 0x03 << (BRD_GREEN_LED_PIN*2)) | (0x03 << (BRD_BLUE_LED_PIN*2)), (0x01 << (BRD_RED_LED_PIN*2)) | (0x01 << (BRD_GREEN_LED_PIN*2)) | (0x01 << (BRD_BLUE_LED_PIN*2)) );

	// Set for Fast speed
  	MODIFY_REG(__BRD_ALL_LED_GPIO->OSPEEDR, (0x03 << (BRD_RED_LED_PIN*2)) | (0x03 << (BRD_GREEN_LED_PIN*2)) | (0x03 << (BRD_BLUE_LED_PIN*2)), (0x02 << (BRD_RED_LED_PIN*2)) | (0x02 << (BRD_GREEN_LED_PIN*2)) | (0x02 << (BRD_BLUE_LED_PIN*2)) );

	// Clear Bit for Push/Pull output 
  	CLEAR_BIT(__BRD_ALL_LED_GPIO->OTYPER, ((0x01 << BRD_RED_LED_PIN) | (0x01 << BRD_GREEN_LED_PIN) | (0x01 << BRD_BLUE_LED_PIN)));       

	// Activate the Pull-up or Pull down resistor for the current IO
  	MODIFY_REG(__BRD_ALL_LED_GPIO->PUPDR, (0x03 << (BRD_RED_LED_PIN*2)) | (0x03 << (BRD_GREEN_LED_PIN*2)) | (0x03 << (BRD_BLUE_LED_PIN*2)), (0x01 << (BRD_RED_LED_PIN*2)) | (0x01 << (BRD_GREEN_LED_PIN*2)) | (0x01 << (BRD_BLUE_LED_PIN*2)) );    //Set for Pull down output

}

/**
  * @brief  Turns selected LED On.
  * @param  Led: Specifies the Led to be set on.
  *   This parameter can be one of following parameters:
  * @retval None
  */
void BRD_LEDOn(uint8_t ledmask) {

	if ((ledmask & BRD_GREEN_LEDMASK) != 0) {
		BRD_GREEN_LED_GPIO_PORT->BSRR |= BRD_GREEN_LED_GPIO_PIN;
	}

	if ((ledmask & BRD_BLUE_LEDMASK) != 0) {
		BRD_BLUE_LED_GPIO_PORT->BSRR |= BRD_BLUE_LED_GPIO_PIN;
	}

	if ((ledmask & BRD_RED_LEDMASK) != 0) {
			BRD_RED_LED_GPIO_PORT->BSRR |= BRD_RED_LED_GPIO_PIN;
	}
}

/**
  * @brief  Turns selected LED Off.
  * @param  Led: Specifies the Led to be set off.
  *   This parameter can be one of following parameters:
  * @retval None
  */
void BRD_LEDOff(uint8_t ledmask) {
  //BRD_LED_GPIO_PORT->BSRR |= BRD_LED_PIN << 16;

  if ((ledmask & BRD_GREEN_LEDMASK) != 0) {
  		BRD_GREEN_LED_GPIO_PORT->BSRR |= BRD_GREEN_LED_GPIO_PIN << 16;
  	}

  	if ((ledmask & BRD_BLUE_LEDMASK) != 0) {
  		BRD_BLUE_LED_GPIO_PORT->BSRR |= BRD_BLUE_LED_GPIO_PIN << 16;
  	}

  	if ((ledmask & BRD_RED_LEDMASK) != 0) {
  		BRD_RED_LED_GPIO_PORT->BSRR |= BRD_RED_LED_GPIO_PIN << 16;
  	}
}

/**
  * @brief  Toggles the selected LED.
  * @param  Led: Specifies the Led to be toggled.
  *   This parameter can be one of following parameters:
  * @retval None
  */
void BRD_LEDToggle(uint8_t ledmask) {
  //BRD_LED_GPIO_PORT->ODR ^= BRD_LED_PIN;

  if ((ledmask & BRD_GREEN_LEDMASK) != 0) {
	  BRD_GREEN_LED_GPIO_PORT->ODR ^= BRD_GREEN_LED_GPIO_PIN;
  }

  if ((ledmask & BRD_BLUE_LEDMASK) != 0) {
	  BRD_BLUE_LED_GPIO_PORT->ODR ^= BRD_BLUE_LED_GPIO_PIN;
  }

  if ((ledmask & BRD_RED_LEDMASK) != 0) {
	  BRD_RED_LED_GPIO_PORT->ODR ^= BRD_RED_LED_GPIO_PIN;
  }
}

//UART_HandleTypeDef UART_debug;

/* Initialise Debug UART */
void BRD_debuguart_init() {

#ifndef NODEBUGUART

	//Enable DEBUG UART clock
	__BRD_DEBUG_UART_CLK();

	__BRD_DEBUG_UART_GPIO_CLK();

	// Initialise TX as an output.
  	// Initalise RX as an input.
	// Clear and Set Alternate Function for pin (upper ARF register) 
	MODIFY_REG(BRD_DEBUG_UART_GPIO->AFR[1], ((0x0F) << ((BRD_DEBUG_UART_RX_PIN-8)*4)) | ((0x0F) << ((BRD_DEBUG_UART_TX_PIN-8)*4)), ((BRD_DEBUG_UART_AF << ((BRD_DEBUG_UART_RX_PIN-8)*4)) | (BRD_DEBUG_UART_AF << ((BRD_DEBUG_UART_TX_PIN-8))*4)));
	
	//Clear and Set Alternate Function Push Pull Mode
	MODIFY_REG(BRD_DEBUG_UART_GPIO->MODER, ((0x03 << (BRD_DEBUG_UART_RX_PIN*2)) | (0x03 << (BRD_DEBUG_UART_TX_PIN*2))), ((GPIO_MODE_AF_PP << (BRD_DEBUG_UART_RX_PIN*2)) | (GPIO_MODE_AF_PP << (BRD_DEBUG_UART_TX_PIN*2))));
	
	//Set low speed.
	SET_BIT(BRD_DEBUG_UART_GPIO->OSPEEDR, (GPIO_SPEED_FAST << BRD_DEBUG_UART_RX_PIN) | (GPIO_SPEED_FAST << BRD_DEBUG_UART_TX_PIN));

	//Set Bit for Push/Pull output
	SET_BIT(BRD_DEBUG_UART_GPIO->OTYPER, ((0x01 << BRD_DEBUG_UART_RX_PIN) | (0x01 << BRD_DEBUG_UART_TX_PIN)));

	//Clear and set bits for no push/pull
	MODIFY_REG(BRD_DEBUG_UART_GPIO->PUPDR, (0x03 << (BRD_DEBUG_UART_RX_PIN*2)) | (0x03 << (BRD_DEBUG_UART_TX_PIN*2)), (GPIO_PULLUP << (BRD_DEBUG_UART_RX_PIN*2)) | (GPIO_PULLUP << (BRD_DEBUG_UART_TX_PIN*2)));


	// UART Settings - No hardware flow control, 8 data bits, no parity, 1 start bit and 1 stop bit		
	// Enable USART clock
	__BRD_DEBUG_UART_CLK();

	CLEAR_BIT(BRD_DEBUG_UART->CR1, USART_CR1_UE);  // Disable USART
	
	// Configure word length to 8 bit
	CLEAR_BIT(BRD_DEBUG_UART->CR1, USART_CR1_M);   // M: 00 = 8 data bits, 01 = 9 data bits, 10 = 7 data bits
	
	// Configure oversampling mode: Oversampling by 16 
	CLEAR_BIT(BRD_DEBUG_UART->CR1, USART_CR1_OVER8);  // 0 = oversampling by 16, 1 = oversampling by 8
	
	SET_BIT(BRD_DEBUG_UART->CR1, USART_CR1_RE | USART_CR1_TE);  	// Transmitter and Receiver enable
	CLEAR_BIT(BRD_DEBUG_UART->CR1, USART_CR1_PCE);						//Disable Parity

	// Configure stop bits to 1 stop bit and siable clock output (USART mode only)
	//   00: 1 Stop bit;      01: 0.5 Stop bit
	//   10: 2 Stop bits;     11: 1.5 Stop bit   
	CLEAR_BIT(BRD_DEBUG_UART->CR2, USART_CR2_CPHA | USART_CR2_CPOL | USART_CR2_CLKEN | USART_CR2_LBCL | USART_CR2_STOP);

	// Set Baudrate to 115200 using APB frequency (80,000,000 Hz) and 16 bit sampling
	// NOTE: If using USART1 or USART6, HAL_RCC_GetPCLK2Freq must be used.
	WRITE_REG(BRD_DEBUG_UART->BRR, UART_BRR_SAMPLING16(HAL_RCC_GetPCLK1Freq(), BRD_DEBUG_UART_BAUDRATE));

	//Disable handshaing signals
	CLEAR_BIT(BRD_DEBUG_UART->CR3, USART_CR3_RTSE | USART_CR3_CTSE | USART_CR3_SCEN | USART_CR3_HDSEL | USART_CR3_IREN);

	SET_BIT(BRD_DEBUG_UART->CR1, USART_CR1_UE); // UART enable   v

    //Allow stdio stdout buffer to be used for print, putc, etc.
    setbuf(stdout, NULL);

#endif

}

//Transmit char through debug uart and USB, if enabled
void BRD_debuguart_putc(unsigned char c)
{
	WRITE_REG(BRD_DEBUG_UART->DR, (unsigned char) c);

	// Wait for the character to be sent.
	while((READ_REG(BRD_DEBUG_UART->SR) & USART_SR_TC) == 0);	//wait for transmission to complete


}

//#pragma GCC diagnostic push
//#pragma GCC diagnostic ignored "-Wpointer-sign"
//Transmit string through debug uart and USB, if enabled
void BRD_debuguart_puts(unsigned char *c)
{
	int i;

	for (i = 0; i < (int) strlen(c); i++) {
		WRITE_REG(BRD_DEBUG_UART->DR, (unsigned char) (*(c + i)));
		while((READ_REG(BRD_DEBUG_UART->SR) & USART_SR_TC) == 0);	//wait for transmission to complete

	}

}
//#pragma GCC diagnostic pop

//Transmit message through debug uart if enabled
void BRD_debuguart_putm(unsigned char *c, int len)
{
	int i;

	for (i = 0; i < len; i++) {
	//__HAL_UART_FLUSH_DRREGISTER(&UART_debug) = (uint8_t) (*(c + i));
		WRITE_REG(BRD_DEBUG_UART->DR, (unsigned char) (*(c + i)));
		while((READ_REG(BRD_DEBUG_UART->SR) & USART_SR_TC) == 0);	//wait for transmission to complete

	}

}


/* Debug UART getc */
unsigned char BRD_debuguart_getc(long unsigned int blocktime) {

	uint8_t rx_char = '\0';
	uint32_t prev_tick = 0;

	//Check if the blocktime is valid.
	if (blocktime > 0) {

		prev_tick = HAL_GetTick();		//Get current time
		
		//Implement block time
		while ((HAL_GetTick() - prev_tick) < blocktime) {

			//If byte received, return immediately.
			if ((BRD_DEBUG_UART->SR & USART_SR_RXNE) != 0){
				rx_char = READ_REG(BRD_DEBUG_UART->DR);
				return rx_char;
			}
		}
	} else {
		//If byte received, return immediately.
		if ((BRD_DEBUG_UART->SR & USART_SR_RXNE) != 0){
			rx_char = READ_REG(BRD_DEBUG_UART->DR);
			return rx_char;
		}
	}
	
	return rx_char;
}

void HAL_Delayus(uint32_t us) {
	volatile uint32_t cycles = (SystemCoreClock/1000000L)*us;
	volatile uint32_t start =  DWT->CYCCNT;
	do {
//__ASM volatile ("NOP");
  //  __ASM volatile ("NOP");
    //__ASM volatile ("NOP");
	//	start++;
	} while(DWT->CYCCNT - start < cycles);
	//}while(start < cycles); //while(DWT->CYCCNT - start < cycles);

}

void BRD_sysmon_init() {

	// Enable the GPIO G Clock
  	SYSMON_CHAN0_GPIO_CLK();

  	//Initialise G9 as an output.
  	SYSMON_CHAN0_GPIO->MODER &= ~(0x03 << (SYSMON_CHAN0_PIN * 2));  //clear bits
  	SYSMON_CHAN0_GPIO->MODER |= (0x01 << (SYSMON_CHAN0_PIN * 2));   //Set for push pull

  	SYSMON_CHAN0_GPIO->OSPEEDR &= ~(0x03<<(SYSMON_CHAN0_PIN * 2));
	SYSMON_CHAN0_GPIO->OSPEEDR |=   0x02<<(SYSMON_CHAN0_PIN * 2);  // Set for Fast speed

  	SYSMON_CHAN0_GPIO->OTYPER &= ~(0x01 << SYSMON_CHAN0_PIN);       //Clear Bit for Push/Pull utput

  	// Activate the Pull-up or Pull down resistor for the current IO
  	SYSMON_CHAN0_GPIO->PUPDR &= ~(0x03 << (SYSMON_CHAN0_PIN * 2));   //Clear Bits
  	SYSMON_CHAN0_GPIO->PUPDR |= ((0x01) << (SYSMON_CHAN0_PIN * 2));  //Set for Pull down output
  
	SYSMON_CHAN1_GPIO_CLK();

  	//Initialise G9 as an output.
  	SYSMON_CHAN1_GPIO->MODER &= ~(0x03 << (SYSMON_CHAN1_PIN * 2));  //clear bits
  	SYSMON_CHAN1_GPIO->MODER |= (0x01 << (SYSMON_CHAN1_PIN * 2));   //Set for push pull

  	SYSMON_CHAN1_GPIO->OSPEEDR &= ~(0x03<<(SYSMON_CHAN1_PIN * 2));
	SYSMON_CHAN1_GPIO->OSPEEDR |=   0x02<<(SYSMON_CHAN1_PIN * 2);  // Set for Fast speed

  	SYSMON_CHAN1_GPIO->OTYPER &= ~(0x01 << SYSMON_CHAN1_PIN);       //Clear Bit for Push/Pull utput

  	// Activate the Pull-up or Pull down resistor for the current IO
  	SYSMON_CHAN1_GPIO->PUPDR &= ~(0x03 << (SYSMON_CHAN1_PIN * 2));   //Clear Bits
  	SYSMON_CHAN1_GPIO->PUPDR |= ((0x01) << (SYSMON_CHAN1_PIN * 2));  //Set for Pull down output

	SYSMON_CHAN2_GPIO_CLK();

  	//Initialise G9 as an output.
  	SYSMON_CHAN2_GPIO->MODER &= ~(0x03 << (SYSMON_CHAN2_PIN * 2));  //clear bits
  	SYSMON_CHAN2_GPIO->MODER |= (0x01 << (SYSMON_CHAN2_PIN * 2));   //Set for push pull

  	SYSMON_CHAN2_GPIO->OSPEEDR &= ~(0x03<<(SYSMON_CHAN2_PIN * 2));
	SYSMON_CHAN2_GPIO->OSPEEDR |=   0x02<<(SYSMON_CHAN2_PIN * 2);  // Set for Fast speed

  	SYSMON_CHAN2_GPIO->OTYPER &= ~(0x01 << SYSMON_CHAN2_PIN);       //Clear Bit for Push/Pull utput

  	// Activate the Pull-up or Pull down resistor for the current IO
  	SYSMON_CHAN2_GPIO->PUPDR &= ~(0x03 << (SYSMON_CHAN2_PIN * 2));   //Clear Bits
  	SYSMON_CHAN2_GPIO->PUPDR |= ((0x01) << (SYSMON_CHAN2_PIN * 2));  //Set for Pull down output

	//Set PG7 high, to turn off LD7 LD8 (LED usb low current)
	__GPIOG_CLK_ENABLE();

	// Initalise G7 as an input.
  	GPIOG->MODER &= ~(0x03 << (7 * 2));			//Clear bits for input mode

 	GPIOG->OSPEEDR &= ~(0x03<<(7 * 2));
	GPIOG->OSPEEDR |=   0x02<<(7 * 2);  // Fast speed

	GPIOG->PUPDR &= ~(0x03 << (7 * 2));			//Clear bits for no push/pull
  	GPIOG->PUPDR |= ((0x02) << (7 * 2));  //Set for Pull down output
}
