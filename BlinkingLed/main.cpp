#include <avr/io.h>
#include <util/delay.h>


// Port and pin definitions for 74HC595 Shift Register
#define HC595_PORT	PORTC	// Define port for 74HC595 Shift Register
#define DS_PIN     	PC0 		// DS = Data Serial input
#define ST_CP_PIN  	PC1		// ST_CP = Storage Register Clock Pin (latch pin)
#define SH_CP_PIN  	PC2		// SH_CP = Shift Register Clock Pin
#define MR_PIN  		PC3		// MR = Master Reclear, active LOW, default HIGH
#define OE_PIN  		PC4		// OE = Output Enable, active LOW, default LOW

// Set data serial pin using these two functions
#define DS_low()  	HC595_PORT &= ~(1 << DS_PIN)  // clear data serial bit
#define DS_high()	HC595_PORT |=  (1 << DS_PIN)  // set data serial bit

// latch shift register to output using these two functions
#define ST_CP_low()  HC595_PORT &= ~(1 << ST_CP_PIN) // c
#define ST_CP_high() HC595_PORT |=  (1 << ST_CP_PIN)

// clock data serial bit in shift register using these two functions
#define SH_CP_low()  HC595_PORT &= ~(1 << SH_CP_PIN)
#define SH_CP_high() HC595_PORT |=  (1 << SH_CP_PIN)

// Master Reclear using these two functions
#define MR_low()  	HC595_PORT &= ~(1 << MR_PIN)
#define MR_high() 	HC595_PORT |=  (1 << MR_PIN)

// Output Enable using these two functions
#define OE_low()  	HC595_PORT &= ~(1 << OE_PIN)
#define OE_high() 	HC595_PORT |=  (1 << OE_PIN)

#define DELAY		50

// Number of bytes needed to store output pin.
// Example: 16 (led)pins needed? Take two bytes = 2 x 8 = 16 bits
#define PIN_BYTES	4

volatile unsigned char ledPins[PIN_BYTES];

//Define functions
//===============================================
void ioinit(void);
void output_led_state(unsigned int __led_state);
void setLedPins(unsigned char ledBytes[PIN_BYTES]);
void knightRider(unsigned char loops);
//===============================================

int main (void)
{
   ioinit(); //Setup IO pins and defaults

   while(1)
   {
	   knightRider(0);
	   _delay_ms(2000);
	   knightRider(5);
	   _delay_ms(2000);
   }

   return 0;
}

void ioinit (void)
{
	// Set 74HC595 pins to output
    DDRC  |= (1 << DS_PIN) | (1 << SH_CP_PIN) | (1 << ST_CP_PIN) | (1 << OE_PIN) | (1 << MR_PIN);
    // Set 74HC595 pins (see above definition for initial values)
    PORTC |= (0 << DS_PIN) | (0 << SH_CP_PIN) | (0 << ST_CP_PIN) | (0 << OE_PIN) | (1 << MR_PIN);
}

void output_led_state(unsigned int __led_state)
{
   SH_CP_low();
   ST_CP_low();
   for (int i=15;i>=0;i--)
   {
      if ((_BV(i) & __led_state) == _BV(i))  //bit_is_set doesn’t work on unsigned int so we do this instead
         DS_high();
      else
         DS_low();


      SH_CP_high();
      SH_CP_low();
   }
   ST_CP_high();
}

/* This function sets 16 leds on or off. Depending on the value of the bit that represents the led.
 * To save memory the bits are stored in a two byte unsigned integer.
 * Setting the leds on or off is just a matter of iterating all bits
 */
void setLedPins(unsigned char ledBytes[PIN_BYTES])
{
   SH_CP_low();
   ST_CP_low();
   for (int byteCount=0; byteCount < PIN_BYTES; byteCount++){
	   for (int i=0; i<8; i++)
	   {
		   if ((ledBytes[byteCount] & (1 << i)) == (1 << i))
			 DS_high();
		  else
			 DS_low();


		  SH_CP_high();
		  SH_CP_low();
	   }
	   ST_CP_high();
   }
}

void knightRider(unsigned char loops){
	if (loops == 0)
		loops = 1;
	while (loops > 0){
		for (int i=15;i>=0;i--)
		{
		   output_led_state(_BV(i) ^ 0b1111000011110000);
		   _delay_ms(DELAY);
		}
		for (int i=1;i<15;i++)
		{
		   output_led_state(_BV(i) ^ 0b1111000011110000);
		   _delay_ms(DELAY);
		}
		loops--;
	}
	output_led_state(_BV(15) ^ 0b1111000011110000);
}
