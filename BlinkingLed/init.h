/*
 * init.h
 *
 *  Created on: 27 dec. 2017
 *      Author: harm
 */

#ifndef INIT_H_
#define INIT_H_

void ioinit (void);
void initTimer1();
ISR(TIMER1_COMPA_vect);

#endif /* INIT_H_ */
