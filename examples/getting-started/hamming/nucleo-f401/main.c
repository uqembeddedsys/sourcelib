/**
  ******************************************************************************
  * @file    getting_started/hamming/main.c
  * @author  MDS & KB
  * @date    04022022
  * @brief   Hamming encoder example.
  *			 Bytes received from the VCP are Hamming encoded and displayed.
  ******************************************************************************
  *
  */

#include "board.h"
#include "processor_hal.h"
#include "debug_log.h"

void hardware_init();
uint16_t hamming_byte_encoder(uint8_t input);

void main(void) {

	uint8_t unencodedbyte;
	uint16_t CodedWord;

	HAL_Init();	//Initialise NP2
	hardware_init();	//Initialise hardware modules

	unencodedbyte = 0x34;

	/* Main processing loop */
    while (1) {

		// Hamming encode received character 
		CodedWord = hamming_byte_encoder(unencodedbyte);

		debug_log("coded word %X\n\r", CodedWord);


    	BRD_LEDGreenToggle();	//Toggle LED on/off
    	HAL_Delay(1000);	//Delay function
  	}
}


/**
  * Implement Hamming Code + parity checking
  * Hamming code is based on the following generator and parity check matrices
  * G = [ 0 1 1 | 1 0 0 0 ;
  *       1 0 1 | 0 1 0 0 ;
  *       1 1 0 | 0 0 1 0 ;
  *       1 1 1 | 0 0 0 1 ;
  *
  * hence H =
  * [ 1 0 0 | 0 1 1 1 ;
  *   0 1 0 | 1 0 1 1 ;
  *   0 0 1 | 1 1 0 1 ];
  *
  * y = x * G, syn = H * y'
  *
  *
  * NOTE: !! is used to get 1 out of non zeros
  */
uint8_t hamming_hbyte_encoder(uint8_t in) {

	uint8_t d0, d1, d2, d3;
	uint8_t p0 = 0, h0, h1, h2;
	uint8_t z;
	uint8_t out;

	/* extract bits */
	d0 = !!(in & 0x1);
	d1 = !!(in & 0x2);
	d2 = !!(in & 0x4);
	d3 = !!(in & 0x8);

	/* calculate hamming parity bits */
	h0 = d1 ^ d2 ^ d3;
	h1 = d0 ^ d2 ^ d3;
	h2 = d0 ^ d1 ^ d3;

	/* generate out byte without parity bit P0 */
	out = (h0 << 1) | (h1 << 2) | (h2 << 3) |
		(d0 << 4) | (d1 << 5) | (d2 << 6) | (d3 << 7);

	/* calculate even parity bit */
	for (z = 1; z<8; z++)
		p0 = p0 ^ !!(out & (1 << z));

	out |= p0;

	return(out);

}

/*
 * Implement Hamming Code on a full byte of input
 * This means that 16-bits out output is needed
 */
uint16_t hamming_byte_encoder(uint8_t input) {

	uint16_t out;

	/* first encode D0..D3 (first 4 bits),
	 * then D4..D7 (second 4 bits).
	 */
	out = hamming_hbyte_encoder(input & 0xF) |
		(hamming_hbyte_encoder(input >> 4) << 8);

	return(out);

}

/*
 * Initialise Hardware
 */
void hardware_init() {

	BRD_LEDInit();		//Initialise Blue LED
	
	BRD_debuguart_init();  //Initialise UART for debug log output

	// Turn off LED
	BRD_LEDGreenOff();

}

