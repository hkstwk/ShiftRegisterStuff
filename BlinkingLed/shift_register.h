/*
 * shift_register.h
 *
 *  Created on: 27 dec. 2017
 *      Author: harm
 */

#ifndef SHIFT_REGISTER_H_
#define SHIFT_REGISTER_H_

// Port and pin definitions for 74HC595 Shift Register
#define HC595_PORT	PORTC	// Define port for 74HC595 Shift Register
#define HC595_DDR	DDRC		// Define Data Direction Register for HC595_PORT
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

// Number of bits, currently based on two shift registers 74HC595
#define  INT_BITS	16

#endif /* SHIFT_REGISTER_H_ */
