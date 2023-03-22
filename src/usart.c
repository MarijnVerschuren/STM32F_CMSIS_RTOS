//
// Created by marijn on 3/20/23.
//
#include "usart.h"


/*!< init */
void enable_USART_clock(USART_TypeDef* usart) {
	if ((((uint32_t)usart) - APB1PERIPH_BASE) > 0x00010000UL)	{ RCC->APB2ENR |= (0b1u << ((uint32_t)(usart - APB2PERIPH_BASE) >> 10u)); }
	else														{ RCC->APB1ENR |= (0b1u << ((uint32_t)(usart - AHB1PERIPH_BASE) >> 10u)); }
}
void disable_USART_clock(USART_TypeDef* usart) {
	if ((((uint32_t)usart) - APB1PERIPH_BASE) > 0x00010000UL)	{ RCC->APB2ENR &= ~(0b1u << ((uint32_t)(usart - APB2PERIPH_BASE) >> 10u)); }
	else														{ RCC->APB1ENR &= ~(0b1u << ((uint32_t)(usart - AHB1PERIPH_BASE) >> 10u)); }
}
void config_USART(USART_TypeDef* usart, USART_GPIO_TypeDef rx, USART_GPIO_TypeDef tx) {
	uint8_t rx_af = (rx >> 8);
	uint8_t tx_af = (tx >> 8);
	// arguments passed to the int_to_port function are filtered to be <= 0x7
	GPIO_TypeDef* rx_port = int_to_port(rx >> 4);
	GPIO_TypeDef* tx_port = int_to_port(tx >> 4);
	rx &= 0xf; tx &= 0xf;

}