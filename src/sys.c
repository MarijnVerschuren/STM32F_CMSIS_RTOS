//
// Created by marijn on 3/1/23.
//
#include "sys.h"


/* M = 16
 * N = 192
 * P = 2 (0)
 * Q = 4
 * */


void sys_clock_init(void) {
	// HSE / M * N / P  =  25Mhz / 15 * 120 / 2 = 100Mhz
	RCC->PLLCFGR = (																								/*
				PLL_M: division factor for the main PLL and audio PLL (PLLI2S) input clock. Info:
					the software has to set these bits correctly to ensure that the VCO
					input frequency ranges from 1 to 2 MHz. It is recommended to select
					a frequency of 2 MHz to limit PLL jitter.														*/
			((15 << RCC_PLLCFGR_PLLM_Pos) & RCC_PLLCFGR_PLLM_Msk)	|												/*
				PLL_N: main PLL multiplication factor for VCO. Info:
					the software has to set these bits correctly to ensure that
					the VCO output frequency is between 192 and 432 MHz.											*/
			((120 << RCC_PLLCFGR_PLLN_Pos) & RCC_PLLCFGR_PLLN_Msk)	|												/*
				PLL_P: main PLL division factor for main system clock
				PLL output clock frequency = VCO frequency / PLL_P:
					00: PLLP = 2
					01: PLLP = 4
					10: PLLP = 6
					11: PLLP = 8																					*/
			((0 << RCC_PLLCFGR_PLLP_Pos) & RCC_PLLCFGR_PLLP_Msk)	|												/*
				PLLSRC: PLL and PLLI2S clock source:
 					0: HSI
					1: HSE																							*/
			(1 * RCC_PLLCFGR_PLLSRC_HSE)							|												/*
 				PLL_Q: main PLL division factor for USB OTG FS, SDIO and RNG clocks									*/
			((0 << RCC_PLLCFGR_PLLQ_Pos) & RCC_PLLCFGR_PLLQ_Msk)
	);

	// turn on HSE and PLL
	RCC->CR = (
			RCC_CR_HSEON |	/* switch HSE ON */
			RCC_CR_PLLON	/* switch PLL ON */
	);


	//PWR->CR = PWR_CR_VOS_1 | PWR_CR_DBP; /*  Enable Backup Domain Access (leave VOS default)       */
	//RCC->BDCR = (
	//		RCC_BDCR_LSEON                   | /*  Switch HSE ON                                         */
	//		RCC_BDCR_RTCSEL_0                | /*  LSE oscillator clock used as RTC clock                */
	//		RCC_BDCR_RTCEN                     /*  RTC clock enable                                      */
	//);


	/* Relation between CPU clock frequency and flash memory read time.
	To correctly read data from flash memory, the number of wait states (LATENCY) must be
	correctly programmed in the flash access control register (FLASH_ACR) according to the
	frequency of the CPU clock (HCLK) and the supply voltage of the device.
	The prefetch buffer must be disabled when the supply voltage is below 2.1 V. The
	correspondence between wait states and CPU clock frequency is given in Table 6.
	- when VOS[1:0] = 0x01, the maximum frequency of HCLK = 60 MHz.
	- when VOS[1:0] = 0x10, the maximum frequency of HCLK = 84 MHz.

	table 6. Number of wait states according to CPU clock (HCLK) frequency
	===========================================================================================
	wait states (WS)	|							HCLK (MHz)
	(LATENCY)			|  Voltage range     Voltage range     Voltage range     Voltage range
						|  2.7 V - 3.6 V     2.4 V - 2.7 V     2.1 V - 2.4 V     1.71 V - 2.1 V
	===========================================================================================
	0 WS (1 CPU cycle)    0 < HCLK ≤ 30     0 < HCLK ≤ 24     0 < HCLK ≤ 18     0 < HCLK ≤ 16
	1 WS (2 CPU cycles)  30 < HCLK ≤ 60    24 < HCLK ≤ 48    18 < HCLK ≤ 36    16 < HCLK ≤ 32
	2 WS (3 CPU cycles)  60 < HCLK ≤ 84    48 < HCLK ≤ 72    36 < HCLK ≤ 54    32 < HCLK ≤ 48
	3 WS (4 CPU cycles)        -           72 < HCLK ≤ 84    54 < HCLK ≤ 72    48 < HCLK ≤ 64
	4 WS (5 CPU cycles)        -                 -           72 < HCLK ≤ 84    64 < HCLK ≤ 80
	5 WS (6 CPU cycles)        -                 -                 -           80 < HCLK ≤ 84
	*/

	FLASH->ACR = (																									/*
 				LATENCY: flash read latency (look in table above for more info)										*/
			((2 << FLASH_ACR_LATENCY_Pos) & FLASH_ACR_LATENCY_7WS)	|
			(1 * FLASH_ACR_PRFTEN)									|	/* enable prefetch							*/
			(1 * FLASH_ACR_ICEN)									|	/* enable instruction cache					*/
			(1 * FLASH_ACR_DCEN)										/* enable data cache						*/
	);

	while(!(RCC->CR & RCC_CR_HSERDY)) { /* wait till HSE is ready */ }
	while(!(RCC->CR & RCC_CR_PLLRDY)) { /* wait till PLL is ready */ }

	RCC->CFGR = (																									/*
 				SW: system clock switch:
 					00: HSI
 					01: HSE
 					10: PLL
 					11: ERROR																						*/
			((2 << RCC_CFGR_SW_Pos) & RCC_CFGR_SW_Msk)			|													/*
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
					1111: clock divided by 512																		*/
			((0 << RCC_CFGR_HPRE_Pos) & RCC_CFGR_HPRE_Msk)			|												/*
				PPRE1: APB1 prescaler:
					0xx: clock not divided
					1xx: clock divided by (2 << xx)																	*/
			((4 << RCC_CFGR_PPRE1_Pos) & RCC_CFGR_PPRE1_Msk)		|												/*
				PPRE2: APB2 prescaler:
					0xx: clock not divided
					1xx: clock divided by (2 << xx)																	*/
			((0 << RCC_CFGR_PPRE2_Pos) & RCC_CFGR_PPRE2_Msk)		|												/*
 				RTCPRE: HSE prescaler for RTC:
 					00000: no clock
 					00001: no clock
 					00010: HSE / 2
 					...
 					11111: HSE / 31																					*/
			((0 << RCC_CFGR_RTCPRE_Pos) & RCC_CFGR_RTCPRE_Msk)		|												/*
 				MCO1: micro controller clock output 1:
 					00: HSI
 					01: LSE
 					10: HSE
 					11: PLL																							*/
			((0 << RCC_CFGR_MCO1_Pos) & RCC_CFGR_MCO1_Msk)			|												/*
				I2SSRC: I2S clock source:
					0: PLLI2S selected
					1: External clock mapped on the I2S_CKIN pin used as I2S clock									*/
			(0 * RCC_CFGR_I2SSRC)									|												/*
 				MCO1PRE: MCO1 prescaler:
 					0xx: no division
 					1xx: clock divided by (2 + xx)																	*/
			((0 << RCC_CFGR_MCO1PRE_Pos) & RCC_CFGR_MCO1PRE_Msk)	|												/*
 				MCO2PRE: MCO2 prescaler:
 					0xx: no division
 					1xx: clock divided by (2 + xx)																	*/
			((0 << RCC_CFGR_MCO2PRE_Pos) & RCC_CFGR_MCO2PRE_Msk)	|												/*
 				MCO2: micro controller clock output 2:
 					00: SYSCLK
 					01: PLLI2S
 					10: HSE
 					11: PLL																							*/
			((0 << RCC_CFGR_MCO2_Pos) & RCC_CFGR_MCO2_Msk)
	);

	while((RCC->CFGR & RCC_CFGR_SWS) != ((2 << RCC_CFGR_SWS_Pos) & RCC_CFGR_SWS_Msk)) { /* wait untill the selected clock is enabled*/ }

	/* configure SysTick timer. By default the clock source of SysTick is AHB/8 = 12.5 MHz */
	SysTick->LOAD = 12500UL - 1;                /* set reload register */
	SysTick->VAL  = 1000UL - 1;                 /* load counter value  */
	SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;    /* start SysTick timer */
}