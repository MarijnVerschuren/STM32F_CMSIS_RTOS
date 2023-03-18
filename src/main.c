#include "main.h"
#include "gpio.h"	// all pins are defined here
#include "exti.h"
#include "tim.h"	// all timers and delays are defined here
#include "sys.h"


#ifdef STM32F4xx
#define LED_PIN 13
#define LED_GPIO_PORT GPIOC
#define BTN_PIN 0
#define BTN_GPIO_PORT GPIOA


extern void EXTI0_IRQHandler(void) {
	EXTI->PR = EXTI_PR_PR0;
	toggle_pin(LED_PIN, LED_GPIO_PORT);
}

extern void TIM2_IRQHandler(void) {
	TIM2->SR &= ~TIM_SR_UIF;
	toggle_pin(LED_PIN, LED_GPIO_PORT);
}

int main(void) {
	sys_clock_init();

	// initialize GPIO peripheral clock (on enabled ports)
	enable_GPIO_port(LED_GPIO_PORT);
	enable_GPIO_port(BTN_GPIO_PORT);

	// initialize EXTI
	EXTI_init();
	enable_EXTI(BTN_PIN, BTN_GPIO_PORT, 1, 0);
	start_EXTI(BTN_PIN);  // EXTI0_IRQHandler

	// configure pins
	config_pin(LED_PIN, LED_GPIO_PORT, GPIO_output, GPIO_medium_speed, GPIO_no_pull, push_pull);
	config_pin(BTN_PIN, BTN_GPIO_PORT, GPIO_input, GPIO_medium_speed, GPIO_pull_up, push_pull);

	// set initial state of the pins
	write_pin(LED_PIN, LED_GPIO_PORT, 1);  // led is active low

	TIM_init(TIM2, 100000, 2000, 1);
	start_TIM_update_irq(TIM2);  // TIM2_IRQHandler
	TIM_start(TIM2);

	// TODO: UART, PWM

	for(;;) {}
}
#elif defined(STM32F3xx)
#define LED_PIN 5
#define LED_GPIO_PORT GPIOA
#define BTN_PIN 13
#define BTN_GPIO_PORT GPIOC


extern void EXTI15_10_IRQHandler(void) {
	if(EXTI->PR & EXTI_PR_PR13) {
		EXTI->PR = EXTI_PR_PR13;  // clear interrupt pin (write to it will clear it)
		toggle_pin(LED_PIN, LED_GPIO_PORT);
	}
}

extern void TIM2_IRQHandler(void) {
	TIM2->SR &= ~TIM_SR_UIF;
	toggle_pin(LED_PIN, LED_GPIO_PORT);
}

int main(void) {
	// initialize GPIO peripheral clock (on enabled ports)
	enable_GPIO_port(LED_GPIO_PORT);
	enable_GPIO_port(BTN_GPIO_PORT);

	// initialize EXTI
	EXTI_init();
	enable_EXTI(BTN_PIN, BTN_GPIO_PORT, 1, 0);
	start_EXTI(BTN_PIN);

	// configure pins
	config_pin(LED_PIN, LED_GPIO_PORT, GPIO_output, GPIO_medium_speed, GPIO_no_pull, push_pull);
	config_pin(BTN_PIN, BTN_GPIO_PORT, GPIO_input, GPIO_medium_speed, GPIO_pull_up, push_pull);

	// set initial state of the pins
	write_pin(LED_PIN, LED_GPIO_PORT, 1);  // led is active low

	TIM_init(TIM2, 1000, 1000, 1);
	start_TIM_update_irq(TIM2);
	TIM_start(TIM2);

	// suspend cpu until an interrupt occurs
	for(;;) { __WFI(); }
}
#endif
