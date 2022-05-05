/**
  ******************************************************************************
  * @file    os/fr_queueset/main.c
  * @author  MDS
  * @date    24032018
  * @brief   FreeRTOS queue set demonstration. Creates a queue to
  *			 to send an item (Green LED value) from one task to another. Use 
  *          semaphore to toggle the Green LED. Use a semaphore given by the
  *          pushbutton to toggle the green LED (after 5 presses detected). 
  *          Queues and semaphores are added to the queue set in the 
  *          receiver task.
  * 
  *			 configUSE_QUEUE_SETS must be set to 1 in FreeRTOSConfig.h
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
SemaphoreHandle_t ledSemaphore;	// Semaphore for LED flasher
SemaphoreHandle_t pbSemaphore;	// Semaphore for pushbutton interrupt

static void hardware_init();
void senderTask( void );
void receiverTask( void );
void giveTask( void );
void pb_callback(uint16_t GPIO_Pin);

// Task Priorities 
#define SENDERTASK_PRIORITY					( tskIDLE_PRIORITY + 2 )
#define RECEIVERTASK_PRIORITY				( tskIDLE_PRIORITY + 2 )
#define GIVETASK_PRIORITY					( tskIDLE_PRIORITY + 2 )

// Task Stack Allocations 
#define SENDERTASK_STACK_SIZE		( configMINIMAL_STACK_SIZE * 2 )
#define RECEIVERTASK_STACK_SIZE		( configMINIMAL_STACK_SIZE * 2 )
#define GIVETASK_STACK_SIZE		( configMINIMAL_STACK_SIZE * 2 )

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
	xTaskCreate( (void *) &giveTask, (const signed char *) "GIVE", GIVETASK_STACK_SIZE, NULL, GIVETASK_PRIORITY, NULL );

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

		// Determine action to take based on counter lower 3 bit values.
		// Set on if counter is 1.
		if ((count & 0x07) == (1 << 0)) {
			onVal = 1;
		} else {
			onVal = 0;
		}
		
		//Set off if counter is 2.
		if ((count & 0x07) == (1 << 1)) {
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
 * Give Task.Gives LED Semaphore every second.
 */
void giveTask( void ) {

	for (;;) {


		if (ledSemaphore != NULL) {	// Check if semaphore exists

			// Give LED Semaphore
			xSemaphoreGive(ledSemaphore);
		}

		// Wait for 1000ms
		vTaskDelay(1000);

	}
}


/*
 *  Receiver Task. Used to receive messages.
 */
void receiverTask( void ) {

	struct Message RecvMessage;
	QueueSetHandle_t xQueueSet;
	QueueSetMemberHandle_t xActivatedMember;
	int presscount = 0;

	// Create QueueSet 
	xQueueSet = xQueueCreateSet(10 + 1 + 1);	//Size of Queueset = Size of Queue (10) + Size of Binary Semaphore (1) + Size of Binary Semaphore (1)

	// Create elements - note elements must be created BEFORE being added
	MessageQueue = xQueueCreate(10, sizeof(RecvMessage));		// Create queue of length 10 Message items 

	ledSemaphore = xSemaphoreCreateBinary();					// Create LED Semaphore 
	pbSemaphore = xSemaphoreCreateBinary();						// Create LED Semaphore 

	 /* Add the queues and semaphores to the set.  Reading from these queues and
    semaphore can only be performed after a call to xQueueSelectFromSet() has
    returned the queue or semaphore handle from this point on. */
    xQueueAddToSet(MessageQueue, xQueueSet);
    xQueueAddToSet(ledSemaphore, xQueueSet);
    xQueueAddToSet(pbSemaphore, xQueueSet);

	BRD_LEDGreenOff();

	for (;;) {

		xActivatedMember = xQueueSelectFromSet(xQueueSet, 20);

		/* Which set member was selected?  Receives/takes can use a block time
        of zero as they are guaranteed to pass because xQueueSelectFromSet()
        would not have returned the handle unless something was available. */
        if (xActivatedMember == MessageQueue) {

			// Check for item received - block atmost for 10 ticks
			if (xQueueReceive( MessageQueue, &RecvMessage, 10 )) {

				// Turn on Green LED
				if (RecvMessage.on) {
					BRD_LEDGreenOn();
				}

				// Turn off Green LED
				if (RecvMessage.off) {
					BRD_LEDGreenOff();
				}
        	}

		} else if (xActivatedMember == ledSemaphore) {	// Check if LED semaphore occurs 

			// We were able to obtain the semaphore and can now access the shared resource. 
			xSemaphoreTake(ledSemaphore, 0 );

            // Toggle Green LED
			BRD_LEDGreenToggle();

		} else if (xActivatedMember == pbSemaphore) {	// Check if pb semaphore occurs 

			// We were able to obtain the semaphore and can now access the shared resource. 
			xSemaphoreTake( pbSemaphore, 0 );
			
			presscount++;

			//Toggle Green LED when 5 pushbutton presses are detected.
			if (presscount > 5) {
				BRD_LEDGreenToggle();
				presscount = 0;
			}
		}

		// Delay for 10ms
		vTaskDelay(1);
	}
}


/*
 * Hardware Initialisation.
 */
static void hardware_init( void ) {

	portDISABLE_INTERRUPTS();	//Disable interrupts

	BRD_LEDInit();				//Initialise Green LED

	// Turn off LED
	BRD_LEDGreenOff();

	// Enable GPIOC Clock
	__GPIOC_CLK_ENABLE();

    GPIOC->OSPEEDR |= (GPIO_SPEED_FAST << 13);	//Set fast speed.
	GPIOC->PUPDR &= ~(0x03 << (13 * 2));			//Clear bits for no push/pull
	GPIOC->MODER &= ~(0x03 << (13 * 2));			//Clear bits for input mode

	// Enable EXTI clock
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	//select trigger source (port c, pin 13) on EXTICR4.
	SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13;
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;

	EXTI->RTSR |= EXTI_RTSR_TR13;	//enable rising dedge
	EXTI->FTSR &= ~EXTI_FTSR_TR13;	//disable falling edge
	EXTI->IMR |= EXTI_IMR_IM13;		//Enable external interrupt

	//Enable priority (10) and interrupt callback. Do not set a priority lower than 5.
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 10, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

	portENABLE_INTERRUPTS();	//Enable interrupts
}

/*
 * Pushbutton callback function
 */
void pb_callback(uint16_t GPIO_Pin)
{
	BaseType_t xHigherPriorityTaskWoken;

	if (GPIO_Pin == 13) {

		// Is it time for another Task() to run?
		xHigherPriorityTaskWoken = pdFALSE;

		if (pbSemaphore != NULL) {	// Check if semaphore exists 
			xSemaphoreGiveFromISR( pbSemaphore, &xHigherPriorityTaskWoken );		// Give PB Semaphore from ISR
		}

		// Perform context switching, if required.
		portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
	}
}

/*
 * Interrupt handler for EXTI 15 to 10 IRQ Handler
 */ 
void EXTI15_10_IRQHandler(void) {

	NVIC_ClearPendingIRQ(EXTI15_10_IRQn);

	// PR: Pending register
	if ((EXTI->PR & EXTI_PR_PR13) == EXTI_PR_PR13) {

		// cleared by writing a 1 to this bit
		EXTI->PR |= EXTI_PR_PR13;	//Clear interrupt flag.

		pb_callback(13);   // Callback for C13
	}
}
