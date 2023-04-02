#include "main.h"
#include "gpio.h"	// all pins are defined here
#include "exti.h"
#include "tim.h"	// all timers and delays are defined here
#include "sys.h"
#include "usart.h"
#include "pwm.h"


#ifdef STM32F4xx


int main(void) {
	// sys_clock: 25Mhz / 15 * 120 / 2 = 100Mhz
	SYS_CLK_Config_TypeDef* sys_config = new_SYS_CLK_config();
	set_SYS_PLL_config(sys_config, 15, 120, PLL_P_DIV2, 0, PLL_SRC_HSE);
	set_SYS_CLOCK_config(sys_config, SYS_CLK_SRC_PLL, AHB_CLK_NO_DIV, APBx_CLK_DIV2, APBx_CLK_NO_DIV, 0);
	set_SYS_FLASH_config(sys_config, FLASH_LATENCY4, 1, 1, 1);  // latency is set automatically (when need be)
	set_SYS_tick_config(sys_config, 1, 1);
	sys_clock_init(sys_config); free(sys_config);

	uint8_t* rx_buffer = malloc(32);  // it is assumed that this is successful
	// TODO: make function that creates a buffer for you (with write ptr)
	config_UART(USART1_TX_A9, USART1_RX_A10, 115200);
	start_USART_receive_irq(USART1, rx_buffer, 32, 1);
	/*config_PWM(TIM4_CH4_B9, 100, 20000);
	TIM4->CCR4 = 1500;*/

	for(;;) {
		USART_print(USART1, "hello USART1!?", 100);	delay_ms(333);
	}
}


#elif defined(STM32F3xx)
/* STM32F3xx:
 * 	_WFI(); works here without any additional config
 */
#endif