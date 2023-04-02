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


#define __weak __attribute__((weak))


// TODO: I2S (maybe?)
/*	I2SSRC: I2S clock source:
		0: PLLI2S selected
		1: External clock mapped on the I2S_CKIN pin used as I2S clock */
// (config->I2S_external_clock * RCC_CFGR_I2SSRC)

#endif //STM32F_MAIN_H