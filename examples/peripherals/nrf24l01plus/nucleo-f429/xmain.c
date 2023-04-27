/**
 ******************************************************************************
 * @file    ex10_spi/main.c
 * @author  MDS, NPG
 * @date    02022015, 07122017
 * @brief   SPI Read nrf24L01+ status (0x07) register
 * 			NOTE: This example does not send or receive with the nrf24L01+.
 * 			CONNECTION: nrf24l01 MUST be connected to the Side Board RA connector.
 * 			REFERENCE: nrf24L01+ datasheet
 ******************************************************************************
 *
 */

/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "processor_hal.h"
#include "debug_log.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static SPI_HandleTypeDef SpiHandle;
/* Private function prototypes -----------------------------------------------*/
void Hardware_init(void);
uint8_t spi_sendbyte(uint8_t sendbyte);
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Main program
 * @param  None
 * @retval None
 */
int main(void) {

	uint8_t status;

	HAL_Init(); // Initialise Board
	Hardware_init(); // Initialise hardware peripherals

	/* Infinite loop */
	while (1) {

		HAL_GPIO_WritePin(BRD_SPI_RA_CS_GPIO_PORT, BRD_SPI_RA_CS_PIN, 0);   // Set chip select low

		spi_sendbyte(0x07);                                             // Send status register address

		status = spi_sendbyte(0xFF);                                    // Send dummy byte, to read status register
		debug_log("nrf24L01+ Status register value: %#.2x\r\n", status);// See page 59 of nrf24L01+ datasheet for status register definition
																		// NOTE: default value of the status register is 0x0E

		HAL_GPIO_WritePin(BRD_SPI_RA_CS_GPIO_PORT, BRD_SPI_RA_CS_PIN, 1);   // Set chip select high

		BRD_LEDGreenToggle();                                             //Toggle blue LED on/off
		HAL_Delay(1000);                                                //Delay for 1s (1000ms)

	}
}

/**
 * @brief  Initialise Hardware
 * @param  None
 * @retval None
 */
void Hardware_init(void) {

	GPIO_InitTypeDef GPIO_spi;

	BRD_debuguart_init();  //Initialise UART for debug log output

	BRD_LEDInit(); //Initialise LEDs

	/* Turn off LEDs */
	BRD_LEDGreenOff();

	// Enable SPI_B peripheral clock
	__BRD_SPI_RA_CLK();

	// Enable GPIO Pin clocks
	__BRD_SPI_RA_SCK_GPIO_CLK();
	__BRD_SPI_RA_MISO_GPIO_CLK();
	__BRD_SPI_RA_MOSI_GPIO_CLK();
	__BRD_SPI_RA_CS_GPIO_CLK();

	// Initialise GPIO pins as SPI pins

	// SPI SCK pin configuration
	GPIO_spi.Pin      = BRD_SPI_RA_SCK_PIN;
	GPIO_spi.Mode     = GPIO_MODE_AF_PP;
	GPIO_spi.Speed    = GPIO_SPEED_FREQ_LOW;
	GPIO_spi.Pull     = GPIO_PULLDOWN;
	GPIO_spi.Alternate= BRD_SPI_RA_SCK_AF;
	HAL_GPIO_Init(BRD_SPI_RA_SCK_GPIO_PORT, &GPIO_spi);

	// SPI MISO pin configuration
	GPIO_spi.Pin      = BRD_SPI_RA_MISO_PIN;
	GPIO_spi.Mode     = GPIO_MODE_AF_PP;
	GPIO_spi.Speed    = GPIO_SPEED_FREQ_LOW;
	GPIO_spi.Pull     = GPIO_PULLUP;		// MUST be set as pullup
	GPIO_spi.Alternate= BRD_SPI_RA_MISO_AF;
	HAL_GPIO_Init(BRD_SPI_RA_MISO_GPIO_PORT, &GPIO_spi);

	// SPI MOSI pin configuration
	GPIO_spi.Pin      = BRD_SPI_RA_MOSI_PIN;
	GPIO_spi.Mode     = GPIO_MODE_AF_PP;
	GPIO_spi.Speed    = GPIO_SPEED_FREQ_LOW;
	GPIO_spi.Pull     = GPIO_PULLDOWN;
	GPIO_spi.Alternate= BRD_SPI_RA_MOSI_AF;
	HAL_GPIO_Init(BRD_SPI_RA_MOSI_GPIO_PORT, &GPIO_spi);

	// SPI Configuration
	SpiHandle.Instance = (SPI_TypeDef *)(BRD_SPI_RA);

	__HAL_SPI_DISABLE(&SpiHandle);

	SpiHandle.Init.Mode             = SPI_MODE_MASTER;
	SpiHandle.Init.BaudRatePrescaler= SPI_BAUDRATEPRESCALER_16;
	SpiHandle.Init.Direction        = SPI_DIRECTION_2LINES;
	SpiHandle.Init.CLKPhase         = SPI_PHASE_1EDGE;
	SpiHandle.Init.CLKPolarity      = SPI_POLARITY_LOW;
	SpiHandle.Init.CRCCalculation   = SPI_CRCCALCULATION_DISABLED;
	SpiHandle.Init.CRCPolynomial    = 0;
	SpiHandle.Init.DataSize         = SPI_DATASIZE_8BIT;
	SpiHandle.Init.FirstBit         = SPI_FIRSTBIT_MSB;
	SpiHandle.Init.NSS              = SPI_NSS_SOFT;
	SpiHandle.Init.TIMode           = SPI_TIMODE_DISABLED;

	HAL_SPI_Init(&SpiHandle);

	__HAL_SPI_ENABLE(&SpiHandle);

	// Configure GPIO pin for SPI chip select
	GPIO_spi.Pin                    = BRD_SPI_RA_CS_PIN;
	GPIO_spi.Mode                   = GPIO_MODE_OUTPUT_PP;
	GPIO_spi.Pull                   = GPIO_PULLUP;
	GPIO_spi.Speed                  = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(BRD_SPI_RA_CS_GPIO_PORT, &GPIO_spi);

	// Set chip select high
	HAL_GPIO_WritePin(BRD_SPI_RA_CS_GPIO_PORT, BRD_SPI_RA_CS_PIN, 1);
}

/**
  * @brief  Send byte through SPI.
  * @param  sendbyte: byte to be transmitted via SPI.
  * @retval None
  */
uint8_t spi_sendbyte(uint8_t sendbyte) {

	uint8_t readbyte;

	HAL_SPI_TransmitReceive(&SpiHandle, &sendbyte, &readbyte, 1, 10);	//Send and receive 1 byte via SPI. Time out is 10ms.

	// Return the Byte read from the SPI bus
	return readbyte;
}

