/*
 * OLED_pixel.c
 *
 *  Created on: Jan 4, 2021
 *      Author: LyLeo
 */
#include "oled_pixel.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>  // For memcpy
#include "fonts.h"

char ssd1306_WriteChar(char ch, FontDef Font, SSD1306_COLOR color)
{
    ssd1306_WriteChar_int(ch, Font, color);

    /*uint32_t i, b, j;

    // Check remaining space on current line
    if (SSD1306_WIDTH <= (SSD1306.CurrentX + Font.FontWidth) ||
        SSD1306_HEIGHT <= (SSD1306.CurrentY + Font.FontHeight))
    {
        // Not enough space on current line
        return 0;
    }

    // Translate font to screenbuffer
    for (i = 0; i < Font.FontHeight; i++)
    {
        b = Font.data[(ch - 32) * Font.FontHeight + i];
        for (j = 0; j < Font.FontWidth; j++)
        {
            if ((b << j) & 0x8000)
            {
                ssd1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR) color);
            }
            else
            {
                ssd1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR)!color);
            }
        }
    }

    // The current space is now taken
    SSD1306.CurrentX += Font.FontWidth;//*/

    // Return written char for validation
    return ch;
}

//
//  Draw 1 char to the screen buffer
//  ch      => Character to write
//  Font    => Font to use
//  color   => Black or White
//
/*char ssd1306_WriteChar(char ch, FontDef Font, SSD1306_COLOR color)
{
    uint32_t i, b, j;
    uint8_t x, y;

    ssd1306_GetCursorX(x);
    ssd1306_GetCursorY(y);


    // Check remaining space on current line
    if (SSD1306_WIDTH <= (x + Font.FontWidth) ||
        SSD1306_HEIGHT <= (y + Font.FontHeight))
    {
        // Not enough space on current line
        return 0;
    }

    // Translate font to screenbuffer
    for (i = 0; i < Font.FontHeight; i++)
    {
        b = Font.data[(ch - 32) * Font.FontHeight + i];
        for (j = 0; j < Font.FontWidth; j++)
        {
            if ((b << j) & 0x8000)
            {
                ssd1306_DrawPixel(x + j, (y + i), (SSD1306_COLOR) color);
            }
            else
            {
                ssd1306_DrawPixel(x + j, (y + i), (SSD1306_COLOR)!color);
            }
        }
    }

    // The current space is now taken
    x += Font.FontWidth;
    ssd1306_SetCursorX(x);

    // Return written char for validation
    return ch;
}//*/

//
//  Write full string to screenbuffer
//
char ssd1306_WriteString(char* str, FontDef Font, SSD1306_COLOR color)
{
   ssd1306_WriteString_int(str, Font,  color);

    // Everything ok
    return *str;
}