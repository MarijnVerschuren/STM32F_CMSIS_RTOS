//
// Created by marijn on 3/20/23.
//
#include "usart.h"


/*!< static */
static inline uint16_t UART_division(USART_TypeDef* uart, uint32_t baud) {
	if ((((uint32_t)uart) ^ APB1PERIPH_BASE) > 0x00010000UL)	{ return APB2_clock_frequency / baud; }
	else														{ return APB1_clock_frequency / baud; }
}
static inline void enable_USART_clock(USART_TypeDef* usart) {
	if ((((uint32_t)usart) ^ APB1PERIPH_BASE) > 0x00010000UL)	{ RCC->APB2ENR |= (0b1u << (((uint32_t)usart - APB2PERIPH_BASE) >> 10u)); }
	else														{ RCC->APB1ENR |= (0b1u << (((uint32_t)usart - APB1PERIPH_BASE) >> 10u)); }
}
static inline void USART_GPIO_to_args(USART_GPIO_TypeDef usart_pin, USART_TypeDef** usart, uint8_t* alternate_function, GPIO_TypeDef** port, uint8_t* pin) {
	uint8_t dev_id =		(usart_pin >> 16);
	(*usart) =				id_to_dev(*((dev_id_t*)&dev_id));
	(*alternate_function) =	(usart_pin >> 8) & 0xfu;
	(*port) =				int_to_GPIO(usart_pin >> 4);
	(*pin) =				usart_pin & 0xfu;
}


/*!< irq vars */
typedef struct {
	uint8_t* rx_buf;
	uint32_t rx_size;
	uint32_t rx_index;
	uint32_t rx_fifo;
} USART_buf_Typedef;

USART_buf_Typedef usart_buf_1;
USART_buf_Typedef usart_buf_2;
USART_buf_Typedef usart_buf_6;


/*!< irq handlers */
void USART_irq_handler(USART_TypeDef* usart, USART_buf_Typedef* buf) {
	if(usart->SR & USART_SR_RXNE && buf->rx_buf) {
		buf->rx_buf[buf->rx_index] = usart->DR;
		buf->rx_index += 1;
		if (buf->rx_index >= buf->rx_size) {
			if (buf->rx_fifo) { buf->rx_index = 0; return; }	// reset offset
			usart->CR1 &= ~USART_CR1_RXNEIE;					// turn off irq
		}
	}
	// TODO: other events
}

void USART1_IRQHandler(void) { USART_irq_handler(USART1, &usart_buf_1); }
void USART2_IRQHandler(void) { USART_irq_handler(USART2, &usart_buf_2); }
void USART6_IRQHandler(void) { USART_irq_handler(USART6, &usart_buf_6); }


/*!< init / enable / disable */
void disable_USART(USART_TypeDef* usart) {
	if ((((uint32_t)usart) ^ APB1PERIPH_BASE) > 0x00010000UL)	{ RCC->APB2ENR &= ~(0b1u << (((uint32_t)usart - APB2PERIPH_BASE) >> 10u)); }
	else														{ RCC->APB1ENR &= ~(0b1u << (((uint32_t)usart - APB1PERIPH_BASE) >> 10u)); }
}

void fconfig_UART(USART_GPIO_TypeDef tx, USART_GPIO_TypeDef rx, uint32_t baud, USART_oversampling_TypeDef oversampling) {
	uint8_t			tx_enable = tx != USART_PIN_DISABLE, rx_enable = rx != USART_PIN_DISABLE;
	if (!tx_enable && !rx_enable) { return; }  // quit when no pins are enabled
	USART_TypeDef	*tx_uart = NULL,	*rx_uart = NULL,	*uart = NULL;
	GPIO_TypeDef 	*tx_port = NULL,	*rx_port = NULL;
	uint8_t			tx_pin = 0,	rx_pin = 0,	tx_af = 0, rx_af = 0;
	if (tx_enable) { USART_GPIO_to_args(tx, &tx_uart, &tx_af, &tx_port, &tx_pin); uart = tx_uart; }
	if (rx_enable) { USART_GPIO_to_args(rx, &rx_uart, &rx_af, &rx_port, &rx_pin); uart = rx_uart; }
	if (tx_enable && rx_enable && tx_uart != rx_uart) { return; }  // error if tx and rx are on different usart devices

	// config pins
	fconfig_GPIO(tx_port, tx, GPIO_alt_func, GPIO_no_pull, GPIO_push_pull, GPIO_very_high_speed, tx_af);
	fconfig_GPIO(rx_port, rx, GPIO_alt_func, GPIO_no_pull, GPIO_push_pull, GPIO_very_high_speed, rx_af);

	// config uart registers
	uint16_t uart_div = UART_division(uart, baud) * (oversampling + 1);
	enable_USART_clock(uart);
	uart->BRR = ((uart_div & 0xfff0) | ((uart_div & 0xf) >> oversampling));
	uart->CR1 = (
		(USART_CR1_TE * tx_enable) |
		(USART_CR1_RE * rx_enable) |
		USART_CR1_UE
	);
}

void config_UART(USART_GPIO_TypeDef tx, USART_GPIO_TypeDef rx, uint32_t baud) { fconfig_UART(tx, rx, baud, USART_OVERSAMPLING_16); }


/*!< irq */
void start_USART_receive_irq(USART_TypeDef* usart, uint8_t* buffer, uint32_t size, uint8_t fifo) {
	usart->CR1 |= USART_CR1_RXNEIE;
	uint32_t irqn;
	USART_buf_Typedef* usart_buf;
	if (usart == USART1)		{ irqn = USART1_IRQn; usart_buf = &usart_buf_1; }
	else if (usart == USART2)	{ irqn = USART2_IRQn; usart_buf = &usart_buf_2; }
	else if (usart == USART6)	{ irqn = USART6_IRQn; usart_buf = &usart_buf_6; }
	else { return; }  // error
	usart_buf->rx_buf = buffer;	usart_buf->rx_size = size;
	usart_buf->rx_index = 0;	usart_buf->rx_fifo = fifo;
	NVIC->ISER[((irqn) >> 5UL)] = (uint32_t)(1UL << ((irqn) & 0x1FUL));  // NVIC_EnableIRQ
}
void stop_USART_receive_irq(USART_TypeDef* usart) { usart->CR1 &= ~USART_CR1_RXNEIE; }
void disable_USART_irq(USART_TypeDef* usart) {
	uint32_t irqn;
	if (usart == USART1) { irqn = USART1_IRQn; }
	else if (usart == USART2) { irqn = USART2_IRQn; }
	else if (usart == USART6) { irqn = USART6_IRQn; }
	else { return; }  // error
	NVIC->ICER[((irqn) >> 5UL)] = (uint32_t)(1UL << ((irqn) & 0x1FUL));  // NVIC_DisableIRQ
	__DSB(); __ISB();  // flush processor pipeline before fetching
}


/*!< input / output */
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
		while (!(usart->SR & USART_SR_TXE)) { if (tick - start > timeout) { return -1; } }
		usart->DR = *str++;
	}
	return 0;
}
