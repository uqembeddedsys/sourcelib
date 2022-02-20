/**
  ******************************************************************************
  * @file    serial/spi/main.c
  * @author  MDS
  * @date    02032018
  * @brief   Transmits and receives via SPI1
  * 		 Uses D13/PA5 (SCK), D12/PA6 (MOSI), D11/PA7 (MISO), D10/PD14 (CS)
  *			 Uses the following macros:
  * 		 MODIFY_REG( Register, clear mask (bits to clear), set mask (bits to set))
  *          WRITE_REG( Register, value to set)
  *			 READ_REG( (Register)
  *			 SET_BIT ( Register, bit mask to set)
  *			 CLEAR_BIT ( Register, bit mask to clear)
  ******************************************************************************
  *
  */

#include "board.h"
#include "processor_hal.h"

//A5 as SPI SCK, A6 as SPI MISO and A7 as SPI MOSI pin
#define SPI_DEV_SCK_PIN		5
#define SPI_DEV_MISO_PIN	6
#define SPI_DEV_MOSI_PIN	7
#define SPI_DEV_GPIO		GPIOA
#define SPI_DEV_GPIO_AF 	GPIO_AF5_SPI1
#define SPI_DEV_GPIO_CLK()	__GPIOA_CLK_ENABLE()

#define SPI_DEV				SPI1
#define SPI_DEV_CLK()		__SPI1_CLK_ENABLE()

//SPI CS pin with PD14
#define SPI_DEV_CS_PIN			14
#define SPI_DEV_CS_GPIO			GPIOD
#define SPI_DEV_CS_GPIO_CLK()	__GPIOD_CLK_ENABLE()

#define SPI_DEV_CS_HIGH() SET_BIT(SPI_DEV_CS_GPIO->ODR, 0x01 << SPI_DEV_CS_PIN)
#define SPI_DEV_CS_LOW() CLEAR_BIT(SPI_DEV_CS_GPIO->ODR, 0x01 << SPI_DEV_CS_PIN)

void hardware_init(void);
uint8_t Spi_sendbyte(uint8_t sendbyte);

/*
 * Main program
 */
int main(void) {

	uint8_t received_value;

	HAL_Init(); // Initialise Board
	hardware_init(); // Initialise hardware peripherals

	// Cyclic Executive (CE) loop
	while (1) {

		received_value = Spi_sendbyte(0x0F);     // Send a byte via SPI
		
		BRD_LEDBlueToggle();                      //Toggle blue LED on/off
		HAL_Delay(1000);                          //Delay for 1s (1000ms)

	}
}

/*
 * Initialise Hardware
 */
