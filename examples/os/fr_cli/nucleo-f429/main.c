/**
  ******************************************************************************
  * @file    os/fr_cli/main.c
  * @author  MDS
  * @date    04042021
  * @brief   FreeRTOS CLI example. Uses the CLI library and stream buffers
  *          to implement an echo command. Uses PD9 (TX), PD8 (RX). 
  *          To view the output and set the input, use a serial terminal:
  * 		 PuTTY (windows, COMx baud 115200, no flow, no parity ) 
  * 		 screen (OSX/Linux - screen /dev/ttyACM0 115200)
  *
  ******************************************************************************
  *
  */

#include "board.h"
#include "processor_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stream_buffer.h"
#include "FreeRTOS_CLI.h"
#include "string.h"

#define UART_DEV_TX_PIN		9
#define UART_DEV_RX_PIN		8
#define UART_DEV_GPIO		GPIOD
#define UART_DEV_GPIO_AF 	GPIO_AF7_USART3
#define UART_DEV_GPIO_CLK()	__GPIOD_CLK_ENABLE()

#define UART_DEV		USART3
#define UART_DEV_CLK()	__USART3_CLK_ENABLE()
#define UART_DEV_BAUD	115200					 //NOTE: If using USART1 or USART6, HAL_RCC_GetPCLK2Freq must be used.
#define UART_DEV_IRQn	USART3_IRQn

void cliTask(void);
void Uart_callback(void);
void hardware_init();

static BaseType_t prvEchoCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
static TaskHandle_t cliTask_handle = NULL;
StreamBufferHandle_t xStreamBuffer;

CLI_Command_Definition_t xEcho = {	// Structure that defines the "echo" command line command.
	"echo",
	"echo: Echo the input.\r\n",
	prvEchoCommand,
	1
};

// Task Priorities (Idle Priority is the lowest priority)
#define LEDTASK_PRIORITY				( tskIDLE_PRIORITY + 2 )
#define CLI_PRIORITY					( tskIDLE_PRIORITY + 2 )

// Task Stack Allocations (must be a multiple of the minimal stack size)
#define LEDTASK_STACK_SIZE		( configMINIMAL_STACK_SIZE * 2 )
#define CLI_TASK_STACK_SIZE		( configMINIMAL_STACK_SIZE * 3 )


/*
 * Starts all the other tasks, then starts the scheduler. Must not have a Cyclic Executive.
 */
