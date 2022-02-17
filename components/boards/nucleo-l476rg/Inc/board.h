/* board.h
 * Created: 21/11/2017
 * Updated: 19/02/2018 - the great un-re-naming
 * Author: mds, npg
 */

#ifndef BOARD_BOARD_H
#define BOARD_BOARD_H

#include <stdio.h>

#define BRD_A0              0                   // ADC ADC123_IN3 Mask
#define BRD_A0_PIN          GPIO_PIN_0          // Pin Index
#define BRD_A0_GPIO_PORT    GPIOA               // GPIO Port for pin
#define __BRD_A0_GPIO_CLK() __GPIOA_CLK_ENABLE()// GPIO Port clock
#define BRD_A0_EXTI_IRQ     EXTI0_IRQn          // External interrupt IRQ
#define BRD_A0_ADC_CHAN     ADC_CHANNEL_3

#define BRD_A1              1                   // ADC ADC123_IN1
#define BRD_A1_PIN          GPIO_PIN_1
#define BRD_A1_GPIO_PORT    GPIOA
#define __BRD_A1_GPIO_CLK() __GPIOA_CLK_ENABLE()
#define BRD_A1_EXTI_IRQ     EXTI1_IRQn
#define BRD_A1_ADC_CHAN     ADC_CHANNEL_10

#define BRD_A2              4                   // ADC ADC123_IN13
#define BRD_A2_PIN          GPIO_PIN_4
#define BRD_A2_GPIO_PORT    GPIOA
#define __BRD_A2_GPIO_CLK() __GPIOA_CLK_ENABLE()
#define BRD_A2_EXTI_IRQ     EXTI4_IRQn
#define BRD_A2_ADC_CHAN     ADC_CHANNEL_13

#define BRD_A3              0                   // ADC ADC3_IN9
#define BRD_A3_PIN          GPIO_PIN_0
#define BRD_A3_GPIO_PORT    GPIOB
#define __BRD_A3_GPIO_CLK() __GPIOB_CLK_ENABLE()
#define BRD_A3_EXTI_IRQ     EXTI0_IRQn
#define BRD_A3_ADC_CHAN     ADC_CHANNEL_9

#define BRD_A4              1                   // ADC ADC3_IN15 SB147,SB157,SB138,SB143
#define BRD_A4_PIN          GPIO_PIN_1
#define BRD_A4_GPIO_PORT    GPIOC
#define __BRD_A4_GPIO_CLK() __GPIOC_CLK_ENABLE()
#define BRD_A4_EXTI_IRQ     EXTI1_IRQn
#define BRD_A4_ADC_CHAN     ADC_CHANNEL_15

#define BRD_A5              0                  // ADC ADC3_IN8 SB147,SB157,SB138,SB143
#define BRD_A5_PIN          GPIO_PIN_0
#define BRD_A5_GPIO_PORT    GPIOC
#define __BRD_A5_GPIO_CLK() __GPIOC_CLK_ENABLE()
#define BRD_A5_EXTI_IRQ     EXTI0_IRQn
#define BRD_A5_ADC_CHAN     ADC_CHANNEL_8

#define BRD_D0              3                   // USART6 USART_A_RX
#define BRD_D0_PIN          GPIO_PIN_3
#define BRD_D0_GPIO_PORT    GPIOA
#define __BRD_D0_GPIO_CLK() __GPIOA_CLK_ENABLE()
#define BRD_D0_EXTI_IRQ     EXTI3_IRQn

#define BRD_D1              2                  // USART6 USART_A_TX
#define BRD_D1_PIN          GPIO_PIN_2
#define BRD_D1_GPIO_PORT    GPIOA
#define __BRD_D1_GPIO_CLK() __GPIOA_CLK_ENABLE()
#define BRD_D1_EXTI_IRQ     EXTI2_IRQn

#define BRD_D2              10                  // - I/O
#define BRD_D2_PIN          GPIO_PIN_10
#define BRD_D2_GPIO_PORT    GPIOA
#define __BRD_D2_GPIO_CLK() __GPIOA_CLK_ENABLE()
#define BRD_D2_EXTI_IRQ     EXTI15_10_IRQn

