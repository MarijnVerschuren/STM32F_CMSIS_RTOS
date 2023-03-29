//
// Created by marijn on 3/27/23.
//

#ifndef STM32F_CMSIS_PWM_H
#define STM32F_CMSIS_PWM_H

#include "main.h"
#include "tim.h"
#include "gpio.h"


/* data layout:
 * struct:
 * 		alternate_function_number	: 4;  // MSB
 * 		port_number					: 4;
 * 		pin_number					: 4;  // LSB
 * functions:
 * 		TIMx_ETR -> external trigger (used in external counters)
 * 		TIMx_CHx -> channel output (used in pwm)
 * 		TIMx_CHxN -> channel inverse out
 * 		TIMx_BKIN -> ?
 * */
typedef enum {
	TIM_PIN_DISABLE =	0x000,
	// TIM1
	TIM1_BKIN_A6 =		0x106,	TIM1_CH1N_A7 =		0x107,
	TIM1_CH1_A8 =		0x108,	TIM1_CH2_A9 =		0x109,
	TIM1_CH3_A10 =		0x10A,	TIM1_CH4_A11 =		0x10B,
	TIM1_ETR_A12 =		0x10C,	TIM1_CH2N_B0 =		0x110,
	TIM1_CH3N_B1 =		0x111,	TIM1_BKIN_B12 =		0x11C,
	TIM1_CH1N_B13 =		0x11D,	TIM1_CH2N_B14 =		0x11E,
	TIM1_CH3N_B15 =		0x11F,	TIM1_ETR_E7 =		0x147,
	TIM1_CH1N_E8 =		0x148,	TIM1_CH1_E9 =		0x149,
	TIM1_CH2N_E10 =		0x14A,	TIM1_CH2_E11 =		0x14B,
	TIM1_CH3N_E12 =		0x14C,	TIM1_CH3_E13 =		0x14D,
	TIM1_CH4_E14 =		0x14E,	TIM1_BKIN_E15 =		0x14F,
	// TIM2
	TIM2_CH1_A0 =		0x100,	TIM2_ETR_A0 =		0x100,
	TIM2_CH2_A1 =		0x101,	TIM2_CH3_A2 =		0x102,
	TIM2_CH4_A3 =		0x103,	TIM2_CH1_A5 =		0x105,
	TIM2_ETR_A5 =		0x105,	TIM2_CH1_A15 =		0x10F,
	TIM2_ETR_A15 =		0x10F,	TIM2_CH2N_B0 =		0x110,
	TIM2_CH3N_B1 =		0x111,	TIM2_CH2_B3 =		0x113,
	TIM2_CH3_B10 =		0x11A,	TIM2_CH4_B11 =		0x11B,
	// TIM3
	TIM3_CH1_A6 =		0x206,	TIM3_CH2_A7 =		0x207,
	TIM3_CH3_B0 =		0x210,	TIM3_CH4_B1 =		0x211,
	TIM3_CH1_B4 =		0x214,	TIM3_CH2_B5 =		0x215,
	TIM3_CH1_C6 =		0x226,	TIM3_CH2_C7 =		0x227,
	TIM3_CH3_C8 =		0x228,	TIM3_CH4_C9 =		0x229,
	TIM3_ETR_D2 =		0x232,
	// TIM4
	TIM4_CH1_B6 =		0x216,	TIM4_CH2_B7 =		0x217,
	TIM4_CH3_B8 =		0x218,	TIM4_CH4_B9 =		0x219,
	TIM4_CH1_D12 =		0x23C,	TIM4_CH2_D13 =		0x23D,
	TIM4_CH3_D14 =		0x23E,	TIM4_CH4_D15 =		0x23F,
	TIM4_ETR_E0 =		0x240,
	// TIM5
	TIM5_CH1_A0 =		0x200,	TIM5_CH2_A1 =		0x201,
	TIM5_CH3_A2 =		0x202,	TIM5_CH4_A3 =		0x203,
	// TIM9
	TIM9_CH1_A2 =		0x302,	TIM9_CH2_A3 =		0x303,
	TIM9_CH1_E5 =		0x345,	TIM9_CH2_E6 =		0x346,
	// TIM10
	TIM10_CH1_B8 =		0x318,
	// TIM11
	TIM11_CH1_B9 =		0x319
} PWM_GPIO_TypeDef;
typedef enum {
	TIMx_CH1 = 0x0,
	TIMx_CH2 = 0x1,
	TIMx_CH3 = 0x2,
	TIMx_CH4 = 0x3,
} TIM_channel_TypeDef;
// TODO: https://www.youtube.com/watch?v=f6_Jnzsnwgs

void config_PWM(TIM_TypeDef* tim, TIM_channel_TypeDef channel, PWM_GPIO_TypeDef pin, uint32_t prescaler, uint32_t period, uint8_t polarity);



#endif //STM32F_CMSIS_PWM_H
