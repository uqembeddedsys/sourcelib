/**
  ******************************************************************************
  * @file    os/fr_queue/main.c
  * @author  MDS
  * @date    22032018
  * @brief   FreeRTOS queue demonstration.Creates a queue to
  *			 to send an item from one task to another. The output will turn the
  *			 Green LED on or off.
  *
  *			 NOTE: THE IDLE TASK MUST BE DISABLED.
  ******************************************************************************
  *
  */

#include "board.h"
#include "processor_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

struct Message {	// Example Message consists of sequence number and LED values
	int Sequence_Number;
	unsigned char on;
	unsigned char off;
};

QueueHandle_t MessageQueue;	// Queue used

static void hardware_init();
void senderTask( void );
void receiverTask( void );

// Task Priorities 
#define SENDERTASK_PRIORITY					( tskIDLE_PRIORITY + 2 )
#define RECEIVERTASK_PRIORITY				( tskIDLE_PRIORITY + 2 )

// Task Stack Allocations 
#define SENDERTASK_STACK_SIZE		( configMINIMAL_STACK_SIZE * 2 )
#define RECEIVERTASK_STACK_SIZE		( configMINIMAL_STACK_SIZE * 2 )

/*
 * Starts all the other tasks, then starts the scheduler.
 */
int main( void ) {

	HAL_Init();		//Only HAL_Init() must be called before task creation.

	//
	//Only functions that create tasks must be called in main.
	//

	// Start sender and receiver tasks
    xTaskCreate( (void *) &senderTask, (const signed char *) "SEND", SENDERTASK_STACK_SIZE, NULL, SENDERTASK_PRIORITY, NULL );
	xTaskCreate( (void *) &receiverTask, (const signed char *) "RECV", RECEIVERTASK_STACK_SIZE, NULL, RECEIVERTASK_PRIORITY, NULL );

	/* Start the scheduler.

	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used here. */

	vTaskStartScheduler();

	/* We should never get here as control is now taken by the scheduler. */
  	return 0;
}

/*
 * Sender Task.Send a message to the queue, every second.
 */
void senderTask( void ) {

	unsigned char onVal;
	unsigned char offVal;
	int count;

	struct Message SendMessage;

	hardware_init();

	MessageQueue = xQueueCreate(10, sizeof(SendMessage));		// Create queue of length 10 Message items 

	count = 0;
	onVal = 0;
	offVal = 0;

	SendMessage.Sequence_Number = 0;

	for (;;) {

		// Determine action to take based on counter lower 2 bit values.
		// Set on if counter is 1.
		if ((count & 0x03) == (1 << 0)) {
			onVal = 1;
		} else {
			onVal = 0;
		}
		
		//Set off if counter is 2.
		if ((count & 0x03) == (1 << 1)) {
			offVal = 1;
		} else {
			offVal = 0;
		}

		if (MessageQueue != NULL) {	// Check if queue exists 

			// Send message to the front of the queue - wait atmost 10 ticks 
			SendMessage.on = onVal;
			SendMessage.off = offVal;

			xQueueSendToFront(MessageQueue, ( void * ) &SendMessage, ( portTickType ) 10 );
		}

		SendMessage.Sequence_Number++;		// Increment Sequence Number 

		count++;

		// Wait for 1000ms 
		vTaskDelay(1000);

	}
}

/*
 * Receiver Task. Used to receive messages.
 */
void receiverTask( void ) {

	struct Message RecvMessage;

	BRD_LEDGreenOff();

	for (;;) {

		if (MessageQueue != NULL) {	// Check if queue exists

			// Check for item received - block atmost for 10 ticks
			if (xQueueReceive( MessageQueue, &RecvMessage, 10 )) {

				// Turn green LED on
				if (RecvMessage.on) {
					BRD_LEDGreenOn();
				}

				// Turn green off
				if (RecvMessage.off) {
					BRD_LEDGreenOff();
				}
        	}
		}

		// Delay for 10ms
		vTaskDelay(10);
	}
}


/*
 * Hardware Initialisation.
 */
void hardware_init( void ) {

	portDISABLE_INTERRUPTS();	//Disable interrupts
	BRD_LEDInit();				//Initialise Green LED
	BRD_LEDGreenOff();				//Turn off Green  LED
	portENABLE_INTERRUPTS();	//Enable interrupts

}
