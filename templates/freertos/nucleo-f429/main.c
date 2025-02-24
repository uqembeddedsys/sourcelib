/**
  ******************************************************************************
  * File:    freertos/template/main.c
  * Author:  MY_NAME_IS
  * Date:    04022018
  * Brief:   FreeRTOS Task template
  *
  * Reference: DONT_COPY_BLINDLY.pdf
  ******************************************************************************
  *
  */

#include "board.h"
#include "processor_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

void hardware_init();
void someTask( void );

// Task Priorities (Idle Priority is the lowest priority)
#define SOMETASK_PRIORITY					( tskIDLE_PRIORITY + 2 )

// Task Stack Allocations (must be a multiple of the minimal stack size)
#define SOMETASK_STACK_SIZE		( configMINIMAL_STACK_SIZE * 2 )


/*
 * Starts all the other tasks, then starts the scheduler. Must not have a Cyclic Executive.
 */
int main( void ) {

	HAL_Init();		//Only HAL_Init() must be called before task creation.

	//
	//Only functions that create tasks must be called in main.
	//

	// Start the task.
    xTaskCreate( (void *) &someTask, (const signed char *) "SOME", SOMETASK_STACK_SIZE, NULL, SOMETASK_PRIORITY, NULL );

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
 * Some Task.
 */
void someTask( void ) {

	hardware_init();

	// Cyclic Executive (CE) loop
	for (;;) {


		// Delay the task for 1ms.
		vTaskDelay(1);

	}
}

/*
 * Hardware Initialisation - MUST be alled from a task and not in main.
 */
void hardware_init( void ) {

	portDISABLE_INTERRUPTS();	//Disable interrupts

	//Initalise hardware

	portENABLE_INTERRUPTS();	//Enable interrupts
}
