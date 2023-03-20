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
void config_USART(USART_TypeDef* usart) {

}
