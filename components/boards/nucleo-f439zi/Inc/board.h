/* board.h
 * Created: 21/11/2017
 * Updated: 19/02/2018 - the great un-re-naming
 * Author: mds, npg
 */

#ifndef BOARD_BOARD_H
#define BOARD_BOARD_H

#include <stdio.h>

#define BRD_A0              3                   // ADC ADC123_IN3 Mask
#define BRD_A0_PIN          GPIO_PIN_3          // Pin Index
#define BRD_A0_GPIO_PORT    GPIOA               // GPIO Port for pin
#define __BRD_A0_GPIO_CLK() __GPIOA_CLK_ENABLE()// GPIO Port clock
#define BRD_A0_EXTI_IRQ     EXTI3_IRQn          // External interrupt IRQ
#define BRD_A0_ADC_CHAN     ADC_CHANNEL_3

#define BRD_A1              0                   // ADC ADC123_IN1
#define BRD_A1_PIN          GPIO_PIN_0
#define BRD_A1_GPIO_PORT    GPIOC
#define __BRD_A1_GPIO_CLK() __GPIOC_CLK_ENABLE()
#define BRD_A1_EXTI_IRQ     EXTI0_IRQn
#define BRD_A1_ADC_CHAN     ADC_CHANNEL_10

#define BRD_A2              3                   // ADC ADC123_IN13
#define BRD_A2_PIN          GPIO_PIN_3
#define BRD_A2_GPIO_PORT    GPIOC
#define __BRD_A2_GPIO_CLK() __GPIOC_CLK_ENABLE()
#define BRD_A2_EXTI_IRQ     EXTI3_IRQn
#define BRD_A2_ADC_CHAN     ADC_CHANNEL_13

#define BRD_A3              3                   // ADC ADC3_IN9
#define BRD_A3_PIN          GPIO_PIN_3
#define BRD_A3_GPIO_PORT    GPIOF
#define __BRD_A3_GPIO_CLK() __GPIOF_CLK_ENABLE()
#define BRD_A3_EXTI_IRQ     EXTI3_IRQn
#define BRD_A3_ADC_CHAN     ADC_CHANNEL_9

#define BRD_A4              5                   // ADC ADC3_IN15 SB147,SB157,SB138,SB143
#define BRD_A4_PIN          GPIO_PIN_5
#define BRD_A4_GPIO_PORT    GPIOF
#define __BRD_A4_GPIO_CLK() __GPIOF_CLK_ENABLE()
#define BRD_A4_EXTI_IRQ     EXTI9_5_IRQn
#define BRD_A4_ADC_CHAN     ADC_CHANNEL_15

#define BRD_A5              10                  // ADC ADC3_IN8 SB147,SB157,SB138,SB143
#define BRD_A5_PIN          GPIO_PIN_10
#define BRD_A5_GPIO_PORT    GPIOF
#define __BRD_A5_GPIO_CLK() __GPIOF_CLK_ENABLE()
#define BRD_A5_EXTI_IRQ     EXTI15_10_IRQn
#define BRD_A5_ADC_CHAN     ADC_CHANNEL_8

#define BRD_A6              1                   // ADC12_IN9 ADC_A_IN
#define BRD_A6_PIN          GPIO_PIN_1
#define BRD_A6_GPIO_PORT    GPIOB
#define __BRD_A6_GPIO_CLK() __GPIOB_CLK_ENABLE()
#define BRD_A6_EXTI_IRQ     EXTI1_IRQn
#define BRD_A6_ADC_CHAN     ADC_CHANNEL_9

#define BRD_A7              2                   // ADC123_IN12 ADC_B_IN#
#define BRD_A7_PIN          GPIO_PIN_2
#define BRD_A7_GPIO_PORT    GPIOC
#define __BRD_A7_GPIO_CLK() __GPIOC_CLK_ENABLE()
#define BRD_A7_EXTI_IRQ     EXTI2_IRQn
#define BRD_A7_ADC_CHAN     ADC_CHANNEL_12

