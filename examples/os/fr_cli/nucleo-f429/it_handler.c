/**
 ******************************************************************************
 * @file    it_handler.c
 * @author  MDS
 * @date    02022021
 * @brief   Must only be used to for interrupt handlers. Only callbacks must be
 *          called in this file. No other code must be present.
 ******************************************************************************
 *
 */

#include "main.h"

/*
 * Interrupt handler for USART3 IRQ Handler
 */ 
void USART3_IRQHandler(void) {

	BRD_LEDGreenToggle();

	Uart_callback();   // Callback for USART3
}