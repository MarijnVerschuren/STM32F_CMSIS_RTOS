//
// Created by marijn on 4/2/23.
//

#ifndef STM32F_CMSIS_DEV_H
#define STM32F_CMSIS_DEV_H
#include "main.h"


// device id
typedef enum {
	DEV_CLOCK_ID_AHB1 = 0,
	DEV_CLOCK_ID_AHB2 = 1,
	DEV_CLOCK_ID_APB1 = 2,
	DEV_CLOCK_ID_APB2 = 3,
	DEV_CLOCK_INVALID = 4,
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


dev_id_t dev_to_id(void* dev);
void* id_to_dev(dev_id_t id);


#endif //STM32F_CMSIS_DEV_H