int main( void ) {

	HAL_Init();		//Only HAL_Init() must be called before task creation.

	//
	//Only functions that create tasks must be called in main.
	//

	// Start the CLI task.
	xTaskCreate( (void *) &cliTask, (const signed char *) "CLI", CLI_TASK_STACK_SIZE, NULL, CLI_PRIORITY, &cliTask_handle );

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
 * CLI Receive Task.
 */
void cliTask(void) {

	int i;
	char cRxedChar;
	char cInputString[100];
	int InputIndex = 0;
	char *pcOutputString;
	BaseType_t xReturned;

	uint32_t ulNotifiedValue;
	size_t xReceivedBytes;

	const size_t xStreamBufferSizeBytes = 10, xTriggerLevel = 1;

    // Create a stream buffer
    xStreamBuffer = xStreamBufferCreate( xStreamBufferSizeBytes, xTriggerLevel );

	// Register teh CLI command
	FreeRTOS_CLIRegisterCommand(&xEcho);

	/* Initialise pointer to CLI output buffer. */
	memset(cInputString, 0, sizeof(cInputString));
	pcOutputString = FreeRTOS_CLIGetOutputBuffer();

	for (;;) {
		
		//Get any characters from the uart stream buffer.
		xReceivedBytes = xStreamBufferReceive( xStreamBuffer, ( void * ) &ulNotifiedValue, sizeof( ulNotifiedValue ), pdMS_TO_TICKS( 20 ) );

		cRxedChar = (uint8_t) ulNotifiedValue;

		// Process if character if not Null
		if (xReceivedBytes > 0) {
			if (cRxedChar != '\0') {
			

				// Echo character
				WRITE_REG(UART_DEV->DR, (unsigned char) cRxedChar);

				// Wait for character to be transmitted.
				while((READ_REG(UART_DEV->SR) & USART_SR_TC) == 0);

				// Process only if return is received. 
				if (cRxedChar == '\r') {

					// Put null character in command input string. 
					cInputString[InputIndex] = '\0';
					xReturned = pdTRUE;
					
					// Process command input string. 
					while (xReturned != pdFALSE) {

						// Returns pdFALSE, when all strings have been returned 
						xReturned = FreeRTOS_CLIProcessCommand( cInputString, pcOutputString, configCOMMAND_INT_MAX_OUTPUT_SIZE );

						// Display CLI command output string (not thread safe)
						portENTER_CRITICAL();
						for (i = 0; i < (int) strlen(pcOutputString); i++) {

							// Transmit 1 character
							WRITE_REG(UART_DEV->DR, (unsigned char) (*(pcOutputString + i)));

							// Wait for character to be transmitted.
							while((READ_REG(UART_DEV->SR) & USART_SR_TC) == 0);

						}
						portEXIT_CRITICAL();

						vTaskDelay(5);
					}

					memset(cInputString, 0, sizeof(cInputString));
					InputIndex = 0;

				} else {

					if( cRxedChar == '\r' ) {

						// Ignore the character. 
					} else if( cRxedChar == '\b' ) {

						// Backspace was pressed.  Erase the last character in the
						// string - if any.
						if( InputIndex > 0 ) {
							InputIndex--;
							cInputString[ InputIndex ] = '\0';
						}

					} else {

						//  A character was entered.  Add it to the string
						// entered so far.  When a \n is entered the complete
						// string will be passed to the command interpreter.
						if( InputIndex < 20 ) {
							cInputString[ InputIndex ] = cRxedChar;
							InputIndex++;
						}
					}
				}
			}
		}

		vTaskDelay(50);
	}
}

/*
 * Echo Command.
 */
static BaseType_t prvEchoCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString ) {

	long lParam_len;
	const char *cCmd_string;

	// Get parameters from command string 
	cCmd_string = FreeRTOS_CLIGetParameter(pcCommandString, 1, &lParam_len);

	// Write command echo output string to write buffer. 
	xWriteBufferLen = sprintf((char *) pcWriteBuffer, "\n\r%s\n\r", cCmd_string);

	// Return pdFALSE, as there are no more strings to return 
	// Only return pdTRUE, if more strings need to be printed 
	return pdFALSE;
}

/*
 * Uart callback function - receive character.
 */
void Uart_callback(void) {

	uint8_t rxChar;

	// Must be Initialised to pdFALSE!
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	uint32_t ulPreviousValue;

	if ((UART_DEV->SR & USART_SR_RXNE) != 0) {

		//Receive character from data register (automatically clears flag when read occurs)
		rxChar = READ_REG(UART_DEV->DR);

		xStreamBufferSendFromISR( xStreamBuffer, &rxChar,1, &xHigherPriorityTaskWoken );
	}
}

/*
 * Interrupt handler for USART3 IRQ Handler
 */ 
void USART3_IRQHandler(void) {

	BRD_LEDGreenToggle();

	Uart_callback();   // Callback for USART3
}


/*
 * Hardware Initialisation - MUST be alled from a task and not in main.
 */
