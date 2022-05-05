/**
 ******************************************************************************
 * @file    i2c/main.c
 * @author  MDS
 * @date    11042022
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
#include "debug_log.h"

#define I2C_DEV_SDA_PIN		9
#define I2C_DEV_SCL_PIN		8
#define I2C_DEV_GPIO		GPIOB
#define I2C_DEV_GPIO_AF 	GPIO_AF4_I2C1
#define I2C_DEV_GPIO_CLK()	__GPIOB_CLK_ENABLE()

#define I2C_DEV				I2C1
#define I2C_DEV_CLOCKSPEED 	100000

#define MMA8452Q_ADDRESS		0x1D << 1		//MMA8452Q I2C address
#define MMA8452Q_WHO_AM_I_REG	0x0D		//MMA8452Q "Who am I" register address
#define WRITE_SIZE				1		//Number of bytes to write
#define READ_SIZE				1		//Number of bytes to read

void hardware_init(void);

/*
 * Main program
 */
int main(void) {

	uint8_t read_reg_val;

	HAL_Init(); 		// Initialise Board
	hardware_init(); 	// Initialise hardware peripherals

	// Cyclic Executive (CE) loop
	while (1) {

		//Clear Flags
		CLEAR_BIT(I2C_DEV->CR2, (I2C_CR2_SADD | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_AUTOEND | I2C_CR2_RD_WRN | I2C_CR2_START | I2C_CR2_STOP));
		
		//Set for Write Operation
		CLEAR_BIT(I2C_DEV->CR2, I2C_CR2_RD_WRN);

		//Set MMA8452Q Device address and Size of data to write (1). Size excludes device address.
		SET_BIT(I2C_DEV->CR2, (uint32_t)(((uint32_t)MMA8452Q_ADDRESS & I2C_CR2_SADD) | ((WRITE_SIZE << 16 ) & I2C_CR2_NBYTES)));
		SET_BIT(I2C_DEV->CR2, I2C_CR2_START);	// Generate the START condition

		//Write MMA8452Q Register address 
		WRITE_REG(I2C_DEV->TXDR, MMA8452Q_WHO_AM_I_REG & I2C_TXDR_TXDATA);

		//Wait for Write Operation to complete
		while( (I2C_DEV->ISR & I2C_ISR_TXE) == 0 );

		//Set for Read Operation
		SET_BIT(I2C_DEV->CR2, I2C_CR2_RD_WRN);

		//Set MMA8452Q Device address and Size of data to read (1). Size excludes device address.
		SET_BIT(I2C_DEV->CR2, (uint32_t)(((uint32_t)MMA8452Q_ADDRESS & I2C_CR2_SADD) | ((READ_SIZE << 16 ) & I2C_CR2_NBYTES)));
		SET_BIT(I2C_DEV->CR2, I2C_CR2_START);	// Generate the RE-START condition
				
		// Wait to read 
		while( (READ_REG(I2C_DEV->ISR) & I2C_ISR_RXNE) == 0 );

		// Read received value 
		read_reg_val = READ_REG(I2C_DEV->RXDR);

		// Generate NACK 
		SET_BIT(I2C_DEV->CR2, I2C_CR2_NACK);

		// Generate the STOP condition 
		SET_BIT(I2C_DEV->CR2, I2C_CR2_STOP);

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

	//---------------------------- I2C TIMINGR Configuration ------------------
	WRITE_REG(I2C_DEV->TIMINGR, 0);	
	// Configure clock prescaler
	// SysTimer = 80 MHz, PRESC = 7,  80MHz/(1 + 7) = 10 MHz
	CLEAR_BIT(I2C_DEV->TIMINGR, I2C_TIMINGR_PRESC); // Clear the prescaler 
	SET_BIT(I2C_DEV->TIMINGR, 7U << 28);           // Set clock prescaler to 7
	// tSCL = tSYNC1 + tSYNC2 + {[(SCLH+1) + (SCLL+1)] x (PRESC+1) x tI2CCLK}
	// SCL Frequency = 8 MHz / 100 = 80 kHz
	// Configure SCL high, low period
	SET_BIT(I2C_DEV->TIMINGR, 49U);       // SCLL: SCL low period (master mode) > 4.7 us
	SET_BIT(I2C_DEV->TIMINGR, 49U << 8);  // SCLH: SCL high period (master mode) > 4.0 us	 
	// Configure SDA setup, and hold time
	SET_BIT(I2C_DEV->TIMINGR, 1U << 20);  // SCLDEL: Data setup time > 1.0 us
	SET_BIT(I2C_DEV->TIMINGR, 2U << 16);  // SDADEL: Data hold time  > 1.25 us	

	//---------------------------- Own address 1 register (I2C_OAR1) -----------
	// I2C Own address1 and ack own address1 mode
	// Before STM32 sends its start sequence, it listens to the I2C lines waiting for its address
	// This is helpful if STM32 is used as slave
	CLEAR_BIT(I2C_DEV->OAR1, I2C_OAR1_OA1EN);
	WRITE_REG(I2C_DEV->OAR1, I2C_OAR1_OA1EN | MMA8452Q_ADDRESS); // 7-bit own address
	
	// Enable clock stretching
	CLEAR_BIT(I2C_DEV->CR1, I2C_CR1_NOSTRETCH);

	// Disable own address 2
	CLEAR_BIT(I2C_DEV->OAR1, I2C_OAR2_OA2EN);
	
	//---------------------------- I2Cx CR2 Configuration ----------------------
	CLEAR_BIT(I2C_DEV->CR2, I2C_CR2_ADD10);  // 0 = 7-bit mode, 1 = 10-bit mode
	
	// Enable the AUTOEND by default, and enable NACK (should be disable only during Slave process 
	// 1: Automatic end mode: a STOP condition is automatically sent when NBYTES data are transferred.
	// The AUTOEND bit has no effect when the RELOAD bit is set.
	SET_BIT(I2C_DEV->CR2, I2C_CR2_AUTOEND);
	
	// For slave mode: set NACK
	// The bit is set by software, cleared by hardware when the NACK is sent, or when a STOP
	// condition or an Address Matched is received, or when PE=0.
	//I2C_DEV->CR2 |= I2C_CR2_NACK;
	//---------------------------------------------------------------------------

  	// Enable the selected I2C peripheral
	SET_BIT(I2C_DEV->CR1, I2C_CR1_PE);

}
