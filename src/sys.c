//
// Created by marijn on 3/1/23.
//
#include "sys.h"


SYS_CLK_Config_TypeDef* new_SYS_CLK_config(void) {
	SYS_CLK_Config_TypeDef* config = malloc(sizeof(SYS_CLK_Config_TypeDef));
	// reset PLL config
	// PLL_freq = src / m * n / p
	config->PLL_M = 1;
	config->PLL_N = 2;
	config->PLL_P = PLL_P_DIV2;
	config->PLL_Q = 0;					// disable
	config->PLL_source = PLL_SRC_HSI;	// 16 MHz
	// reset flash config
	config->FLASH_latency = FLASH_LATENCY1;
	config->FLASH_prefetch =			0;
	config->FLASH_instruction_cache =	0;
	config->FLASH_data_cache =			0;
	// reset sys clock config
	config->SYS_CLK_source = SYS_CLK_SRC_HSI;
	config->AHB_prescaler =				0;	// no div
	config->APB1_prescaler =			0;	// no div
	config->APB2_prescaler =			0;	// no div
	config->RTC_prescaler =				0;	// disable
	config->MCO1_source =				0;	// HSI
	config->I2S_external_clock =		0;
	config->MCO1_prescaler =			0;	// no div
	config->MCO2_prescaler =			0;	// no div
	config->MCO2_source =				0;	// SYS_CLK
	// disable sys tick
	config->SYS_tick_reload =			0;
	config->SYS_tick_start_value =		0;
	config->SYS_tick_control =			0;
	return config;
}

