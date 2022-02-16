/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @author  Ac6
  * @version V1.0
  * @date    02-Feb-2015
  * @brief   Default Interrupt Service Routines.
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#ifdef USE_FREERTOS_SYSTICK     //USE_RTOS_SYSTICK
#include "FreeRTOS.h"
#endif
#include "stm32f4xx_it.h"
#include "board.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#ifdef ENABLE_ETHERNET
extern ETH_HandleTypeDef EthHandle;
#endif

#ifdef ENABLE_HID
extern PCD_HandleTypeDef hpcd;
extern USBD_HandleTypeDef USBD_Device;
#endif

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            	  	    Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
    while (1)
  {
    BRD_LEDInit();
    BRD_LEDGreenOn();
    BRD_LEDBlueOn();
	BRD_LEDRedOn();

  }
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
    BRD_LEDInit();
    BRD_LEDGreenOff();
    BRD_LEDBlueOff();
	BRD_LEDRedOn();

  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
	BRD_LEDInit();		//Initialise Blue LED
	BRD_LEDGreenOff();
    BRD_LEDBlueOn();
	BRD_LEDRedOn();
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
	BRD_LEDInit();		//Initialise Blue LED
    BRD_LEDGreenOn();
    BRD_LEDBlueOff();
	BRD_LEDRedOn();
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
	BRD_LEDInit();		//Initialise Blue LED
    BRD_LEDGreenOn();
    BRD_LEDBlueOn();
	BRD_LEDRedOn();
  }
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
/* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
	BRD_LEDInit();		//Initialise Blue LED
    BRD_LEDGreenOn();
    BRD_LEDBlueOn();
	BRD_LEDRedOff();
  }
}

/**
  * @brief  This function handles SysTick Handler, but only if no RTOS defines it.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();

#ifdef USE_FREERTOS_SYSTICK
	xPortSysTickHandler();
#endif
}

#ifdef ENABLE_HID
/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/
/**
  * @brief  This function handles USB-On-The-Go FS global interrupt request.
  * @param  None
  * @retval None
  */
void OTG_FS_IRQHandler(void)
{
  HAL_PCD_IRQHandler(&hpcd);
}

/**
  * @brief  This function handles USB OTG FS Wakeup IRQ Handler.
  * @param  None
  * @retval None
  */
 
void OTG_FS_WKUP_IRQHandler(void)
{
  if((&hpcd)->Init.low_power_enable)
  {
    /* Reset SLEEPDEEP bit of Cortex System Control Register */
    SCB->SCR &= (uint32_t)~((uint32_t)(SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk));  
    
    
    /* Configures system clock after wake-up from STOP: enable HSE, PLL and select 
    PLL as system clock source (HSE and PLL are disabled in STOP mode) */
    
    __HAL_RCC_HSE_CONFIG(RCC_HSE_ON);
    
    /* Wait till HSE is ready */  
    while(__HAL_RCC_GET_FLAG(RCC_FLAG_HSERDY) == RESET)
    {}
    
    /* Enable the main PLL. */
    __HAL_RCC_PLL_ENABLE();
    
    /* Wait till PLL is ready */  
    while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLRDY) == RESET)
    {}
    
    /* Select PLL as SYSCLK */
    MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_SYSCLKSOURCE_PLLCLK);
    
    while (__HAL_RCC_GET_SYSCLK_SOURCE() != RCC_CFGR_SWS_PLL)
    {}
    
    /* ungate PHY clock */
    __HAL_PCD_UNGATE_PHYCLOCK((&hpcd)); 
  }
  /* Clear EXTI pending Bit*/
  __HAL_USB_OTG_FS_WAKEUP_EXTI_CLEAR_FLAG();
}
#endif

/**
  * @brief  This function handles Ethernet interrupt request.
  * @param  None
  * @retval None
  */
void ETH_IRQHandler(void)
{
#ifdef ENABLE_ETHERNET
  HAL_ETH_IRQHandler(&EthHandle);
#endif
}

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/


