//
// Created by marijn on 4/1/23.
//
#include "i2c.h"


/*!< static */
static inline void I2C_GPIO_to_args(uint32_t i2c_pin, I2C_TypeDef** i2c, uint8_t* alternate_function, GPIO_TypeDef** port, uint8_t* pin) {
	uint8_t dev_id =		(i2c_pin >> 16);
	(*i2c) =				id_to_dev(*((dev_id_t*)&dev_id));
	(*alternate_function) =	(i2c_pin >> 8) & 0xfu;
	(*port) =				int_to_GPIO(i2c_pin >> 4);
	(*pin) =				i2c_pin & 0xfu;
}
static inline void enable_I2C_clock(I2C_TypeDef* i2c) {
	RCC->APB1ENR |= (0b1u << (((uint32_t)i2c - APB1PERIPH_BASE) >> 10u));
}
static inline uint32_t I2C_transmit(I2C_TypeDef* i2c, const uint8_t* buffer, uint32_t size, uint32_t timeout) {
	uint64_t start = tick;
	for (uint32_t i = 0; i < size; i++) {
		while (!(i2c->SR1 & I2C_SR1_TXE)) {
			if (tick - start > timeout) { return i; }
		} i2c->DR = buffer[i];
	} while (!(i2c->SR1 & I2C_SR1_BTF)) {  // wait for BTF to set
		if (tick - start > timeout) { return size; }
	} return size;
}
static inline uint32_t I2C_receive(I2C_TypeDef* i2c, uint8_t* buffer, uint32_t size, uint32_t timeout) {
	uint64_t start = tick;
	for (uint32_t i = 0; i < size; i++) {
		if (size - i > 1) { i2c->CR1 |= I2C_CR1_ACK; }  // generate ACK signal
		else { i2c->CR1 &= ~I2C_CR1_ACK; i2c->CR1 |= I2C_CR1_STOP; }  // stop transmission after next byte
		while (!(i2c->SR1 & I2C_SR1_RXNE)) {
			if (tick - start > timeout) { return i; }
		} buffer[i] = i2c->DR;
	} return size;
}
static inline uint8_t I2C_start(I2C_TypeDef* i2c, uint8_t address, uint32_t timeout) {  // start and address
	uint64_t start = tick;
	i2c->CR1 |= I2C_CR1_PE;					// enable device
	i2c->CR1 |= I2C_CR1_ACK;				// enable acknowledgement generation (RX)
	i2c->CR1 |= I2C_CR1_START;				// generate start condition
	while (!(  // wait until the start condition is sent
			(i2c->SR1 & I2C_SR1_SB) &&		// start bit detected
			(i2c->SR2 & I2C_SR2_MSL) &&		// master mode (set when SR1_SB is set)
			(i2c->SR2 & I2C_SR2_BUSY)		// data transfer over bus started
	)) { if (tick - start > timeout) { return -1; } }

	i2c->DR = address;  // send the address
	while (!(i2c->SR1 & I2C_SR1_ADDR)) { if (tick - start > timeout) { return -1; } }  // wait until the address is sent

	(void)i2c->SR2;	 // clear ADDR flag in SR1 by reading SR2 after SR1
	return 0;
}
static inline void I2C_stop(I2C_TypeDef* i2c) {
	I2C1->CR1 |= I2C_CR1_STOP;  // send stop condition
	I2C1->CR1 &= ~I2C_CR1_PE;	// disable device
}


/*!< init / enable / disable */
void disable_I2C_clock(I2C_TypeDef* i2c) {
	RCC->APB1ENR &= ~(0b1u << (((uint32_t)i2c - APB1PERIPH_BASE) >> 10u));
}

