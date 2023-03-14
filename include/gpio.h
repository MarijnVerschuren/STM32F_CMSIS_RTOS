//
// Created by marijn on 2/13/23.
//
#ifndef STM32F_GPIO_H
#define STM32F_GPIO_H
#include "main.h"



typedef enum {
	GPIO_input =		0b00,
	GPIO_output =		0b01,
	GPIO_alt_func =		0b10,
	GPIO_analog =		0b11
} GPIO_MODE_TypeDef;
typedef enum {
	GPIO_low_speed =	0b00,	// 0bx0
	GPIO_medium_speed =	0b01,
	GPIO_high_speed =	0b11
} GPIO_SPEED_TypeDef;
typedef enum {
	GPIO_no_pull =		0b00,
	GPIO_pull_up =		0b01,
	GPIO_pull_down =	0b10,
	GPIO_reserved =		0x11
} GPIO_PULL_TypeDef;

/*!< misc */
uint8_t port_to_int(GPIO_TypeDef* port);
/*!< init / disable */
void GPIO_port_init(GPIO_TypeDef* port);
void GPIO_port_disable(GPIO_TypeDef* port);
void set_pin_mode(uint8_t pin, GPIO_TypeDef* port, GPIO_MODE_TypeDef mode);
void set_pin_config(uint8_t pin, GPIO_TypeDef* port, GPIO_SPEED_TypeDef speed, GPIO_PULL_TypeDef pull);
void pin_init(uint8_t pin, GPIO_TypeDef* port, GPIO_MODE_TypeDef mode, GPIO_SPEED_TypeDef speed, GPIO_PULL_TypeDef pull);
/*!< output */
void write_pin(uint8_t pin, GPIO_TypeDef* port, uint8_t data);
void toggle_pin(uint8_t pin, GPIO_TypeDef* port);
/*!< input */
uint8_t read_pin(uint8_t pin, GPIO_TypeDef* port);

#endif //STM32F_GPIO_H
