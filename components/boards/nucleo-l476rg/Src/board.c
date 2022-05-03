/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "processor_hal.h"
#include <stdio.h>
#include <string.h>

extern UART_HandleTypeDef UART_debug;

/**
  * @brief  Initialise the NP2 board by turning on the power headers
  * @param  Led: Specifies the Led to be configured.
  *   This parameter can be one of following parameters:
  * @retval None
  */
void BRD_init() {

	HAL_Init();
	BRD_debuguart_init();

	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	DWT->CYCCNT = 0;

	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk; // Start DWT cycle counter used for HAL_Delayus();


}



/**
  * @brief  Configures LED GPIO.
  * @param  Led: Specifies the Led to be configured.
  *   This parameter can be one of following parameters:

  * @retval None
  */
void BRD_LEDInit() {
  GPIO_InitTypeDef  GPIO_InitStructure;

  /* Enable the GPIO_LED Clock */
  __BRD_GREEN_LED_GPIO_CLK();

  /* Configure the GPIO_LED pin */
  GPIO_InitStructure.Pin = BRD_GREEN_LED_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Pull = GPIO_PULLUP;
  GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(BRD_GREEN_LED_GPIO_PORT, &GPIO_InitStructure);

}

/**
  * @brief  Turns selected LED On.
  * @param  Led: Specifies the Led to be set on.
  *   This parameter can be one of following parameters:
  * @retval None
  */
void BRD_LEDOn(uint8_t ledmask) {

	if ((ledmask & BRD_GREEN_LEDMASK) != 0) {
		BRD_GREEN_LED_GPIO_PORT->BSRR |= BRD_GREEN_LED_PIN;
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
  		BRD_GREEN_LED_GPIO_PORT->BSRR |= BRD_GREEN_LED_PIN << 16;
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
	  BRD_GREEN_LED_GPIO_PORT->ODR ^= BRD_GREEN_LED_PIN;
  }

}

UART_HandleTypeDef UART_debug;

/* Initialise Debug UART */
void BRD_debuguart_init()
{
	GPIO_InitTypeDef GPIO_serialtx;

	//Enable DEBUG UART clock
	__BRD_DEBUG_UART_CLK();

	UART_debug.Instance = (USART_TypeDef *)(BRD_DEBUG_UART);
    UART_debug.Init.BaudRate   = BRD_DEBUG_UART_BAUDRATE;
    UART_debug.Init.WordLength = UART_WORDLENGTH_8B;
    UART_debug.Init.StopBits   = UART_STOPBITS_1;
    UART_debug.Init.Parity     = UART_PARITY_NONE;
    UART_debug.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	UART_debug.Init.Mode = UART_MODE_TX_RX;

#ifdef ENABLE_DEBUG_UART
	__BRD_DEBUG_UART_TX_GPIO_CLK();
	__BRD_DEBUG_UART_RX_GPIO_CLK();

  	/* Configure the GPIO TX pin for DEBUG UART */
  	GPIO_serialtx.Pin = BRD_DEBUG_UART_TX_PIN;
  	GPIO_serialtx.Mode = GPIO_MODE_AF_PP;
  	GPIO_serialtx.Pull = GPIO_PULLUP;
  	GPIO_serialtx.Speed = GPIO_SPEED_FAST;
	GPIO_serialtx.Alternate = BRD_DEBUG_UART_TX_AF;
  	HAL_GPIO_Init(BRD_DEBUG_UART_TX_GPIO_PORT, &GPIO_serialtx);

  	GPIO_serialtx.Pin = BRD_DEBUG_UART_RX_PIN;
	GPIO_serialtx.Mode = GPIO_MODE_AF_PP;
	GPIO_serialtx.Pull = GPIO_PULLUP;
	GPIO_serialtx.Speed = GPIO_SPEED_FAST;
	GPIO_serialtx.Alternate = BRD_DEBUG_UART_RX_AF;
	HAL_GPIO_Init(BRD_DEBUG_UART_RX_GPIO_PORT, &GPIO_serialtx);

    //Allow stdio stdout buffer to be used for print, putc, etc.
    setbuf(stdout, NULL);

#endif

	HAL_UART_Init(&UART_debug);		//Initialise DEBUG UART
}

//Transmit char through debug uart and USB, if enabled
void BRD_debuguart_putc(unsigned char c)
{
	//__HAL_UART_FLUSH_DRREGISTER(&UART_debug) = (uint8_t) c;
	HAL_UART_Transmit(&UART_debug, &c, 1, 0xFFFF);
    __HAL_UART_FLUSH_DRREGISTER(&UART_debug);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpointer-sign"
//Transmit string through debug uart and USB, if enabled
void BRD_debuguart_puts(unsigned char *c)
{
	int i;

	for (i = 0; i < (int) strlen(c); i++) {
		BRD_debuguart_putc((uint8_t) (*(c + i)));
	//__HAL_UART_FLUSH_DRREGISTER(&UART_debug) = (uint8_t) (*(c + i));

	}

}
#pragma GCC diagnostic pop

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
unsigned char BRD_debuguart_getc() {

	uint8_t rx_char = '\0';

	//Non Block receive - 0 delay (set to HAL_MAX_DELAY for blocking)
	/*if (HAL_UART_Receive(&UART_debug, &rx_char, 1, 20) == HAL_OK) {
		return rx_char;
	} */

	while((READ_REG(BRD_DEBUG_UART->ISR) & USART_ISR_RXNE) == 0);
	 rx_char = READ_REG(BRD_DEBUG_UART->RDR);

	return rx_char;

	//return (uint8_t)(__HAL_UART_FLUSH_DRREGISTER(&UART_debug) & (uint8_t)0x00FF);
}

void HAL_Delayus(uint32_t us) {
	volatile uint32_t cycles = (SystemCoreClock/1000000L)*us;
	volatile uint32_t start = DWT->CYCCNT;
	do {

	} while(DWT->CYCCNT - start < cycles);

}

