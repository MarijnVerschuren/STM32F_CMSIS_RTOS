//
// Created by marijn on 4/1/23.
//

#ifndef STM32F_CMSIS_I2C_H
#define STM32F_CMSIS_I2C_H
#include "main.h"
#include "base.h"
#include "gpio.h"
#include "sys.h"


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
	I2C1_SMBA_B5 =		0x550415,
	I2C1_SCL_B6 =		0x550416,
	I2C1_SDA_B7 =		0x550417,
	I2C1_SCL_B8 =		0x550418,
	I2C1_SDA_B9 =		0x550419,
	// I2C2
	I2C2_SCL_B10 =		0x56041A,
	I2C2_SDA_B11 =		0x56041B,
	I2C2_SMBA_B12 =		0x56041C,
	I2C2_SDA_B3 =		0x560913,
	I2C2_SDA_B9 =		0x560919,
	// I2C3
	I2C3_SCL_A8 =		0x570408,
	I2C3_SMBA_A9 =		0x570409,
	I2C3_SDA_C9 =		0x570429,
	I2C3_SDA_B4 =		0x570914,
	I2C3_SDA_B8 =		0x570918,
} I2C_GPIO_t;


/*!< init / enable / disable */
void disable_I2C_clock(I2C_TypeDef* i2c);
void config_I2C(I2C_GPIO_t scl, I2C_GPIO_t sda);
/*!< test */
void I2C_test(void);
/*!< input / output */
uint32_t I2C_write(I2C_TypeDef* i2c, uint8_t i2c_address, const uint8_t* buffer, uint32_t size, uint32_t timeout);
uint32_t I2C_read(I2C_TypeDef* i2c, uint8_t i2c_address, uint8_t* buffer, uint32_t size, uint32_t timeout);
uint32_t I2C_write_reg(I2C_TypeDef* i2c, uint8_t i2c_address, uint8_t reg_address, const uint8_t* buffer, uint32_t size, uint32_t timeout);
uint32_t I2C_read_reg(I2C_TypeDef* i2c, uint8_t i2c_address, uint8_t reg_address, uint8_t* buffer, uint32_t size, uint32_t timeout);
// TODO: functions inspo: https://stm32f4-discovery.net/2014/05/library-09-i2c-for-stm32f4xx/


#endif //STM32F_CMSIS_I2C_H
