/**
 ******************************************************************************
 * @file    i2c/main.c
 * @author  MDS
 * @date    03042021
 * @brief   I2C example with the MMA8462Q. Reads and checks the WHO_AM_I 
 *          register (Address: 0x0D) for the set value (0x2A). If the value is
 *          correct, the green LED is toggled.
 *  		REFERENCE: MMA8462 Datasheet (p15)
 * 			Uses D15 (SCL) & D14 (SDA)
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

#define I2C_DEV_SDA_PIN		9
#define I2C_DEV_SCL_PIN		8
#define I2C_DEV_GPIO		GPIOB
#define I2C_DEV_GPIO_AF 	GPIO_AF4_I2C1
#define I2C_DEV_GPIO_CLK()	__GPIOB_CLK_ENABLE()

#define I2C_DEV				I2C1
#define I2C_DEV_CLOCKSPEED 	100000

#define MMA8452Q_ADDRESS		0x1D << 1		//MMA8452Q I2C address
#define MMA8452Q_WHO_AM_I_REG	0x0D		//MMA8452Q "Who am I" register address

void hardware_init(void);

/*
 * Main program
 */
int main(void) {

	uint8_t read_reg_val;
	uint32_t status;

	HAL_Init(); 		// Initialise Board
	hardware_init(); 	// Initialise hardware peripherals

	// Cyclic Executive (CE) loop
	while (1) {

		CLEAR_BIT(I2C_DEV->SR1, I2C_SR1_AF);	//Clear Flags
		SET_BIT(I2C_DEV->CR1, I2C_CR1_START);	// Generate the START condition

		// Wait the START condition has been correctly sent 
		while((READ_REG(I2C_DEV->SR1) & I2C_SR1_SB) == 0);

		// Send Peripheral Device Write address 
		WRITE_REG(I2C_DEV->DR, I2C_7BIT_ADD_WRITE(MMA8452Q_ADDRESS));

		// Wait for address to be acknowledged 
		while((READ_REG(I2C_DEV->SR1) & I2C_SR1_ADDR) == 0);

		// Clear ADDR Flag by reading SR1 and SR2.
		status = READ_REG(I2C_DEV->SR2);

		// Send Read Register Address - WHO_AM_I Register Address 
		WRITE_REG(I2C_DEV->DR, MMA8452Q_WHO_AM_I_REG);

		// Wait until register Address byte is transmitted 
		while(((READ_REG(I2C_DEV->SR1) & I2C_SR1_TXE) == 0) && ((READ_REG(I2C_DEV->SR1) & I2C_SR1_BTF) == 0));

		// Generate the START condition, again 
		SET_BIT(I2C_DEV->CR1, I2C_CR1_START);

		// Wait the START condition has been correctly sent 
		while((READ_REG(I2C_DEV->SR1) & I2C_SR1_SB) == 0);

		// Send Read Address 
		WRITE_REG(I2C_DEV->DR, I2C_7BIT_ADD_READ(MMA8452Q_ADDRESS));

		// Wait for address to be acknowledged 
		while((READ_REG(I2C_DEV->SR1) & I2C_SR1_ADDR) == 0);

		//Clear ADDR Flag by reading SR1 and SR2.
		status = READ_REG(I2C_DEV->SR2);
				
		// Wait to read 
		while((READ_REG(I2C_DEV->SR1) & I2C_SR1_RXNE) == 0);

		// Read received value 
		read_reg_val = READ_REG(I2C_DEV->DR);

		// Generate NACK 
		CLEAR_BIT(I2C_DEV->CR1, I2C_CR1_ACK);

		// Generate the STOP condition 
		SET_BIT(I2C_DEV->CR1, I2C_CR1_STOP);

		//Check WHO_AM_I Register value is 0x2A
		if (read_reg_val == 0x2A) {
			BRD_LEDGreenToggle();
		} 

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

	//Clear and Set Alternate Function for pin (upper ARF register) 
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
}
