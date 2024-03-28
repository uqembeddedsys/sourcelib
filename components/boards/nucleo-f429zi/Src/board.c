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
	BRD_debuguart_init();

	//CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

	//DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk; // Start DWT cycle counter used for HAL_Delayus();
	//DWT->CYCCNT = 0;

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

UART_HandleTypeDef UART_debug;

/* Initialise Debug UART */
void BRD_debuguart_init() {

#ifdef ENABLE_DEBUG_UART

	//Enable DEBUG UART clock
	__BRD_DEBUG_UART_CLK();

	UART_debug.Instance = (USART_TypeDef *)(BRD_DEBUG_UART);
    UART_debug.Init.BaudRate   = BRD_DEBUG_UART_BAUDRATE;
    UART_debug.Init.WordLength = UART_WORDLENGTH_8B;
    UART_debug.Init.StopBits   = UART_STOPBITS_1;
    UART_debug.Init.Parity     = UART_PARITY_NONE;
    UART_debug.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	UART_debug.Init.Mode = UART_MODE_TX_RX;

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

    //Allow stdio stdout buffer to be used for print, putc, etc.
    setbuf(stdout, NULL);

	HAL_UART_Init(&UART_debug);		//Initialise DEBUG UART
#endif

}

//Transmit char through debug uart and USB, if enabled
void BRD_debuguart_putc(unsigned char c)
{
	//__HAL_UART_FLUSH_DRREGISTER(&UART_debug) = (uint8_t) c;
	HAL_UART_Transmit(&UART_debug, &c, 1, 0xFFFF);
    __HAL_UART_FLUSH_DRREGISTER(&UART_debug);
}

//#pragma GCC diagnostic push
//#pragma GCC diagnostic ignored "-Wpointer-sign"
//Transmit string through debug uart and USB, if enabled
void BRD_debuguart_puts(unsigned char *c)
{
	int i;

	for (i = 0; i < (int) strlen(c); i++) {
	__HAL_UART_FLUSH_DRREGISTER(&UART_debug) = (uint8_t) (*(c + i));

	}

}
//#pragma GCC diagnostic pop

//Transmit message through debug uart and USB, if enabled
void BRD_debuguart_putm(unsigned char *c, int len)
{
	// int i;
	// for (i = 0; i < len; i++) {
	// __HAL_UART_FLUSH_DRREGISTER(&UART_debug) = (uint8_t) (*(c + i));
	// }

	HAL_UART_Transmit(&UART_debug, c, len, 0xFFFF);
    __HAL_UART_FLUSH_DRREGISTER(&UART_debug);

}


/* Debug UART getc */
unsigned char BRD_debuguart_getc(int blocktime) {

	uint8_t rx_char = '\0';

	//Non Block receive - 0 delay (set to HAL_MAX_DELAY for blocking)
	if (blocktime == 0) { 

		if ((UART_debug.Instance->SR & USART_SR_RXNE) != 0) {
			rx_char = UART_debug.Instance->DR;
		} else {
			rx_char = '\0';
		}

		__HAL_UART_FLUSH_DRREGISTER(&UART_debug);

		return rx_char;
	} else {
	if (HAL_UART_Receive(&UART_debug, &rx_char, 1, blocktime) == HAL_OK) {
		return rx_char;
	} else {
		return '\0';
	}
}

	//return (uint8_t)(__HAL_UART_FLUSH_DRREGISTER(&UART_debug) & (uint8_t)0x00FF);
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

