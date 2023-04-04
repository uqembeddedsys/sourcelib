/**
  ******************************************************************************
  * @file    os/fr_sysmon/main.c
  * @author  MDS
  * @date    03042023
  * @brief   FreeRTOS System Monitor example. Creates 3 tasks for use with
  *          System Monitor. System monitor used is on pins D0, D1 and D3
  *          An osciloscope or logic analyser can be used to view the 
  *          system monitor signals.
  * 
  * 		 Reference: Task Critical Section
  * 		 https://www.freertos.org/taskENTER_CRITICAL_taskEXIT_CRITICAL.html
  *
  ******************************************************************************
  *
  */

#include "board.h"
#include "processor_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

void hardware_init();
void Task1_Task( void );
void Task2_Task( void );
void Task3_Task( void );

// Task Priorities (Idle Priority is the lowest priority)
#define TASK1_PRIORITY					( tskIDLE_PRIORITY + 2 )
#define TASK2_PRIORITY					( tskIDLE_PRIORITY + 2 )
#define TASK3_PRIORITY					( tskIDLE_PRIORITY + 2 )

// Task Stack Allocations (must be a multiple of the minimal stack size)
#define TASK1_STACK_SIZE		( configMINIMAL_STACK_SIZE * 2 )
#define TASK2_STACK_SIZE		( configMINIMAL_STACK_SIZE * 2 )
#define TASK3_STACK_SIZE		( configMINIMAL_STACK_SIZE * 2 )


//System monitor pins
#define SYSMON_CHAN0_PIN		9
#define SYSMON_CHAN0_GPIO		GPIOG
#define SYSMON_CHAN0_GPIO_CLK()	__GPIOG_CLK_ENABLE()

#define SYSMON_CHAN1_PIN		14
#define SYSMON_CHAN1_GPIO		GPIOG
#define SYSMON_CHAN1_GPIO_CLK()	__GPIOG_CLK_ENABLE()

#define SYSMON_CHAN2_PIN		15
#define SYSMON_CHAN2_GPIO		GPIOF
#define SYSMON_CHAN2_GPIO_CLK()	__GPIOF_CLK_ENABLE()

//System monitor macros
#define SYSMON_CHAN0_SET()		SYSMON_CHAN0_GPIO->ODR |= (0x01 << SYSMON_CHAN0_PIN);
#define SYSMON_CHAN0_CLR()		SYSMON_CHAN0_GPIO->ODR &= ~(0x01 << SYSMON_CHAN0_PIN);
#define SYSMON_CHAN1_SET()		SYSMON_CHAN1_GPIO->ODR |= (0x01 << SYSMON_CHAN1_PIN);
#define SYSMON_CHAN1_CLR()		SYSMON_CHAN1_GPIO->ODR &= ~(0x01 << SYSMON_CHAN1_PIN);
#define SYSMON_CHAN2_SET()		SYSMON_CHAN2_GPIO->ODR |= (0x01 << SYSMON_CHAN2_PIN);
#define SYSMON_CHAN2_CLR()		SYSMON_CHAN2_GPIO->ODR &= ~(0x01 << SYSMON_CHAN2_PIN);


/*
 * Starts all the other tasks, then starts the scheduler. Must not have a Cyclic Executive.
 */
