/**
 ******************************************************************************
 * @file    peripherals/oled/main.c
 * @author  MDS
 * @date    03042021
 * @brief   I2C example with the SSD1306 OLED Display (0.91 inch, 128x32 pixel). 
 * 			A boundary box, text and a moving cursor is shown on the OLED.
 *          The Green LED will toggle to indicate periodic writing of the OLED.
 *  		REFERENCE: SSD1306 Datasheet
 * 			Uses D15 (I2C_A_SCL) & D14 (I2C_A_SDA)
 *			Uses the following macros:
 * 				 MODIFY_REG( Register, clear mask (bits to clear), set mask (bits to set))
 *               WRITE_REG( Register, value to set)
 *				 READ_REG( (Register)
 *				 SET_BIT (Register, bit mask to set)
 *				 CLEAR_BIT (Register, bit mask to clear)
 ******************************************************************************
 *
 */
#include "board.h"
#include "processor_hal.h"
#include "oled_pixel.h"
#include "oled_string.h"
#include "fonts.h"


#define I2C_DEV_SDA_PIN		9
#define I2C_DEV_SCL_PIN		8
#define I2C_DEV_GPIO		GPIOB
#define I2C_DEV_GPIO_AF 	GPIO_AF4_I2C1
#define I2C_DEV_GPIO_CLK()	__GPIOB_CLK_ENABLE()

#define I2C_DEV				I2C1
#define I2C_DEV_CLOCKSPEED 	100000

void hardware_init(void);

/*
 * Main program
 */
int main(void) {

	uint8_t read_reg_val;
	uint32_t status;
	int i, dx,j;

	HAL_Init(); 		// Initialise Board
	hardware_init(); 	// Initialise hardware peripherals

	dx = 0;

	// Cyclic Executive (CE) loop
	while (1) {

		//Clear Screen
		
		ssd1306_Fill(Black);

		//Draw Horizontal lines of boundary box
		for (i=0; i < SSD1306_WIDTH; i++) {

			ssd1306_DrawPixel(i, 0, SSD1306_WHITE);					//top line
			ssd1306_DrawPixel(i, SSD1306_HEIGHT-1, SSD1306_WHITE);	//bottom line
		}

		//Draw Vertical lines of boundary box
		for (i=0; i < SSD1306_HEIGHT; i++) {

			ssd1306_DrawPixel(0, i, SSD1306_WHITE);					//left line
			ssd1306_DrawPixel(SSD1306_WIDTH-1, i, SSD1306_WHITE);	//right line

		}

		//Draw moving cursor
		for (i=0; i < 10; i++) {

			if ((i + dx) < SSD1306_WIDTH) {
				ssd1306_DrawPixel(i+dx, (SSD1306_HEIGHT/2)+10, SSD1306_WHITE);
			}
		}

		// Advance moving cursor
		dx = (dx + 10)%SSD1306_WIDTH;

		//Show text and udpate screen
		ssd1306_SetCursor(10,10);
		ssd1306_WriteString("CSSE3010 OLED Test", Font_6x8, SSD1306_WHITE);
		ssd1306_UpdateScreen();

		BRD_LEDGreenToggle();	//Toggle green LED on/off
		HAL_Delay(1000);		//Delay for 1s (1000ms)
	}
}

/*
 * Initialise Hardware
 */
