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


/* variables */
io_buffer_t* uart_buf;
TaskHandle_t* idle_task;
TaskHandle_t* watchdog_task;


/* CMSIS */
extern void TIM1_UP_TIM10_IRQHandler(void) {
	TIM10->SR &= ~TIM_SR_UIF;
}
extern void EXTI0_IRQHandler(void) {
	EXTI->PR = EXTI_PR_PR0;
}


/* RTOS */
void idle(void* memory_pool) {
	for (;;) {

	}
}
void watchdog(void* memory_pool) {
	for (;;) {
		reset_watchdog();
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
	uart_buf = new_buffer(1024);
	if (!uart_buf) { return -1; }  // allocation error
	config_UART(USART1_TX_A9, USART1_RX_A10, 115200);
	start_USART_read_irq(USART1, uart_buf, 1);

	// UART buffer polling interrupt
	config_TIM(TIM10, 1000, 20000);  // 5 Hz
	start_TIM_update_irq(TIM10);  // TIM1_UP_TIM10_IRQHandler
	start_TIM(TIM10);

	// watchdog
	config_watchdog(0, 0xfff);  // 512 ms
	start_watchdog();

	/* RTOS */
	if (xTaskCreate(
		idle,
		"idle",
		configMINIMAL_STACK_SIZE,	// stack size in words (160 B)
		NULL,						// stack parameter
		(tskIDLE_PRIORITY | portPRIVILEGE_BIT),
		idle_task
	) != pdPASS) {
		__NOP();
	}
	if (xTaskCreate(
		watchdog,
		"watchdog",
		configMINIMAL_STACK_SIZE,	// stack size in words (160 B)
		NULL,						// stack parameter
		(tskIDLE_PRIORITY | portPRIVILEGE_BIT),
		watchdog_task
	) != pdPASS) {
		__NOP();
	}

	// start scheduler
	vTaskStartScheduler();

	return 0;
}

#endif