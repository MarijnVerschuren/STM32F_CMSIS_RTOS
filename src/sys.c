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
	RCC->PLLCFGR = (																	/*
				PLL_M: division factor for the main PLL and audio PLL (PLLI2S) input clock caution:
					the software has to set these bits correctly to ensure that the VCO
					input frequency ranges from 1 to 2 MHz. It is recommended to select
					a frequency of 2 MHz to limit PLL jitter.							*/
			((15 << RCC_PLLCFGR_PLLM_Pos) & RCC_PLLCFGR_PLLM_Msk)	|					/*
				PLL_N: main PLL multiplication factor for VCO. Caution:
					the software has to set these bits correctly to ensure that
					the VCO output frequency is between 192 and 432 MHz.				*/
			((120 << RCC_PLLCFGR_PLLN_Pos) & RCC_PLLCFGR_PLLN_Msk)	|					/*
				PLL_P: main PLL division factor for main system clock
				PLL output clock frequency = VCO frequency / PLL_P with:
					00: PLLP = 2
					01: PLLP = 4
					10: PLLP = 6
					11: PLLP = 8														*/
			((0 << RCC_PLLCFGR_PLLP_Pos) & RCC_PLLCFGR_PLLP_Msk)	|					/*
 				0: HSI clock selected as PLL and PLLI2S clock
				1: HSE oscillator clock selected as PLL and PLLI2S clock				*/
			(1 * RCC_PLLCFGR_PLLSRC_HSE)							|					/*
 				PLL_Q: main PLL division factor for USB OTG FS, SDIO and RNG clocks		*/
			((0 << RCC_PLLCFGR_PLLQ_Pos) & RCC_PLLCFGR_PLLQ_Msk)
	);

	RCC->CR = (
			RCC_CR_HSEON |	/* Switch HSE ON */
			RCC_CR_PLLON	/* Switch PLL ON */
	);


	// TODO:

	/*
    After system reset, the RTC registers are protected against parasitic write access with the
    DBP bit of the PWR power control register (PWR_CR). The DBP bit must be set to enable
    RTC registers write access.                                                                  */

	PWR->CR = PWR_CR_VOS_1 | PWR_CR_DBP; /*  Enable Backup Domain Access (leave VOS default)       */
	RCC->BDCR = (
			RCC_BDCR_LSEON                   | /*  Switch HSE ON                                         */
			RCC_BDCR_RTCSEL_0                | /*  LSE oscillator clock used as RTC clock                */
			RCC_BDCR_RTCEN                     /*  RTC clock enable                                      */
	);


	// while(!(RCC->BDCR & RCC_BDCR_LSERDY)) { /*  */ }

	// PWR->CR = PWR_CR_VOS_1;              /*  Disable Backup Domain Access                          */
	// RCC->APB1ENR = 0;                    /*  Disable PWR interface                                 */

	/*
	Relation between CPU clock frequency and Flash memory read time
	To correctly read data from Flash memory, the number of wait states (LATENCY) must be
	correctly programmed in the Flash access control register (FLASH_ACR) according to the
	frequency of the CPU clock (HCLK) and the supply voltage of the device.
	The prefetch buffer must be disabled when the supply voltage is below 2.1 V. The
	correspondence between wait states and CPU clock frequency is given in Table 6.
	- when VOS[1:0] = 0x01, the maximum value of fHCLK = 60 MHz.
	- when VOS[1:0] = 0x10, the maximum value of fHCLK = 84 MHz.

	Table 6. Number of wait states according to CPU clock (HCLK) frequency
	===========================================================================================
	Wait states (WS)  |                         HCLK (MHz)
	(LATENCY)      |  Voltage range     Voltage range     Voltage range     Voltage range
	|  2.7 V - 3.6 V     2.4 V - 2.7 V     2.1 V - 2.4 V     1.71 V - 2.1 V
	===========================================================================================
	0 WS (1 CPU cycle)    0 < HCLK ≤ 30     0 < HCLK ≤ 24     0 < HCLK ≤ 18     0 < HCLK ≤ 16
	1 WS (2 CPU cycles)  30 < HCLK ≤ 60    24 < HCLK ≤ 48    18 < HCLK ≤ 36    16 < HCLK ≤ 32
	2 WS (3 CPU cycles)  60 < HCLK ≤ 84    48 < HCLK ≤ 72    36 < HCLK ≤ 54    32 < HCLK ≤ 48
	3 WS (4 CPU cycles)        -           72 < HCLK ≤ 84    54 < HCLK ≤ 72    48 < HCLK ≤ 64
	4 WS (5 CPU cycles)        -                 -           72 < HCLK ≤ 84    64 < HCLK ≤ 80
	5 WS (6 CPU cycles)        -                 -                 -           80 < HCLK ≤ 84
	*/

	FLASH->ACR = (
			0 * FLASH_ACR_LATENCY_0WS        | /*                                                        */
			0 * FLASH_ACR_LATENCY_1WS        | /*                                                        */
			1 * FLASH_ACR_LATENCY_2WS        | /*                                                        */
			0 * FLASH_ACR_LATENCY_3WS        | /*                                                        */
			0 * FLASH_ACR_LATENCY_4WS        | /*                                                        */
			0 * FLASH_ACR_LATENCY_5WS        | /*                                                        */
			0 * FLASH_ACR_LATENCY_6WS        | /*                                                        */
			0 * FLASH_ACR_LATENCY_7WS        | /*                                                        */
			1 * FLASH_ACR_PRFTEN             | /*  0x00000100                                            */
			1 * FLASH_ACR_ICEN               | /*  0x00000200                                            */
			1 * FLASH_ACR_DCEN               | /*  0x00000400                                            */
			0 * FLASH_ACR_ICRST              | /*  0x00000800                                            */
			0 * FLASH_ACR_DCRST              | /*  0x00001000                                            */
			0 * FLASH_ACR_BYTE0_ADDRESS      | /*  0x40023C00                                            */
			0 * FLASH_ACR_BYTE2_ADDRESS        /*  0x40023C03                                            */
	);

	while(!(RCC->CR & RCC_CR_HSERDY))  { /*  Wait till HSE is ready */ }
	while(!(RCC->CR & RCC_CR_PLLRDY))  { /*  Wait till PLL is ready */ }

	RCC->CFGR = (
		0 * RCC_CFGR_SW                  | /*  0x00000003 SW[1:0] bits (System clock Switch)         */
		0 * RCC_CFGR_SW_0                | /*    0x00000001                                          */
		0 * RCC_CFGR_SW_1                | /*    0x00000002                                          */
		0 * RCC_CFGR_SW_HSI              | /*    HSI selected as system clock                        */
		0 * RCC_CFGR_SW_HSE              | /*    HSE selected as system clock                        */
		1 * RCC_CFGR_SW_PLL              | /*    PLL selected as system clock                        */

		0 * RCC_CFGR_SWS                 | /*  0x0000000C SWS[1:0] bits (System Clock Switch Status) */
		0 * RCC_CFGR_SWS_0               | /*    0x00000004                                          */
		0 * RCC_CFGR_SWS_1               | /*    0x00000008                                          */
		0 * RCC_CFGR_SWS_HSI             | /*    HSI oscillator used as system clock                 */
		0 * RCC_CFGR_SWS_HSE             | /*    HSE oscillator used as system clock                 */
		0 * RCC_CFGR_SWS_PLL             | /*    PLL used as system clock                            */
		/*
		HPRE: AHB prescaler
		Set and cleared by software to control AHB clock division factor.
		Caution: The clocks are divided with the new prescaler factor
		from 1 to 16 AHB cycles after HPRE write.
		0xxx: system clock not divided
		1000: system clock divided by 2
		1001: system clock divided by 4
		1010: system clock divided by 8
		1011: system clock divided by 16
		1100: system clock divided by 64
		1101: system clock divided by 128
		1110: system clock divided by 256
		1111: system clock divided by 512
		================================================================================          */

		0 * RCC_CFGR_HPRE                | /*  0x000000F0 HPRE[3:0] bits (AHB prescaler)             */
		0 * RCC_CFGR_HPRE_0              | /*    0x00000010                                          */
		0 * RCC_CFGR_HPRE_1              | /*    0x00000020                                          */
		0 * RCC_CFGR_HPRE_2              | /*    0x00000040                                          */
		0 * RCC_CFGR_HPRE_3              | /*    0x00000080                                          */
		1 * RCC_CFGR_HPRE_DIV1           | /*    SYSCLK not divided                                  */
		0 * RCC_CFGR_HPRE_DIV2           | /*    SYSCLK divided by 2                                 */
		0 * RCC_CFGR_HPRE_DIV4           | /*    SYSCLK divided by 4                                 */
		0 * RCC_CFGR_HPRE_DIV8           | /*    SYSCLK divided by 8                                 */
		0 * RCC_CFGR_HPRE_DIV16          | /*    SYSCLK divided by 16                                */
		0 * RCC_CFGR_HPRE_DIV64          | /*    SYSCLK divided by 64                                */
		0 * RCC_CFGR_HPRE_DIV128         | /*    SYSCLK divided by 128                               */
		0 * RCC_CFGR_HPRE_DIV256         | /*    SYSCLK divided by 256                               */
		0 * RCC_CFGR_HPRE_DIV512         | /*    SYSCLK divided by 512                               */

		0 * RCC_CFGR_PPRE1               | /*  0x00001C00 PRE1[2:0] bits (APB1 prescaler)            */
		0 * RCC_CFGR_PPRE1_0             | /*    0x00000400                                          */
		0 * RCC_CFGR_PPRE1_1             | /*    0x00000800                                          */
		0 * RCC_CFGR_PPRE1_2             | /*    0x00001000                                          */
		0 * RCC_CFGR_PPRE1_DIV1          | /*    HCLK not divided                                    */
		1 * RCC_CFGR_PPRE1_DIV2          | /*    HCLK divided by 2                                   */
		0 * RCC_CFGR_PPRE1_DIV4          | /*    HCLK divided by 4                                   */
		0 * RCC_CFGR_PPRE1_DIV8          | /*    HCLK divided by 8                                   */
		0 * RCC_CFGR_PPRE1_DIV16         | /*    HCLK divided by 16                                  */

		0 * RCC_CFGR_PPRE2               | /*  0x0000E000 PRE2[2:0] bits (APB2 prescaler)            */
		0 * RCC_CFGR_PPRE2_0             | /*    0x00002000                                          */
		0 * RCC_CFGR_PPRE2_1             | /*    0x00004000                                          */
		0 * RCC_CFGR_PPRE2_2             | /*    0x00008000                                          */
		1 * RCC_CFGR_PPRE2_DIV1          | /*    HCLK not divided                                    */
		0 * RCC_CFGR_PPRE2_DIV2          | /*    HCLK divided by 2                                   */
		0 * RCC_CFGR_PPRE2_DIV4          | /*    HCLK divided by 4                                   */
		0 * RCC_CFGR_PPRE2_DIV8          | /*    HCLK divided by 8                                   */
		0 * RCC_CFGR_PPRE2_DIV16         | /*    HCLK divided by 16                                  */

		0 * RCC_CFGR_RTCPRE              | /*  0x001F0000                                            */
		0 * RCC_CFGR_RTCPRE_0            | /*    0x00010000                                          */
		0 * RCC_CFGR_RTCPRE_1            | /*    0x00020000                                          */
		0 * RCC_CFGR_RTCPRE_2            | /*    0x00040000                                          */
		0 * RCC_CFGR_RTCPRE_3            | /*    0x00080000                                          */
		0 * RCC_CFGR_RTCPRE_4            | /*    0x00100000                                          */

		0 * RCC_CFGR_MCO1                | /*  0x00600000                                            */
		0 * RCC_CFGR_MCO1_0              | /*    0x00200000                                          */
		0 * RCC_CFGR_MCO1_1              | /*    0x00400000                                          */

		0 * RCC_CFGR_I2SSRC              | /*  0x00800000                                            */

		0 * RCC_CFGR_MCO1PRE             | /*  0x07000000                                            */
		0 * RCC_CFGR_MCO1PRE_0           | /*    0x01000000                                          */
		0 * RCC_CFGR_MCO1PRE_1           | /*    0x02000000                                          */
		0 * RCC_CFGR_MCO1PRE_2           | /*    0x04000000                                          */

		0 * RCC_CFGR_MCO2PRE             | /*  0x38000000                                            */
		0 * RCC_CFGR_MCO2PRE_0           | /*    0x08000000                                          */
		0 * RCC_CFGR_MCO2PRE_1           | /*    0x10000000                                          */
		0 * RCC_CFGR_MCO2PRE_2           | /*    0x20000000                                          */

		0 * RCC_CFGR_MCO2                | /*  0xC0000000                                            */
		0 * RCC_CFGR_MCO2_0              | /*    0x40000000                                          */
		0 * RCC_CFGR_MCO2_1                /*    0x80000000                                          */
	);
	/*
Wait till System clock is ready
===============================                               */

	while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) { /*  */ }
	/*
Configure SysTick timer
By default the clock source of SysTick is AHB/8 = 12.5 MHz.
===========================================================                */

	SysTick->LOAD = 12500UL - 1;                /* set reload register */
	SysTick->VAL  = 1000UL - 1;                 /* load counter value  */
	SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;    /* start SysTick timer */
}