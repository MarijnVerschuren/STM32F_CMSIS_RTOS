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



/*typedef enum {
	PERIPHERAL_REG_AHB1 = 0,
	PERIPHERAL_REG_AHB2 = 1,
	PERIPHERAL_REG_APB1 = 2,
	PERIPHERAL_REG_APB2 = 3
} peripheral_reg_t;

typedef struct {
	uint8_t num: 5;
	uint8_t reg: 3;
} peripheral_id_t;

typedef struct {
	peripheral_id_t peripheral_id;
	uint8_t alt_func:	4;
	uint8_t port_num:	4;
	uint8_t pin_num:	4;
	uint8_t _:			4;
} peripheral_pin_t;


peripheral_id_t TIM_to_id(TIM_TypeDef* tim) {
	if ((((uint32_t)tim) - APB1PERIPH_BASE) >= 0x00010000UL) {
		return (peripheral_id_t){(uint32_t)(tim - APB2PERIPH_BASE) >> 10u, PERIPHERAL_REG_APB2};
	}	return (peripheral_id_t){(uint32_t)(tim - AHB1PERIPH_BASE) >> 10u, PERIPHERAL_REG_APB1};
}
TIM_TypeDef* id_to_TIM(peripheral_id_t id) {
	if (id.reg == PERIPHERAL_REG_APB2) {
		return (TIM_TypeDef*)((id.num << 10u) + APB2PERIPH_BASE);
	} else if (id.reg != PERIPHERAL_REG_APB1) { return nullptr; }
	return (TIM_TypeDef*)((id.num << 10u) + APB1PERIPH_BASE);
}

peripheral_id_t USART_to_id(USART_TypeDef* usart) {
	if ((((uint32_t)usart) ^ APB1PERIPH_BASE) > 0x00010000UL) {
		return (peripheral_id_t){(uint32_t)(usart - APB2PERIPH_BASE) >> 10u, PERIPHERAL_REG_APB2};
	}	return (peripheral_id_t){(uint32_t)(usart - APB1PERIPH_BASE) >> 10u, PERIPHERAL_REG_APB1};
}
USART_TypeDef* id_to_USART(peripheral_id_t id) {
	if (id.reg == PERIPHERAL_REG_APB2) {
		return (USART_TypeDef*)((id.num << 10) + APB2PERIPH_BASE);
	} else if (id.reg != PERIPHERAL_REG_APB1) { return nullptr; }
	return (USART_TypeDef*)((id.num << 10) + APB1PERIPH_BASE);
}

*/



// TODO: insert USART and TIM number into the GPIO typedefs

/* TODO: I2S (maybe?)
				I2SSRC: I2S clock source:
					0: PLLI2S selected
					1: External clock mapped on the I2S_CKIN pin used as I2S clock										*/
// (config->I2S_external_clock * RCC_CFGR_I2SSRC)								|

#endif //STM32F_MAIN_H