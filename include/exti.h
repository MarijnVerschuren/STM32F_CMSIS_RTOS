//
// Created by marijn on 2/13/23.
//
#ifndef STM32F_EXTI_H
#define STM32F_EXTI_H
#include "main.h"
#include "gpio.h"


/*!< init */
void EXTI_init(void);
/*!< enable / disable */
void enable_EXTI(uint8_t EXTI_line, GPIO_TypeDef* EXTI_port, uint8_t falling_edge, uint8_t rising_edge);
void start_EXTI(uint8_t EXTI_line);
void stop_EXTI(uint8_t EXTI_line);

#endif //STM32F_EXTI_H
