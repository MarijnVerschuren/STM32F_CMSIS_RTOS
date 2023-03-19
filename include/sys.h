//
// Created by marijn on 3/1/23.
//

#ifndef STM32F_CLOCK_H
#define STM32F_CLOCK_H

#include "main.h"


/* M = 16
 * N = 192
 * P = 2 (0)
 * Q = 4
 * */
typedef enum {
	PLL_P_DIV2 = 0b00,
	PLL_P_DIV4 = 0b01,
	PLL_P_DIV6 = 0b10,
	PLL_P_DIV8 = 0b11
} PLL_P_TypeDef;

typedef enum {
	PLL_SRC_HSI = 0b0,
	PLL_SRC_HSE = 0b1
} PLL_Source_TypeDef;

typedef enum {
	//===============================================================================================================|
	// flash access latency |                                            H_CLK (MHz)                                 |
	//      (table 6)       |              voltage range      voltage range      voltage range      voltage range    |
	//    in CPU cycles     |__________      2V7 - 3V6          2V4 - 2V7          2V1 - 2V4          1V71 - 2V1     |
	FLASH_LATENCY_1_CYCLES = 0b000,  //    0 < H_CLK ≤ 30     0 < H_CLK ≤ 24     0 < H_CLK ≤ 18     0 < H_CLK ≤ 16   |
	FLASH_LATENCY_2_CYCLES = 0b001,  //    30 < H_CLK ≤ 60    24 < H_CLK ≤ 48    18 < H_CLK ≤ 36    16 < H_CLK ≤ 32  |
	FLASH_LATENCY_3_CYCLES = 0b010,  //    60 < H_CLK ≤ 84    48 < H_CLK ≤ 72    36 < H_CLK ≤ 54    32 < H_CLK ≤ 48  |
	FLASH_LATENCY_4_CYCLES = 0b011,  //           -           72 < H_CLK ≤ 84    54 < H_CLK ≤ 72    48 < H_CLK ≤ 64  |
	FLASH_LATENCY_5_CYCLES = 0b100,  //           -                  -           72 < H_CLK ≤ 84    64 < H_CLK ≤ 80  |
	FLASH_LATENCY_6_CYCLES = 0b101,  //           -                  -                  -           80 < H_CLK ≤ 84  |
	FLASH_LATENCY_7_CYCLES = 0b110,  //           -                  -                  -                  -         |
	FLASH_LATENCY_8_CYCLES = 0b111   //           -                  -                  -                  -         |
	//===============================================================================================================|
} FLASH_LATENCY_TypeDef;

typedef enum {
	SYS_CLK_SRC_HSI = 0b00,
	SYS_CLK_SRC_HSE = 0b01,
	SYS_CLK_SRC_PLL = 0b10
} SYS_CLK_Source_TypeDef;

typedef enum {
	AHB_CLK_NO_DIV =	0b0000,
	AHB_CLK_DIV2 =		0b1000,
	AHB_CLK_DIV4 =		0b1001,
	AHB_CLK_DIV8 =		0b1010,
	AHB_CLK_DIV16 =		0b1011,
	AHB_CLK_DIV64 =		0b1100,
	AHB_CLK_DIV128 =	0b1101,
	AHB_CLK_DIV256 =	0b1110,
	AHB_CLK_DIV512 =	0b1111
} AHB_CLK_Prescaler_TypeDef;

typedef enum {
	APBx_CLK_NO_DIV =	0b000,
	APBx_CLK_DIV2 =		0b100,
	APBx_CLK_DIV4 =		0b101,
	APBx_CLK_DIV8 =		0b110,
	APBx_CLK_DIV16 =	0b111
} APBx_CLK_Prescaler_TypeDef;

typedef enum {
	MCO1_CLK_SRC_HSI = 0b00,
	MCO1_CLK_SRC_LSE = 0b01,
	MCO1_CLK_SRC_HSE = 0b10,
	MCO1_CLK_SRC_PLL = 0b11
} MCO1_CLK_Source_TypeDef;

typedef enum {
	MCO2_CLK_SRC_SYS_CLK =	0b00,
	MCO2_CLK_SRC_PLL_I2S =	0b01,
	MCO2_CLK_SRC_HSE =		0b10,
	MCO2_CLK_SRC_PLL =		0b11
} MCO2_CLK_Source_TypeDef;

typedef enum {
	MCOx_CLK_NO_DIV =	0b000,
	MCOx_CLK_DIV2 =		0b100,
	MCOx_CLK_DIV3 =		0b101,
	MCOx_CLK_DIV4 =		0b110,
	MCOx_CLK_DIV5 =		0b111
} MCOx_CLK_Prescaler_TypeDef;

typedef struct {
	// RCC_PLL_CFGR config
	uint32_t PLL_M								: 6;
	uint32_t PLL_N								: 9;
	PLL_P_TypeDef PLL_P							: 2;
	uint32_t PLL_Q								: 4;  // (0, 1 are invalid)
	PLL_Source_TypeDef PLL_source				: 1;
	// FLASH_ACR config
	FLASH_LATENCY_TypeDef FLASH_latency			: 3;
	uint32_t FLASH_prefetch						: 1;
	uint32_t FLASH_instruction_cache			: 1;
	uint32_t FLASH_data_cache					: 1;
	// RCC_CFGR config
	SYS_CLK_Source_TypeDef SYS_CLK_source		: 2;
	AHB_CLK_Prescaler_TypeDef AHB_prescaler		: 4;
	APBx_CLK_Prescaler_TypeDef APB1_prescaler	: 3;
	APBx_CLK_Prescaler_TypeDef APB2_prescaler	: 3;
	uint32_t RTC_prescaler						: 5;
	MCO1_CLK_Source_TypeDef MCO1_source			: 2;
	uint32_t I2S_external_clock					: 1;
	MCOx_CLK_Prescaler_TypeDef MCO1_prescaler	: 3;
	MCOx_CLK_Prescaler_TypeDef MCO2_prescaler	: 3;
	MCO2_CLK_Source_TypeDef MCO2_source			: 2;
	// SYS_TICK config
	uint32_t SYS_tick_reload					: 24;
	uint32_t SYS_tick_start_value				: 24;
	uint32_t SYS_tick_control;
} SYS_CLK_Config_TypeDef;

void sys_clock_init(SYS_CLK_Config_TypeDef* config);

#endif //STM32F
