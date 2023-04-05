//
// Created by marijn on 4/1/23.
//

#ifndef STM32F_CMSIS_I2C_H
#define STM32F_CMSIS_I2C_H
#include "main.h"

/* data layout:
 * struct:
 *		dev_id						: 8;  // MSB
 *			num							: 5;	// MSB
 *			clock						: 3;	// LSB
 *		misc						: 4;  // (reserved)
 *		alternate_function_number	: 4;
 *		port_number					: 4;
 *		pin_number					: 4;  // LSB
 * */
typedef enum {
	I2C_PIN_DISABLE =	0x000000,
	// I2C1
	I2C1_SMBA_B5 =		0x000415,
	I2C1_SCL_B6 =		0x000416,
	I2C1_SDA_B7 =		0x000417,
	I2C1_SCL_B8 =		0x000418,
	I2C1_SDA_B9 =		0x000419,
	// I2C2
	I2C2_SCL_B10 =		0x00041A,
	I2C2_SDA_B11 =		0x00041B,
	I2C2_SMBA_B12 =		0x00041C,
	I2C2_SDA_B3 =		0x000913,
	I2C2_SDA_B9 =		0x000919,
	// I2C3
	I2C3_SCL_A8 =		0x000408,
	I2C3_SMBA_A9 =		0x000409,
	I2C3_SDA_C9 =		0x000429,
	I2C3_SDA_B4 =		0x000914,
	I2C3_SDA_B8 =		0x000918,
} I2C_GPIO_t;

#endif //STM32F_CMSIS_I2C_H
