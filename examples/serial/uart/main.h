/**
 ******************************************************************************
 * @file    main.h
 * @author  MDS
 * @date    05122019
 * @brief   Used for main.c prototypes and includes.
 ******************************************************************************
 *
 */

#ifndef MAIN_H
#define MAIN_H

#include "board.h"
#include "processor_hal.h"

void Hardware_init(void);
void Uart_callback(void);

#endif