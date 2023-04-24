/* -----------------------------------------------------------------------------
# Company name: CSSE3010 - Embedded Systems Design & Interfacing
# Project name: nRF24l01+ driver
# Module name : nrf24l01plus
# Functionality: Provides low level radio calls used to initialise and interface
#        with the nrf24l01+.
#
# Author name: Chris Rieger modified by MDS
# Creation date: 010712
# Revision date (name): 210316 MDS
# Changes implemented (date): 210316 Renamed functions and modified for new spec
#(Comments):
------------------------------------------------------------------------------*/

#ifndef _nrf24l01plus_H
#define _nrf24l01plus_H

#ifdef HAVE_PLATFORMCONF_H
#include "platform-conf.h"
#endif




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




#define NRF24L01P_TX_ADR_WIDTH    5   // 5 unsigned chars TX(RX) address width
#define NRF24L01P_TX_PLOAD_WIDTH  32  // 32 unsigned chars TX payload

#define NRF24L01P_RF_CHANNEL_DEFAULT	76   //40

// SPI(nRF24L01) commands
#define NRF24L01P_READ_REG        0x00	// Define read command to register
#define NRF24L01P_WRITE_REG       0x20	// Define write command to register
#define NRF24L01P_RD_RX_PLOAD     0x61	// Define RX payload register address
#define NRF24L01P_WR_TX_PLOAD     0xA0	// Define TX payload register address
#define NRF24L01P_FLUSH_TX        0xE1	// Define flush TX register command
#define NRF24L01P_FLUSH_RX        0xE2	// Define flush RX register command
#define NRF24L01P_ACTIVATE        0x50	// ACTIVATE additional features
#define NRF24L01P_REUSE_TX_PL     0xE3	// Define reuse TX payload register command
#define NRF24L01P_R_RX_PL_WID     0x60	// Define Read RX-payload width command
#define NRF24L01P_W_ACK_PAYLOAD   0xA8	// Write payload to be used in ACK packet on pipe PPP
#define NRF24L01P_W_TX_PAYLOAD_NOACK 0xB0	// Used in TX mode, Disable AUTOACK on this specific packet
#define NRF24L01P_OP_NOP          0xFF	// Define No Operation, might be used to read status register

// SPI(nRF24L01) registers(addresses)
#define NRF24L01P_CONFIG          0x00	// 'Config' register address
#define NRF24L01P_EN_AA           0x01	// 'Enable Auto Acknowledgment' register address
#define NRF24L01P_EN_RXADDR       0x02	// 'Enabled RX addresses' register address
#define NRF24L01P_SETUP_AW        0x03	// 'Setup address width' register address
#define NRF24L01P_SETUP_RETR      0x04	// 'Setup Auto. Retrans' register address
#define NRF24L01P_RF_CH           0x05	// 'RF channel' register address
#define NRF24L01P_RF_SETUP        0x06	// 'RF setup' register address
#define NRF24L01P_STATUS          0x07	// 'Status' register address
#define NRF24L01P_OBSERVE_TX      0x08	// 'Observe TX' register address
#define NRF24L01P_RPD             0x09	// 'Carrier Detect' register address
#define NRF24L01P_RX_ADDR_P0      0x0A	// 'RX address pipe0' register address
#define NRF24L01P_RX_ADDR_P1      0x0B	// 'RX address pipe1' register address
#define NRF24L01P_RX_ADDR_P2      0x0C	// 'RX address pipe2' register address
#define NRF24L01P_RX_ADDR_P3      0x0D	// 'RX address pipe3' register address
#define NRF24L01P_RX_ADDR_P4      0x0E	// 'RX address pipe4' register address
#define NRF24L01P_RX_ADDR_P5      0x0F	// 'RX address pipe5' register address
#define NRF24L01P_TX_ADDR         0x10	// 'TX address' register address
#define NRF24L01P_RX_PW_P0        0x11	// 'RX payload width, pipe0' register address
#define NRF24L01P_RX_PW_P1        0x12	// 'RX payload width, pipe1' register address
#define NRF24L01P_RX_PW_P2        0x13	// 'RX payload width, pipe2' register address
#define NRF24L01P_RX_PW_P3        0x14	// 'RX payload width, pipe3' register address
#define NRF24L01P_RX_PW_P4        0x15	// 'RX payload width, pipe4' register address
#define NRF24L01P_RX_PW_P5        0x16	// 'RX payload width, pipe5' register address
#define NRF24L01P_FIFO_STATUS     0x17	// 'FIFO Status Register' register address
#define NRF24L01P_DYNPD           0x1C	// 'Enable dynamic payload length' register address
#define NRF24L01P_FEATURE         0x1D	// Additional features register, needed to enable the additional commands

// SPI(nRF24L01) registers(bitmasks)
#define NRF24L01P_ERX_P0		0x01	// Enable Pipe 0 (register EN_RXADDR)
#define NRF24L01P_ERX_P1		0x02	// Enable Pipe 1 (register EN_RXADDR)
#define NRF24L01P_ERX_P2		0x04	// Enable Pipe 2 (register EN_RXADDR)
#define NRF24L01P_ERX_P3		0x08	// Enable Pipe 3 (register EN_RXADDR)
#define NRF24L01P_ERX_P4		0x10	// Enable Pipe 4 (register EN_RXADDR)
#define NRF24L01P_ERX_P5		0x20	// Enable Pipe 5 (register EN_RXADDR)

#define NRF24L01P_FIFO_RX_EMPTY		0x01
#define NRF24L01P_FIFO_RX_FULL		0x02
#define NRF24L01P_FIFO_TX_EMPTY		0x10
#define NRF24L01P_FIFO_TX_FULL		0x20
#define NRF24L01P_FIFO_TX_REUSE		0x40

#define NRF24L01P_RX_DR    0x40
#define NRF24L01P_TX_DS    0x20
#define NRF24L01P_MAX_RT   0x10

void nrf24l01plus_rb(uint8_t reg_addr, uint8_t *buffer, int buffer_len);
void nrf24l01plus_wb(uint8_t reg_addr, uint8_t *buffer, int buffer_len);
void nrf24l01plus_init();
void nrf24l01plus_wr(uint8_t reg_addr, uint8_t val);
uint8_t nrf24l01plus_rr(uint8_t reg_addr);
void nrf24l01plus_ce(void);
void nrf24l01plus_cepin(void);
int nrf24l01plus_txFifoEmpty(void);
int nrf24l01plus_rxFifoEmpty(void);
int nrf24l01plus_recv(uint8_t *rx_buf);
void nrf24l01plus_send(uint8_t *tx_buf);

int nrf24l01plus_irq_status();

#endif
