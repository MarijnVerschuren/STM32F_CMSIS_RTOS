//
// Created by marijn on 2/13/23.
//
#include "gpio.h"


/*!< misc */
uint8_t port_to_int(GPIO_TypeDef* port) {
	return ((uint32_t)(port - AHB1PERIPH_BASE) >> 10u) & 0x7u;
}
GPIO_TypeDef* int_to_port(uint8_t num) {
	return (GPIO_TypeDef*)(((num & 0x7u) << 10) + AHB1PERIPH_BASE);
}
/*!< init / disable */
void enable_GPIO_port_clock(GPIO_TypeDef* port) {
	#ifdef STM32F3xx
	uint8_t pos = 16 + ((port_to_int(port) + 1) % 8);
	RCC->AHBENR |= 0b1u << pos;
	#elif defined(STM32F4xx)
	RCC->AHB1ENR |= 0b1u << port_to_int(port);
	#endif
}
void disable_GPIO_port_clock(GPIO_TypeDef* port) {
	#ifdef STM32F3xx
	uint8_t pos = 16 + ((port_to_int(port) + 1) % 8);
	RCC->AHBENR &= ~(0b1u << pos);
	#elif defined(STM32F4xx)
	RCC->AHB1ENR &= ~(0b1u << port_to_int(port));
	#endif
}
/* TODO
void lock_pin_config(uint8_t pin, GPIO_TypeDef* port) {
}
void unlock_pin_config(uint8_t pin, GPIO_TypeDef* port) {
}*/
void reset_pin_config(GPIO_TypeDef* port, uint8_t pin) {
	port->MODER &= ~(0b11u << (pin << 1u));
	port->OSPEEDR &= ~(0b11u << (pin << 1u));
	port->PUPDR &= ~(0b11u << (pin << 1u));
	port->OTYPER &= (0b1u << pin);
}
void config_pin(GPIO_TypeDef* port, uint8_t pin, GPIO_MODE_TypeDef mode, GPIO_SPEED_TypeDef speed, GPIO_PULL_TypeDef pull, GPIO_OT_TypeDef out_type) {
	reset_pin_config(port, pin);
	port->MODER |= (mode << (pin << 1u));
	port->OSPEEDR |= (speed << (pin << 1u));
	port->PUPDR |= (pull << (pin << 1u));
	port->OTYPER |= out_type << pin;
}
void config_pin_alt(GPIO_TypeDef* port, uint8_t pin, uint8_t alternate_function) {
	uint8_t pos = ((pin & 0x7) << 2);
	port->MODER |= (GPIO_alt_func<< (pin << 1u));					// let the rest of the config as is
	port->AFR[pin >> 3] &= ~(0xf << pos);							// clear AFR entry
	port->AFR[pin >> 3] |= ((alternate_function & 0xf) << pos);		// set AFR entry
}
/*!< output */
void write_pin(GPIO_TypeDef* port, uint8_t pin, uint8_t data) {
	port->BSRR |= (1u << (pin << !data));
}
void toggle_pin(GPIO_TypeDef* port, uint8_t pin) {
	port->ODR ^= (1u << pin);
}
/*!< input */
uint8_t read_pin(GPIO_TypeDef* port, uint8_t pin) {
	return (port->IDR >> pin) & 1u;
}