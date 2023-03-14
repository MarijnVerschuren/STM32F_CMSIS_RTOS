//
// Created by marijn on 2/13/23.
//
#ifndef STM32F_TIM_H
#define STM32F_TIM_H
#include "main.h"


/*!< misc */
uint8_t tim_to_int(TIM_TypeDef* tim);
/*!< init / disable */
void TIM_init(TIM_TypeDef* tim, uint16_t prescaler, uint32_t limit, uint8_t update_interrupt);
void TIM_disable(TIM_TypeDef* tim);
/*!< actions */
void TIM_start(TIM_TypeDef* tim);
void TIM_stop(TIM_TypeDef* tim);


#endif //STM32F_TIM_H
