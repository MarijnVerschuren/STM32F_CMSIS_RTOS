//
// Created by marijn on 2/13/23.
//
#include "tim.h"


/*!< misc */
uint8_t tim_to_int(TIM_TypeDef* tim) {
	if ((((uint32_t)tim) - APB1PERIPH_BASE) >= 0x00010000UL)	{ return ((uint32_t)(tim - APB2PERIPH_BASE) >> 10u) & 0xfu; }
	else														{ return ((uint32_t)(tim - AHB1PERIPH_BASE) >> 10u) & 0xfu; }
}
/*!< init / disable */
void TIM_init(TIM_TypeDef* tim, uint16_t prescaler, uint32_t limit, uint8_t update_interrupt) {
	if ((((uint32_t)tim) - APB1PERIPH_BASE) >= 0x00010000UL)	{ RCC->APB2ENR |= (0b1u << (((uint32_t)(tim - APB2PERIPH_BASE) >> 10u) & 0xfu)); }
	else														{ RCC->APB1ENR |= (0b1u << (((uint32_t)(tim - AHB1PERIPH_BASE) >> 10u) & 0xfu)); }
	//RCC_TypeDef* ptr = RCC;
	tim->PSC = prescaler;
	tim->ARR = limit;
	if (update_interrupt) { tim->DIER |= TIM_DIER_UIE; }  // enable update interrupt (rollover interrupt)
}
void TIM_disable(TIM_TypeDef* tim) {
	if ((((uint32_t)tim) - APB1PERIPH_BASE) >= 0x00010000UL)	{ RCC->APB2ENR &= ~(0b1u << (((uint32_t)(tim - APB2PERIPH_BASE) >> 10u) & 0xfu)); }
	else														{ RCC->APB1ENR &= ~(0b1u << (((uint32_t)(tim - AHB1PERIPH_BASE) >> 10u) & 0xfu)); }
}
/*!< actions */
void TIM_start(TIM_TypeDef* tim) { tim->CR1 |= TIM_CR1_CEN; }
void TIM_stop(TIM_TypeDef* tim) { tim->CR1 &= ~TIM_CR1_CEN; }
