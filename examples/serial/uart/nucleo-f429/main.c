/**
 ******************************************************************************
 * @file    serial/uart/main.c
 * @author  MDS
 * @date    02042021
 * @brief   USART polling example program - transmits '0' to '9' via USART 3
 * 			Uses PD9 (TX), PD8 (RX). To view the output, use a serial terminal:
 * 			PuTTY (windows, COMx baud 115200, no flow, no parity ) 
 * 			screen (OSX/Linux - screen /dev/ttyACM0 115200)
 *			Uses the following macros:
 * 		 	MODIFY_REG( Register, clear mask (bits to clear), set mask (bits to set))
 *          WRITE_REG( Register, value to set)
 *			READ_REG( (Register)
 *			SET_BIT ( Register, bit mask to set)
 *			CLEAR_BIT ( Register, bit mask to clear) 
 ******************************************************************************
 *
 */
#include "board.h"
#include "processor_hal.h"
#include "main.h"

#define UART_DEV_TX_PIN		8
#define UART_DEV_RX_PIN		9
#define UART_DEV_GPIO		GPIOD
#define UART_DEV_GPIO_AF 	GPIO_AF7_USART3
#define UART_DEV_GPIO_CLK()	__GPIOD_CLK_ENABLE()

#define UART_DEV		USART3
#define UART_DEV_CLK()	__USART3_CLK_ENABLE()
#define UART_DEV_BAUD	115200					 //NOTE: If using USART1 or USART6, HAL_RCC_GetPCLK2Freq must be used.
#define UART_DEV_IRQn	USART3_IRQn

void hardware_init(void);

/*
 * Main program
 */
int main(void) {

	uint8_t txChar;
	int txCount = 0;

	HAL_Init();
	hardware_init(); // Initialise hardware peripherals

	// Cyclic Executive (CE) loop
	while (1) {

		txChar = '0' + txCount; // Send characters '0' to '9' in ASCII
		txCount = (txCount + 1) % 10; // '0' to '9'

		// Transmit 1 character
		WRITE_REG(UART_DEV->DR, (unsigned char) txChar);

		// Wait for character to be transmitted.
		while((READ_REG(UART_DEV->SR) & USART_SR_TC) == 0);
	
		BRD_LEDBlueToggle();	//Toggle blue LED on/off
		HAL_Delay(1000);		//Delay for 1s (1000ms)
	}
}

/*
 * Initialise Hardware
 */
void hardware_init(void) {

	BRD_LEDInit(); //Initialise LEDs

	// Turn off LEDs
	BRD_LEDRedOff();
	BRD_LEDGreenOff();
	BRD_LEDBlueOff();

	// Enable UART DEV GPIO clock
	UART_DEV_GPIO_CLK();

	//Clear and Set Alternate Function for pin (upper ARF register) 
	MODIFY_REG(UART_DEV_GPIO->AFR[1], ((0x0F) << ((UART_DEV_RX_PIN-8) * 4)) | ((0x0F) << ((UART_DEV_TX_PIN-8)* 4)), ((UART_DEV_GPIO_AF << ((UART_DEV_RX_PIN-8) * 4)) | (UART_DEV_GPIO_AF << ((UART_DEV_TX_PIN-8)) * 4)));
	
	//Clear and Set Alternate Function Push Pull Mode
	MODIFY_REG(UART_DEV_GPIO->MODER, ((0x03 << (UART_DEV_RX_PIN * 2)) | (0x03 << (UART_DEV_TX_PIN * 2))), ((GPIO_MODE_AF_PP << (UART_DEV_RX_PIN * 2)) | (GPIO_MODE_AF_PP << (UART_DEV_TX_PIN * 2))));
	
	//Set low speed.
	SET_BIT(UART_DEV_GPIO->OSPEEDR, (GPIO_SPEED_LOW << UART_DEV_RX_PIN) | (GPIO_SPEED_LOW << UART_DEV_TX_PIN));

	//Set Bit for Push/Pull output
	SET_BIT(UART_DEV_GPIO->OTYPER, ((0x01 << UART_DEV_RX_PIN) | (0x01 << UART_DEV_TX_PIN)));

	//Clear and set bits for no push/pull
	MODIFY_REG(UART_DEV_GPIO->PUPDR, (0x03 << (UART_DEV_RX_PIN * 2)) | (0x03 << (UART_DEV_TX_PIN * 2)), (GPIO_PULLUP << (UART_DEV_RX_PIN * 2)) | (GPIO_PULLDOWN << (UART_DEV_TX_PIN * 2)));

	// UART Settings - No hardware flow control, 8 data bits, no parity, 1 start bit and 1 stop bit		
	// Enable USART clock
	UART_DEV_CLK();

	CLEAR_BIT(UART_DEV->CR1, USART_CR1_UE);  // Disable USART
	
	// Configure word length to 8 bit
	CLEAR_BIT(UART_DEV->CR1, USART_CR1_M);   // M: 00 = 8 data bits, 01 = 9 data bits, 10 = 7 data bits
	
	// Configure oversampling mode: Oversampling by 16 
	CLEAR_BIT(UART_DEV->CR1, USART_CR1_OVER8);  // 0 = oversampling by 16, 1 = oversampling by 8
	
	SET_BIT(UART_DEV->CR1, USART_CR1_RE | USART_CR1_TE);  	// Transmitter and Receiver enable
	CLEAR_BIT(UART_DEV->CR1, USART_CR1_PCE);						//Disable Parity

	// Configure stop bits to 1 stop bit and siable clock output (USART mode only)
	//   00: 1 Stop bit;      01: 0.5 Stop bit
	//   10: 2 Stop bits;     11: 1.5 Stop bit   
	CLEAR_BIT(UART_DEV->CR2, USART_CR2_CPHA | USART_CR2_CPOL | USART_CR2_CLKEN | USART_CR2_LBCL | USART_CR2_STOP);

	// Set Baudrate to 115200 using APB frequency (80,000,000 Hz) and 16 bit sampling
	// NOTE: If using USART1 or USART6, HAL_RCC_GetPCLK2Freq must be used.
	WRITE_REG(UART_DEV->BRR, UART_BRR_SAMPLING16(HAL_RCC_GetPCLK1Freq(), UART_DEV_BAUD));

	//Disable handshaing signals
	CLEAR_BIT(UART_DEV->CR3, USART_CR3_RTSE | USART_CR3_CTSE | USART_CR3_SCEN | USART_CR3_HDSEL | USART_CR3_IREN);

	//Enable receive interrupt
	SET_BIT(UART_DEV->CR1, USART_CR1_RXNEIE);

	//Enable priority (10) and interrupt callback. Do not set a priority lower than 5.
	HAL_NVIC_SetPriority(UART_DEV_IRQn, 10, 0);
	HAL_NVIC_EnableIRQ(UART_DEV_IRQn);

	SET_BIT(UART_DEV->CR1, USART_CR1_UE); // UART enable   
}

/*
 * Uart callback function - receive character.
 */
void Uart_callback(void) {

	uint8_t rxChar;

	if ((UART_DEV->SR & USART_SR_RXNE) != 0) {

		//Receive character from data register (automatically clears flag when read occurs)
		rxChar = READ_REG(UART_DEV->DR);

		// Reflect character back
		WRITE_REG(UART_DEV->DR, (unsigned char) rxChar);

		// Wait for character to be transmitted.
		while((READ_REG(UART_DEV->SR) & USART_SR_TC) == 0);
	}
}