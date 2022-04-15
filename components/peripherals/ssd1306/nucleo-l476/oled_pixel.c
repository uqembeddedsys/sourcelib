/*
 * OLED_pixel.c
 *
 *  Created on: Jan 4, 2021
 *      Author: LyLeo
 *
 *      Modified from https://github.com/4ilo/ssd1306-stm32HAL
 */

#include "oled_pixel.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>  // For memcpy

#ifdef USE_FREERTOS_SYSTICK     //If FreeRTOS is being used, change delay function
#include "FreeRTOS.h"
#include "task.h"
#define OLED_HAL_DELAY(x)   vTaskDelay(x)
#else
#define OLED_HAL_DELAY(x)   HAL_Delay(x)
#endif

void ssd1306_Reset(void) {
    /* for I2C - do nothing */
}

// Send a byte to the command register
void ssd1306_WriteCommand(uint8_t byte) {
    
    uint32_t status;
    //HAL_I2C_Mem_Write(&I2CHandle, (ADDR << 1), 0x00, sizeof(uint8_t), &byte, sizeof(uint8_t), HAL_MAX_DELAY);

    //CLEAR_BIT(OLED_I2C_DEV->SR1, I2C_SR1_AF);	//Clear Flags
	SET_BIT(OLED_I2C_DEV->CR2, I2C_CR2_START);	// Generate the START condition

	// Wait the START condition has been correctly sent 
	//while((READ_REG(OLED_I2C_DEV->SR1) & I2C_SR1_SB) == 0);
    while((OLED_I2C_DEV->ISR & I2C_ISR_TXIS) == 0 && ((OLED_I2C_DEV->ISR & I2C_ISR_NACKF) == 0) ); 

	// Send Peripheral Device Write address 
	WRITE_REG(OLED_I2C_DEV->TXDR, ((OLED_I2C_DEV_ADDR << 1) | I2C_CR2_RD_WRN));

	// Wait for address to be acknowledged 
	while((READ_REG(OLED_I2C_DEV->ISR) & I2C_ISR_TXIS) == 0);

	//Clear ADDR Flag by reading SR1 and SR2.
	//status = READ_REG(OLED_I2C_DEV->ISR);

	// Send Control byte, set Co bit to 1 (control words) and D/C to 0 (no data), followed by '000000'
	WRITE_REG(OLED_I2C_DEV->TXDR, 0x80);

    // Wait for control byte to transmit
    while((READ_REG(OLED_I2C_DEV->ISR) & I2C_ISR_TXIS) == 0); // && ((READ_REG(OLED_I2C_DEV->SR1) & I2C_SR1_BTF) == 0));

    // Send command 
	WRITE_REG(OLED_I2C_DEV->TXDR, byte);

    // Wait for command byte to transmit
    while((READ_REG(OLED_I2C_DEV->ISR) & I2C_ISR_TXIS) == 0); // && ((READ_REG(OLED_I2C_DEV->SR1) & I2C_SR1_BTF) == 0));

	// Generate the STOP condition 
	SET_BIT(OLED_I2C_DEV->CR2, I2C_CR2_STOP);

    //OLED_HAL_DELAY(10);
    //*/
}

// Send data
void ssd1306_WriteData(uint8_t* buffer, size_t buff_size) {
    //HAL_I2C_Mem_Write(&I2CHandle, (ADDR << 1), 0x40, sizeof(uint8_t), buffer, buff_size, HAL_MAX_DELAY);
    
    uint32_t status;
    int i;

    //CLEAR_BIT(OLED_I2C_DEV->SR1, I2C_SR1_AF);	//Clear Flags
	SET_BIT(OLED_I2C_DEV->CR2, I2C_CR2_START);	// Generate the START condition

	// Wait the START condition has been correctly sent 
	//while((READ_REG(OLED_I2C_DEV->SR1) & I2C_SR1_SB) == 0);
    while((OLED_I2C_DEV->ISR & I2C_ISR_TXIS) == 0 && ((OLED_I2C_DEV->ISR & I2C_ISR_NACKF) == 0) ); 

	// Send Peripheral Device Write address 
	WRITE_REG(OLED_I2C_DEV->TXDR, ((OLED_I2C_DEV_ADDR << 1) | I2C_CR2_RD_WRN));

	// Wait for address to be acknowledged 
	while((READ_REG(OLED_I2C_DEV->ISR) & I2C_ISR_TXIS) == 0);

	//Clear ADDR Flag by reading SR1 and SR2.
	//status = READ_REG(OLED_I2C_DEV->ISR);

    // Send Control byte, set Co bit to 0 and D/C to 1 (data only), followed by '000000'
    WRITE_REG(OLED_I2C_DEV->TXDR, 0x40);

    // Wait for first control byte to send.
    while((READ_REG(OLED_I2C_DEV->ISR) & I2C_ISR_TXIS) == 0); // && ((READ_REG(OLED_I2C_DEV->SR1) & I2C_SR1_BTF) == 0));

    // Send data byte 
    for (i=0; i < buff_size; i++ ) {
        
        WRITE_REG(OLED_I2C_DEV->TXDR, buffer[i]);

        // Wait for data byte to be sent
        while((READ_REG(OLED_I2C_DEV->ISR) & I2C_ISR_TXIS) == 0); // && ((READ_REG(OLED_I2C_DEV->SR1) & I2C_SR1_BTF) == 0));
    }

    // Generate stop condition
    SET_BIT(OLED_I2C_DEV->CR2, I2C_CR2_STOP);

    //OLED_HAL_DELAY(10);
    //*/
}


