/**
  ******************************************************************************
  * @file    os/fr_queue/main.c
  * @author  MDS
  * @date    22032018
  * @brief   FreeRTOS queue demonstration.Creates a queue to
  *			 to send an item from one task to another. Output is seen on the 
  *			 LEDS (Red, Blue, Green).
  *
  *			 NOTE: THE IDLE TASK MUST BE DISABLED.
  ******************************************************************************
  *
  */

#include "board.h"
#include "processor_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stream_buffer.h"


struct Message {	// Example Message consists of sequence number and LED values
	int Sequence_Number;
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};

StreamBufferHandle_t xStreamBuffer;

static void Hardware_init();
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

	unsigned char redvalue;
	unsigned char greenvalue;
	unsigned char bluevalue;
	int count;

	size_t xReceivedBytes;

	const size_t xStreamBufferSizeBytes = 10, xTriggerLevel = 1;

	Hardware_init();

    // Create a stream buffer
    xStreamBuffer = xStreamBufferCreate( xStreamBufferSizeBytes, xTriggerLevel );

	for (;;) {

		// Set values for LEDs.
		redvalue = (count & (1 << 2));
		greenvalue = (count & (1 << 0));
		bluevalue = (count & (1 << 1));
		
		if (MessageQueue != NULL) {	// Check if queue exists 

			// Send message to the front of the queue - wait atmost 10 ticks 
			SendMessage.red = redvalue;
			SendMessage.green = greenvalue;
			SendMessage.blue = bluevalue;

			xQueueSendToFront(MessageQueue, ( void * ) &SendMessage, ( portTickType ) 10 );
		}


		// Wait for 1000ms 
		vTaskDelay(1000);

	}
}

/*
 * Receiver Task. Used to receive messages.
 */
void receiverTask( void ) {

	struct Message RecvMessage;

	BRD_LEDBlueOff();

	for (;;) {

		if (MessageQueue != NULL) {	// Check if queue exists

			// Check for item received - block atmost for 10 ticks
			if (xQueueReceive( MessageQueue, &RecvMessage, 10 )) {

				// Set or clear red LED
				if (RecvMessage.red) {
					BRD_LEDRedOn();
				} else {
					BRD_LEDRedOff();
				}

				// Set or clear green LED
				if (RecvMessage.green) {
					BRD_LEDGreenOn();
				} else {
					BRD_LEDGreenOff();
				}

				// Set or clear blue LED
				if (RecvMessage.blue) {
					BRD_LEDBlueOn();
				} else {
					BRD_LEDBlueOff();
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
void Hardware_init( void ) {

	portDISABLE_INTERRUPTS();	//Disable interrupts
	BRD_LEDInit();				//Initialise Blue LED
	BRD_LEDBlueOff();				//Turn off Blue LED
	portENABLE_INTERRUPTS();	//Enable interrupts

}