#define BRD_A8              4                   // ADC3_IN14 ADC_C_IN
#define BRD_A8_PIN          GPIO_PIN_4
#define BRD_A8_GPIO_PORT    GPIOF
#define __BRD_A8_GPIO_CLK() __GPIOF_CLK_ENABLE()
#define BRD_A8_EXTI_IRQ     EXTI4_IRQn
#define BRD_A8_ADC_CHAN     ADC_CHANNEL_14

#define BRD_D0              9                   // USART6 USART_A_RX
#define BRD_D0_PIN          GPIO_PIN_9
#define BRD_D0_GPIO_PORT    GPIOG
#define __BRD_D0_GPIO_CLK() __GPIOG_CLK_ENABLE()
#define BRD_D0_EXTI_IRQ     EXTI9_5_IRQn

#define BRD_D1              14                  // USART6 USART_A_TX
#define BRD_D1_PIN          GPIO_PIN_14
#define BRD_D1_GPIO_PORT    GPIOG
#define __BRD_D1_GPIO_CLK() __GPIOG_CLK_ENABLE()
#define BRD_D1_EXTI_IRQ     EXTI15_10_IRQn

#define BRD_D2              15                  // - I/O
#define BRD_D2_PIN          GPIO_PIN_15
#define BRD_D2_GPIO_PORT    GPIOF
#define __BRD_D2_GPIO_CLK() __GPIOF_CLK_ENABLE()
#define BRD_D2_EXTI_IRQ     EXTI15_10_IRQn

#define BRD_D3              13                  // TIM1_CH3 TIMER_A_PWM3
#define BRD_D3_PIN          GPIO_PIN_13
#define BRD_D3_GPIO_PORT    GPIOE
#define __BRD_D3_GPIO_CLK() __GPIOE_CLK_ENABLE()
#define BRD_D3_EXTI_IRQ     EXTI15_10_IRQn

#define BRD_D4              14                  // - I/O
#define BRD_D4_PIN          GPIO_PIN_14
#define BRD_D4_GPIO_PORT    GPIOF
#define __BRD_D4_GPIO_CLK() __GPIOF_CLK_ENABLE()
#define BRD_D4_EXTI_IRQ     EXTI15_10_IRQn

#define BRD_D5              11                  // TIM1_CH2 TIMER_A_PWM2
#define BRD_D5_PIN          GPIO_PIN_11
#define BRD_D5_GPIO_PORT    GPIOE
#define __BRD_D5_GPIO_CLK() __GPIOE_CLK_ENABLE()
#define BRD_D5_EXTI_IRQ     EXTI15_10_IRQn

#define BRD_D6              9                   // TIM1_CH1 TIMER_A_PWM1
#define BRD_D6_PIN          GPIO_PIN_9
#define BRD_D6_GPIO_PORT    GPIOE
#define __BRD_D6_GPIO_CLK() __GPIOE_CLK_ENABLE()
#define BRD_D6_EXTI_IRQ     EXTI9_5_IRQn

#define BRD_D7              13                  // - I/O
#define BRD_D7_PIN          GPIO_PIN_13
#define BRD_D7_GPIO_PORT    GPIOF
#define __BRD_D7_GPIO_CLK() __GPIOF_CLK_ENABLE()
#define BRD_D7_EXTI_IRQ     EXTI15_10_IRQn

#define BRD_D8              12                  // I/O I/O
#define BRD_D8_PIN          GPIO_PIN_12
#define BRD_D8_GPIO_PORT    GPIOF
#define __BRD_D8_GPIO_CLK() __GPIOF_CLK_ENABLE()
#define BRD_D8_EXTI_IRQ     EXTI15_10_IRQn

#define BRD_D9              15                  // TIM4_CH4 TIM_B_PWM2
#define BRD_D9_PIN          GPIO_PIN_15
#define BRD_D9_GPIO_PORT    GPIOD
#define __BRD_D9_GPIO_CLK() __GPIOD_CLK_ENABLE()
#define BRD_D9_EXTI_IRQ     EXTI15_10_IRQn

