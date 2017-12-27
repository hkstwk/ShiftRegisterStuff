#include <avr/io.h>
#include <util/delay.h>
#include "../shift_register.h"
#include "../defines.h"

extern volatile uint16_t ledPins;

uint16_t ROL(uint16_t bitPattern, unsigned char digits){
	return (bitPattern << digits) | bitPattern >> (INT_BITS - digits);
}

uint16_t ROR(uint16_t bitPattern, unsigned char digits){
	return (bitPattern >> digits) | bitPattern << (INT_BITS - digits);
}

void leftRotate(unsigned char loops, uint16_t bitPattern){
	ledPins = bitPattern;
	while (loops > 0){
		ledPins = ROL(ledPins, 1);
		_delay_ms(5*DELAY);
		loops--;
	}
}

void rightRotate(unsigned char loops, uint16_t bitPattern){
	ledPins = bitPattern;
	while (loops > 0){
		ledPins = ROR(ledPins, 1);
		_delay_ms(5*DELAY);
		loops--;
	}
}
