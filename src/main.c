// RTOS
#include <FreeRTOSConfig.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>

// CMSIS
#include "gpio.h"	// all pins are defined here
#include "exti.h"
#include "tim.h"	// all timers and delays are defined here
#include "usart.h"
#include "pwm.h"
#include "crc.h"
#include "i2c.h"
#include "rng.h"
#include "encoder.h"
#include "watchdog.h"



#ifdef STM32F4xx

#define LED_GPIO_PORT GPIOC
#define LED_PIN 13
#define BTN_GPIO_PORT GPIOA
#define BTN_PIN 0


extern void RTOS_tick_handler(void);

extern void TIM1_UP_TIM10_IRQHandler(void) {
	TIM10->SR &= ~TIM_SR_UIF;
	GPIO_toggle(LED_GPIO_PORT, LED_PIN);
}
extern void EXTI0_IRQHandler(void) {
	EXTI->PR = EXTI_PR_PR0;
	GPIO_toggle(LED_GPIO_PORT, LED_PIN);
}

uint32_t task_0_cnt = 0;
uint32_t task_1_cnt = 0;

void task_0(void* args) {
	uint32_t delay = *((uint32_t*)args);
	uint32_t prev = tick;
	for (;;) {
		if (tick - prev > delay) {
			prev = tick;
			task_0_cnt++;
		}
	}
}
void task_1(void* args) {
	uint32_t delay = *((uint32_t*)args);
	uint32_t prev = tick;
	for (;;) {
		if (tick - prev > delay) {
			prev = tick;
			task_1_cnt++;
		}
	}
}


int main(void) {
	/* CMSIS */
	// sys_clock: 25Mhz / 15 * 120 / 2 = 100Mhz
	SYS_CLK_Config_t* sys_config = new_SYS_CLK_config();
	set_SYS_PLL_config(sys_config, 15, 120, PLL_P_DIV2, 0, PLL_SRC_HSE);
	set_SYS_CLOCK_config(sys_config, SYS_CLK_SRC_PLL, AHB_CLK_NO_DIV, APBx_CLK_DIV2, APBx_CLK_NO_DIV, 0);
	set_SYS_FLASH_config(sys_config, FLASH_LATENCY4, 1, 1, 1);  // latency is set automatically (when need be)
	set_SYS_tick_config(sys_config, 1, 1, RTOS_tick_handler);
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

	// UART input
	io_buffer_t* uart_buf = new_buffer(1024);
	if (!uart_buf) { return -1; }  // allocation error
	config_UART(USART1_TX_A9, USART1_RX_A10, 115200);
	start_USART_read_irq(USART1, uart_buf, 1);

	// UART buffer polling interrupt
	config_TIM(TIM10, 1000, 20000);  // 5 Hz
	start_TIM_update_irq(TIM10);  // TIM1_UP_TIM10_IRQHandler
	start_TIM(TIM10);

	// PWM output
	config_PWM(TIM4_CH4_B9, 100, 20000); TIM4->CCR4 = 550;

	// RNG generator
	start_RNG();

	// I2C
	config_I2C(I2C1_SCL_B6, I2C1_SDA_B7, 0);

	// S0S90 external counter
	config_encoder_S0S90(TIM2_CH1_A0, TIM2_CH2_A1);
	start_encoder_S0S90(TIM2);

	// watchdog
	config_watchdog(0, 0xfff);  // 512 ms
	//start_watchdog();

	/* RTOS */
	uint32_t delay_0 = 500;
	uint32_t delay_1 = 1000;

	xTaskCreate(
		task_0,
		"task 0",
		40,			// stack size in words (160 B)
		&delay_0,	// stack parameter
		1,			// priority
		NULL
	);
	xTaskCreate(
		task_1,
		"task 1",
		40,			// stack size in words (160 B)
		&delay_1,	// stack parameter
		2,			// priority
		NULL
	);
	/*
	TaskFunction_t pxTaskCode,
	const char * const pcName,
	const configSTACK_DEPTH_TYPE usStackDepth,
	void * const pvParameters,
	UBaseType_t uxPriority,
	TaskHandle_t * const pxCreatedTask
	 */

	// start scheduler
	vTaskStartScheduler();

	// idle task
	//for(;;) {
	//	reset_watchdog();
	//}

	return 0;
}


#elif defined(STM32F3xx)
/* STM32F3xx:
 * 	_WFI(); works here without any additional config
 */
#endif