#define BRD_D10             14                  // SPI1_CS/TIM4_CH3 SPI_A_CS/TIM_B_PWM3
#define BRD_D10_PIN         GPIO_PIN_14
#define BRD_D10_GPIO_PORT   GPIOD
#define __BRD_D10_GPIO_CLK()__GPIOD_CLK_ENABLE()
#define BRD_D10_EXTI_IRQ    EXTI15_10_IRQn

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

#define BRD_D16             6                   // I2S_2 I2S_A_MCK
#define BRD_D16_PIN         GPIO_PIN_6
#define BRD_D16_GPIO_PORT   GPIOC
#define __BRD_D16_GPIO_CLK()__GPIOC_CLK_ENABLE()
#define BRD_D16_EXTI_IRQ    EXTI9_5_IRQn

#define BRD_D17             15                  // I2S_2 I2S_A_SD
#define BRD_D17_PIN         GPIO_PIN_15
#define BRD_D17_GPIO_PORT   GPIOB
#define __BRD_D17_GPIO_CLK()__GPIOB_CLK_ENABLE()
#define BRD_D17_EXTI_IRQ    EXTI15_10_IRQn

#define BRD_D18             13                  // I2S_2 I2S_A_CK JP7 Conflict (ETH RMII_TXD1)
#define BRD_D18_PIN         GPIO_PIN_13
#define BRD_D18_GPIO_PORT   GPIOB
#define __BRD_D18_GPIO_CLK()__GPIOB_CLK_ENABLE()
#define BRD_D18_EXTI_IRQ    EXTI15_10_IRQn

#define BRD_D19             12                  // I2S_2 I2S_A_WS
#define BRD_D19_PIN         GPIO_PIN_12
#define BRD_D19_GPIO_PORT   GPIOB
#define __BRD_D19_GPIO_CLK()__GPIOB_CLK_ENABLE()
#define BRD_D19_EXTI_IRQ    EXTI15_10_IRQn

#define BRD_D20             15                  // I2S_3/SPI3 I2S_B_WS
#define BRD_D20_PIN         GPIO_PIN_15
#define BRD_D20_GPIO_PORT   GPIOA
#define __BRD_D20_GPIO_CLK()__GPIOA_CLK_ENABLE()
#define BRD_D20_EXTI_IRQ    EXTI15_10_IRQn

#define BRD_D21             7                   // I2S_3/SPI3 I2S_B_MCK
#define BRD_D21_PIN         GPIO_PIN_7
#define BRD_D21_GPIO_PORT   GPIOC
#define __BRD_D21_GPIO_CLK()__GPIOC_CLK_ENABLE()
#define BRD_D21_EXTI_IRQ    EXTI9_5_IRQn

#define BRD_D22             5                   // I2S_3/SPI3 I2S_B_SD/SPI_B_MOSI
#define BRD_D22_PIN         GPIO_PIN_5
#define BRD_D22_GPIO_PORT   GPIOB
#define __BRD_D22_GPIO_CLK()__GPIOB_CLK_ENABLE()
#define BRD_D22_EXTI_IRQ    EXTI9_5_IRQn

#define BRD_D23             3                   // I2S_3/SPI3 I2S_B_CK/SPI_B_SCK
#define BRD_D23_PIN         GPIO_PIN_3
#define BRD_D23_GPIO_PORT   GPIOB
#define __BRD_D23_GPIO_CLK()__GPIOB_CLK_ENABLE()
#define BRD_D23_EXTI_IRQ    EXTI3_IRQn

#define BRD_D24             4                   // I2S_3/SPI3 SPI_B_NSS
#define BRD_D24_PIN         GPIO_PIN_4
#define BRD_D24_GPIO_PORT   GPIOA
#define __BRD_D24_GPIO_CLK()__GPIOA_CLK_ENABLE()
#define BRD_D24_EXTI_IRQ    EXTI4_IRQn

