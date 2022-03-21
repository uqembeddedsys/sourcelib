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

#define TIMER_COUNTER_FREQ  50000           //Frequency (in Hz)
#define TIMER_1SECOND_PERIOD    50000       //Period for 1s (in count ticks)

void hardware_init(void);
void tui_callback(void);

#endif