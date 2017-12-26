#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// Timer related
// Prescaler means timer counter is updated ever ### ticks, where ### is prescale number
#define PRESCALER1		 (1 << CS10)
#define PRESCALER8		 (1 << CS11)
#define PRESCALER64		((1 << CS10) | (1 << CS11))
#define PRESCALER256		 (1 << CS12)
#define PRESCALER1024	((1 << CS10) | (1 << CS12))

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
#define DELAY_LOOP	1000 // Delay between two animations

// 16-bits storage for anode side of the leds. I use a two byte unsigned integer type to do the job
volatile uint16_t ledPins;

//Define functions
//===============================================
void ioinit(void);
void output_led_state(uint16_t __led_state);
void setLedPins(uint16_t ledRegister);
void knightRider(unsigned char loops);
void knightRider2(unsigned char loops);
void knightRider3(unsigned char loops);
void initTimer1();
//===============================================

int main (void)
{
   ioinit(); //Setup IO pins and defaults
   initTimer1();

   while(1)
   {
	   for (uint16_t i=0; i< 8191; i++){
		   ledPins = i;
		   _delay_us(1000);
	   }
	   ledPins = 0;
	   _delay_ms(1000);

	   knightRider3(3);
	   _delay_ms(DELAY_LOOP);
//	   knightRider2(3);
//	   _delay_ms(DELAY_LOOP);
//	   setLedPins(0xffff);
//	   _delay_ms(DELAY_LOOP);
//	   setLedPins(0x0000);
//	   _delay_ms(DELAY_LOOP);
//	   setLedPins(0xffff ^ 0b0101010101010101);
//	   _delay_ms(DELAY_LOOP);
//	   setLedPins(0xffff ^ 0b0000000011111111);
//	   _delay_ms(DELAY_LOOP);
//	   setLedPins(0xffff ^ 0b1111111100000000);
//	   _delay_ms(DELAY_LOOP);
//	   knightRider(1);
//	   _delay_ms(4*DELAY_LOOP);
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

void output_led_state(uint16_t __led_state)
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
void setLedPins(uint16_t ledRegister)
{
   SH_CP_low();
   ST_CP_low();
   for (uint16_t i=0; i<16; i++)
   {
	   // type cast to uint16_t needed for (1 << i) to prevent build from comparison warning/failure
	   if ((ledRegister & (uint16_t)(1 << i)) == (uint16_t)(1 << i))
		 DS_high();
	  else
		 DS_low();


	  SH_CP_high();
	  SH_CP_low();
   }
   ST_CP_high();
}

void knightRider3(unsigned char loops){
	if (loops == 0)
		loops = 1;
//	while (loops > 0){
		for (int i=15;i>=0;i--)
		{
		   ledPins |= (_BV(i));
//		   setLedPins(ledPins);
		   _delay_ms(DELAY);
		}
		for (int i=0;i<=15;i++)
		{
		   ledPins &= ~(_BV(i));
//		   setLedPins(ledPins);
		   _delay_ms(DELAY);
		}
		loops--;
//	}
}

void knightRider2(unsigned char loops){
	if (loops == 0)
		loops = 1;
//	while (loops > 0){
		for (int i=15;i>=0;i--)
		{
		   ledPins = (_BV(i));
		   setLedPins(ledPins);
		   _delay_ms(DELAY);
		}
		for (int i=0;i<=15;i++)
		{
		   ledPins = (_BV(i));
		   setLedPins(ledPins);
		   _delay_ms(DELAY);
		}
		loops--;
//	}
}

void knightRider(unsigned char loops){
	if (loops == 0)
		loops = 1;
	while (loops > 0){
		for (int i=15;i>=0;i--)
		{
		   output_led_state(_BV(i) ^ 0b1111111111111111);
		   _delay_ms(DELAY);
		}
		for (int i=1;i<=15;i++)
		{
		   output_led_state(_BV(i) ^ 0b1111111111111111);
		   _delay_ms(DELAY);
		}
		loops--;
	}
}

void initTimer1(){
	/* Initialize timer1. On compare match the interrupt will be triggered to fill the Shift Register
	 * with the contents of ledPins, and latch the signals to output.
	 * */
	TCCR1B |= (1 << WGM12); 		// Clear Timer on Compare Match (CTC) mode
	TCCR1B |= PRESCALER64; 		// Counter is updated every 64 ticks. At 16Mhz that is 250.000 updates/second
	TIMSK1 |= (1 << OCIE1A); 	// Enable interrupt on output compare A match
	OCR1A   = 192;				// Refresh rate = (16MHz / 64) / 192 = 1300 Hz = call to interrupt vector
	sei();
}


ISR(TIMER1_COMPA_vect){
   SH_CP_low();
   ST_CP_low();
   for (uint16_t i=0; i<16; i++)
   {
	   // type cast to uint16_t needed for (1 << i) to prevent build from comparison warning/failure
	   if ((ledPins & (uint16_t)(1 << i)) == (uint16_t)(1 << i))
		 DS_high();
	  else
		 DS_low();


	  SH_CP_high();
	  SH_CP_low();
   }
   ST_CP_high();
}