#define BRD_D25             4                   // I2S_3/SPI3 SPI_B_MISO
#define BRD_D25_PIN         GPIO_PIN_4
#define BRD_D25_GPIO_PORT   GPIOB
#define __BRD_D25_GPIO_CLK()__GPIOB_CLK_ENABLE()
#define BRD_D25_EXTI_IRQ    EXTI4_IRQn

#define BRD_D26             6                   // I/O I/O
#define BRD_D26_PIN         GPIO_PIN_6
#define BRD_D26_GPIO_PORT   GPIOB
#define __BRD_D26_GPIO_CLK()__GPIOB_CLK_ENABLE()
#define BRD_D26_EXTI_IRQ    EXTI9_5_IRQn

#define BRD_D27             2                   // I/O I/O
#define BRD_D27_PIN         GPIO_PIN_2
#define BRD_D27_GPIO_PORT   GPIOB
#define __BRD_D27_GPIO_CLK()__GPIOB_CLK_ENABLE()
#define BRD_D27_EXTI_IRQ    EXTI2_IRQn

#define BRD_D28             13                  // I/O I/O
#define BRD_D28_PIN         GPIO_PIN_13
#define BRD_D28_GPIO_PORT   GPIOD
#define __BRD_D28_GPIO_CLK()__GPIOD_CLK_ENABLE()
#define BRD_D28_EXTI_IRQ    EXTI15_10_IRQn

#define BRD_D29             12                  // I/O I/O
#define BRD_D29_PIN         GPIO_PIN_12
#define BRD_D29_GPIO_PORT   GPIOD
#define __BRD_D29_GPIO_CLK()__GPIOD_CLK_ENABLE()
#define BRD_D29_EXTI_IRQ    EXTI15_10_IRQn

#define BRD_D30             11                  // I/O I/O
#define BRD_D30_PIN         GPIO_PIN_11
#define BRD_D30_GPIO_PORT   GPIOD
#define __BRD_D30_GPIO_CLK()__GPIOD_CLK_ENABLE()
#define BRD_D30_EXTI_IRQ    EXTI15_10_IRQn

#define BRD_D31             2                   // I/O I/O CN9 Pin 10 and CN10 Pin 25 both connect to PE2. Conflict D56
#define BRD_D31_PIN         GPIO_PIN_2
#define BRD_D31_GPIO_PORT   GPIOE
#define __BRD_D31_GPIO_CLK()__GPIOE_CLK_ENABLE()
#define BRD_D31_EXTI_IRQ    EXTI2_IRQn

#define BRD_D32             0                   // TIM2_CH1 TIMER_C_PWM1
#define BRD_D32_PIN         GPIO_PIN_0
#define BRD_D32_GPIO_PORT   GPIOA
#define __BRD_D32_GPIO_CLK()__GPIOA_CLK_ENABLE()
#define BRD_D32_EXTI_IRQ    EXTI0_IRQn

#define BRD_D33             0                   // TIM3_CH3 TIMER_D_PWM1
#define BRD_D33_PIN         GPIO_PIN_0
#define BRD_D33_GPIO_PORT   GPIOB
#define __BRD_D33_GPIO_CLK()__GPIOB_CLK_ENABLE()
#define BRD_D33_EXTI_IRQ    EXTI0_IRQn

#define BRD_D34             0                   // TIM4_ETR TIMER_B_ETR
#define BRD_D34_PIN         GPIO_PIN_0
#define BRD_D34_GPIO_PORT   GPIOE
#define __BRD_D34_GPIO_CLK()__GPIOE_CLK_ENABLE()
#define BRD_D34_EXTI_IRQ    EXTI0_IRQn

