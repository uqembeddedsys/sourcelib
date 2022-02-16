
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DEBUG_PRINTF_H
#define __DEBUG_PRINTF_H

extern void debug_putc(char c);
extern void debug_flush();
extern unsigned char debug_getc();
extern void debug_rxflush();
//extern void debug_printf();
extern void debug_log (const char *fmt, ...);


#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
 	#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
	#define GETCHAR_PROTOTYPE int __io_getchar(void)
#else
 	#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
	#define GETCHAR_PROTOTYPE int fgetc(FILE *f)

#endif /* __GNUC__ */

#endif