#define BRD_D3              3                  // TIM1_CH3 TIMER_A_PWM3
#define BRD_D3_PIN          GPIO_PIN_3
#define BRD_D3_GPIO_PORT    GPIOB
#define __BRD_D3_GPIO_CLK() __GPIOB_CLK_ENABLE()
#define BRD_D3_EXTI_IRQ     EXTI3_IRQn

#define BRD_D4              5                  // - I/O
#define BRD_D4_PIN          GPIO_PIN_5
#define BRD_D4_GPIO_PORT    GPIOB
#define __BRD_D4_GPIO_CLK() __GPIOB_CLK_ENABLE()
#define BRD_D4_EXTI_IRQ     EXTI5_IRQn

#define BRD_D5              4                  // TIM1_CH2 TIMER_A_PWM2
#define BRD_D5_PIN          GPIO_PIN_4
#define BRD_D5_GPIO_PORT    GPIOB
#define __BRD_D5_GPIO_CLK() __GPIOB_CLK_ENABLE()
#define BRD_D5_EXTI_IRQ     EXTI4_IRQn

#define BRD_D6              10                   // TIM1_CH1 TIMER_A_PWM1
#define BRD_D6_PIN          GPIO_PIN_10
#define BRD_D6_GPIO_PORT    GPIOB
#define __BRD_D6_GPIO_CLK() __GPIOB_CLK_ENABLE()
#define BRD_D6_EXTI_IRQ     EXTI15_10_IRQn

#define BRD_D7              8                  // - I/O
#define BRD_D7_PIN          GPIO_PIN_8
#define BRD_D7_GPIO_PORT    GPIOA
#define __BRD_D7_GPIO_CLK() __GPIOA_CLK_ENABLE()
#define BRD_D7_EXTI_IRQ     EXTI9_5_IRQn

#define BRD_D8              9                  // I/O I/O
#define BRD_D8_PIN          GPIO_PIN_9
#define BRD_D8_GPIO_PORT    GPIOA
#define __BRD_D8_GPIO_CLK() __GPIOA_CLK_ENABLE()
#define BRD_D8_EXTI_IRQ     EXTI9_5_IRQn

#define BRD_D9              7                  // TIM4_CH4 TIM_B_PWM2
#define BRD_D9_PIN          GPIO_PIN_7
#define BRD_D9_GPIO_PORT    GPIOC
#define __BRD_D9_GPIO_CLK() __GPIOC_CLK_ENABLE()
#define BRD_D9_EXTI_IRQ     EXTI9_5_IRQn

#define BRD_D10             6                  // SPI1_CS/TIM4_CH3 SPI_A_CS/TIM_B_PWM3
#define BRD_D10_PIN         GPIO_PIN_6
#define BRD_D10_GPIO_PORT   GPIOB
#define __BRD_D10_GPIO_CLK()__GPIOB_CLK_ENABLE()
#define BRD_D10_EXTI_IRQ    EXTI9_5_IRQn

#define BRD_D11             7                   // SPI1_MOSI/TIM14_CH1 SPI_A_MOSI/TIM_E_PWM1 SB121,SB122
#define BRD_D11_PIN         GPIO_PIN_7
#define BRD_D11_GPIO_PORT   GPIOA
#define __BRD_D11_GPIO_CLK()__GPIOA_CLK_ENABLE()
#define BRD_D11_EXTI_IRQ    EXTI9_5_IRQn

#define BRD_D12             6                   // SPI1_MISO SPI_A_MISO
#define BRD_D12_PIN         GPIO_PIN_6
#define BRD_D12_GPIO_PORT   GPIOA
#define __BRD_D12_GPIO_CLK()__GPIOA_CLK_ENABLE()
#define BRD_D12_EXTI_IRQ    EXTI9_5_IRQn

#define BRD_D13             5                   // SPI1_SCK SPI_A_SCK
#define BRD_D13_PIN         GPIO_PIN_5
#define BRD_D13_GPIO_PORT   GPIOA
#define __BRD_D13_GPIO_CLK()__GPIOA_CLK_ENABLE()
#define BRD_D13_EXTI_IRQ    EXTI9_5_IRQn

#define BRD_D14             9                   // I2C1_SDA I2C_A_SDA
#define BRD_D14_PIN         GPIO_PIN_9
#define BRD_D14_GPIO_PORT   GPIOB
#define __BRD_D14_GPIO_CLK()__GPIOB_CLK_ENABLE()
#define BRD_D14_EXTI_IRQ    EXTI9_5_IRQn

