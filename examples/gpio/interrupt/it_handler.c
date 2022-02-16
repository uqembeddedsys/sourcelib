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
 * Interrupt handler for EXTI 15 to 10 IRQ Handler
 */ 
void EXTI9_5_IRQHandler(void) {

	Pb_callback(9);   // Callback for C13
}