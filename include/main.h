//
// Created by marijn on 2/13/23.
//

#ifndef STM32F_MAIN_H
#define STM32F_MAIN_H
#include <malloc.h>

#ifdef STM32F3xx
#include "stm32f3xx.h"
#elif defined(STM32F4xx)
#include "stm32f4xx.h"
#endif

#define nullptr ((void*)0)
#define __weak __attribute__((weak))

/* TODO: make a system to split simple and full config
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
*/
// TODO: update function with the following source:
// https://controllerstech.com/how-to-setup-uart-using-registers-in-stm32/

// TODO: make it possible to disable the PLL and HSE

/*  TODO: format these in their respective header files like so: port_num: 4 | pin_num: 4 | af: 4
typedef enum {
	// SYS
	SYS_AF =		0x0,
	// TIM1 TIM2
	TIM1_AF =		0x1,
	TIM2_AF =		0x1,
	// TIM3 TIM4 TIM5
	TIM3_AF =		0x2,
	TIM4_AF =		0x2,
	TIM5_AF =		0x2,
	// TIM9 TIM10 TIM11
	TIM9_AF =		0x3,
	TIM10_AF =		0x3,
	TIM11_AF =		0x3,
	// I2C1 I2C2 I2C3
	I2C1_AF =		0x4,
	I2C2_AF =		0x4,
	I2C3_AF =		0x4,
	// SPI1 I2S1_SPI2 I2S2 SPI3 I2S3
	SPI1_AF =		0x5,
	SPI2_AF =		0x5,
	SPI3_AF =		0x5,
	I2S2_AF =		0x5,
	I2S3_AF =		0x5,
	I2S1_SPI2_AF =	0x5,
	// SPI2 I2S2 SPI3 I2S3 SPI4 I2S4 SPI5 I2S5
	SPI2_AF2 =		0x6,
	SPI3_AF2 =		0x6,
	SPI4_AF =		0x6,
	SPI5_AF =		0x6,
	I2S2_AF2 =		0x6,
	I2S3_AF2 =		0x6,
	I2S4_AF =		0x6,
	I2S5_AF =		0x6,
	// SPI3 I2S3 USART1 USART2
	SPI3_AF3 =		0x7,
	I2S3_AF3 =		0x7,
	USART1_AF =		0x7,
	USART2_AF =		0x7,
	// USART6
	USART6_AF =		0x8,
	// I2C2 I2C3
	I2C2_AF2 =		0x9,
	I2C3_AF2 =		0x9,
	// USB_OTG_FS
	USB_OTG_FS_AF =	0xA,
	// SDIO
	SDIO_AF =		0xC
} GPIO_ALT_FUNCTION_TypeDef;
 */

/* TODO: I2S (maybe?)
				I2SSRC: I2S clock source:
					0: PLLI2S selected
					1: External clock mapped on the I2S_CKIN pin used as I2S clock										*/
// (config->I2S_external_clock * RCC_CFGR_I2SSRC)								|

#endif //STM32F_MAIN_H