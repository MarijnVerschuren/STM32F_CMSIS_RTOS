//
// Created by marijn on 4/5/23.
//

#include "crc.h"


void enable_CRC(void) {
	RCC->AHB1ENR |= RCC_AHB1ENR_CRCEN;	// enable crc clock
	CRC->CR |= 0b1u;					// reset crc
}
void disable_CRC(void) {
	RCC->AHB1ENR |= RCC_AHB1ENR_CRCEN;	// enable crc clock
}