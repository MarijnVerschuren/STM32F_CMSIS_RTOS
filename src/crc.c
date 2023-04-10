//
// Created by marijn on 4/5/23.
//

#include "crc.h"


/*!< enable / disable */
void enable_CRC(void) {
	RCC->AHB1ENR |= RCC_AHB1ENR_CRCEN;	// enable crc clock
	reset_CRC();
}
void disable_CRC(void) {
	RCC->AHB1ENR &= ~RCC_AHB1ENR_CRCEN;	// disable crc clock
}


/*!< usage */
void reset_CRC(void) {
	CRC->CR |= CRC_CR_RESET;			// reset crc
	while (CRC->CR & CRC_CR_RESET);		// wait until done
}