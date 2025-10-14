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

#ifdef DEBUG
#include "debug_log.h"
#endif

//Set default channel and addreess from either a file or directly set.
#ifdef MYCONFIG
#include "myconfig.h"
#ifdef MYRADIOCHAN
	#define DEFAULT_RF_CHANNEL MYRADIOCHAN
	#define default_addr myradiotxaddr
#endif
#else
#define DEFAULT_RF_CHANNEL	40								//Default radio channel
uint8_t default_addr[] = {0x12, 0x34, 0x56, 0x78, 0x90};	//Default address with MSB first
#endif

static SPI_HandleTypeDef SpiHandle;

/**
  * @brief  NRF24l01Plus SPI Intialisation Function.
  * @param  None
  * @retval None
  */
void nrf24l01plus_hal_spi_init() {

	GPIO_InitTypeDef GPIO_spi;

	/* Initialise SPI and Pin clocks*/
	__NRF_SPI_CLK();

	__NRF_SPI_SCK_GPIO_CLK();
	__NRF_SPI_MISO_GPIO_CLK();
	__NRF_SPI_MOSI_GPIO_CLK();
	__NRF_SPI_CS_GPIO_CLK;
	__NRF_MODE_GPIO_CLK;

	/* Initialise SPI and Pin clocks*/
	/* SPI SCK pin configuration */
	GPIO_spi.Pin = NRF_SPI_SCK_PIN;
	GPIO_spi.Mode = GPIO_MODE_AF_PP;
	GPIO_spi.Speed = GPIO_SPEED_HIGH;
	GPIO_spi.Pull = GPIO_PULLDOWN;
	GPIO_spi.Alternate = NRF_SPI_SCK_AF;
	HAL_GPIO_Init(NRF_SPI_SCK_GPIO_PORT, &GPIO_spi);

  	/* SPI MISO pin configuration */
	GPIO_spi.Pin = NRF_SPI_MISO_PIN;
	GPIO_spi.Mode = GPIO_MODE_AF_PP;
	GPIO_spi.Speed = GPIO_SPEED_FAST;
	GPIO_spi.Pull = GPIO_PULLUP;		//Must be set as pull up
	GPIO_spi.Alternate = NRF_SPI_MISO_AF;
	HAL_GPIO_Init(NRF_SPI_MISO_GPIO_PORT, &GPIO_spi);

	/* SPI  MOSI pin configuration */
	GPIO_spi.Pin =  NRF_SPI_MOSI_PIN;
	GPIO_spi.Mode = GPIO_MODE_AF_PP;
	GPIO_spi.Pull = GPIO_PULLDOWN;
	GPIO_spi.Alternate = NRF_SPI_MOSI_AF;
	HAL_GPIO_Init(NRF_SPI_MOSI_GPIO_PORT, &GPIO_spi);

	SpiHandle.Instance = (SPI_TypeDef *)(BRD_SPI_RA);

	 __HAL_SPI_DISABLE(&SpiHandle);

  SpiHandle.Init.Mode              = SPI_MODE_MASTER;
  SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32; //SPI_BAUDRATEPRESCALER_32; //56;
  SpiHandle.Init.Direction         = SPI_DIRECTION_2LINES;
  SpiHandle.Init.CLKPhase          = SPI_PHASE_1EDGE;
  SpiHandle.Init.CLKPolarity       = SPI_POLARITY_LOW;
  SpiHandle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLED;
  SpiHandle.Init.CRCPolynomial     = 0; //7;
  SpiHandle.Init.DataSize          = SPI_DATASIZE_8BIT;
  SpiHandle.Init.FirstBit          = SPI_FIRSTBIT_MSB;		//MSB
  SpiHandle.Init.NSS               = SPI_NSS_SOFT;
  SpiHandle.Init.TIMode            = SPI_TIMODE_DISABLED;

  HAL_SPI_Init(&SpiHandle);

 __HAL_SPI_ENABLE(&SpiHandle);

	/* Configure GPIO PIN for SPI Chip select, TFT CS, TFT DC */
	GPIO_spi.Pin = NRF_SPI_CS_PIN;				   //Pin
	GPIO_spi.Mode = GPIO_MODE_OUTPUT_PP;		 //Output Mode
	GPIO_spi.Pull = GPIO_PULLUP;			       //Enable Pull up, down or no pull resister
	GPIO_spi.Speed = GPIO_SPEED_FAST;			   //Pin latency
	HAL_GPIO_Init(NRF_SPI_CS_GPIO_PORT, &GPIO_spi);	//Initialise Pin

	/* Configure GPIO PIN for RX/TX mode */
	GPIO_spi.Pin = NRF_MODE_PIN;
	GPIO_spi.Mode = GPIO_MODE_OUTPUT_PP;		//Output Mode
	GPIO_spi.Pull = GPIO_PULLUP;			      //Enable Pull up, down or no pull resister
	GPIO_spi.Speed = GPIO_SPEED_FAST;			  //Pin latency
	HAL_GPIO_Init(NRF_MODE_GPIO_PORT, &GPIO_spi);	//Initialise Pin

#ifdef NRF_IRQ_ENABLE
	__NRF_IRQ_GPIO_CLK;
	/* Configure GPIO PIN for RX/TX mode */
	GPIO_spi.Pin = NRF_IRQ_PIN;
	GPIO_spi.Mode = GPIO_MODE_INPUT;		//Output Mode
	GPIO_spi.Pull = GPIO_PULLDOWN;			      //Enable Pull up, down or no pull resister
	GPIO_spi.Speed = GPIO_SPEED_FAST;			  //Pin latency
	HAL_GPIO_Init(NRF_IRQ_GPIO_PORT, &GPIO_spi);	//Initialise Pin
#endif

	/* Set chip select high */
	NRF_CS_HIGH();
}


