//
// Created by marijn on 3/20/23.
//

#ifndef STM32F_CMSIS_USART_H
#define STM32F_CMSIS_USART_H
#include "main.h"
#include "gpio.h"


/*!< init */
void enable_USART_clock(USART_TypeDef* usart);
void disable_USART_clock(USART_TypeDef* usart);
void config_USART(USART_TypeDef* usart);  // TODO: https://mcturra2000.wordpress.com/2021/11/19/using-cmsis-to-read-write-on-uart-on-stm32f4/
/*!< enable / disable */



#endif //STM32F_CMSIS_USART_H
