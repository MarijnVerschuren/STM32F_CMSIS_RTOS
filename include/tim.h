//
// Created by marijn on 2/13/23.
//
#ifndef STM32F_TIM_H
#define STM32F_TIM_H
#include "main.h"


/*!< variables */  // TODO
// extern volatile uint64_t ticks;

/*!< init / disable */
void enable_TIM_clock(TIM_TypeDef* tim, uint32_t prescaler, uint32_t limit, uint8_t update_interrupt);
void disable_TIM_clock(TIM_TypeDef* tim);
/*!< actions */
void TIM_start(TIM_TypeDef* tim);
void TIM_stop(TIM_TypeDef* tim);
/*!< irq */
void start_TIM_update_irq(TIM_TypeDef* tim);
void stop_TIM_update_irq(TIM_TypeDef* tim);
// these are only for TIM1 but this can change on other generations
void start_TIM_capture_compare_irq(TIM_TypeDef* tim);
void stop_TIM_capture_compare_irq(TIM_TypeDef* tim);
void start_TIM_break_irq(TIM_TypeDef* tim);
void stop_TIM_break_irq(TIM_TypeDef* tim);
void start_TIM_trigger_commutation_irq(TIM_TypeDef* tim);
void stop_TIM_trigger_commutation_irq(TIM_TypeDef* tim);


#endif //STM32F_TIM_H
