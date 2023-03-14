//
// Created by marijn on 2/13/23.
//
#include "exti.h"


void EXTI_init(void) {
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
}

void enable_EXTI(uint8_t EXTI_line, GPIO_TypeDef* EXTI_port, uint8_t falling_edge, uint8_t rising_edge) {
	EXTI_line &= 0xfu;  // only allow values upto 15
	uint8_t pos = (EXTI_line & 0x3u);  // index in the register [0:3]
	SYSCFG->EXTICR[EXTI_line >> 2u] &= ~(0xfu << (pos << 2));  // clear EXTI port in the register
	SYSCFG->EXTICR[EXTI_line >> 2u] |= port_to_int(EXTI_port) << (pos << 2);  // set EXTI port in the register
	// set triggers
	EXTI->FTSR |= ((falling_edge & 0b1u) << EXTI_line);
	EXTI->RTSR |= ((rising_edge & 0b1u) << EXTI_line);
	EXTI->IMR |= (0b1u << EXTI_line);  // unmask interrupt
}