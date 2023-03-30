#include "main.h"
#include "gpio.h"	// all pins are defined here
#include "exti.h"
#include "tim.h"	// all timers and delays are defined here
#include "sys.h"
#include "usart.h"
#include "pwm.h"


#ifdef STM32F4xx
#define LED_PIN 13
#define LED_GPIO_PORT GPIOC
#define BTN_PIN 0
#define BTN_GPIO_PORT GPIOA


extern void EXTI0_IRQHandler(void) {
	EXTI->PR = EXTI_PR_PR0;
	GPIO_toggle(LED_GPIO_PORT, LED_PIN);
}

extern void TIM2_IRQHandler(void) {
	TIM2->SR &= ~TIM_SR_UIF;
	GPIO_toggle(LED_GPIO_PORT, LED_PIN);
}


int main(void) {
	// sys_clock: 25Mhz / 15 * 120 / 2 = 100Mhz
	SYS_CLK_Config_TypeDef* sys_config = new_SYS_CLK_config();
	set_SYS_PLL_config(sys_config, 15, 120, PLL_P_DIV2, 0, PLL_SRC_HSE);
	set_SYS_CLOCK_config(sys_config, SYS_CLK_SRC_PLL, AHB_CLK_NO_DIV, APBx_CLK_DIV2, APBx_CLK_NO_DIV, 0);
	set_SYS_FLASH_config(sys_config, FLASH_LATENCY4, 1, 1, 1);  // latency is set automatically (when need be)
	set_SYS_tick_config(sys_config, 1, 1);
	sys_clock_init(sys_config); free(sys_config);

	// initialize EXTI
	enable_EXTI(BTN_PIN, BTN_GPIO_PORT, 1, 0);
	start_EXTI(BTN_PIN);  // EXTI0_IRQHandler

	// configure pins
	config_GPIO(LED_GPIO_PORT, LED_PIN, GPIO_output, GPIO_no_pull, GPIO_push_pull);
	config_GPIO(BTN_GPIO_PORT, BTN_PIN, GPIO_input, GPIO_pull_up, GPIO_push_pull);

	// configure the timer
	enable_TIM(TIM2, 100000, 1000);
	start_TIM_update_irq(TIM2);  // TIM2_IRQHandler
	start_TIM(TIM2);

	// set initial state of the pins
	GPIO_write(LED_GPIO_PORT, LED_PIN, 1);  // led is active low

	config_UART(USART1, 115200, USART1_TX_A9, USART1_RX_A10);
	config_UART(USART2, 115200, USART2_TX_A2, USART2_RX_A3);
	config_UART(USART6, 115200, USART6_TX_A11, USART6_RX_A12);

	//config_PWM(TIM4, 4, TIM4_CH4_B9, 100000, 100, 0);
	fconfig_GPIO(GPIOB, 9, GPIO_alt_func, GPIO_no_pull, GPIO_push_pull, GPIO_high_speed, 2);
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	TIM4->CCMR2 &= ~TIM_CCMR2_CC4S;
	TIM4->CCER &= ~TIM_CCER_CC4P;
	TIM4->CCMR2 |= TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1;
	TIM4->PSC = 50;
	TIM4->ARR = 2000;
	TIM4->CCMR2 |= TIM_CCMR2_OC4PE;
	TIM4->CR1 |= TIM_CR1_ARPE;
	TIM4->CCER |= TIM_CCER_CC4E;
	TIM4->CR1 |= TIM_CR1_CEN;


	TIM4->CCR4 = 1000;
	int8_t add = 1;
	for(;;) {
		/*USART_print(USART1, "hello USART1!?", 100);	delay_ms(333);
		USART_print(USART2, "hello USART2!!?", 100);	delay_ms(333);
		USART_print(USART6, "hello USART6!!!?", 100);	delay_ms(333);*/
		/*TIM4->CCR4 += add;
		if (TIM4->CCR4 > 2350)		{ add = -1; }
		else if(TIM4->CCR4 < 1350)	{ add = 1; }
		delay_ms(5);*/
	}
}


#elif defined(STM32F3xx)
/* STM32F3xx:
 * 	_WFI(); works here without any additional config
 */
#endif