#define BRD_D35             11                  // TIM2_CH4 TIMER_C_PWM3
#define BRD_D35_PIN         GPIO_PIN_11
#define BRD_D35_GPIO_PORT   GPIOB
#define __BRD_D35_GPIO_CLK()__GPIOB_CLK_ENABLE()
#define BRD_D35_EXTI_IRQ    EXTI15_10_IRQn

#define BRD_D36             10                  // TIM2_CH3 TIMER_C_PWM2
#define BRD_D36_PIN         GPIO_PIN_10
#define BRD_D36_GPIO_PORT   GPIOB
#define __BRD_D36_GPIO_CLK()__GPIOB_CLK_ENABLE()
#define BRD_D36_EXTI_IRQ    EXTI15_10_IRQn

#define BRD_D37             15                  // TIM1_BKIN1 TIMER_A_BKIN1
#define BRD_D37_PIN         GPIO_PIN_15
#define BRD_D37_GPIO_PORT   GPIOE
#define __BRD_D37_GPIO_CLK()__GPIOE_CLK_ENABLE()
#define BRD_D37_EXTI_IRQ    EXTI15_10_IRQn

#define BRD_D38             14                  // I/O I/O
#define BRD_D38_PIN         GPIO_PIN_14
#define BRD_D38_GPIO_PORT   GPIOE
#define __BRD_D38_GPIO_CLK()__GPIOE_CLK_ENABLE()
#define BRD_D38_EXTI_IRQ    EXTI15_10_IRQn

#define BRD_D39             12                  // TIM1_CH3N TIMER_A_PWM3N
#define BRD_D39_PIN         GPIO_PIN_12
#define BRD_D39_GPIO_PORT   GPIOE
#define __BRD_D39_GPIO_CLK()__GPIOE_CLK_ENABLE()
#define BRD_D39_EXTI_IRQ    EXTI15_10_IRQn

#define BRD_D40             10                  // TIM1_CH2N TIMER_A_PWM2N
#define BRD_D40_PIN         GPIO_PIN_10
#define BRD_D40_GPIO_PORT   GPIOE
#define __BRD_D40_GPIO_CLK()__GPIOE_CLK_ENABLE()
#define BRD_D40_EXTI_IRQ    EXTI15_10_IRQn

#define BRD_D41             7                   // TIM1_ETR TIMER_A_ETR
#define BRD_D41_PIN         GPIO_PIN_7
#define BRD_D41_GPIO_PORT   GPIOE
#define __BRD_D41_GPIO_CLK()__GPIOE_CLK_ENABLE()
#define BRD_D41_EXTI_IRQ    EXTI9_5_IRQn

#define BRD_D42             8                   // TIM1_CH1N TIMER_A_PWM1N
#define BRD_D42_PIN         GPIO_PIN_8
#define BRD_D42_GPIO_PORT   GPIOE
#define __BRD_D42_GPIO_CLK()__GPIOE_CLK_ENABLE()
#define BRD_D42_EXTI_IRQ    EXTI9_5_IRQn

#define BRD_D43             8                   // SDMMC_D SDMMC/I2S_A
#define BRD_D43_PIN         GPIO_PIN_8
#define BRD_D43_GPIO_PORT   GPIOC
#define __BRD_D43_GPIO_CLK()__GPIOC_CLK_ENABLE()
#define BRD_D43_EXTI_IRQ    EXTI9_5_IRQn

#define BRD_D44             9                   // SDMMC_D1/I2S_A_CKIN I/O
#define BRD_D44_PIN         GPIO_PIN_9
#define BRD_D44_GPIO_PORT   GPIOC
#define __BRD_D44_GPIO_CLK()__GPIOC_CLK_ENABLE()
#define BRD_D44_EXTI_IRQ    EXTI9_5_IRQn

#define BRD_D45             10                  // SDMMC_D2 I/O
#define BRD_D45_PIN         GPIO_PIN_10
#define BRD_D45_GPIO_PORT   GPIOC
#define __BRD_D45_GPIO_CLK()__GPIOC_CLK_ENABLE()
#define BRD_D45_EXTI_IRQ    EXTI15_10_IRQn

