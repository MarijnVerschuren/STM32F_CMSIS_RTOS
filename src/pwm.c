//
// Created by marijn on 3/27/23.
//
#include "pwm.h"


void config_PWM(TIM_TypeDef* tim, TIM_channel_TypeDef channel, PWM_GPIO_TypeDef pin, uint32_t prescaler, uint32_t period, uint8_t invert_polarity) {
	uint8_t af = (pin >> 8);
	// arguments passed to the int_to_port function are filtered to be <= 0x7
	GPIO_TypeDef* port = int_to_GPIO(pin >> 4);
	pin &= 0xf;
	fconfig_GPIO(port, pin, GPIO_alt_func, GPIO_no_pull, GPIO_push_pull, GPIO_very_high_speed, af);
	uint8_t pos = (channel << 2);
	tim->CCER &= ~(1 << (pos + 1));	// clear bit
	tim->CCER |= invert_polarity << (pos + 1);
	if (channel & 0x2) {
		// clear capture compare mode and output compare bits
		tim->CCMR2 &= ~((0x3u << ((channel & 0b1u) << 3)) | (0x7u << (4 + ((channel & 0b1u) << 3))));
		tim->CCMR2 |= 0b110 << (4 + ((channel & 0b1u) << 3));	// set mode to PWM (0b110)
		tim->CCMR2 |= 0b1 << (3 + ((channel & 0b1u) << 3));		// enable preload
	} else {
		// clear capture compare mode and output compare bits
		tim->CCMR1 &= ~((0x3u << ((channel & 0b1u) << 3)) | (0x7u << (4 + ((channel & 0b1u) << 3))));
		tim->CCMR1 |= 0b110 << (4 + ((channel & 0b1u) << 3));	// set mode to PWM (0b110)
		tim->CCMR1 |= 0b1 << (3 + ((channel & 0b1u) << 3));		// enable preload
	}
	tim->CR1 |= TIM_CR1_ARPE;
	tim->PSC = prescaler;
	tim->ARR = period;
	tim->CCR4 = 50;
	tim->CCER |= 1 << pos;
	tim->CR1 |= TIM_CR1_CEN;
}