/**
  * @brief  NRF24l01Plus Initalise Function. Initialise registers of NRF24l01Plus.
  * @param  None
  * @retval None
  */
void nrf24l01plus_hal_init() {

	BRD_delayInit();			//Enable uS Delay

	nrf24l01plus_hal_spi_init();	//Enable SPI

	// Set CE low for idle state
	NRF_CE_LOW();

	//Power down nrf before powering up.
	nrf24l01plus_hal_wb(NRF24L01P_WRITE_REG | NRF24L01P_TX_ADDR, default_addr, 5);		  // Writes TX_Address to nRF24L01
	nrf24l01plus_hal_wb(NRF24L01P_WRITE_REG | NRF24L01P_RX_ADDR_P0, default_addr, 5);	//NRF24L01P_TX_ADR_WIDTH);

  	nrf24l01plus_hal_wr(NRF24L01P_EN_AA, 0x00);      							                  // Disable Auto.Ack
  	nrf24l01plus_hal_wr(NRF24L01P_EN_RXADDR, 0x01);  							                  // Enable Pipe0
  	nrf24l01plus_hal_wr(NRF24L01P_RX_PW_P0, NRF24L01P_TX_PLOAD_WIDTH); 	// Select same RX payload width as TX Payload width

  	//SPI_Write_Reg(WRITE_REG + SETUP_RETR, 0x1a);       // 500us + 86us, 10 retransmissions. (not needed - no auto ack)
  	nrf24l01plus_hal_wr(NRF24L01P_RF_CH, DEFAULT_RF_CHANNEL); 	// Select RF channel
  	nrf24l01plus_hal_wr(NRF24L01P_RF_SETUP, 0x06);   							// TX_PWR:0dBm, Datarate:1Mbps

	nrf24l01plus_hal_wr(NRF24L01P_CONFIG, 0x02);	     							// Set PWR_UP bit, enable CRC(2 unsigned chars) & Prim:TX. MAX_RT & TX_DS enabled..

}


/**
  * @brief  nrf24L01plus SPI Send/Receive Byte Function.
  * @param  Byte to be transmitted.
  * @retval Returns byte received.
  */
__weak uint8_t nrf24l01plus_hal_spi_SendRecv_Byte(uint8_t byte) {

	uint8_t rxbyte;
	uint8_t rxbyte2;
	HAL_StatusTypeDef status;

	/* *hspi, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size, uint32_t Timeout); */
	status = HAL_SPI_TransmitReceive(&SpiHandle, &byte, &rxbyte, 1, 200); //HAL_MAX_DELAY);

#ifdef ENABLE_EMU
	//NP2_RUNMON_SPI2();
	HAL_Delay(10);
	np2_runmon_spidump(byte);
	HAL_Delay(100);

	rxbyte = debug_getc();
	rxbyte2 = debug_getc();
	if (rxbyte2 > rxbyte) {
		return rxbyte2;
	}
	debug_flush();
#endif
	return rxbyte;
}


