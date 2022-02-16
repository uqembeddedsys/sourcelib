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
 * Interrupt handler for Timer 2 Interrupt Handler
 */ 
void TIM2_IRQHandler(void)
{

	Tui_callback();   // Callback for timer update interrupt
}