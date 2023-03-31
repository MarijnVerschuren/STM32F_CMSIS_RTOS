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


// device id
typedef enum {
	DEV_CLOCK_ID_AHB1 = 0,
	DEV_CLOCK_ID_AHB2 = 1,
	DEV_CLOCK_ID_APB1 = 2,
	DEV_CLOCK_ID_APB2 = 3
} dev_clock_id_t;

typedef struct {
	uint8_t num: 5;
	uint8_t reg: 3;  // dev_clock_id_t
} dev_id_t;

typedef struct {
	dev_id_t dev_id;	// 8 bit
	uint8_t dev_info	: 4;
	uint8_t alt_func	: 4;
	uint8_t port_num	: 4;
	uint8_t pin_num		: 4;
} dev_pin_t;


// TODO: I2S (maybe?)
/*	I2SSRC: I2S clock source:
		0: PLLI2S selected
		1: External clock mapped on the I2S_CKIN pin used as I2S clock */
// (config->I2S_external_clock * RCC_CFGR_I2SSRC)

#endif //STM32F_MAIN_H