void ssd1306_SetDisplayOn(const uint8_t on) {
    uint8_t value;
    if (on) {
        value = 0xAF;   // Display on
        SSD1306.DisplayOn = 1;
    } else {
        value = 0xAE;   // Display off
        SSD1306.DisplayOn = 0;
    }
    ssd1306_WriteCommand(value);
}

/* Fills the Screenbuffer with values from a given buffer of a fixed length */
SSD1306_Error_t ssd1306_FillBuffer(uint8_t* buf, uint32_t len) {
    SSD1306_Error_t ret = SSD1306_ERR;
    if (len <= SSD1306_BUFFER_SIZE) {
        memcpy(SSD1306_Buffer,buf,len);
        ret = SSD1306_OK;
    }
    return ret;
}

// Fill the whole screen with the given color
void ssd1306_Fill(SSD1306_COLOR color) {
    /* Set memory */
    int i;

    for(i = 0; i < sizeof(SSD1306_Buffer); i++) {
        SSD1306_Buffer[i] = (color == Black) ? 0x00 : 0xFF;
    }
}

// Write the screenbuffer with changed to the screen
void ssd1306_UpdateScreen(void) {
    // Write data to each page of RAM. Number of pages
    // depends on the screen height:
    //
    //  * 32px   ==  4 pages
    //  * 64px   ==  8 pages
    //  * 128px  ==  16 pages
    for(uint8_t i = 0; i < SSD1306_HEIGHT/8; i++) {
        ssd1306_WriteCommand(0xB0 + i); // Set the current RAM page address.
        ssd1306_WriteCommand(0x00);
        ssd1306_WriteCommand(0x10);
        ssd1306_WriteData(&SSD1306_Buffer[SSD1306_WIDTH*i],SSD1306_WIDTH);
    }
}

//    Draw one pixel in the screenbuffer
//    X => X Coordinate
//    Y => Y Coordinate
//    color => Pixel color
void ssd1306_DrawPixel(uint8_t x, uint8_t y, SSD1306_COLOR color) {
    if(x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) {
        // Don't write outside the buffer
        return;
    }

    // Check if pixel should be inverted
    if(SSD1306.Inverted) {
        color = (SSD1306_COLOR)!color;
    }

    // Draw in the right color
    if(color == White) {
        SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
    } else {
        SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
    }
}


// Position the cursor
void ssd1306_SetCursor(uint8_t x, uint8_t y) {
    SSD1306.CurrentX = x;
    SSD1306.CurrentY = y;
}

// Position the cursor
void ssd1306_GetCursorX(uint8_t x) {
    x = SSD1306.CurrentX;
}

// Position the cursor
void ssd1306_GetCursorY(uint8_t y) {
    y = SSD1306.CurrentY;
}

void ssd1306_SetCursorX(uint8_t x) {
    SSD1306.CurrentX = x;
}

void ssd1306_SetCursorY(uint8_t y) {
    SSD1306.CurrentY = y;
}

void ssd1306_SetContrast(const uint8_t value) {
    const uint8_t kSetContrastControlRegister = 0x81;
    ssd1306_WriteCommand(kSetContrastControlRegister);
    ssd1306_WriteCommand(value);
}

uint8_t ssd1306_GetDisplayOn() {
    return SSD1306.DisplayOn;
}

