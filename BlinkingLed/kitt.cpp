#include <avr/io.h>
#include <util/delay.h>
#include "defines.h"

extern volatile uint16_t ledPins;

void knightRider3(unsigned char loops){
	if (loops == 0)
		loops = 1;
	while (loops > 0){
		for (int i=15;i>=0;i--)
		{
		   ledPins |= (_BV(i));
		   _delay_ms(DELAY);
		}
		for (int i=0;i<=15;i++)
		{
		   ledPins &= ~(_BV(i));
		   _delay_ms(DELAY);
		}
		loops--;
	}
}

void knightRider2(unsigned char loops){
	if (loops == 0)
		loops = 1;
	while (loops > 0){
		for (int i=15;i>=0;i--)
		{
		   ledPins = (_BV(i));
		   _delay_ms(DELAY);
		}
		for (int i=0;i<=15;i++)
		{
		   ledPins = (_BV(i));
		   _delay_ms(DELAY);
		}
		loops--;
	}
}

void knightRider1(unsigned char loops){
	if (loops == 0)
		loops = 1;
	while (loops > 0){
		for (int i=15;i>=0;i--)
		{
		   ledPins = (_BV(i) ^ 0b1111111111111111);
		   _delay_ms(DELAY);
		}
		for (int i=1;i<=15;i++)
		{
		   ledPins = (_BV(i) ^ 0b1111111111111111);
		   _delay_ms(DELAY);
		}
		loops--;
	}
}
