#include "main.h"
#include "gpio.h"	// all pins are defined here
#include "exti.h"
#include "tim.h"	// all timers and delays are defined here
#include "sys.h"
#include "usart.h"


#ifdef STM32F4xx
#define LED_PIN 13
#define LED_GPIO_PORT GPIOC
#define BTN_PIN 0
#define BTN_GPIO_PORT GPIOA


SYS_CLK_Config_TypeDef* sys_config = nullptr;


extern void EXTI0_IRQHandler(void) {
	EXTI->PR = EXTI_PR_PR0;
	GPIO_toggle(LED_GPIO_PORT, LED_PIN);
}

extern void TIM2_IRQHandler(void) {
	TIM2->SR &= ~TIM_SR_UIF;
	GPIO_toggle(LED_GPIO_PORT, LED_PIN);
}


int main(void) {
	sys_config = new_SYS_CLK_config();  // sys_clock: 25Mhz / 15 * 120 / 2 = 100Mhz
	set_SYS_PLL_config(sys_config, 15, 120, PLL_P_DIV2, 0, PLL_SRC_HSE);
	set_SYS_CLOCK_config(sys_config, SYS_CLK_SRC_PLL, AHB_CLK_NO_DIV, APBx_CLK_DIV2, APBx_CLK_NO_DIV, 0);
	set_SYS_FLASH_config(sys_config, FLASH_LATENCY4, 1, 1, 1);  // latency is set automatically (when need be)
	set_SYS_tick_config(sys_config, 1, 1);
	sys_clock_init(sys_config);

	// initialize EXTI
	enable_EXTI(BTN_PIN, BTN_GPIO_PORT, 1, 0);
	start_EXTI(BTN_PIN);  // EXTI0_IRQHandler

	// configure pins
	config_GPIO(LED_GPIO_PORT, LED_PIN, GPIO_output, GPIO_medium_speed, GPIO_no_pull, GPIO_push_pull, 0);
	config_GPIO(BTN_GPIO_PORT, BTN_PIN, GPIO_input, GPIO_medium_speed, GPIO_pull_up, GPIO_push_pull, 0);

	// configure the timer
	enable_TIM(TIM2, 100000, 4000);
	start_TIM_update_irq(TIM2);  // TIM2_IRQHandler
	start_TIM(TIM2);

	// set initial state of the pins
	GPIO_write(LED_GPIO_PORT, LED_PIN, 1);  // led is active low

	config_UART(USART1, 115200, USART1_TX_A9, USART1_RX_A10);
	config_UART(USART2, 115200, USART2_TX_A2, USART2_RX_A3);
	config_UART(USART6, 115200, USART6_TX_A11, USART6_RX_A12);

	for(;;) {
		USART_print(USART1, "hello UART1\n", 100);	delay_ms(250);
		USART_print(USART2, "hello UART2\n", 100);	delay_ms(250);
		USART_print(USART6, "hello UART6\n", 100);	delay_ms(250);
		USART_transmit(USART1, '\n', 10);			delay_ms(250);
	}
}


#elif defined(STM32F3xx)
/* STM32F3xx:
 * 	_WFI(); works here without any additional config
 */
#endif