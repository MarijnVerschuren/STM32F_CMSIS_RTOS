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


SYS_CLK_Config_TypeDef* clock_config = nullptr;
/*
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
*/


extern void EXTI0_IRQHandler(void) {
	EXTI->PR = EXTI_PR_PR0;
	toggle_pin(LED_GPIO_PORT, LED_PIN);
}

extern void TIM2_IRQHandler(void) {
	TIM2->SR &= ~TIM_SR_UIF;
	toggle_pin(LED_GPIO_PORT, LED_PIN);
}


int main(void) {
	clock_config = new_SYS_CLK_config();  // TODO: make functions that set the struct for nice notation
	// HSE / M * N / P  =  25Mhz / 15 * 120 / 2 = 100Mhz
	clock_config->PLL_M =						15;
	clock_config->PLL_N =						120;
	clock_config->PLL_P =						PLL_P_DIV2;
	clock_config->PLL_source =					PLL_SRC_HSE;
	clock_config->SYS_CLK_source =				SYS_CLK_SRC_PLL;
	clock_config->APB1_prescaler =				APBx_CLK_DIV2;
	clock_config->APB2_prescaler =				APBx_CLK_NO_DIV;
	clock_config->SYS_tick_enable =				1;
	clock_config->SYS_tick_interrupt_enable =	1;
	clock_config->FLASH_prefetch =				1;
	clock_config->FLASH_instruction_cache =		1;
	clock_config->FLASH_data_cache =			1;
	sys_clock_init(clock_config);

	// TODO: remove the enable clock functions (incorporate it into the config funcs) (do leave a disable func)

	// initialize GPIO peripheral clock (on enabled ports)
	enable_GPIO_port_clock(LED_GPIO_PORT);
	enable_GPIO_port_clock(BTN_GPIO_PORT);

	// initialize EXTI
	enable_EXTI_clock();
	enable_EXTI(BTN_PIN, BTN_GPIO_PORT, 1, 0);
	start_EXTI(BTN_PIN);  // EXTI0_IRQHandler

	// configure pins
	config_pin(LED_GPIO_PORT, LED_PIN, GPIO_output, GPIO_medium_speed, GPIO_no_pull, GPIO_push_pull);
	config_pin(BTN_GPIO_PORT, BTN_PIN, GPIO_input, GPIO_medium_speed, GPIO_pull_up, GPIO_push_pull);

	// configure the timer
	enable_TIM_clock(TIM2, 100000, 500, 1);
	start_TIM_update_irq(TIM2);  // TIM2_IRQHandler
	TIM_start(TIM2);

	// set initial state of the pins
	write_pin(LED_GPIO_PORT, LED_PIN, 1);  // led is active low

	// TODO: update function with the following source:
	// https://controllerstech.com/how-to-setup-uart-using-registers-in-stm32/
	// TODO: fix USART1/6 try source:
	// https://stackoverflow.com/questions/43441008/stm32f4-usart1-sends-garbage
	config_UART(USART1, 115200, USART1_TX_A9, USART1_RX_A10);
	config_UART(USART2, 115200, USART2_TX_A2, USART2_RX_A3);
	config_UART(USART6, 115200, USART6_TX_A11, USART6_RX_A12);

	for(;;) {
		USART_print(USART1, "hello UART1!\n", 100);
		delay_ms(250);
		USART_print(USART2, "hello UART2!\n", 100);
		delay_ms(250);
		USART_print(USART6, "hello UART6!\n", 100);
		delay_ms(250);
		USART_transmit(USART1, '\n', 10);
		delay_ms(250);
	}
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


/*  TODO: format these like so: port_num: 4 | pin_num: 4 | af: 4
typedef enum {
	// SYS
	SYS_AF =		0x0,
	// TIM1 TIM2
	TIM1_AF =		0x1,
	TIM2_AF =		0x1,
	// TIM3 TIM4 TIM5
	TIM3_AF =		0x2,
	TIM4_AF =		0x2,
	TIM5_AF =		0x2,
	// TIM9 TIM10 TIM11
	TIM9_AF =		0x3,
	TIM10_AF =		0x3,
	TIM11_AF =		0x3,
	// I2C1 I2C2 I2C3
	I2C1_AF =		0x4,
	I2C2_AF =		0x4,
	I2C3_AF =		0x4,
	// SPI1 I2S1_SPI2 I2S2 SPI3 I2S3
	SPI1_AF =		0x5,
	SPI2_AF =		0x5,
	SPI3_AF =		0x5,
	I2S2_AF =		0x5,
	I2S3_AF =		0x5,
	I2S1_SPI2_AF =	0x5,
	// SPI2 I2S2 SPI3 I2S3 SPI4 I2S4 SPI5 I2S5
	SPI2_AF2 =		0x6,
	SPI3_AF2 =		0x6,
	SPI4_AF =		0x6,
	SPI5_AF =		0x6,
	I2S2_AF2 =		0x6,
	I2S3_AF2 =		0x6,
	I2S4_AF =		0x6,
	I2S5_AF =		0x6,
	// SPI3 I2S3 USART1 USART2
	SPI3_AF3 =		0x7,
	I2S3_AF3 =		0x7,
	USART1_AF =		0x7,
	USART2_AF =		0x7,
	// USART6
	USART6_AF =		0x8,
	// I2C2 I2C3
	I2C2_AF2 =		0x9,
	I2C3_AF2 =		0x9,
	// USB_OTG_FS
	USB_OTG_FS_AF =	0xA,
	// SDIO
	SDIO_AF =		0xC
} GPIO_ALT_FUNCTION_TypeDef;
 */