//
// Created by marijn on 2/13/23.
//
#ifndef STM32F_TIM_H
#define STM32F_TIM_H
#include "main.h"
#include "base.h"


/*!< misc */
dev_id_t TIM_to_id(TIM_TypeDef* tim);
TIM_TypeDef* id_to_TIM(dev_id_t id);
/*!< init / enable / disable */
void config_TIM(TIM_TypeDef* tim, uint32_t prescaler, uint32_t limit);
void disable_TIM(TIM_TypeDef* tim);
/*!< actions */
void start_TIM(TIM_TypeDef* tim);
void stop_TIM(TIM_TypeDef* tim);
/*!< irq */
void start_TIM_update_irq(TIM_TypeDef* tim);
void stop_TIM_update_irq(TIM_TypeDef* tim);
void start_TIM_capture_compare_irq(TIM_TypeDef* tim);
void stop_TIM_capture_compare_irq(TIM_TypeDef* tim);
void start_TIM_break_irq(TIM_TypeDef* tim);
void stop_TIM_break_irq(TIM_TypeDef* tim);
void start_TIM_trigger_commutation_irq(TIM_TypeDef* tim);
void stop_TIM_trigger_commutation_irq(TIM_TypeDef* tim);


#endif //STM32F_TIM_H
