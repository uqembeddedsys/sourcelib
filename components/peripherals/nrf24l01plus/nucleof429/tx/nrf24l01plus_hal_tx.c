/**
  ******************************************************************************
  * @file    nrf24l01plus.c
  * @author  MDS
  * @date    21-March-2016
  * @brief   nrf9051 SPI operations, initialisation and read/write functions
  *          
  * 
  ******************************************************************************
  *
  */

#include "board.h"
#include "processor_hal.h"
#include "nrf24l01plus_hal_tx.h"
#include "nrf24l01plus_hal_pin.h"

#ifdef DEBUG
#include "debug_log.h"
#endif


/**
  * @brief  NRF24l01Plus send packet Function.
  * @param  Transmit buffer
  * @retval None
  */
void nrf24l01plus_hal_send(uint8_t *tx_buf) {


	nrf24l01plus_hal_wr(NRF24L01P_CONFIG, 0x72);     // Set PWR_UP bit, disable CRC(2 unsigned chars) & Prim:TX.
	nrf24l01plus_hal_wb(NRF24L01P_WR_TX_PLOAD, tx_buf, NRF24L01P_TX_PLOAD_WIDTH);   // write playload to TX_FIFO

	NRF_CE_LOW();
	HAL_Delayus(20);	
	NRF_CE_HIGH();
	HAL_Delayus(20); // Send one packet
	NRF_CE_LOW();
	HAL_Delayus(4500); // Transition TX mode to Standby-I

	nrf24l01plus_hal_wr(NRF24L01P_CONFIG, 0x73);
	NRF_CE_HIGH();

	return;

}

/**
  * @brief  NRF24l01Plus rxFifoEmpy Function. Check NRF24l01Plus rx fifo.
  * @param  None
  * @retval Returns 1 if empty else return 0
  */
int nrf24l01plus_txFifoEmpty(void) {
    unsigned char fifoStatus;

    fifoStatus = nrf24l01plus_rr(NRF24L01P_FIFO_STATUS);

#ifdef DEBUG
	debug_log("DEBUG: fifoStatus: %X\n\r", fifoStatus);
#endif

    return (fifoStatus & NRF24L01P_FIFO_TX_EMPTY);

}