void hardware_init( void ) {

	portDISABLE_INTERRUPTS();	//Disable interrupts

	BRD_LEDInit();				//Initialise Blue LED
	BRD_LEDGreenOff();			//Turn off Red LED


	// Enable UART DEV GPIO clock
	UART_DEV_GPIO_CLK();

	//Clear and Set Alternate Function for pin (upper ARF register) 
	MODIFY_REG(UART_DEV_GPIO->AFR[1], ((0x0F) << ((UART_DEV_RX_PIN-8) * 4)) | ((0x0F) << ((UART_DEV_TX_PIN-8)* 4)), ((UART_DEV_GPIO_AF << ((UART_DEV_RX_PIN-8) * 4)) | (UART_DEV_GPIO_AF << ((UART_DEV_TX_PIN-8)) * 4)));
	
	//Clear and Set Alternate Function Push Pull Mode
	MODIFY_REG(UART_DEV_GPIO->MODER, ((0x03 << (UART_DEV_RX_PIN * 2)) | (0x03 << (UART_DEV_TX_PIN * 2))), ((GPIO_MODE_AF_PP << (UART_DEV_RX_PIN * 2)) | (GPIO_MODE_AF_PP << (UART_DEV_TX_PIN * 2))));
	
	//Set low speed.
	SET_BIT(UART_DEV_GPIO->OSPEEDR, (GPIO_SPEED_LOW << UART_DEV_RX_PIN) | (GPIO_SPEED_LOW << UART_DEV_TX_PIN));

	//Set Bit for Push/Pull output
	SET_BIT(UART_DEV_GPIO->OTYPER, ((0x01 << UART_DEV_RX_PIN) | (0x01 << UART_DEV_TX_PIN)));

	//Clear and set bits for no push/pull
	MODIFY_REG(UART_DEV_GPIO->PUPDR, (0x03 << (UART_DEV_RX_PIN * 2)) | (0x03 << (UART_DEV_TX_PIN * 2)), (GPIO_PULLUP << (UART_DEV_RX_PIN * 2)) | (GPIO_PULLDOWN << (UART_DEV_TX_PIN * 2)));

	// UART Settings - No hardware flow control, 8 data bits, no parity, 1 start bit and 1 stop bit		
	// Enable USART clock
	UART_DEV_CLK();

	CLEAR_BIT(UART_DEV->CR1, USART_CR1_UE);  // Disable USART
	
	// Configure word length to 8 bit
	CLEAR_BIT(UART_DEV->CR1, USART_CR1_M);   // M: 00 = 8 data bits, 01 = 9 data bits, 10 = 7 data bits
	
	// Configure oversampling mode: Oversampling by 16 
	CLEAR_BIT(UART_DEV->CR1, USART_CR1_OVER8);  // 0 = oversampling by 16, 1 = oversampling by 8
	
	SET_BIT(UART_DEV->CR1, USART_CR1_RE | USART_CR1_TE);  	// Transmitter and Receiver enable
	CLEAR_BIT(UART_DEV->CR1, USART_CR1_PCE);						//Disable Parity

	// Configure stop bits to 1 stop bit and siable clock output (USART mode only)
	//   00: 1 Stop bit;      01: 0.5 Stop bit
	//   10: 2 Stop bits;     11: 1.5 Stop bit   
	CLEAR_BIT(UART_DEV->CR2, USART_CR2_CPHA | USART_CR2_CPOL | USART_CR2_CLKEN | USART_CR2_LBCL | USART_CR2_STOP);

	// Set Baudrate to 115200 using APB frequency (80,000,000 Hz) and 16 bit sampling
	// NOTE: If using USART1 or USART6, HAL_RCC_GetPCLK2Freq must be used.
	WRITE_REG(UART_DEV->BRR, UART_BRR_SAMPLING16(HAL_RCC_GetPCLK1Freq(), UART_DEV_BAUD));

	//Disable handshaing signals
	CLEAR_BIT(UART_DEV->CR3, USART_CR3_RTSE | USART_CR3_CTSE | USART_CR3_SCEN | USART_CR3_HDSEL | USART_CR3_IREN);

	//Enable receive interrupt
	SET_BIT(UART_DEV->CR1, USART_CR1_RXNEIE);

	//Enable priority (10) and interrupt callback. Do not set a priority lower than 5.
	HAL_NVIC_SetPriority(UART_DEV_IRQn, 10, 0);
	HAL_NVIC_EnableIRQ(UART_DEV_IRQn);

	SET_BIT(UART_DEV->CR1, USART_CR1_UE); // UART enable   

	portENABLE_INTERRUPTS();	//Enable interrupts
}