void config_I2C(I2C_GPIO_t scl, I2C_GPIO_t sda) {
	uint8_t freq = APB1_clock_frequency / 1000000ul;
	if (freq < 2 || scl == I2C_PIN_DISABLE || sda == I2C_PIN_DISABLE) { return; }
	I2C_TypeDef		*scl_i2c =	NULL,	*sda_i2c =	NULL,	*i2c = NULL;
	GPIO_TypeDef	*scl_port =	NULL,	*sda_port =	NULL;
	uint8_t			scl_af = 0, sda_af = 0, scl_pin = 0, sda_pin = 0;
	I2C_GPIO_to_args(scl, &scl_i2c, &scl_af, &scl_port, &scl_pin);
	I2C_GPIO_to_args(sda, &sda_i2c, &sda_af, &sda_port, &sda_pin);
	if (scl_i2c != sda_i2c) { return; }; i2c = scl_i2c;
	fconfig_GPIO(scl_port, scl_pin, GPIO_alt_func, GPIO_pull_up, GPIO_open_drain, GPIO_very_high_speed, scl_af);
	fconfig_GPIO(sda_port, sda_pin, GPIO_alt_func, GPIO_pull_up, GPIO_open_drain, GPIO_very_high_speed, sda_af);
	enable_I2C_clock(i2c);
	i2c->CR1 &= ~I2C_CR1_PE;	// disable device
	i2c->CR2 = freq & 0x1Ful;	// set frequency
	i2c->CCR = 5 * freq;		// set scl freq to 1 MHz
	i2c->TRISE = freq + 1;		// set max rise time
}


/*!< test */
void I2C_test(void) {  // THIS WORKS
	I2C_start(I2C1, 0x55 << 1, 10);
	I2C_stop(I2C1);
}
/*!< input / output */
uint32_t I2C_write(I2C_TypeDef* i2c, uint8_t i2c_address, const uint8_t* buffer, uint32_t size, uint32_t timeout) {
	uint64_t start = tick;
	//while (i2c->SR2 & I2C_SR2_BUSY) { if (tick - start > timeout) { return 0; } }  // wait until idle
	if (I2C_start(i2c, i2c_address << 1, timeout - (tick - start)))				{ return 0; }
	uint32_t tx_size = I2C_transmit(i2c, buffer, size, timeout - (tick - start));
	I2C_stop(i2c); return tx_size;
}
uint32_t I2C_read(I2C_TypeDef* i2c, uint8_t i2c_address, uint8_t* buffer, uint32_t size, uint32_t timeout) {
	uint64_t start = tick;
	while (i2c->SR2 & I2C_SR2_BUSY) { if (tick - start > timeout) { return 0; } }  // wait until idle
	if (I2C_start(i2c, (i2c_address << 1) | 0b1ul, timeout - (tick - start)))	{ return 0; }
	uint32_t rx_size = I2C_receive(i2c, buffer, size, timeout - (tick - start));
	I2C_stop(i2c); return rx_size;
}
uint32_t I2C_write_reg(I2C_TypeDef* i2c, uint8_t i2c_address, uint8_t reg_address, const uint8_t* buffer, uint32_t size, uint32_t timeout) {
	uint64_t start = tick;
	while (i2c->SR2 & I2C_SR2_BUSY) { if (tick - start > timeout) { return 0; } }  // wait until idle
	if (I2C_start(i2c, i2c_address << 1, timeout - (tick - start)))				{ return 0; }
	if (!I2C_transmit(i2c, &reg_address, 1, timeout - (tick - start)))		{ return 0; }
	uint32_t tx_size = I2C_transmit(i2c, buffer, size, timeout - (tick - start));
	I2C_stop(i2c); return tx_size;
}
uint32_t I2C_read_reg(I2C_TypeDef* i2c, uint8_t i2c_address, uint8_t reg_address, uint8_t* buffer, uint32_t size, uint32_t timeout) {
	uint64_t start = tick;
	while (i2c->SR2 & I2C_SR2_BUSY) { if (tick - start > timeout) { return 0; } }  // wait until idle
	if (I2C_start(i2c, i2c_address << 1, timeout - (tick - start)))				{ return 0; }
	if (!I2C_transmit(i2c, &reg_address, 1, timeout - (tick - start)))		{ return 0; }
	if (I2C_start(i2c, (i2c_address << 1) | 0b1ul, timeout - (tick - start)))	{ return 0; }
	uint32_t rx_size = I2C_receive(i2c, buffer, size, timeout - (tick - start));
	I2C_stop(i2c); return rx_size;
}