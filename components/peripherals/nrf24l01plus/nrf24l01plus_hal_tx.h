/* -----------------------------------------------------------------------------
# Company name: CSSE3010 - Embedded Systems Design & Interfacing
# Project name: nRF24l01+ driver
# Module name : nrf24l01plus
# Functionality: Provides low level radio calls used to initialise and interface
#        with the nrf24l01+.
#
# Author name: Chris Rieger modified by MDS
# Creation date: 010712, 
# Revision date (name): 210316, 240925 MDS
# Changes implemented (date): 210316 Renamed functions and modified for new spec
#(Comments):
------------------------------------------------------------------------------*/

#ifndef _NRF24l01PLUS_HAL_H
#define _NRF24l01PLUS_HAL_H

#ifdef HAVE_PLATFORMCONF_H
#include "platform-conf.h"
#endif

#include "nrf24l01plus_reg.h"

/* Define Pins, if not specified by a platform-conf.h file */

/* Mode Pin */
#ifndef NRF_MODE_PIN
#define NRF_MODE_PIN		GPIO_PIN_15
#endif

#ifndef NRF_MODE_GPIO_PORT
#define NRF_MODE_GPIO_PORT 	GPIOD
#endif

#ifndef __NRF_MODE_GPIO_CLK
#define __NRF_MODE_GPIO_CLK 	__GPIOD_CLK_ENABLE()
#endif

/* SPI CS Pin */
#ifndef NRF_SPI_CS_PIN
#define NRF_SPI_CS_PIN	BRD_SPI_RA_CS_PIN
#endif

#ifndef NRF_SPI_CS_GPIO_PORT
#define NRF_SPI_CS_GPIO_PORT	BRD_SPI_RA_CS_GPIO_PORT
#endif

#ifndef __NRF_SPI_CS_GPIO_CLK
#define __NRF_SPI_CS_GPIO_CLK	__BRD_SPI_RA_CS_GPIO_CLK()
#endif


/* SPI SCK Pin */
#ifndef NRF_SPI_SCK_PIN
#define NRF_SPI_SCK_PIN			BRD_SPI_RA_SCK_PIN
#endif

#ifndef NRF_SPI_SCK_GPIO_PORT
#define NRF_SPI_SCK_GPIO_PORT	BRD_SPI_RA_SCK_GPIO_PORT
#endif

#ifndef __NRF_SPI_SCK_GPIO_CLK
#define __NRF_SPI_SCK_GPIO_CLK()	__BRD_SPI_RA_SCK_GPIO_CLK()
#endif

#ifndef NRF_SPI_SCK_AF
#define NRF_SPI_SCK_AF		BRD_SPI_RA_SCK_AF
#endif


/* SPI MISO Pin */
#ifndef NRF_SPI_MISO_PIN
#define NRF_SPI_MISO_PIN	BRD_SPI_RA_MISO_PIN
#endif

#ifndef NRF_SPI_MISO_GPIO_PORT
#define NRF_SPI_MISO_GPIO_PORT	BRD_SPI_RA_MISO_GPIO_PORT
#endif

#ifndef __NRF_SPI_MISO_GPIO_CLK
#define __NRF_SPI_MISO_GPIO_CLK()	__BRD_SPI_RA_MISO_GPIO_CLK()
#endif

#ifndef NRF_SPI_MISO_AF
#define NRF_SPI_MISO_AF		BRD_SPI_RA_MISO_AF
#endif


/* SPI MOSI Pin */
#ifndef NRF_SPI_MOSI_PIN
#define NRF_SPI_MOSI_PIN	BRD_SPI_RA_MOSI_PIN
#endif

#ifndef NRF_SPI_MOSI_GPIO_PORT
#define NRF_SPI_MOSI_GPIO_PORT	BRD_SPI_RA_MOSI_GPIO_PORT
#endif

#ifndef __NRF_SPI_MOSI_GPIO_CLK
#define __NRF_SPI_MOSI_GPIO_CLK()	__BRD_SPI_RA_MOSI_GPIO_CLK()
#endif

#ifndef NRF_SPI_MOSI_AF
#define NRF_SPI_MOSI_AF		BRD_SPI_RA_MOSI_AF
#endif

#ifndef NRF_SPI
#define NRF_SPI	BRD_SPI_RA
#endif

#ifndef __NRF_SPI_CLK
#define __NRF_SPI_CLK()	__BRD_SPI_RA_CLK()
#endif

#ifndef NRF_IRQ_PIN
#define NRF_IRQ_PIN	BRD_SPI_MOSI_PIN
#endif

#ifndef NRF_IRQ_GPIO_PORT
#define NRF_IRQ_GPIO_PORT	BRD_SPI_MOSI_GPIO_PORT
#endif


/* Set/Reset CE pin */
#define NRF_CE_HIGH()	HAL_GPIO_WritePin(NRF_MODE_GPIO_PORT, NRF_MODE_PIN, 1)
#define NRF_CE_LOW()	HAL_GPIO_WritePin(NRF_MODE_GPIO_PORT, NRF_MODE_PIN, 0)

/*Set/Reset CS pin */
#define NRF_CS_HIGH() 	HAL_GPIO_WritePin(NRF_SPI_CS_GPIO_PORT, NRF_SPI_CS_PIN, 1)
#define NRF_CS_LOW() 	HAL_GPIO_WritePin(NRF_SPI_CS_GPIO_PORT, NRF_SPI_CS_PIN, 0)


/* Initialisation Functions. Only required to call nrf24l01plus_hal_init */
void nrf24l01plus_hal_spi_init();
void nrf24l01plus_hal_init();

/* Register functions */
void nrf24l01plus_hal_wr(uint8_t reg_addr, uint8_t val);
uint8_t nrf24l01plus_hal_rr(uint8_t reg_addr);
void nrf24l01plus_hal_wb(uint8_t reg_addr, uint8_t *buffer, int buffer_len);
void nrf24l01plus_hal_rb(uint8_t reg_addr, uint8_t *buffer, int buffer_len);
uint8_t nrf24l01plus_hal_spi_SendRecv_Byte(uint8_t byte);

/* Transmit Functions */
void nrf24l01plus_hal_send(uint8_t *tx_buf);
int nrf24l01plus_hal_txFifoEmpty(void);


#endif
