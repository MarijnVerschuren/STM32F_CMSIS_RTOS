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

#endif //STM32F_MAIN_H