#define BRD_D46             11                  // SDMMC_D3 I/O
#define BRD_D46_PIN         GPIO_PIN_11
#define BRD_D46_GPIO_PORT   GPIOC
#define __BRD_D46_GPIO_CLK()__GPIOC_CLK_ENABLE()
#define BRD_D46_EXTI_IRQ    EXTI15_10_IRQn

#define BRD_D47             12                  // SDMMC_CK I/O
#define BRD_D47_PIN         GPIO_PIN_12
#define BRD_D47_GPIO_PORT   GPIOC
#define __BRD_D47_GPIO_CLK()__GPIOC_CLK_ENABLE()
#define BRD_D47_EXTI_IRQ    EXTI15_10_IRQn

#define BRD_D48             2                   // SDMMC_CMD I/O
#define BRD_D48_PIN         GPIO_PIN_2
#define BRD_D48_GPIO_PORT   GPIOD
#define __BRD_D48_GPIO_CLK()__GPIOD_CLK_ENABLE()
#define BRD_D48_EXTI_IRQ    EXTI2_IRQn

#define BRD_D49             2                   // I/O I/O
#define BRD_D49_PIN         GPIO_PIN_2
#define BRD_D49_GPIO_PORT   GPIOG
#define __BRD_D49_GPIO_CLK()__GPIOG_CLK_ENABLE()
#define BRD_D49_EXTI_IRQ    EXTI2_IRQn

#define BRD_D50             3                   // I/O I/O
#define BRD_D50_PIN         GPIO_PIN_3
#define BRD_D50_GPIO_PORT   GPIOG
#define __BRD_D50_GPIO_CLK()__GPIOG_CLK_ENABLE()
#define BRD_D50_EXTI_IRQ    EXTI3_IRQn

#define BRD_D51             7                   // USART_2 USART_B_SCLK
#define BRD_D51_PIN         GPIO_PIN_7
#define BRD_D51_GPIO_PORT   GPIOD
#define __BRD_D51_GPIO_CLK()__GPIOD_CLK_ENABLE()
#define BRD_D51_EXTI_IRQ    EXTI9_5_IRQn

#define BRD_D52             6                   // USART_2 USART_B_RX
#define BRD_D52_PIN         GPIO_PIN_6
#define BRD_D52_GPIO_PORT   GPIOD
#define __BRD_D52_GPIO_CLK()__GPIOD_CLK_ENABLE()
#define BRD_D52_EXTI_IRQ    EXTI9_5_IRQn

#define BRD_D53             5                   // USART_2 USART_B_TX
#define BRD_D53_PIN         GPIO_PIN_5
#define BRD_D53_GPIO_PORT   GPIOD
#define __BRD_D53_GPIO_CLK()__GPIOD_CLK_ENABLE()
#define BRD_D53_EXTI_IRQ    EXTI9_5_IRQn

#define BRD_D54             4                   // USART_2 USART_B_RTS
#define BRD_D54_PIN         GPIO_PIN_4
#define BRD_D54_GPIO_PORT   GPIOD
#define __BRD_D54_GPIO_CLK()__GPIOD_CLK_ENABLE()
#define BRD_D54_EXTI_IRQ    EXTI4_IRQn

#define BRD_D55             3                   // USART_2 USART_B_CTS
#define BRD_D55_PIN         GPIO_PIN_3
#define BRD_D55_GPIO_PORT   GPIOD
#define __BRD_D55_GPIO_CLK()__GPIOD_CLK_ENABLE()
#define BRD_D55_EXTI_IRQ    EXTI3_IRQn

#define BRD_D56             2                   // SAI_1_A SAI_A_MCLK CN9 Pin 10 and CN10 Pin 25 both connect to PE2. Conflict D31
#define BRD_D56_PIN         GPIO_PIN_2
#define BRD_D56_GPIO_PORT   GPIOE
#define __BRD_D56_GPIO_CLK()__GPIOE_CLK_ENABLE()
#define BRD_D56_EXTI_IRQ    EXTI2_IRQn