#define BRD_D15             8                   // I2C1_SCL I2C_A_SCL
#define BRD_D15_PIN         GPIO_PIN_8
#define BRD_D15_GPIO_PORT   GPIOB
#define __BRD_D15_GPIO_CLK()__GPIOB_CLK_ENABLE()
#define BRD_D15_EXTI_IRQ    EXTI9_5_IRQn




/**********************  BOARD LEDs  *****************************/

//LED definitions (Green - LD0, Blue - LD1, Red - LD2)
#define BRD_GREEN_LED					0	//LD1
#define BRD_GREEN_LEDMASK				1 << BRD_GREEN_LED
#define BRD_GREEN_LED_PIN               GPIO_PIN_5
#define BRD_GREEN_LED_GPIO_PORT         GPIOA
#define __BRD_GREEN_LED_GPIO_CLK()      __GPIOA_CLK_ENABLE()

void BRD_LEDInit();
void BRD_LEDOn(uint8_t ledmask);
void BRD_LEDOff(uint8_t ledmask);
void BRD_LEDToggle(uint8_t ledmask);

#define BRD_LEDGreenOn() BRD_LEDOn(BRD_GREEN_LEDMASK)
//#define BRD_LEDBlueOn() BRD_LEDOn(BRD_GREEN_LEDMASK)
//#define BRD_LEDRedOn() BRD_LEDOn(BRD_GREEN_LEDMASK)

#define BRD_LEDGreenOff() BRD_LEDOff(BRD_GREEN_LEDMASK)
//#define BRD_LEDBlueOff() BRD_LEDOff(BRD_GREEN_LEDMASK)
//#define BRD_LEDRedOff() BRD_LEDOff(BRD_GREEN_LEDMASK)

#define BRD_LEDGreenToggle() BRD_LEDToggle(BRD_GREEN_LEDMASK)
//#define BRD_LEDBlueToggle() BRD_LEDToggle(BRD_GREEN_LEDMASK)
//#define BRD_LEDRedToggle() BRD_LEDToggle(BRD_GREEN_LEDMASK)
/**********************  BOARD LEDs  *****************************/



/**********************  DEBUG UART  *****************************/
//Debug uart definitions
#define BRD_DEBUG_UART						USART2
#define BRD_DEBUG_UART_BAUDRATE				115200
#define __BRD_DEBUG_UART_CLK()				__USART2_CLK_ENABLE()

#define BRD_DEBUG_UART_TX_AF				GPIO_AF7_USART2
#define BRD_DEBUG_UART_TX_PIN				GPIO_PIN_2
#define BRD_DEBUG_UART_TX_GPIO_PORT			GPIOA
#define __BRD_DEBUG_UART_TX_GPIO_CLK()		__GPIOA_CLK_ENABLE()

#define BRD_DEBUG_UART_RX_AF				GPIO_AF7_USART2
#define BRD_DEBUG_UART_RX_PIN				GPIO_PIN_3
#define BRD_DEBUG_UART_RX_GPIO_PORT			GPIOA
#define __BRD_DEBUG_UART_RX_GPIO_CLK()		__GPIOA_CLK_ENABLE()

void BRD_debuguart_init();
void BRD_debuguart_putc(unsigned char c);
unsigned char BRD_debuguart_getc();
void BRD_debuguart_puts(unsigned char *c);
void BRD_debuguart_putm(unsigned char *c, int len);
unsigned char BRD_debuguart_getc();

unsigned char debug_getuc(void);
/**********************  DEBUG UART ******************************/


/*********************  USER BUTTON  *****************************/
#define BRD_USER_BUTTON_PIN GPIO_PIN_13
#define BRD_USER_BUTTON_GPIO_PORT GPIOC
#define BRD_USER_BUTTON_GPIO_CLK_ENABLE() __HAL_RCC_GPIOC_CLK_ENABLE()
#define BRD_USER_BUTTON_GPIO_CLK_DISABLE() __HAL_RCC_GPIOC_CLK_DISABLE()
#define BRD_USER_BUTTON_EXTI_LINE GPIO_PIN_13
#define BRD_USER_BUTTON_EXTI_IRQn EXTI15_10_IRQn
/*********************  USER BUTTON  *****************************/

