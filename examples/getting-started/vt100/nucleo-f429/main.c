/**
 ******************************************************************************
 * @file    vt100/main.c
 * @author  MDS
 * @date    01052020
 * @brief   Ref: http://ascii-table.com/ansi-escape-sequences-vt-100.php
 * 				https://en.wikipedia.org/wiki/Escape_sequences_in_C
 ******************************************************************************
 *
 */
#include "board.h"
#include "debug_log.h"
#include "processor_hal.h"

#define SCREEN_CLEAR()  debug_log("\e[2J")
#define SCREEN_HOME()  debug_log("\e[H")

#define FG_BLACK 	30
#define FG_RED		31
#define FG_GREEN	32
#define FG_YELLOW	33
#define FG_BLUE		34
#define FG_MAGENTA	35
#define FG_CYAN		36
#define FG_WHITE	37
#define BG_BLACK	40
#define BG_RED		41
#define BG_GREEN	42
#define BG_YELLOW	43
#define BG_BLUE		44
#define BG_MAGENTA	45
#define BG_CYAN		46
#define BG_WHITE	47

#define CELL_BLACK "\e[7;30;40m"
#define CELL_RED	"\e[7;31;41m"
#define CELL_GREEN	"\e[7;32;42m"
#define CELL_YELLOW		"\e[7;33;43m"
#define CELL_BLUE		"\e[7;34;44m"
#define CELL_MAGENTA	"\e[7;35;45m"
#define CELL_CYAN		"\e[7;36;46m"
#define CELL_WHITE 		"\e[7;37;47m"

char *cell_colour_palatte[] = {CELL_BLACK, CELL_RED, CELL_GREEN, CELL_YELLOW, CELL_BLUE, CELL_MAGENTA, CELL_CYAN, CELL_WHITE};
#define SET_CELL_COLOUR(fg, bg) debug_log("\e[7;%d;%dm",fg,bg)


void hardware_init(void);

int main(void) {

	int i, j;

	HAL_Init();
	hardware_init();

	while (1) {

		SCREEN_CLEAR();
		SCREEN_HOME();

		for (j=0; j < 8; j++){

			for (i=0; i < 8; i++){
				debug_log("%s  ", cell_colour_palatte[i]);
			}

			for (i=0; i < 8; i++){
				debug_log("%s  ", cell_colour_palatte[i]);
			}

			debug_log("\n\r");
		}

		debug_log("\n\r");

		SET_CELL_COLOUR(FG_CYAN, BG_BLUE);
		debug_log("I \u2764 CSSE3010");

		BRD_LEDGreenToggle();
		HAL_Delay(1000);

	}
}

void hardware_init(void) {

BRD_LEDInit();		//Initialise LEDS

BRD_debuguart_init();  //Initialise UART for debug log output

// Turn off Green LED
BRD_LEDGreenOff();

}