#define BRD_D57             4                   // SAI_1_A SAI_A_FS
#define BRD_D57_PIN         GPIO_PIN_4
#define BRD_D57_GPIO_PORT   GPIOE
#define __BRD_D57_GPIO_CLK()__GPIOE_CLK_ENABLE()
#define BRD_D57_EXTI_IRQ    EXTI4_IRQn

#define BRD_D58             5                   // SAI_1_A SAI_A_SCK
#define BRD_D58_PIN         GPIO_PIN_5
#define BRD_D58_GPIO_PORT   GPIOE
#define __BRD_D58_GPIO_CLK()__GPIOE_CLK_ENABLE()
#define BRD_D58_EXTI_IRQ    EXTI9_5_IRQn

#define BRD_D59             6                   // SAI_1_A SAI_A_SD
#define BRD_D59_PIN         GPIO_PIN_6
#define BRD_D59_GPIO_PORT   GPIOE
#define __BRD_D59_GPIO_CLK()__GPIOE_CLK_ENABLE()
#define BRD_D59_EXTI_IRQ    EXTI9_5_IRQn

#define BRD_D60             3                   // SAI_1_B SAI_B_SD
#define BRD_D60_PIN         GPIO_PIN_3
#define BRD_D60_GPIO_PORT   GPIOE
#define __BRD_D60_GPIO_CLK()__GPIOE_CLK_ENABLE()
#define BRD_D60_EXTI_IRQ    EXTI3_IRQn

#define BRD_D61             8                   // SAI_1_B SAI_B_SCK
#define BRD_D61_PIN         GPIO_PIN_8
#define BRD_D61_GPIO_PORT   GPIOF
#define __BRD_D61_GPIO_CLK()__GPIOF_CLK_ENABLE()
#define BRD_D61_EXTI_IRQ    EXTI9_5_IRQn

#define BRD_D62             7                   // SAI_1_B SAI_B_MCLK
#define BRD_D62_PIN         GPIO_PIN_7
#define BRD_D62_GPIO_PORT   GPIOF
#define __BRD_D62_GPIO_CLK()__GPIOF_CLK_ENABLE()
#define BRD_D62_EXTI_IRQ    EXTI9_5_IRQn

#define BRD_D63             9                   // SAI_1_B SAI_B_FS
#define BRD_D63_PIN         GPIO_PIN_9
#define BRD_D63_GPIO_PORT   GPIOF
#define __BRD_D63_GPIO_CLK()__GPIOF_CLK_ENABLE()
#define BRD_D63_EXTI_IRQ    EXTI9_5_IRQn

#define BRD_D64             1                   // I/O
#define BRD_D64_PIN         GPIO_PIN_1
#define BRD_D64_GPIO_PORT   GPIOG
#define __BRD_D64_GPIO_CLK()__GPIOG_CLK_ENABLE()
#define BRD_D64_EXTI_IRQ    EXTI1_IRQn

#define BRD_D65             0                   // I/O I/O
#define BRD_D65_PIN         GPIO_PIN_0
#define BRD_D65_GPIO_PORT   GPIOG
#define __BRD_D65_GPIO_CLK()__GPIOG_CLK_ENABLE()
#define BRD_D65_EXTI_IRQ    EXTI0_IRQn

#define BRD_D66             1                   // CAN_1 CAN_TX
#define BRD_D66_PIN         GPIO_PIN_1
#define BRD_D66_GPIO_PORT   GPIOD
#define __BRD_D66_GPIO_CLK()__GPIOD_CLK_ENABLE()
#define BRD_D66_EXTI_IRQ    EXTI1_IRQn

#define BRD_D67             0                   // CAN_1 CAN_RX
#define BRD_D67_PIN         GPIO_PIN_0
#define BRD_D67_GPIO_PORT   GPIOD
#define __BRD_D67_GPIO_CLK()__GPIOD_CLK_ENABLE()
#define BRD_D67_EXTI_IRQ    EXTI0_IRQn