/*********************  I2C Signals  *****************************/
#define BRD_I2C_A					I2C1
#define __BRD_I2C_A_CLK()			__I2C1_CLK_ENABLE()
#define __BRD_I2C_A_FORCE_RESET()	__HAL_RCC_I2C1_FORCE_RESET()
#define __BRD_I2C_A_RELEASE_RESET()	__HAL_RCC_I2C1_RELEASE_RESET()

#define BRD_SDA_A_PIN				GPIO_PIN_9
#define BRD_SDA_A_GPIO_PORT			GPIOB
#define __BRD_SDA_A_GPIO_CLK()		__GPIOB_CLK_ENABLE()
#define BRD_SDA_A_AF					GPIO_AF4_I2C1

#define BRD_SCL_A_PIN				GPIO_PIN_8
#define BRD_SCL_A_GPIO_PORT			GPIOB
#define __BRD_SCL_A_GPIO_CLK()		__GPIOB_CLK_ENABLE()
#define BRD_SCL_A_AF					GPIO_AF4_I2C1
/*********************  I2C Signals  *****************************/

/*********************  SPI Signals  *****************************/
#define BRD_SPI_B	SPI3_BASE
#define __BRD_SPI_B_CLK() __SPI3_CLK_ENABLE()

#define BRD_SPI_B_SCK_PIN	GPIO_PIN_3
#define BRD_SPI_B_SCK_GPIO_PORT GPIOB
#define BRD_SPI_B_SCK_AF GPIO_AF6_SPI3
#define __BRD_SPI_B_SCK_GPIO_CLK() __GPIOB_CLK_ENABLE()


#define BRD_SPI_B_MISO_PIN	GPIO_PIN_4
#define BRD_SPI_B_MISO_GPIO_PORT GPIOB
#define BRD_SPI_B_MISO_AF GPIO_AF6_SPI3
#define __BRD_SPI_B_MISO_GPIO_CLK() __GPIOB_CLK_ENABLE()

#define BRD_SPI_B_MOSI_PIN	GPIO_PIN_5
#define BRD_SPI_B_MOSI_GPIO_PORT GPIOB
#define BRD_SPI_B_MOSI_AF GPIO_AF6_SPI3
#define __BRD_SPI_B_MOSI_GPIO_CLK() __GPIOB_CLK_ENABLE()

#define BRD_SPI_B_CS_PIN GPIO_PIN_4
#define BRD_SPI_B_CS_GPIO_PORT GPIOA
#define BRD_SPI_B_CS_AF GPIO_AF6_SPI3
#define __BRD_SPI_B_CS_GPIO_CLK() __GPIOA_CLK_ENABLE()
/*********************  SPI Signals For RA (on sideboard) **************************/
#ifndef	NOSIDEBOARD

#define BRD_SPI_RA	SPI2_BASE
#define __BRD_SPI_RA_CLK() __SPI2_CLK_ENABLE()

#define BRD_SPI_RA_SCK_PIN	GPIO_PIN_10
#define BRD_SPI_RA_SCK_GPIO_PORT GPIOB
#define BRD_SPI_RA_SCK_AF GPIO_AF5_SPI2
#define __BRD_SPI_RA_SCK_GPIO_CLK() __GPIOB_CLK_ENABLE()

#define BRD_SPI_RA_MISO_PIN	GPIO_PIN_2
#define BRD_SPI_RA_MISO_GPIO_PORT GPIOC
#define BRD_SPI_RA_MISO_AF GPIO_AF5_SPI2
#define __BRD_SPI_RA_MISO_GPIO_CLK() __GPIOC_CLK_ENABLE()

#define BRD_SPI_RA_MOSI_PIN	GPIO_PIN_3
#define BRD_SPI_RA_MOSI_GPIO_PORT GPIOC
#define BRD_SPI_RA_MOSI_AF GPIO_AF5_SPI2
#define __BRD_SPI_RA_MOSI_GPIO_CLK() __GPIOC_CLK_ENABLE()

#define BRD_SPI_RA_CS_PIN GPIO_PIN_12
#define BRD_SPI_RA_CS_GPIO_PORT GPIOB
#define BRD_SPI_RA_CS_AF GPIO_AF5_SPI2
#define __BRD_SPI_RA_CS_GPIO_CLK() __GPIOB_CLK_ENABLE()

#endif


void BRD_init();
void HAL_Delayus(uint32_t us);




#endif /* BOARD_BOARD_H_ */

