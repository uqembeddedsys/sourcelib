/**
  ******************************************************************************
  * @file    platform-conf.h 
  * @author  MDS
  * @date    22042023
  * @brief   nrf24l01 CE and CSN pin definitions
  ******************************************************************************
  *  
  */ 
#ifndef PLATFORM_CONF_H
#define PLATFORM_CONF_H

//nrf24l01 CE pin
#define NRF_MODE_PIN		GPIO_PIN_15
#define NRF_MODE_GPIO_PORT 	GPIOD
#define __NRF_MODE_GPIO_CLK 	__GPIOD_CLK_ENABLE()

#endif