#define BRD_D68             0                   // I2C_2 I2C_B_SDA
#define BRD_D68_PIN         GPIO_PIN_0
#define BRD_D68_GPIO_PORT   GPIOF
#define __BRD_D68_GPIO_CLK()__GPIOF_CLK_ENABLE()
#define BRD_D68_EXTI_IRQ    EXTI0_IRQn

#define BRD_D69             1                   // I2C_2 I2C_B_SCL
#define BRD_D69_PIN         GPIO_PIN_1
#define BRD_D69_GPIO_PORT   GPIOF
#define __BRD_D69_GPIO_CLK()__GPIOF_CLK_ENABLE()
#define BRD_D69_EXTI_IRQ    EXTI1_IRQn

#define BRD_D70             2                   // I2C_2 I2C_B_SMBA
#define BRD_D70_PIN         GPIO_PIN_2
#define BRD_D70_GPIO_PORT   GPIOF
#define __BRD_D70_GPIO_CLK()__GPIOF_CLK_ENABLE()
#define BRD_D70_EXTI_IRQ    EXTI2_IRQn

#define BRD_D71             7                   // I/O I/O JP6 Conflict (ETH RMII_DV)
#define BRD_D71_PIN         GPIO_PIN_7
#define BRD_D71_GPIO_PORT   GPIOA
#define __BRD_D71_GPIO_CLK()__GPIOA_CLK_ENABLE()
#define BRD_D71_EXTI_IRQ    EXTI9_5_IRQn

// #define BRD_D72                                 //NC
// #define BRD_D72_PIN                             //NC
// #define BRD_D72_GPIO_PORT                       //NC
// #define __BRD_D72_GPIO_CLK()                    //NC
// #define BRD_D72_EXTI_IRQ                        //NC


/**********************  BOARD LEDs  *****************************/

//LED definitions (Green - LD0, Blue - LD1, Red - LD2)
#define BRD_GREEN_LED					0	//LD1
#define BRD_GREEN_LEDMASK				1 << BRD_GREEN_LED
#define BRD_GREEN_LED_PIN               GPIO_PIN_0
#define BRD_GREEN_LED_GPIO_PORT         GPIOB
#define __BRD_GREEN_LED_GPIO_CLK()      __GPIOB_CLK_ENABLE()

#define BRD_BLUE_LED					1	//LD2
#define BRD_BLUE_LEDMASK				1 << BRD_BLUE_LED
#define BRD_BLUE_LED_PIN                GPIO_PIN_7
#define BRD_BLUE_LED_GPIO_PORT          GPIOB
#define __BRD_BLUE_LED_GPIO_CLK()       __GPIOB_CLK_ENABLE()

#define BRD_RED_LED						2	//LD3
#define BRD_RED_LEDMASK					1 << BRD_RED_LED
#define BRD_RED_LED_PIN                GPIO_PIN_14
#define BRD_RED_LED_GPIO_PORT          GPIOB
#define __BRD_RED_LED_GPIO_CLK()       __GPIOB_CLK_ENABLE()

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

#define BRD_DEBUG_UART_TX_AF				GPIO_AF7_USART3
#define BRD_DEBUG_UART_TX_PIN				GPIO_PIN_8
#define BRD_DEBUG_UART_TX_GPIO_PORT			GPIOD
#define __BRD_DEBUG_UART_TX_GPIO_CLK()		__GPIOD_CLK_ENABLE()

#define BRD_DEBUG_UART_RX_AF				GPIO_AF7_USART3
#define BRD_DEBUG_UART_RX_PIN				GPIO_PIN_9
#define BRD_DEBUG_UART_RX_GPIO_PORT			GPIOD
#define __BRD_DEBUG_UART_RX_GPIO_CLK()		__GPIOD_CLK_ENABLE()

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

