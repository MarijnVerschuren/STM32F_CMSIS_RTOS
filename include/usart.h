//
// Created by marijn on 3/20/23.
//

#ifndef STM32F_CMSIS_USART_H
#define STM32F_CMSIS_USART_H
#include "main.h"
#include "gpio.h"
#include "sys.h"


/* data layout:
 * struct:
 *     alternate_function_number	: 4;  // MSB
 *     port_number					: 4;
 *     pin_number					: 4;  // LSB
 * */
typedef enum {
	USART_PIN_DISABLE =	0x000,
	// USART1
	USART1_CLK_A8 =		0x708,
	USART1_TX_A9 =		0x709,
	USART1_RX_A10 =		0x70A,
	USART1_CTS_A11 =	0x70B,
	USART1_RTS_A12 =	0x70C,
	USART1_TX_A15 =		0x70F,
	USART1_RX_B3 =		0x713,
	USART1_TX_B6 =		0x716,
	USART1_RX_B7 =		0x717,
	// USART2
	USART2_CTS_A0 =		0x700,
	USART2_RTS_A1 =		0x701,
	USART2_TX_A2 =		0x702,
	USART2_RX_A3 =		0x703,
	USART2_CLK_A4 =		0x704,
	USART2_CTS_D3 =		0x733,
	USART2_RTS_D4 =		0x734,
	USART2_TX_D5 =		0x735,
	USART2_RX_D6 =		0x736,
	USART2_CLK_D7 =		0x737,
	// USART6
	USART6_TX_A11 =		0x80B,
	USART6_RX_A12 =		0x80C,
	USART6_TX_C6 =		0x826,
	USART6_RX_C7 =		0x827,
	USART6_CLK_C8 =		0x828
} USART_GPIO_TypeDef;
typedef enum {
	USART_OVERSAMPLING_16 =	0,
	USART_OVERSAMPLING_8 =	1,
} USART_oversampling_TypeDef;


/*!< init / enable / disable */
void disable_USART(USART_TypeDef* usart);
void fconfig_UART(USART_TypeDef* uart, uint32_t baud, USART_GPIO_TypeDef tx, USART_GPIO_TypeDef rx, USART_oversampling_TypeDef oversampling);
void config_UART(USART_TypeDef* uart, uint32_t baud, USART_GPIO_TypeDef tx, USART_GPIO_TypeDef rx);
/*!< input / output */
uint8_t USART_transmit(USART_TypeDef* usart, uint8_t byte, uint32_t timeout);
uint8_t USART_receive(USART_TypeDef* usart, uint8_t* byte, uint32_t timeout);
uint32_t USART_write(USART_TypeDef* usart, const uint8_t* buffer, uint32_t size, uint32_t timeout);
uint32_t USART_read(USART_TypeDef* usart, uint8_t* buffer, uint32_t size, uint32_t timeout);
uint8_t USART_print(USART_TypeDef* usart, char* str, uint32_t timeout);

#endif //STM32F_CMSIS_USART_H
