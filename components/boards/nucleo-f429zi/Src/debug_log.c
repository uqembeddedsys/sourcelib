/*
 * debug_printf.c
 *
 *  Created on: 22Nov.,2017
 *      Author: dso005
 */

#include <stdio.h>
#include "board.h"
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

#include "debug_log.h"
//#include <syscalls.c>

//#define SIDEBOARD_IN_USE

void debug_putc(char c) {

	BRD_debuguart_putc(c);

}

void debug_flush() {

}

unsigned char debug_getc(void) {
	uint8_t c = '\0';


	c = BRD_debuguart_getc(10);

	return c;
}

void debug_rxflush() {

}

/* Private function prototypes -----------------------------------------------*/
//#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
// 	#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
//	#define GETCHAR_PROTOTYPE int __io_getchar(int ch)
//#else
// 	#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
//	#define GETCHAR_PROTOTYPE int fgetc(FILE *f)

//#endif /* __GNUC__ */

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  // Place your implementation of fputc here
  // e.g. write a character to the EVAL_COM1 and Loop until the end of transmission
  //HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, 0xFFFF);
	//__HAL_UART_FLUSH_DRREGISTER(&UartHandle) = (uint8_t) ch;
	//HAL_Delay(100);

	BRD_debuguart_putc( (uint8_t)ch);
  return ch;
}//*/


GETCHAR_PROTOTYPE
{
	uint8_t c = '\0';

	c = BRD_debuguart_getc(10);

	//if (c != '\0') {
	//	return -1;
	//} else {
		return c;
	//}

}//*/

void debug_log (const char *fmt, ...) {

//	printf("debug here\n");
	va_list args;

	  va_start (args, fmt);


	  vprintf (fmt, args);

	  va_end (args);
}

