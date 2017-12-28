/*
 * timer.h
 *
 *  Created on: 27 dec. 2017
 *      Author: harm
 */

#ifndef TIMER_H_
#define TIMER_H_

// Timer related
// Prescaler means timer counter is updated ever ### ticks, where ### is prescale number
#define PRESCALER1		 (1 << CS10)
#define PRESCALER8		 (1 << CS11)
#define PRESCALER64		((1 << CS10) | (1 << CS11))
#define PRESCALER256		 (1 << CS12)
#define PRESCALER1024	((1 << CS10) | (1 << CS12))

#endif /* TIMER_H_ */
