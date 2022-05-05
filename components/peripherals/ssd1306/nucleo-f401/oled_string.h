/*
 * OLED_pixel.c
 *
 *  Created on: Jan 4, 2021
 *      Author: LyLeo
 *
 *      Modified from https://github.com/4ilo/ssd1306-stm32HAL
 */

#ifndef INC_OLED_STRING_H_
#define INC_OLED_STRING_H_

#include "oled_pixel.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>  // For memcpy
#include "fonts.h"

//
//  Draw 1 char to the screen buffer
//  ch      => Character to write
//  Font    => Font to use
//  color   => Black or White
//
char ssd1306_WriteChar(char ch, FontDef Font, SSD1306_COLOR color);

//
//  Write full string to screenbuffer
//
char ssd1306_WriteString(char* str, FontDef Font, SSD1306_COLOR color);

#endif