//
// Created by marijn on 3/1/23.
//

#ifndef STM32F_CLOCK_H
#define STM32F_CLOCK_H

#include "main.h"


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
	FLASH_LATENCY1 =         0b000,  //    0 < H_CLK ≤ 30     0 < H_CLK ≤ 24     0 < H_CLK ≤ 18     0 < H_CLK ≤ 16   |
	FLASH_LATENCY2 =         0b001,  //    30 < H_CLK ≤ 60    24 < H_CLK ≤ 48    18 < H_CLK ≤ 36    16 < H_CLK ≤ 32  |
	FLASH_LATENCY3 =         0b010,  //    60 < H_CLK ≤ 84    48 < H_CLK ≤ 72    36 < H_CLK ≤ 54    32 < H_CLK ≤ 48  |
	FLASH_LATENCY4 =         0b011,  //           -           72 < H_CLK ≤ 84    54 < H_CLK ≤ 72    48 < H_CLK ≤ 64  |
	FLASH_LATENCY5 =         0b100,  //           -                  -           72 < H_CLK ≤ 84    64 < H_CLK ≤ 80  |
	FLASH_LATENCY6 =         0b101,  //           -                  -                  -           80 < H_CLK ≤ 84  |
	FLASH_LATENCY7 =         0b110,  //           -                  -                  -                  -         |
	FLASH_LATENCY8 =         0b111   //           -                  -                  -                  -         |
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

typedef struct {  //					: 136
	// RCC_PLL_CFGR config				: 22
	uint32_t PLL_M						: 6;
	uint32_t PLL_N						: 9;
	uint32_t PLL_P						: 2;  // PLL_P_TypeDef
	uint32_t PLL_Q						: 4;  // (0, 1 are invalid)
	uint32_t PLL_source					: 1;  // PLL_Source_TypeDef
	// FLASH_ACR config					: 6
	uint32_t FLASH_latency				: 3;  // FLASH_LATENCY_TypeDef
	uint32_t FLASH_prefetch				: 1;
	uint32_t FLASH_instruction_cache	: 1;
	uint32_t FLASH_data_cache			: 1;
	// RCC_CFGR config					: 28
	uint32_t SYS_CLK_source				: 2;  // SYS_CLK_Source_TypeDef
	uint32_t AHB_prescaler				: 4;  // AHB_CLK_Prescaler_TypeDef
	uint32_t APB1_prescaler				: 3;  // APBx_CLK_Prescaler_TypeDef
	uint32_t APB2_prescaler				: 3;  // APBx_CLK_Prescaler_TypeDef
	uint32_t RTC_prescaler				: 5;
	uint32_t MCO1_source				: 2;  // MCO1_CLK_Source_TypeDef
	uint32_t I2S_external_clock			: 1;
	uint32_t MCO1_prescaler				: 3;  // MCOx_CLK_Prescaler_TypeDef
	uint32_t MCO2_prescaler				: 3;  // MCOx_CLK_Prescaler_TypeDef
	uint32_t MCO2_source				: 2;  // MCO2_CLK_Source_TypeDef
	// SYS_TICK config					: 80
	uint32_t SYS_tick_reload			: 24;
	uint32_t SYS_tick_start_value		: 24;
	uint32_t SYS_tick_control;
} SYS_CLK_Config_TypeDef;


SYS_CLK_Config_TypeDef* new_SYS_CLK_config(void);
void sys_clock_init(SYS_CLK_Config_TypeDef* config);


#endif //STM32F