int main( void ) {

	HAL_Init();		//Only HAL_Init() must be called before task creation.

	//
	//Only functions that create tasks must be called in main.
	//

	// Start the task to flash the LED.
    xTaskCreate( (void *) &Task1_Task, (const signed char *) "T1", TASK1_STACK_SIZE, NULL, TASK1_PRIORITY, NULL );
	xTaskCreate( (void *) &Task2_Task, (const signed char *) "T2", TASK2_STACK_SIZE, NULL, TASK2_PRIORITY, NULL );
	xTaskCreate( (void *) &Task3_Task, (const signed char *) "T3", TASK3_STACK_SIZE, NULL, TASK3_PRIORITY, NULL );

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
 * Task 1.
 */
void Task1_Task( void ) {

	hardware_init();

	SYSMON_CHAN0_CLR();				//Clear LA Channel 0

	for (;;) {

		SYSMON_CHAN0_SET();			//Set LA Channel 0	
		BRD_LEDGreenToggle();		//Random instruction	

		vTaskDelay(3);			//Extra Delay for 3ms	
		SYSMON_CHAN0_CLR();			//Clear LA Channel 0	
        
	}
}

/*
 * Task 2.
 */
void Task2_Task( void ) {

	hardware_init();

	SYSMON_CHAN1_CLR();				//Clear LA Channel 1

	for (;;) {

		SYSMON_CHAN1_SET();			//Set LA Channel 1	
		BRD_LEDGreenToggle();		//Random instruction	

		vTaskDelay(3);				//Extra Delay for 3ms	
		SYSMON_CHAN1_CLR();			//Clear LA Channel 1	
        
	}
}

/*
 * Task3.
 */
void Task3_Task( void ) {

	hardware_init();

	SYSMON_CHAN2_CLR();				//Clear LA Channel 2

	for (;;) {

		SYSMON_CHAN2_SET();			//Set LA Channel 2	
		BRD_LEDGreenToggle();		//Random instruction	

		SYSMON_CHAN2_CLR();			//Clear LA Channel 2	
	}
}

/*
 * Hardware Initialisation - MUST be alled from a task and not in main.
 */
void hardware_init( void ) {

	taskENTER_CRITICAL();	//Stop any interruption of the critical section

	BRD_LEDInit();				//Initialise Green LED
	BRD_LEDGreenOff();			//Turn off Green LED

	// Enable the GPIO G Clock
  	SYSMON_CHAN0_GPIO_CLK();

  	//Initialise G9 as an output.
  	SYSMON_CHAN0_GPIO->MODER &= ~(0x03 << (SYSMON_CHAN0_PIN * 2));  //clear bits
  	SYSMON_CHAN0_GPIO->MODER |= (0x01 << (SYSMON_CHAN0_PIN * 2));   //Set for push pull

  	SYSMON_CHAN0_GPIO->OSPEEDR &= ~(0x03<<(SYSMON_CHAN0_PIN * 2));
	SYSMON_CHAN0_GPIO->OSPEEDR |=   0x02<<(SYSMON_CHAN0_PIN * 2);  // Set for Fast speed

  	SYSMON_CHAN0_GPIO->OTYPER &= ~(0x01 << SYSMON_CHAN0_PIN);       //Clear Bit for Push/Pull utput

  	// Activate the Pull-up or Pull down resistor for the current IO
  	SYSMON_CHAN0_GPIO->PUPDR &= ~(0x03 << (SYSMON_CHAN0_PIN * 2));   //Clear Bits
  	SYSMON_CHAN0_GPIO->PUPDR |= ((0x01) << (SYSMON_CHAN0_PIN * 2));  //Set for Pull down output
  
	SYSMON_CHAN1_GPIO_CLK();

  	//Initialise G9 as an output.
  	SYSMON_CHAN1_GPIO->MODER &= ~(0x03 << (SYSMON_CHAN1_PIN * 2));  //clear bits
  	SYSMON_CHAN1_GPIO->MODER |= (0x01 << (SYSMON_CHAN1_PIN * 2));   //Set for push pull

  	SYSMON_CHAN1_GPIO->OSPEEDR &= ~(0x03<<(SYSMON_CHAN1_PIN * 2));
	SYSMON_CHAN1_GPIO->OSPEEDR |=   0x02<<(SYSMON_CHAN1_PIN * 2);  // Set for Fast speed

  	SYSMON_CHAN1_GPIO->OTYPER &= ~(0x01 << SYSMON_CHAN1_PIN);       //Clear Bit for Push/Pull utput

  	// Activate the Pull-up or Pull down resistor for the current IO
  	SYSMON_CHAN1_GPIO->PUPDR &= ~(0x03 << (SYSMON_CHAN1_PIN * 2));   //Clear Bits
  	SYSMON_CHAN1_GPIO->PUPDR |= ((0x01) << (SYSMON_CHAN1_PIN * 2));  //Set for Pull down output

	SYSMON_CHAN2_GPIO_CLK();

  	//Initialise G9 as an output.
  	SYSMON_CHAN2_GPIO->MODER &= ~(0x03 << (SYSMON_CHAN2_PIN * 2));  //clear bits
  	SYSMON_CHAN2_GPIO->MODER |= (0x01 << (SYSMON_CHAN2_PIN * 2));   //Set for push pull

  	SYSMON_CHAN2_GPIO->OSPEEDR &= ~(0x03<<(SYSMON_CHAN2_PIN * 2));
	SYSMON_CHAN2_GPIO->OSPEEDR |=   0x02<<(SYSMON_CHAN2_PIN * 2);  // Set for Fast speed

  	SYSMON_CHAN2_GPIO->OTYPER &= ~(0x01 << SYSMON_CHAN2_PIN);       //Clear Bit for Push/Pull utput

  	// Activate the Pull-up or Pull down resistor for the current IO
  	SYSMON_CHAN2_GPIO->PUPDR &= ~(0x03 << (SYSMON_CHAN2_PIN * 2));   //Clear Bits
  	SYSMON_CHAN2_GPIO->PUPDR |= ((0x01) << (SYSMON_CHAN2_PIN * 2));  //Set for Pull down output

	taskEXIT_CRITICAL();
}
