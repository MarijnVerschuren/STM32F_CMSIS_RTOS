//
// Created by marijn on 3/27/23.
//
#include "pwm.h"



/*!< static */
static inline void PWM_GPIO_to_args(uint32_t pwm_pin, TIM_TypeDef** tim, uint8_t* channel, uint8_t* alternate_function, GPIO_TypeDef** port, uint8_t* pin) {
	uint8_t dev_id =		(pwm_pin >> 16);
	(*tim) =				id_to_dev(*((dev_id_t*)&dev_id));
	(*channel) =			(pwm_pin >> 14) & 0x3u;
	(*alternate_function) =	(pwm_pin >> 8) & 0xfu;
	(*port) =				int_to_GPIO(pwm_pin >> 4);
	(*pin) =				pwm_pin & 0xfu;
}


/*!< init / enable / disable */
void config_PWM(PWM_GPIO_t pwm_pin, uint32_t prescaler, uint32_t period) {
	TIM_TypeDef* tim; GPIO_TypeDef* port;	// registers
	uint8_t channel, pin, af;				// params
	PWM_GPIO_to_args(pwm_pin, &tim, &channel, &af, &port, &pin);
	config_TIM(tim, prescaler, period);
	(&tim->CCMR1)[channel >> 1] &=	~(0x3u << ((channel & 0b1u) << 3));	// clear register
	tim->CCER &=					~(0b1u << (1 + (channel << 2)));	// default polarity
	(&tim->CCMR1)[channel >> 1] |=	(
		(0b110 << (4 + ((channel & 0b1u) << 3))) |						// set PWM mode
		(0b1u << (3 + ((channel & 0b1u) << 3)))							// enable preload
	);
	tim->CR1 |= TIM_CR1_ARPE;											// enable auto-reload preload
	tim->CCER |= (0b1u << (channel << 2));								// enable capture compare
	(&tim->CCR1)[channel] = 0;											// set duty cycle to 0
	tim->CR1 |= TIM_CR1_CEN;											// start timer
}
/*{
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
}*/
