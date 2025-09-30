/* board.h
 * Created: 21/11/2017
 * Updated: 19/02/2018 - the great un-re-naming
 * Author: mds, npg
 */

#ifndef BOARD_BOARD_H
#define BOARD_BOARD_H

#include <stdio.h>
#include <stdint.h>

/**********************  BOARD LEDs  *****************************/

//LED definitions (Green - LD0, Blue - LD1, Red - LD2)
#define BRD_GREEN_LED					0	//LD1
#define BRD_GREEN_LEDMASK				1 << BRD_GREEN_LED
#define BRD_GREEN_LED_PIN               0
#define BRD_GREEN_LED_GPIO_PIN          GPIO_PIN_0
#define BRD_GREEN_LED_GPIO_PORT         GPIOB
#define __BRD_GREEN_LED_GPIO_CLK()      __GPIOB_CLK_ENABLE()

#define BRD_BLUE_LED					1	//LD2
#define BRD_BLUE_LEDMASK				1 << BRD_BLUE_LED
#define BRD_BLUE_LED_PIN                7
#define BRD_BLUE_LED_GPIO_PIN           GPIO_PIN_7
#define BRD_BLUE_LED_GPIO_PORT          GPIOB
#define __BRD_BLUE_LED_GPIO_CLK()       __GPIOB_CLK_ENABLE()

#define BRD_RED_LED						2	//LD3
#define BRD_RED_LEDMASK					1 << BRD_RED_LED
#define BRD_RED_LED_PIN                 14
#define BRD_RED_LED_GPIO_PIN            GPIO_PIN_14
#define BRD_RED_LED_GPIO_PORT           GPIOB
#define __BRD_RED_LED_GPIO_CLK()        __GPIOB_CLK_ENABLE()

#define __BRD_ALL_LED_GPIO_CLK()        __GPIOB_CLK_ENABLE()
#define __BRD_ALL_LED_GPIO              GPIOB

void BRD_LEDInit();
void BRD_LEDOn(uint8_t ledmask);
void BRD_LEDOff(uint8_t ledmask);
void BRD_LEDToggle(uint8_t ledmask);

#define BRD_LEDGreenOn() BRD_LEDOn(BRD_GREEN_LEDMASK)
#define BRD_LEDBlueOn() BRD_LEDOn(BRD_BLUE_LEDMASK)
#define BRD_LEDRedOn() BRD_LEDOn(BRD_RED_LEDMASK)

#define BRD_LEDGreenOff() BRD_LEDOff(BRD_GREEN_LEDMASK)
#define BRD_LEDBlueOff() BRD_LEDOff(BRD_BLUE_LEDMASK)
#define BRD_LEDRedOff() BRD_LEDOff(BRD_RED_LEDMASK)

#define BRD_LEDGreenToggle() BRD_LEDToggle(BRD_GREEN_LEDMASK)
#define BRD_LEDBlueToggle() BRD_LEDToggle(BRD_BLUE_LEDMASK)
#define BRD_LEDRedToggle() BRD_LEDToggle(BRD_RED_LEDMASK)
/**********************  BOARD LEDs  *****************************/



/**********************  DEBUG UART  *****************************/
//Debug uart definitions
#define BRD_DEBUG_UART						USART3
#define BRD_DEBUG_UART_BAUDRATE				115200
#define __BRD_DEBUG_UART_CLK()				__USART3_CLK_ENABLE()
#define BRD_DEBUG_UART_GPIO                 GPIOD
#define __BRD_DEBUG_UART_GPIO_CLK()		__GPIOD_CLK_ENABLE()
#define BRD_DEBUG_UART_AF				GPIO_AF7_USART3

#define BRD_DEBUG_UART_TX_PIN               8				
#define BRD_DEBUG_UART_TX_GPIO_PIN          GPIO_PIN_8
#define BRD_DEBUG_UART_TX_GPIO_PORT			GPIOD

#define BRD_DEBUG_UART_RX_PIN				9
#define BRD_DEBUG_UART_RX_GPIO_PIN          GPIO_PIN_9
#define BRD_DEBUG_UART_RX_GPIO_PORT			GPIOD

void BRD_debuguart_init();
void BRD_debuguart_putc(unsigned char c);
unsigned char BRD_debuguart_getc();
void BRD_debuguart_puts(unsigned char *c);
void BRD_debuguart_putm(unsigned char *c, int len);
unsigned char BRD_debuguart_getc(long unsigned int blocktime);

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

/*********************  GPIO Signals for sysmon **************************/
#ifndef	NOSYSMON

//System monitor pins
#ifndef SYSMON_CHAN0_PIN
#define SYSMON_CHAN0_PIN		14
#endif

#ifndef SYSMON_CHAN0_GPIO
#define SYSMON_CHAN0_GPIO		GPIOG
#endif

#ifndef SYSMON_CHAN0_GPIO_CLK
#define SYSMON_CHAN0_GPIO_CLK()	__GPIOG_CLK_ENABLE()
#endif

#ifndef SYSMON_CHAN1_PIN
#define SYSMON_CHAN1_PIN		10
#endif

#ifndef SYSMON_CHAN1_GPIO
#define SYSMON_CHAN1_GPIO		GPIOD
#endif

#ifndef SYSMON_CHAN1_GPIO_CLK
#define SYSMON_CHAN1_GPIO_CLK()	__GPIOD_CLK_ENABLE()
#endif

#ifndef SYSMON_CHAN2_PIN
#define SYSMON_CHAN2_PIN		4
#endif

#ifndef SYSMON_CHAN2_GPIO
#define SYSMON_CHAN2_GPIO		GPIOG
#endif

#ifndef SYSMON_CHAN2_GPIO_CLK
#define SYSMON_CHAN2_GPIO_CLK()	__GPIOG_CLK_ENABLE()
#endif

//System monitor macros for set clear and toggle
#define SYSMON_CHAN0_SET()		SYSMON_CHAN0_GPIO->ODR |= (0x01 << SYSMON_CHAN0_PIN);
#define SYSMON_CHAN0_CLR()		SYSMON_CHAN0_GPIO->ODR &= ~(0x01 << SYSMON_CHAN0_PIN);
#define SYSMON_CHAN0_TOGGLE()	SYSMON_CHAN0_GPIO->ODR ^= (0x01 << SYSMON_CHAN0_PIN);

#define SYSMON_CHAN1_SET()		SYSMON_CHAN1_GPIO->ODR |= (0x01 << SYSMON_CHAN1_PIN);
#define SYSMON_CHAN1_CLR()		SYSMON_CHAN1_GPIO->ODR &= ~(0x01 << SYSMON_CHAN1_PIN);
#define SYSMON_CHAN1_TOGGLE()	SYSMON_CHAN1_GPIO->ODR ^= (0x01 << SYSMON_CHAN1_PIN);

#define SYSMON_CHAN2_SET()		SYSMON_CHAN2_GPIO->ODR |= (0x01 << SYSMON_CHAN2_PIN);
#define SYSMON_CHAN2_CLR()		SYSMON_CHAN2_GPIO->ODR &= ~(0x01 << SYSMON_CHAN2_PIN);
#define SYSMON_CHAN2_TOGGLE()	SYSMON_CHAN2_GPIO->ODR ^= (0x01 << SYSMON_CHAN2_PIN);

void BRD_sysmon_init();

#endif

void BRD_delayInit();
void HAL_Delayus(uint32_t us);




#endif /* BOARD_BOARD_H_ */

