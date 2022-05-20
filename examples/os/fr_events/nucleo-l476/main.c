/**
  ******************************************************************************
  * @file    os/fr_events/main.c
  * @author  MDS
  * @date    08052016
  * @brief   FreeRTOS LED Flashing program using group events. Creates an event flag to
  *			 signal a task to toggle the onboard Green LED.
  *
  ******************************************************************************
  *
  */

#include "board.h"
#include "processor_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

#define EVT_LED_ON			1 << 0		//LED Event Flag
#define EVT_LED_OFF			1 << 1		//Pushbutton Event Flag
#define LEDCTRL_EVENT		EVT_LED_ON | EVT_LED_OFF	//Control Event Group Mask

EventGroupHandle_t ledctrlEventGroup;		//Control Event Group

static void hardware_init();
void ApplicationIdleHook( void ); // The idle hook is just used to stream data to the USB port.
void giveTask( void );
void takeTask( void );

// Task Priorities
#define GIVETASK_PRIORITY					( tskIDLE_PRIORITY + 2 )
#define TAKETASK_PRIORITY					( tskIDLE_PRIORITY + 2 )

// Task Stack Allocations
#define GIVETASK_STACK_SIZE		( configMINIMAL_STACK_SIZE * 2 )
#define TAKETASK_STACK_SIZE		( configMINIMAL_STACK_SIZE * 2 )


/*
 * Starts all the other tasks, then starts the scheduler. Must not have a Cyclic Executive.
 */
int main( void ) {

	HAL_Init();		//Only HAL_Init() must be called before task creation.

	//
	//Only functions that create tasks must be called in main.
	//

	// Start give and take tasks.
    xTaskCreate( (void *) &giveTask, (const signed char *) "GIVE", GIVETASK_STACK_SIZE, NULL, GIVETASK_PRIORITY, NULL );
	xTaskCreate( (void *) &takeTask, (const signed char *) "TAKE", TAKETASK_STACK_SIZE, NULL, TAKETASK_PRIORITY, NULL );

	/* Start the scheduler.

	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used here. */

	vTaskStartScheduler();

  	return 0;
}

/**
  * @brief  Give Task. Set LED ON and OFF events every second.
  * @param  None
  * @retval None
  */
void giveTask( void ) {

	EventBits_t uxBits;
	uint8_t mode = 0;

	hardware_init();

	// Create Event Group
	ledctrlEventGroup = xEventGroupCreate();

	for (;;) {

		// If mode is 0, turn on LED else turn LED off.
		if ((mode & 0x01) == 0) {

			// Set LED ON event
			uxBits = xEventGroupSetBits(ledctrlEventGroup, EVT_LED_ON);

		} else {

			// Set LED OFF event
			uxBits = xEventGroupSetBits(ledctrlEventGroup, EVT_LED_OFF);
		}

		mode = ~mode;	//Invert mode

		// Wait for 1000ms
		vTaskDelay(1000);
	}
}


/*
 * Take Task. Used to check for LED ON and OFF events. Toggles LED.
 */
void takeTask( void ) {

	EventBits_t uxBits;

	BRD_LEDGreenOff();

	for (;;) {

		/* Wait a maximum of 100ms for either bit 0 or bit 1 to be set within
    	   the event group.  Clear the bits before exiting. */
    	uxBits = xEventGroupWaitBits(ledctrlEventGroup, LEDCTRL_EVENT, pdTRUE, pdFALSE, 10);

		// Check if LED Event flag is set
		if ((uxBits & EVT_LED_ON) != 0) {

            // Turn on LED
			BRD_LEDGreenOn();
		}

		/* Check if LED Event flag is set */
		if ((uxBits & EVT_LED_OFF) != 0) {

            // Turn off LED
			BRD_LEDGreenOff();
		}

		vTaskDelay(1);
	}
}


/**
  * @brief  Hardware Initialisation.
  * @param  None
  * @retval None
  */
static void hardware_init( void ) {

	GPIO_InitTypeDef GPIO_InitStructure;

	portDISABLE_INTERRUPTS();	//Disable interrupts

	BRD_LEDInit();				//Initialise Green LED
	BRD_LEDGreenOff();				//Turn off Green LED

	portENABLE_INTERRUPTS();	//Enable interrupts

}
