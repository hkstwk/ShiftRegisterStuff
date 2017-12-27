/*
 * rotation.h
 *
 *  Created on: 27 dec. 2017
 *      Author: harm
 */

#ifndef ROTATION_H_
#define ROTATION_H_

#include <avr/io.h>

void snake(unsigned char loops);
void leftRotate(unsigned char loops, uint16_t bitPattern);
void rightRotate(unsigned char loops, uint16_t bitPattern);


#endif /* ROTATION_H_ */
