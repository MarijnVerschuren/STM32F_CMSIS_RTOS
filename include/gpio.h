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
	GPIO_low_speed =		0b00,
	GPIO_medium_speed =		0b01,
	GPIO_high_speed =		0b10,
	GPIO_very_high_speed =	0b11
} GPIO_SPEED_TypeDef;
typedef enum {
	GPIO_no_pull =		0b00,
	GPIO_pull_up =		0b01,
	GPIO_pull_down =	0b10,
	GPIO_reserved =		0x11
} GPIO_PULL_TypeDef;
typedef enum {
	GPIO_push_pull =	0b0,
	GPIO_open_drain =	0b1
} GPIO_OT_TypeDef;


/*!< misc */
uint8_t port_to_int(GPIO_TypeDef* port);
GPIO_TypeDef* int_to_port(uint8_t num);
/*!< init / enable / disable */
// TODO
//void lock_pin_config(uint8_t pin, GPIO_TypeDef* port);
//void unlock_pin_config(uint8_t pin, GPIO_TypeDef* port);
void enable_GPIO_port_clock(GPIO_TypeDef* port);
void disable_GPIO_port_clock(GPIO_TypeDef* port);
void reset_pin_config(GPIO_TypeDef* port, uint8_t pin);
void config_pin(GPIO_TypeDef* port, uint8_t pin, GPIO_MODE_TypeDef mode, GPIO_SPEED_TypeDef speed, GPIO_PULL_TypeDef pull, GPIO_OT_TypeDef out_type);
void config_pin_alt(GPIO_TypeDef* port, uint8_t pin, uint8_t alternate_function);
/*!< output */
void write_pin(GPIO_TypeDef* port, uint8_t pin, uint8_t data);
void toggle_pin(GPIO_TypeDef* port, uint8_t pin);
/*!< input */
uint8_t read_pin(GPIO_TypeDef* port, uint8_t pin);

#endif //STM32F_GPIO_H
