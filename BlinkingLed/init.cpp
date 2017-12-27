#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "shift_register.h"
#include "timer.h"

// 16-bits storage for anode side of the leds. I use a two byte unsigned integer type to do the job
extern volatile uint16_t ledPins;

void ioinit (void)
{
	// Set 74HC595 pins to output
    HC595_DDR  |= (1 << DS_PIN) | (1 << SH_CP_PIN) | (1 << ST_CP_PIN) | (1 << OE_PIN) | (1 << MR_PIN);
    // Set 74HC595 pins (see above definition for initial values)
    HC595_PORT |= (0 << DS_PIN) | (0 << SH_CP_PIN) | (0 << ST_CP_PIN) | (0 << OE_PIN) | (1 << MR_PIN);
}

/* Initialize timer1. On compare match the interrupt will be triggered to fill the Shift Register
 * with the contents of ledPins, and latch the signals to output.
 */
void initTimer1(){
	TCCR1B |= (1 << WGM12); 		// Clear Timer on Compare Match (CTC) mode
	TCCR1B |= PRESCALER64; 		// Counter is updated every 64 ticks. At 16Mhz that is 250.000 updates/second
	TIMSK1 |= (1 << OCIE1A); 	// Enable interrupt on output compare A match
	OCR1A   = 192;				// Refresh rate = (16MHz / 64) / 192 = 1300 Hz = call to interrupt vector
	sei();						// Set global interrupt enable
}

/* This interrupt sets 16 leds on or off. Depending on the value of the bit that represents the led.
 * To save memory the bits are stored in a two byte unsigned integer (uint16_t).
 * Setting the leds on or off is just a matter of iterating all 16 bits and push them in shift register
 *
 * See shift_register.h for definitions and low level functions used here.
 */
ISR(TIMER1_COMPA_vect){
   SH_CP_low();
   ST_CP_low();
   for (uint16_t i=15; i<65535; i--)
   {
	   // type cast to uint16_t needed for (1 << i) to prevent build from comparison warning/failure
	   // If position i of ledPins contains a 1, set Data Serial to 1. Else set Data Serial to 0.
	   if ((ledPins & (uint16_t)(1 << i)) == (uint16_t)(1 << i))
		 DS_high();
	  else
		 DS_low();

	   // clock the value of Data Serial into the shift register
	  SH_CP_high();
	  SH_CP_low();
   }

   // latch the shift registers to output. All 16 bits are handled in one cycle.
   ST_CP_high();
}
