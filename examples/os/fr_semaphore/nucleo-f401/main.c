/**
  ******************************************************************************
  * @file    os/fr_semaphore/main.c
  * @author  MDS
  * @date    02022015
  * @brief   FreeRTOS LED Flashing program using semaphores.Creates a semaphore to
  *			 signal a task to toggle the onboard Green LED. Creates another semaphore
  *			 to start/stop the LED Flashing, using the onboard pushbutton.
  *
  *			 Press the pushbutton to start and stop LED flashing (run kermusb).
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

SemaphoreHandle_t ledSemaphore;	// Semaphore for LED flasher
SemaphoreHandle_t pbSemaphore;	// Semaphore for pushbutton interrupt

static void hardware_init();
void giveTask( void );
void takeTask( void );
void pb_callback(uint16_t GPIO_Pin);

// Task Priorities
#define GIVETASK_PRIORITY					( tskIDLE_PRIORITY + 2 )
#define TAKETASK_PRIORITY					( tskIDLE_PRIORITY + 2 )

// Task Stack Allocations
#define GIVETASK_STACK_SIZE		( configMINIMAL_STACK_SIZE * 2 )
#define TAKETASK_STACK_SIZE		( configMINIMAL_STACK_SIZE * 2 )


/*
 * Starts all the other tasks, then starts the scheduler.
 */
int main( void ) {

	HAL_Init();		//Only HAL_Init() must be called before task creation.

	//
	//Only functions that create tasks must be called in main.
	//

	// Start task one, two and three. 
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

/*
 * Give Task.Gives LED Semaphore every second.
 */
void giveTask( void ) {

	// Create Semaphores 
	ledSemaphore = xSemaphoreCreateBinary();
	pbSemaphore = xSemaphoreCreateBinary();

	hardware_init();

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
 * Take Task. Used to take semaphore events. Toggles LED.
 */
void takeTask( void ) {

	uint8_t mode = 1;
	int presscount = 0;

	BRD_LEDGreenOff();

	for (;;) {

		if (ledSemaphore != NULL) {	// Check if semaphore exists 

			/* See if we can obtain the LED semaphore. If the semaphore is not available
          	 wait 10 ticks to see if it becomes free. */
			if ( xSemaphoreTake( ledSemaphore, 10 ) == pdTRUE ) {

            	// We were able to obtain the semaphore and can now access the shared resource.

            	// Toggle LED, if in correct mode
				if (mode == 1) {
					BRD_LEDGreenToggle();
				}
        	}
		}

		if (pbSemaphore != NULL) {	// Check if semaphore exists

			/* See if we can obtain the PB semaphore. If the semaphore is not available
           	wait 10 ticks to see if it becomes free. */
			if( xSemaphoreTake( pbSemaphore, 10 ) == pdTRUE ) {

            	// We were able to obtain the semaphore and can now access the shared resource. 

            	// Invert mode to stop or start LED flashing
				mode = ~mode & 0x01;
				presscount++;

				//Toggle Green LED when 5 pushbutton presses are detected.
				if (presscount > 5) {
					BRD_LEDGreenToggle();
					presscount = 0;
				}
        	}
		}

		vTaskDelay(1);
	}
}

/*
 * Hardware Initialisation.
 */
static void hardware_init( void ) {

	portDISABLE_INTERRUPTS();	//Disable interrupts

	BRD_LEDInit();				//Initialise Green LED

	// Turn off LEDs
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
		
		// Toggle Green LED
		BRD_LEDGreenToggle();

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

