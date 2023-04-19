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
#include "encoder.h"


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

	// RNG generator
	start_RNG();

	// I2C TODO: ...
	// config_I2C(I2C1_SCL_B6, I2C1_SDA_B7, 0);

	volatile uint8_t i2c_tx_data[5] = {
		RNG_generate(0xff),
		RNG_generate(0xff),
		RNG_generate(0xff),
		RNG_generate(0xff),
		RNG_generate(0xff)
	};
	volatile uint8_t i2c_rx_data[5] = {0, 0, 0, 0, 0 };

	// S0S90 external counter
	config_encoder_S0S90(TIM2_CH1_A0, TIM2_CH2_A1);
	start_encoder_S0S90(TIM2);

	// main loop
	for(;;) {
		/* TODO: fix error where slave interferes withe the SCL line!!!!! */
		// TODO: move PWM_GPIO_t to tim.h and call it TIM_GPIO_t
		//I2C_master_write(I2C1, 0x50, i2c_tx_data, 5, 15);
		//I2C_slave_read(I2C1, i2c_rx_data, 5, 1000);
	}
}


#elif defined(STM32F3xx)
/* STM32F3xx:
 * 	_WFI(); works here without any additional config
 */
#endif