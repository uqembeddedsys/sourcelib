/**
  ******************************************************************************
  * @file    os/fr_task/main.c
  * @author  MDS
  * @date    04022018
  * @brief   FreeRTOS Task example. Creates a task to flash the onboard
  *			 Blue LED.
  *
  * 		 Note: only HAL_Init must be called before
  ******************************************************************************
  *
  */

#include "board.h"
#include "processor_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

void hardware_init();
void ledTask( void );

// Task Priorities (Idle Priority is the lowest priority)
#define LEDTASK_PRIORITY					( tskIDLE_PRIORITY + 2 )

// Task Stack Allocations (must be a multiple of the minimal stack size)
#define LEDTASK_STACK_SIZE		( configMINIMAL_STACK_SIZE * 2 )


/*
 * Starts all the other tasks, then starts the scheduler. Must not have a Cyclic Executive.
 */
int main( void ) {

	HAL_Init();		//Only HAL_Init() must be called before task creation.

	//
	//Only functions that create tasks must be called in main.
	//

	// Start the task to flash the LED.
    xTaskCreate( (void *) &ledTask, (const signed char *) "LED", LEDTASK_STACK_SIZE, NULL, LEDTASK_PRIORITY, NULL );

	/* Start the scheduler.

	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used here. */

	vTaskStartScheduler();

  	return 0;
}

/*
 * LED Flashing Task.
 */
void ledTask( void ) {

	hardware_init();

	// Cyclic Executive (CE) loop
	for (;;) {

		// Toggle Green LED
        BRD_LEDGreenToggle();

		// Delay the task for 1000ms.
		vTaskDelay(1000);

	}
}

/*
 * Hardware Initialisation - MUST be alled from a task and not in main.
 */
void hardware_init( void ) {

	taskENTER_CRITICAL();	//Stop any interruption of the critical section

	BRD_LEDInit();				//Initialise Blue LED
	BRD_LEDGreenOff();			//Turn off Blue LED

	taskEXIT_CRITICAL();
}
