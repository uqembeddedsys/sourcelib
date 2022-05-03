/**
  ******************************************************************************
  * @file    os/fr_cli/main.c
  * @author  MDS
  * @date    04042021
  * @brief   FreeRTOS CLI example. Uses the CLI library
  *          to implement an echo command. 
  *          To view the output and set the input, use a serial terminal:
  * 		 PuTTY (windows, COMx baud 115200, no flow, no parity ) 
  * 		 screen (OSX/Linux - screen /dev/ttyACM0 115200)
  * 
  * 		 Green LED will toggle when a character is received from the
  *          serial terminal.
  *
  ******************************************************************************
  *
  */

#include "board.h"
#include "processor_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOS_CLI.h"
#include "debug_log.h"
#include <string.h>

void cliTask(void);
void initCli(void);
void hardware_init();

static BaseType_t prvEchoCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );

CLI_Command_Definition_t xEcho = {	// Structure that defines the "echo" command line command.
	"echo",							// Comamnd String
	"echo: Echo the input.\r\n",	// Help String (Displayed when "help' is typed)
	prvEchoCommand,					// Command Callback that implements the command
	1								// Number of input parameters
};

// Task Priorities (Idle Priority is the lowest priority)
#define CLI_PRIORITY					( tskIDLE_PRIORITY + 2 )

// Task Stack Allocations (must be a multiple of the minimal stack size)
#define CLI_TASK_STACK_SIZE		( configMINIMAL_STACK_SIZE * 2 )


/*
 * Starts all the other tasks, then starts the scheduler. Must not have a Cyclic Executive.
 */
int main( void ) {

	HAL_Init();		//Only HAL_Init() must be called before task creation.

	// Start the CLI.
	initCli();
	
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
 * Initialise CLI
 */
void initCli(void) {

	/* Register CLI commands */
	FreeRTOS_CLIRegisterCommand(&xEcho);

	/* Create CLI Receiving task */
	xTaskCreate( (void *) &cliTask, (const signed char *) "CLI", CLI_TASK_STACK_SIZE, NULL, CLI_PRIORITY, NULL );

}

/*
 * CLI Receiving Task.
 */
void cliTask(void) {

	int i;
	char cRxedChar;
	char cInputString[100];
	int InputIndex = 0;
	char *pcOutputString;
	BaseType_t xReturned;

	hardware_init();

	/* Initialise pointer to CLI output buffer. */
	memset(cInputString, 0, sizeof(cInputString));
	pcOutputString = FreeRTOS_CLIGetOutputBuffer();

	for (;;) {

		/* Receive character from terminal */
		cRxedChar = debug_getc();

		/* Process if character if not Null */
		if (cRxedChar != '\0') {

			BRD_LEDGreenToggle();		//Toggle Green LED when a byte is character.

			/* Echo character */
			debug_putc(cRxedChar);

			/* Process only if return is received. */
			if (cRxedChar == '\r') {

				//Put new line and transmit buffer
				debug_putc('\n');
				debug_flush();

				/* Put null character in command input string. */
				cInputString[InputIndex] = '\0';

				xReturned = pdTRUE;
				/* Process command input string. */
				while (xReturned != pdFALSE) {

					/* Returns pdFALSE, when all strings have been returned */
					xReturned = FreeRTOS_CLIProcessCommand( cInputString, pcOutputString, configCOMMAND_INT_MAX_OUTPUT_SIZE );

					/* Display CLI command output string (not thread safe) */
					portENTER_CRITICAL();
					for (i = 0; i < (int) strlen(pcOutputString); i++) {
						debug_putc(*(pcOutputString + i));
					}
					portEXIT_CRITICAL();

				    vTaskDelay(5);	//Must delay between debug_printfs.
				}

				memset(cInputString, 0, sizeof(cInputString));
				InputIndex = 0;

			} else {

				debug_flush();		//Transmit USB buffer

				if( cRxedChar == '\r' ) {

					/* Ignore the character. */
				} else if( cRxedChar == '\b' ) {

					/* Backspace was pressed.  Erase the last character in the
					 string - if any.*/
					if( InputIndex > 0 ) {
						InputIndex--;
						cInputString[ InputIndex ] = '\0';
					}

				} else {

					/* A character was entered.  Add it to the string
					   entered so far.  When a \n is entered the complete
					   string will be passed to the command interpreter. */
					if( InputIndex < 20 ) {
						cInputString[ InputIndex ] = cRxedChar;
						InputIndex++;
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

	/* Get parameters from command string */
	cCmd_string = FreeRTOS_CLIGetParameter(pcCommandString, 1, &lParam_len);

	/* Write command echo output string to write buffer. */
	xWriteBufferLen = sprintf((char *) pcWriteBuffer, "\n\r%s\n\r", cCmd_string);

	/* Return pdFALSE, as there are no more strings to return */
	/* Only return pdTRUE, if more strings need to be printed */
	return pdFALSE;
}


/*
 * Hardware Initialisation.
 */
void hardware_init( void ) {

	portDISABLE_INTERRUPTS();	//Disable interrupts

	BRD_LEDInit();				//Initialise Green LED
	BRD_LEDGreenOff();			//Turn off Green LED
	BRD_debuguart_init();  		//Initialise UART for debug log output

	portENABLE_INTERRUPTS();	//Enable interrupts
}