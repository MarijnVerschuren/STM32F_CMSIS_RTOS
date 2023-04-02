#include "main.h"
#include "gpio.h"	// all pins are defined here
#include "exti.h"
#include "tim.h"	// all timers and delays are defined here
#include "sys.h"
#include "usart.h"
#include "pwm.h"


#ifdef STM32F4xx

#define LED_GPIO_PORT GPIOC
#define LED_PIN 13


extern void TIM3_IRQHandler(void) {
	TIM3->SR &= ~TIM_SR_UIF;
	GPIO_toggle(LED_GPIO_PORT, LED_PIN);
}


int main(void) {
	// sys_clock: 25Mhz / 15 * 120 / 2 = 100Mhz
	SYS_CLK_Config_t* sys_config = new_SYS_CLK_config();
	set_SYS_PLL_config(sys_config, 15, 120, PLL_P_DIV2, 0, PLL_SRC_HSE);
	set_SYS_CLOCK_config(sys_config, SYS_CLK_SRC_PLL, AHB_CLK_NO_DIV, APBx_CLK_DIV2, APBx_CLK_NO_DIV, 0);
	set_SYS_FLASH_config(sys_config, FLASH_LATENCY4, 1, 1, 1);  // latency is set automatically (when need be)
	set_SYS_tick_config(sys_config, 1, 1);
	sys_clock_init(sys_config); free(sys_config);

	// GPIO output
	config_GPIO(LED_GPIO_PORT, LED_PIN, GPIO_output, GPIO_no_pull, GPIO_push_pull);
	GPIO_write(LED_GPIO_PORT, LED_PIN, 1);  // led is active low

	// UART input
	io_buffer_t* uart_buf = new_buffer(1024);
	config_UART(USART1_TX_A9, USART1_RX_A10, 115200);
	start_USART_receive_irq(USART1, uart_buf, 1);

	// UART buffer polling interrupt
	config_TIM(TIM3, 50000, 1000);  // 1s
	start_TIM_update_irq(TIM3);  // TIM2_IRQHandler
	start_TIM(TIM3);

	// PWM output
	config_PWM(TIM2_CH1_A0, 100, 20000);		TIM2->CCR1 = 600;	// steering
	config_PWM(TIM2_CH3_B10, 100, 20000);	TIM2->CCR3 = 1500;	// throttle

	// main loop
	for(;;) {
		USART_print(USART1, "hello USART1!?", 100);	delay_ms(333);
	}
}


#elif defined(STM32F3xx)
/* STM32F3xx:
 * 	_WFI(); works here without any additional config
 */
#endif