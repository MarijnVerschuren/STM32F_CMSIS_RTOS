#include "main.h"
#include "gpio.h"	// all pins are defined here
#include "exti.h"
#include "tim.h"	// all timers and delays are defined here
#include "sys.h"
#include "usart.h"
#include "pwm.h"
#include "crc.h"
#include "i2c.h"
#include "rng.h"


#ifdef STM32F4xx

#define LED_GPIO_PORT GPIOC
#define LED_PIN 13
#define BTN_GPIO_PORT GPIOA
#define BTN_PIN 0


extern void TIM3_IRQHandler(void) {
	TIM3->SR &= ~TIM_SR_UIF;
	//GPIO_toggle(LED_GPIO_PORT, LED_PIN);
}
extern void EXTI0_IRQHandler(void) {
	EXTI->PR = EXTI_PR_PR0;
	//GPIO_toggle(LED_GPIO_PORT, LED_PIN);
}

int main(void) {
	// sys_clock: 25Mhz / 15 * 120 / 2 = 100Mhz
	SYS_CLK_Config_t* sys_config = new_SYS_CLK_config();
	set_SYS_PLL_config(sys_config, 15, 120, PLL_P_DIV2, 0, PLL_SRC_HSE);
	set_SYS_CLOCK_config(sys_config, SYS_CLK_SRC_PLL, AHB_CLK_NO_DIV, APBx_CLK_DIV2, APBx_CLK_NO_DIV, 0);
	set_SYS_FLASH_config(sys_config, FLASH_LATENCY4, 1, 1, 1);  // latency is set automatically (when need be)
	set_SYS_tick_config(sys_config, 1, 1);
	sys_clock_init(sys_config);
	free(sys_config);

	// GPIO input / output
	config_GPIO(LED_GPIO_PORT, LED_PIN, GPIO_output, GPIO_no_pull, GPIO_push_pull);
	config_GPIO(BTN_GPIO_PORT, BTN_PIN, GPIO_input, GPIO_pull_up, GPIO_push_pull);
	GPIO_write(LED_GPIO_PORT, LED_PIN, 1);  // led is active low

	// EXTI
	config_EXTI(BTN_PIN, BTN_GPIO_PORT, 1, 1);
	start_EXTI(BTN_PIN);

	// initialize CRC
	enable_CRC();  // polynomial: 0x4C11DB7
	// write 1 to CR to clear
	// write bytes to DR to add
	// read DR for CRC

	// UART input
	io_buffer_t* uart_buf = new_buffer(1024);
	if (!uart_buf) { return -1; }  // allocation error
	config_UART(USART1_TX_A9, USART1_RX_A10, 115200);
	start_USART_read_irq(USART1, uart_buf, 1);

	// UART buffer polling interrupt
	config_TIM(TIM3, 50000, 10000);  // 10s
	start_TIM_update_irq(TIM3);  // TIM2_IRQHandler
	start_TIM(TIM3);

	// PWM output
	config_PWM(TIM4_CH4_B9, 100, 20000);		TIM4->CCR4 = 550;

	// I2C test
	config_I2C(I2C1_SCL_B6, I2C1_SDA_B7, 0x0);

	start_RNG();
	volatile uint8_t i2c_tx_data[5] = {
		RNG_generate(0xff),
		RNG_generate(0xff),
		RNG_generate(0xff),
		RNG_generate(0xff),
		RNG_generate(0xff)
	};
	volatile uint8_t i2c_rx_data[5] = {0, 0, 0, 0, 0 };

	// main loop
	for(;;) {
		//delay_ms(RNG_generate(200));
		I2C_master_write(I2C1, 0x40, i2c_tx_data, 5, 10);
		//I2C_master_address(I2C1, 0x40, 10);

		//I2C_slave_read(I2C1, i2c_rx_data, 5, 1000);
		//GPIO_toggle(LED_GPIO_PORT, LED_PIN);

		/* TODO: redo all I2C usage functions
			some functions will exit without proper protocol closure
		 	this has likely led to the line being held low by slave
		*/  // TESTING:  TODO: fix this ^^^^ (i have tested the master and it has issues with this one)

		//I2C_master_address(I2C1, 0x40, 10);  // works but both are master at the same time with this

		/*
		while (I2C1->SR2 & I2C_SR2_BUSY) {}
		I2C1->CR1 |= I2C_CR1_PE;				// enable device
		I2C1->CR1 |= I2C_CR1_ACK;				// enable acknowledgement generation (RX)
		I2C1->CR1 |= I2C_CR1_START;				// generate start condition
		while (!(  // wait until the start condition is sent
				(I2C1->SR1 & I2C_SR1_SB) &&		// start bit detected
				(I2C1->SR2 & I2C_SR2_MSL) &&		// master mode (set when SR1_SB is set)
				(I2C1->SR2 & I2C_SR2_BUSY)		// data transfer over bus started
		)) {}

		I2C1->DR = 0x40 << 1;  // send the address
		while (!(I2C1->SR1 & I2C_SR1_ADDR)) {}  // wait until the address is sent

		(void)I2C1->SR2;	 // clear ADDR flag in SR1 by reading SR2 after SR1

		I2C1->CR1 &= ~I2C_CR1_ACK;
		I2C1->CR1 |= I2C_CR1_STOP;  // send stop condition
		*/
	}
}


#elif defined(STM32F3xx)
/* STM32F3xx:
 * 	_WFI(); works here without any additional config
 */
#endif