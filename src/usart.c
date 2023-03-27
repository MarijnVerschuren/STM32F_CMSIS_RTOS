//
// Created by marijn on 3/20/23.
//
#include "usart.h"


/*!< static */
static uint16_t UART_division(USART_TypeDef* uart, uint32_t baud) {
	if ((((uint32_t)uart) ^ APB1PERIPH_BASE) > 0x00010000UL)	{ return APB2_clock_frequency / baud; }
	else														{ return APB1_clock_frequency / baud; }
}
static void enable_USART_clock(USART_TypeDef* usart) {
	if ((((uint32_t)usart) ^ APB1PERIPH_BASE) > 0x00010000UL)	{ RCC->APB2ENR |= (0b1u << (((uint32_t)usart ^ APB2PERIPH_BASE) >> 10u)); }
	else														{ RCC->APB1ENR |= (0b1u << (((uint32_t)usart ^ APB1PERIPH_BASE) >> 10u)); }
}


/*!< init / enable / disable */
void disable_USART(USART_TypeDef* usart) {
	if ((((uint32_t)usart) ^ APB1PERIPH_BASE) > 0x00010000UL)	{ RCC->APB2ENR &= ~(0b1u << (((uint32_t)usart ^ APB2PERIPH_BASE) >> 10u)); }
	else														{ RCC->APB1ENR &= ~(0b1u << (((uint32_t)usart ^ APB1PERIPH_BASE) >> 10u)); }
}
void config_UART(USART_TypeDef* uart, uint32_t baud, USART_GPIO_TypeDef tx, USART_GPIO_TypeDef rx) {
	uint8_t tx_af = (tx >> 8);
	uint8_t rx_af = (rx >> 8);
	// arguments passed to the int_to_port function are filtered to be <= 0x7
	GPIO_TypeDef* tx_port = int_to_GPIO(tx >> 4);
	GPIO_TypeDef* rx_port = int_to_GPIO(rx >> 4);
	tx &= 0xf; rx &= 0xf;
	// config pins
	config_GPIO(tx_port, tx, GPIO_alt_func, GPIO_very_high_speed, GPIO_no_pull, GPIO_push_pull, tx_af);
	config_GPIO(rx_port, rx, GPIO_alt_func, GPIO_very_high_speed, GPIO_no_pull, GPIO_push_pull, rx_af);
	// config uart registers
	uint16_t uart_div = UART_division(uart, baud);
	//uart_div *= over8 + 1;   // TODO: is this needed for oversampling 8? (default=16)

	enable_USART_clock(uart);

	uart->BRR = (
		((uart_div / 16) << USART_BRR_DIV_Mantissa_Pos ) |
		((uart_div % 16) << USART_BRR_DIV_Fraction_Pos )
	);
	/*
	uart->BRR = (
		((uart_div / 16) << USART_BRR_DIV_Mantissa_Pos ) |
		((uart_div % (16 >> over8)) << USART_BRR_DIV_Fraction_Pos )
	);*/
	uart->CR1 = (
		USART_CR1_RE |
		USART_CR1_TE |
		USART_CR1_UE
	);
}


/*!< input / output */
uint8_t USART_transmit(USART_TypeDef* usart, uint8_t byte, uint32_t timeout) {
	uint64_t start = tick;
	while (!(usart->SR & USART_SR_TXE)) {
		if (tick - start > timeout) {
			return 1;
		}
	}
	usart->DR = byte; return 0;
}
uint8_t USART_receive(USART_TypeDef* usart, uint8_t* byte, uint32_t timeout) {
	uint64_t start = tick;
	while(!(usart->SR & USART_SR_RXNE)) { if ( tick - start > timeout) { return 1; } }
	*byte = usart->DR; return 0;
}
uint32_t USART_write(USART_TypeDef* usart, const uint8_t* buffer, uint32_t size, uint32_t timeout) {
	uint64_t start = tick;
	for (uint32_t i = 0; i < size; i++) {
		while (!(usart->SR & USART_SR_TXE)) { if ( tick - start > timeout) { return i; } }
		usart->DR = buffer[i];
	} return size;
}
uint32_t USART_read(USART_TypeDef* usart, uint8_t* buffer, uint32_t size, uint32_t timeout) {
	uint64_t start = tick;
	for (uint32_t i = 0; i < size; i++) {
		while (!(usart->SR & USART_SR_RXNE)) { if ( tick - start > timeout) { return i; } }
		buffer[i] = usart->DR;
	} return size;
}
uint8_t USART_print(USART_TypeDef* usart, char* str, uint32_t timeout) {
	uint64_t start = tick;
	while (*str) {
		while (!(usart->SR & USART_SR_TXE)) { if ( tick - start > timeout) { return 1; } }
		usart->DR = *str++;
	}
	return 0;
}