/**
  * @brief  NRF24l01Plus Write Register Function. Write value to register.
  * @param  register address, value
  * @retval None
  */
void nrf24l01plus_hal_wr(uint8_t reg_addr, uint8_t val) {

	NRF_CS_LOW();
	HAL_Delayus(5);

	nrf24l01plus_hal_spi_SendRecv_Byte(NRF24L01P_WRITE_REG | reg_addr);
	HAL_Delayus(10);

	nrf24l01plus_hal_spi_SendRecv_Byte(val);
	HAL_Delayus(5);

	NRF_CS_HIGH();
}

/**
  * @brief  NRF24l01Plus Read Register Function. Read register.
  * @param  register address, buffer, buffer length
  * @retval register value
  */
uint8_t nrf24l01plus_hal_rr(uint8_t reg_addr) {

	uint8_t rxbyte;
	uint8_t status;

	NRF_CS_LOW();
	HAL_Delayus(5);

	status = nrf24l01plus_hal_spi_SendRecv_Byte(reg_addr); //Recv STATUS
	HAL_Delayus(10);

	rxbyte = nrf24l01plus_hal_spi_SendRecv_Byte(0xFF);     //Send dummy byte
	HAL_Delayus(5);

	NRF_CS_HIGH();

	return rxbyte;
}

/**
  * @brief  NRF24l01Plus Write buffer Function. Write buffer to register.
  * @param  register address, buffer, buffer length
  * @retval None
  */
void nrf24l01plus_hal_wb(uint8_t reg_addr, uint8_t *buffer, int buffer_len) {

	int i;

	NRF_CS_LOW();

  HAL_Delayus(10);
	nrf24l01plus_hal_spi_SendRecv_Byte(NRF24L01P_WRITE_REG | reg_addr);
	HAL_Delayus(10);

#ifdef DEBUG
	debug_log("DEBUG: WB: ");
#endif

	for (i = 0; i < buffer_len; i++) {
		  /* Return the Byte read from the SPI bus */
      nrf24l01plus_hal_spi_SendRecv_Byte(buffer[i]);
      HAL_Delayus(10);

#ifdef DEBUG
		  debug_log("%X ", buffer[i]);
#endif
	}

#ifdef DEBUG
	debug_log("\n\r");
#endif

	HAL_Delayus(10);
	NRF_CS_HIGH();
	HAL_Delayus(10);
}


/**
  * @brief  NRF24l01Plus Read buffer Function. Read buffer from register.
  * @param  register address, buffer, buffer length
  * @retval None
  */
void nrf24l01plus_hal_rb(uint8_t reg_addr, uint8_t *buffer, int buffer_len) {

	int i;
	NRF_CS_LOW();
	HAL_Delayus(10);

	nrf24l01plus_hal_spi_SendRecv_Byte(reg_addr);

	HAL_Delayus(10);

#ifdef DEBUG
	debug_log("DEBUG:RB ");
#endif
	for (i = 0; i < buffer_len; i++) {

	  /* Return the Byte read from the SPI bus */
    buffer[i] = nrf24l01plus_hal_spi_SendRecv_Byte(0xFF);
    HAL_Delayus(10);

#ifdef DEBUG
    debug_log("%X ", buffer[i]);
#endif
	}

#ifdef DEBUG
	debug_log("\n\r");
#endif

	HAL_Delayus(10);
	NRF_CS_HIGH();
	HAL_Delayus(10);
}



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
int nrf24l01plus_hal_txFifoEmpty(void) {
    unsigned char fifoStatus;

    fifoStatus = nrf24l01plus_hal_rr(NRF24L01P_FIFO_STATUS);

#ifdef DEBUG
	debug_log("DEBUG: fifoStatus: %X\n\r", fifoStatus);
#endif

    return (fifoStatus & NRF24L01P_FIFO_TX_EMPTY);

}