// Initialize the oled screen
void ssd1306_Init(void) {
    // Reset OLED
    ssd1306_Reset();

    // Wait for the screen to boot
    OLED_HAL_DELAY(100);

    // Init OLED
    ssd1306_SetDisplayOn(0); //display off

    ssd1306_WriteCommand(0x20); //Set Memory Addressing Mode
    ssd1306_WriteCommand(0x00); // 00b,Horizontal Addressing Mode; 01b,Vertical Addressing Mode;
                                // 10b,Page Addressing Mode (RESET); 11b,Invalid

    ssd1306_WriteCommand(0xB0); //Set Page Start Address for Page Addressing Mode,0-7

#ifdef SSD1306_MIRROR_VERT
    ssd1306_WriteCommand(0xC0); // Mirror vertically
#else
    ssd1306_WriteCommand(0xC8); //Set COM Output Scan Direction
#endif

    ssd1306_WriteCommand(0x00); //---set low column address
    ssd1306_WriteCommand(0x10); //---set high column address

    ssd1306_WriteCommand(0x40); //--set start line address - CHECK

    ssd1306_SetContrast(0xFF);

#ifdef SSD1306_MIRROR_HORIZ
    ssd1306_WriteCommand(0xA0); // Mirror horizontally
#else
    ssd1306_WriteCommand(0xA1); //--set segment re-map 0 to 127 - CHECK
#endif

#ifdef SSD1306_INVERSE_COLOR
    ssd1306_WriteCommand(0xA7); //--set inverse color
#else
    ssd1306_WriteCommand(0xA6); //--set normal color
#endif

// Set multiplex ratio.
    ssd1306_WriteCommand(0xA8); //--set multiplex ratio(1 to 64) - CHECK

    ssd1306_WriteCommand(0x1F); //

    ssd1306_WriteCommand(0xA4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content

    ssd1306_WriteCommand(0xD3); //-set display offset - CHECK
    ssd1306_WriteCommand(0x00); //-not offset

    ssd1306_WriteCommand(0xD5); //--set display clock divide ratio/oscillator frequency
    ssd1306_WriteCommand(0xF0); //--set divide ratio

    ssd1306_WriteCommand(0xD9); //--set pre-charge period
    ssd1306_WriteCommand(0x22); //

    ssd1306_WriteCommand(0xDA); //--set com pins hardware configuration - CHECK
    ssd1306_WriteCommand(0x02);

    ssd1306_WriteCommand(0xDB); //--set vcomh
    ssd1306_WriteCommand(0x20); //0x20,0.77xVcc

    ssd1306_WriteCommand(0x8D); //--set DC-DC enable
    ssd1306_WriteCommand(0x14); //
    ssd1306_SetDisplayOn(1); //--turn on SSD1306 panel

    // Clear screen
    ssd1306_Fill(Black);

    // Flush buffer to screen
    ssd1306_UpdateScreen();

    // Set default values for screen object
    SSD1306.CurrentX = 0;
    SSD1306.CurrentY = 0;

    SSD1306.Initialized = 1;
}



char ssd1306_WriteChar_int(char ch, FontDef Font, SSD1306_COLOR color)
{
    uint32_t i, b, j;

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
    SSD1306.CurrentX += Font.FontWidth;

    // Return written char for validation
    return ch;
}

//
//  Write full string to screenbuffer
//
char ssd1306_WriteString_int(char* str, FontDef Font, SSD1306_COLOR color)
{
    // Write until null-byte
    while (*str)
    {
        if (ssd1306_WriteChar(*str, Font, color) != *str)
        {
            // Char could not be written
            return *str;
        }

        // Next char
        str++;
    }

    // Everything ok
    return *str;
}

HAL_StatusTypeDef ssd1306_getResp() {
	//HAL_StatusTypeDef sts = HAL_I2C_Mem_Write(&I2CHandle, (0x00 << 1), 0x00, sizeof(uint8_t), 0x00, sizeof(uint8_t), 1000);
    HAL_StatusTypeDef sts;
    uint32_t status;
/*
    CLEAR_BIT(OLED_I2C_DEV->SR1, I2C_SR1_AF);	//Clear Flags
	SET_BIT(OLED_I2C_DEV->CR1, I2C_CR1_START);	// Generate the START condition

	// Wait the START condition has been correctly sent 
	while((READ_REG(OLED_I2C_DEV->SR1) & I2C_SR1_SB) == 0);

	// Send Peripheral Device Write address 
	WRITE_REG(OLED_I2C_DEV->DR, __HAL_I2C_7BIT_ADD_WRITE((0x00 << 1)));

		// Wait for address to be acknowledged 
		while((READ_REG(OLED_I2C_DEV->SR1) & I2C_SR1_ADDR) == 0);

		//Clear ADDR Flag by reading SR1 and SR2.
		status = READ_REG(OLED_I2C_DEV->SR2);

		// Send Read Register Address - WHO_AM_I Register Address 
		WRITE_REG(OLED_I2C_DEV->DR, (0x00));

		// Wait until register Address byte is transmitted 
		while(((READ_REG(OLED_I2C_DEV->SR1) & I2C_SR1_TXE) == 0) && ((READ_REG(OLED_I2C_DEV->SR1) & I2C_SR1_BTF) == 0));

		// Generate the START condition, again 
		SET_BIT(OLED_I2C_DEV->CR1, I2C_CR1_START);

		// Wait the START condition has been correctly sent 
		while((READ_REG(OLED_I2C_DEV->SR1) & I2C_SR1_SB) == 0);

		// Send Read Address 
		WRITE_REG(OLED_I2C_DEV->DR, __HAL_I2C_7BIT_ADD_READ(0));

		// Wait address is acknowledged 
		while((READ_REG(OLED_I2C_DEV->SR1) & I2C_SR1_ADDR) == 0);
		
		// Clear ADDR Flag by reading SR1 and SR2.
		status = READ_REG(OLED_I2C_DEV->SR2);
				
		// Wait to read 
		while((READ_REG(OLED_I2C_DEV->SR1) & I2C_SR1_RXNE) == 0);

		// Read received value 
		sts = READ_REG(OLED_I2C_DEV->DR);

		// Generate NACK 
		CLEAR_BIT(OLED_I2C_DEV->CR1, I2C_CR1_ACK);

		// Generate the STOP condition 
		SET_BIT(OLED_I2C_DEV->CR1, I2C_CR1_STOP);

//*/
	return sts;

}
