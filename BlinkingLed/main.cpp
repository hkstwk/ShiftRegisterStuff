#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "shift_register.h"
#include "timer.h"
#include "defines.h"
#include "init.h"

#include "animations/kitt.h"
#include "animations/rotation.h"

// 16-bits storage for anode side of the leds. I use a two byte unsigned integer type to do the job
volatile uint16_t ledPins;
volatile uint16_t previousLedPins;
volatile uint8_t currentLayer;

int main (void)
{
    LAYER_DDR |= (1 << LAYER0) | (1 << LAYER1) | (1 << LAYER2) | (1 << LAYER3);

    previousLedPins = 0xAB;

    // initialize layer 0; other layers off.
    currentLayer = 0;
    LAYER_PORT = (1 << currentLayer);

   ioinit(); 		// Setup IO pins and defaults
   initTimer1();		// Setup timer/counter1 with interrupt handler for latching output 74HC595

   while(1)
   {
	   knightRider1(3);
	   _delay_ms(DELAY_LOOP);
	   leftRotate(74, 0b0000000111001010);
	   _delay_ms(DELAY_LOOP);
	   rightRotate(70, 0b0000000111001010);
	   _delay_ms(DELAY_LOOP);
	   snake(2);
	   _delay_ms(DELAY_LOOP);
	   for (uint16_t i=0; i< 2047; i++){
		   ledPins = i;
		   _delay_us(1000);
	   }
	   ledPins = 0;
	   _delay_ms(DELAY_LOOP);
	   knightRider3(3);
	   _delay_ms(DELAY_LOOP);
	   knightRider2(3);
	   _delay_ms(5*DELAY_LOOP);
	   ledPins = (0xffff);
	   _delay_ms(5*DELAY_LOOP);
	   ledPins = (0x0000);
	   _delay_ms(DELAY_LOOP);
	   ledPins = (0xffff ^ 0b0101010101010101);
	   _delay_ms(DELAY_LOOP);
	   ledPins = (0xffff ^ 0b0000000011111111);
	   _delay_ms(DELAY_LOOP);
	   ledPins = (0xffff ^ 0b1111111100000000);
	   _delay_ms(DELAY_LOOP);
   }

   return 0;
}

void snake(unsigned char loops){
	int i;
	if (loops == 0)
		loops = 1;
	while (loops > 0){
		for (i=15;i>=0;i--)
		{
		   ledPins |= (_BV(i));
		   _delay_ms(DELAY);
		}
		for (i=15;i>=0;i--)
		{
		   ledPins &= ~(_BV(i));
		   _delay_ms(DELAY);
		}
		for (i=0;i<=15;i++)
		{
		   ledPins |= (_BV(i));
		   _delay_ms(DELAY);
		}
		for (i=0;i<=15;i++)
		{
		   ledPins &= ~(_BV(i));
		   _delay_ms(DELAY);
		}
		loops--;
	}
}