void sys_clock_init(SYS_CLK_Config_TypeDef* config) {
	RCC->PLLCFGR = (																									/*
				PLL_M: division factor for the main PLL and audio PLL (PLLI2S) input clock. Info:
					the software has to set these bits correctly to ensure that the VCO
					input frequency ranges from 1 to 2 MHz. It is recommended to select
					a frequency of 2 MHz to limit PLL jitter.															*/
			((config->PLL_M << RCC_PLLCFGR_PLLM_Pos) & RCC_PLLCFGR_PLLM_Msk)	|										/*
				PLL_N: main PLL multiplication factor for VCO. Info:
					the software has to set these bits correctly to ensure that
					the VCO output frequency is between 192 and 432 MHz.												*/
			((config->PLL_N << RCC_PLLCFGR_PLLN_Pos) & RCC_PLLCFGR_PLLN_Msk)	|										/*
				PLL_P: main PLL division factor for main system clock
				PLL output clock frequency = VCO frequency / PLL_P:
					00: PLL_P = 2
					01: PLL_P = 4
					10: PLL_P = 6
					11: PLL_P = 8																						*/
			((config->PLL_P << RCC_PLLCFGR_PLLP_Pos) & RCC_PLLCFGR_PLLP_Msk)	|										/*
				PLLSRC: PLL and PLLI2S clock source:
 					0: HSI
					1: HSE																								*/
			(config->PLL_source * RCC_PLLCFGR_PLLSRC_HSE)						|										/*
 				PLL_Q: main PLL division factor for USB OTG FS, SDIO and RNG clocks:
 					0000: ERROR
 					0001: ERROR
 					0010: divide PLL by 2
 					...
 					1111: divide PLL by 15																				*/
			((config->PLL_Q << RCC_PLLCFGR_PLLQ_Pos) & RCC_PLLCFGR_PLLQ_Msk)
	);

	// turn on HSE and PLL
	RCC->CR = (
			RCC_CR_HSEON |	/* switch HSE ON */
			RCC_CR_PLLON	/* switch PLL ON */
	);

	// TODO: RTC
	//PWR->CR = PWR_CR_VOS_1 | PWR_CR_DBP; /*  Enable Backup Domain Access (leave VOS default)       */
	//RCC->BDCR = (
	//		RCC_BDCR_LSEON                   | /*  Switch HSE ON                                     */
	//		RCC_BDCR_RTCSEL_0                | /*  LSE oscillator clock used as RTC clock            */
	//		RCC_BDCR_RTCEN                     /*  RTC clock enable                                  */
	//);

	/* Relation between CPU clock frequency and flash memory read time.
	To correctly read data from flash memory, the number of wait states (LATENCY) must be
	correctly programmed in the flash access control register (FLASH_ACR) according to the
	frequency of the CPU clock (HCLK) and the supply voltage of the device.
	The prefetch buffer must be disabled when the supply voltage is below 2.1 V. The
	correspondence between wait states and CPU clock frequency is given in Table 6.
	- when VOS[1:0] = 0x01, the maximum frequency of HCLK = 60 MHz.
	- when VOS[1:0] = 0x10, the maximum frequency of HCLK = 84 MHz.
	*/

	FLASH->ACR = (																										/*
 				LATENCY: flash read latency (see the table at the definition for ACR_LATENCY_TypeDef for more info)		*/
			((config->FLASH_latency << FLASH_ACR_LATENCY_Pos) & FLASH_ACR_LATENCY_7WS)	|
			(config->FLASH_prefetch * FLASH_ACR_PRFTEN)									|	/* enable prefetch			*/
			(config->FLASH_instruction_cache * FLASH_ACR_ICEN)							|	/* enable instruction cache	*/
			(config->FLASH_data_cache * FLASH_ACR_DCEN)										/* enable data cache		*/
	);

	while(!(RCC->CR & RCC_CR_HSERDY)) { /* wait till HSE is ready */ }
	while(!(RCC->CR & RCC_CR_PLLRDY)) { /* wait till PLL is ready */ }

	RCC->CFGR = (																										/*
 				SW: system clock switch:
 					00: HSI
 					01: HSE
 					10: PLL
 					11: ERROR																							*/
			((config->SYS_CLK_source << RCC_CFGR_SW_Pos) & RCC_CFGR_SW_Msk)				|								/*
 				HPRE: AHB prescaler. Info:
					the clocks are divided with the new prescaler factor from 1 to 16 AHB cycles after HPRE write.
					0xxx: clock not divided
					1000: clock divided by 2
					1001: clock divided by 4
					1010: clock divided by 8
					1011: clock divided by 16
					1100: clock divided by 64
					1101: clock divided by 128
					1110: clock divided by 256
					1111: clock divided by 512																			*/
			((config->AHB_prescaler << RCC_CFGR_HPRE_Pos) & RCC_CFGR_HPRE_Msk)			|								/*
				PPRE1: APB1 prescaler:
					0xx: clock not divided
					1xx: clock divided by (2 << xx)																		*/
			((config->APB1_prescaler << RCC_CFGR_PPRE1_Pos) & RCC_CFGR_PPRE1_Msk)		|								/*
				PPRE2: APB2 prescaler:
					0xx: clock not divided
					1xx: clock divided by (2 << xx)																		*/
			((config->APB2_prescaler << RCC_CFGR_PPRE2_Pos) & RCC_CFGR_PPRE2_Msk)		|								/*
 				RTCPRE: HSE prescaler for RTC:
 					00000: no clock
 					00001: no clock
 					00010: HSE / 2
 					...
 					11111: HSE / 31																						*/
			((config->RTC_prescaler << RCC_CFGR_RTCPRE_Pos) & RCC_CFGR_RTCPRE_Msk)		|								/*
 				MCO1: micro controller clock output 1:
 					00: HSI
 					01: LSE
 					10: HSE
 					11: PLL																								*/
			((config->MCO1_source << RCC_CFGR_MCO1_Pos) & RCC_CFGR_MCO1_Msk)			|								/*
				I2SSRC: I2S clock source:
					0: PLLI2S selected
					1: External clock mapped on the I2S_CKIN pin used as I2S clock										*/
			(config->I2S_external_clock * RCC_CFGR_I2SSRC)								|								/*
 				MCO1PRE: MCO1 prescaler:
 					0xx: no division
 					1xx: clock divided by (2 + xx)																		*/
			((config->MCO1_prescaler << RCC_CFGR_MCO1PRE_Pos) & RCC_CFGR_MCO1PRE_Msk)	|								/*
 				MCO2PRE: MCO2 prescaler:
 					0xx: no division
 					1xx: clock divided by (2 + xx)																		*/
			((config->MCO2_prescaler << RCC_CFGR_MCO2PRE_Pos) & RCC_CFGR_MCO2PRE_Msk)	|								/*
 				MCO2: micro controller clock output 2:
 					00: SYSCLK
 					01: PLLI2S
 					10: HSE
 					11: PLL																								*/
			((config->MCO2_source << RCC_CFGR_MCO2_Pos) & RCC_CFGR_MCO2_Msk)
	);

	while((RCC->CFGR & RCC_CFGR_SWS) != ((config->SYS_CLK_source << RCC_CFGR_SWS_Pos) & RCC_CFGR_SWS_Msk)) { /* wait untill the selected clock is enabled*/ }

	/* configure SysTick timer. By default the clock source of SysTick is AHB/8 = 12.5 MHz */
	SysTick->LOAD = config->SYS_tick_reload - 1;			/* set reload register */
	SysTick->VAL  = config->SYS_tick_start_value - 1;		/* load counter value  */
	SysTick->CTRL = config->SYS_tick_control;				/* start SysTick timer */
}