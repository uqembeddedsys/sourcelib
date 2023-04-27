/**
  ******************************************************************************
  * @file    platform-conf.h 
  * @author  MDS
  * @date    22-April-2014
  * @brief   nrf24l01 CE and CSN pin definitions
  ******************************************************************************
  *  
  */ 
#ifndef PLATFORM_CONF_H
#define PLATFORM_CONF_H

//nrf24l01 CE pin
#define NRF_MODE_PIN		BRD_D9_PIN
#define NRF_MODE_GPIO_PORT 	BRD_D9_GPIO_PORT
#define __NRF_MODE_GPIO_CLK 	__BRD_D9_GPIO_CLK()

//nrf24l01 SPI CS pin
#define NRF_SPI_CS_PIN			BRD_D10_PIN
#define NRF_SPI_CS_GPIO_PORT 	BRD_D10_GPIO_PORT
#define __NRF_SPI_CS_GPIO_CLK 	__BRD_D10_GPIO_CLK()

#endif