void hardware_init(void) {

	uint32_t pclk1;
	uint32_t freqrange;

	BRD_LEDInit();	//Initialise LEDs

	// Turn off LEDs
	BRD_LEDGreenOff();

	// Enable GPIO clock
	I2C_DEV_GPIO_CLK();

	//******************************************************
	// IMPORTANT NOTE: SCL Must be Initialised BEFORE SDA
	//******************************************************

	//Clear and Set Alternate Function for pin (lower ARF register) 
	MODIFY_REG(I2C_DEV_GPIO->AFR[1], ((0x0F) << ((I2C_DEV_SCL_PIN-8) * 4)) | ((0x0F) << ((I2C_DEV_SDA_PIN-8)* 4)), ((I2C_DEV_GPIO_AF << ((I2C_DEV_SCL_PIN-8) * 4)) | (I2C_DEV_GPIO_AF << ((I2C_DEV_SDA_PIN-8)) * 4)));
	
	//Clear and Set Alternate Function Push Pull Mode
	MODIFY_REG(I2C_DEV_GPIO->MODER, ((0x03 << (I2C_DEV_SCL_PIN * 2)) | (0x03 << (I2C_DEV_SDA_PIN * 2))), ((GPIO_MODE_AF_OD << (I2C_DEV_SCL_PIN * 2)) | (GPIO_MODE_AF_OD << (I2C_DEV_SDA_PIN * 2))));
	
	//Set low speed.
	SET_BIT(I2C_DEV_GPIO->OSPEEDR, (GPIO_SPEED_LOW << I2C_DEV_SCL_PIN) | (GPIO_SPEED_LOW << I2C_DEV_SDA_PIN));

	//Set Bit for Push/Pull output
	SET_BIT(I2C_DEV_GPIO->OTYPER, ((0x01 << I2C_DEV_SCL_PIN) | (0x01 << I2C_DEV_SDA_PIN)));

	//Clear and set bits for no push/pull
	MODIFY_REG(I2C_DEV_GPIO->PUPDR, (0x03 << (I2C_DEV_SCL_PIN * 2)) | (0x03 << (I2C_DEV_SDA_PIN * 2)), (GPIO_PULLUP << (I2C_DEV_SCL_PIN * 2)) | (GPIO_PULLUP << (I2C_DEV_SDA_PIN * 2)));

	// Configure the I2C peripheral
	// Enable I2C peripheral clock
	__I2C1_CLK_ENABLE();

	// Disable the selected I2C peripheral
	CLEAR_BIT(I2C_DEV->CR1, I2C_CR1_PE);

  	pclk1 = HAL_RCC_GetPCLK1Freq();			// Get PCLK1 frequency
  	freqrange = I2C_FREQRANGE(pclk1);		// Calculate frequency range 

  	//I2Cx CR2 Configuration - Configure I2Cx: Frequency range
  	MODIFY_REG(I2C_DEV->CR2, I2C_CR2_FREQ, freqrange);

	// I2Cx TRISE Configuration - Configure I2Cx: Rise Time
  	MODIFY_REG(I2C_DEV->TRISE, I2C_TRISE_TRISE, I2C_RISE_TIME(freqrange, I2C_DEV_CLOCKSPEED));

   	// I2Cx CCR Configuration - Configure I2Cx: Speed
  	MODIFY_REG(I2C_DEV->CCR, (I2C_CCR_FS | I2C_CCR_DUTY | I2C_CCR_CCR), I2C_SPEED(pclk1, I2C_DEV_CLOCKSPEED, I2C_DUTYCYCLE_2));

   	// I2Cx CR1 Configuration - Configure I2Cx: Generalcall and NoStretch mode
  	MODIFY_REG(I2C_DEV->CR1, (I2C_CR1_ENGC | I2C_CR1_NOSTRETCH), (I2C_GENERALCALL_DISABLE| I2C_NOSTRETCH_DISABLE));

   	// I2Cx OAR1 Configuration - Configure I2Cx: Own Address1 and addressing mode
  	MODIFY_REG(I2C_DEV->OAR1, (I2C_OAR1_ADDMODE | I2C_OAR1_ADD8_9 | I2C_OAR1_ADD1_7 | I2C_OAR1_ADD0), I2C_ADDRESSINGMODE_7BIT);

   	// I2Cx OAR2 Configuration - Configure I2Cx: Dual mode and Own Address2
  	MODIFY_REG(I2C_DEV->OAR2, (I2C_OAR2_ENDUAL | I2C_OAR2_ADD2), I2C_DUALADDRESS_DISABLE);

  	// Enable the selected I2C peripheral
	SET_BIT(I2C_DEV->CR1, I2C_CR1_PE);

	ssd1306_Init();	//Initialise SSD1306 OLED.
}