void hardware_init(void) {

	BRD_LEDInit(); //Initialise LEDs

	// Turn off LEDs
	BRD_LEDRedOff();
	BRD_LEDGreenOff();
	BRD_LEDBlueOff();

	// Enable GPIO Pin clocks
	SPI_DEV_GPIO_CLK();

	// Initialise A5 as SPI SCK, A6 as SPI MISO and A7 as SPI MOSI pins as SPI pins
	//Clear and Set Alternate Function for pin (lower ARF register) 
	MODIFY_REG(SPI_DEV_GPIO->AFR[0], ((0x0F << (SPI_DEV_SCK_PIN * 4)) | (0x0F << (SPI_DEV_MISO_PIN * 4)) | (0x0F << (SPI_DEV_MOSI_PIN * 4))), ((SPI_DEV_GPIO_AF << (SPI_DEV_SCK_PIN * 4)) | (SPI_DEV_GPIO_AF << (SPI_DEV_MISO_PIN * 4)) | (SPI_DEV_GPIO_AF << (SPI_DEV_MOSI_PIN * 4))));
	
	//Clear and Set Alternate Function Push Pull Mode
	MODIFY_REG(SPI_DEV_GPIO->MODER, ((0x03 << (SPI_DEV_SCK_PIN * 2)) | (0x03 << (SPI_DEV_MISO_PIN * 2)) | (0x03 << (SPI_DEV_MOSI_PIN * 2))), ((GPIO_MODE_AF_PP << (SPI_DEV_SCK_PIN * 2)) | (GPIO_MODE_AF_PP << (SPI_DEV_MISO_PIN * 2)) | (GPIO_MODE_AF_PP << (SPI_DEV_MOSI_PIN * 2))));
	
	//Set low speed.
	SET_BIT(SPI_DEV_GPIO->OSPEEDR, (GPIO_SPEED_LOW << SPI_DEV_SCK_PIN) | (GPIO_SPEED_LOW << SPI_DEV_MISO_PIN) | (GPIO_SPEED_LOW << SPI_DEV_MOSI_PIN));

	//Clear and set bits for no push/pull
	MODIFY_REG(SPI_DEV_GPIO->PUPDR, ((0x03 << (SPI_DEV_SCK_PIN * 2)) | (0x03 << (SPI_DEV_MISO_PIN * 2)) | (0x03 << (SPI_DEV_MOSI_PIN * 2))), ((0x02 << (SPI_DEV_SCK_PIN * 2)) | (0x01 << (SPI_DEV_MISO_PIN * 2)) | (0x02 << (SPI_DEV_MOSI_PIN * 2))));

	//Initialise D14 as an output.
	SPI_DEV_CS_GPIO_CLK();

	//Clear and Set for Push Pull Mode
	MODIFY_REG(SPI_DEV_CS_GPIO->MODER, 0x03 << (SPI_DEV_CS_PIN * 2), 0x01 << (SPI_DEV_CS_PIN * 2));
	
	//Set fast speed.
	MODIFY_REG(SPI_DEV_CS_GPIO->OSPEEDR, 0x03 << (SPI_DEV_CS_PIN * 2), GPIO_SPEED_FAST << (SPI_DEV_CS_PIN * 2));

	//Clear Bit for Push/Pull output
	CLEAR_BIT(SPI_DEV_CS_GPIO->OTYPER, 0x01 << SPI_DEV_CS_PIN);

	//Clear and set bits for no push/pull
	//MODIFY_REG(SPI_DEV_CS_GPIO->PUPDR, (0x03 << (SPI_DEV_CS_PIN * 2)), (0x01 << (SPI_DEV_CS_PIN * 2)));
	CLEAR_BIT(SPI_DEV_CS_GPIO->PUPDR, 0x01 << SPI_DEV_CS_PIN);

	SPI_DEV_CS_HIGH();	//Set CS high

	// SPI Configuration	
	SPI_DEV_CLK();

	CLEAR_BIT(SPI_DEV->CR1, SPI_CR1_SPE); 	// Disable SPI
	SET_BIT(SPI_DEV->CR1, SPI_CR1_MSTR);    // Master selection: 0 = Slave, 1 = Master

	// NSSGPIO
	// 1 = Software slave management enabled
	// The value of SSI is forced onto the NSS pin and the IO value of the NSS pin is ignored.
	SET_BIT(SPI_DEV->CR1, SPI_CR1_SSM);  
	SET_BIT(SPI_DEV->CR1, SPI_CR1_SSI);       		  // Manage NSS (slave selection) by using Software

	// Configure duplex or receive-only	
	CLEAR_BIT(SPI_DEV->CR1, SPI_CR1_RXONLY);     // 0 = Full duplex (transmit and receive);  1 = Receive-only
	
	// Data Frame Format - 8 bit selected
	CLEAR_BIT(SPI_DEV->CR1, SPI_CR1_DFF);
	
	// Frame Format
	// 0 = MSB transmitted/received first;  1 = LSB transmitted/received first
	// LSByte to MSByte, MSBit in each byte first
	CLEAR_BIT(SPI_DEV->CR1, SPI_CR1_LSBFIRST);  // MSB first       

	// Clock phase
	// 0 = The first clock transition is the first data capture edge
	// 1 = The second clock transition is the first data capture edge
	CLEAR_BIT(SPI_DEV->CR1, SPI_CR1_CPHA);      // SPI_PHASE_1EDGE
	
	// Clock polarity: 0 = CK to 0 when idle, 1 = CK to 1 when idle
	CLEAR_BIT(SPI_DEV->CR1, SPI_CR1_CPOL);     // SPI_POLARITY_LOW
	
	// Baud rate control: 
	// 000: f_PCLK/2   001: f_PCLK/4   010: f_PCLK/8   011: f_PCLK/16  
	// 100: f_PCLK/32  101: f_PCLK/64  110: f_PCLK/128 111: f_PCLK/256
	// SPI baudrate is set to 10 MHz (PCLK1/SPI_BaudRatePrescaler = 80/8 = 10 MHz) 
    // to verify these constraints: lsm303c SPI interface max baudrate is 10MHz for write/read
    // PCLK1 max frequency is set to 80 MHz 
	SET_BIT(SPI_DEV->CR1, SPI_BAUDRATEPRESCALER_32 << 3);        

	// Bidirectional data mode enable
	// This bit enables half-duplex communication using common single bidirectional data line. 
	// Keep RXONLY bit clear when bidirectional mode is active.
	// 0: 2-line unidirectional data mode selected
	// 1: 1-line bidirectional data mode selected
	CLEAR_BIT(SPI_DEV->CR1, SPI_CR1_BIDIMODE); 	// 0: 2-line unidirectional data mode selected 
	
	// CRC Polynomial
	WRITE_REG(SPI_DEV->CRCPR, 0);
	
	// Hardware CRC calculation disabled
	CLEAR_BIT(SPI_DEV->CR1, SPI_CR1_CRCEN);
	
	// Frame format: 0 = SPI Motorola mode, 1 = SPI TI mode
	CLEAR_BIT(SPI_DEV->CR2, SPI_CR2_FRF);

	// SPI enable
	SET_BIT(SPI_DEV->CR1, SPI_CR1_SPE);   
}

 /*
  * Send byte and receive through SPI.
  */
uint8_t Spi_sendbyte(uint8_t sendbyte) {

	uint8_t readbyte;

	SPI_DEV_CS_LOW();	//Set CS signal low

	while(!(READ_REG(SPI_DEV->SR) & SPI_SR_TXE )); // Wait for TXE (Transmit buffer empty)
	WRITE_REG(SPI_DEV->DR, sendbyte);

	while(!(READ_REG(SPI_DEV->SR) & SPI_SR_RXNE )); // Wait for TXE (Transmit buffer empty)
	readbyte = READ_REG(SPI_DEV->DR);

	SPI_DEV_CS_HIGH();	//Set CS signal high

	// Return the Byte read from the SPI bus
	return readbyte;
}

