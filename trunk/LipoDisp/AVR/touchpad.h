/*
 * touchpad.h
 *
 *  Created on: 23.06.2010
 *      Author: Besitzer
 */

#ifndef TOUCHPAD_H_
#define TOUCHPAD_H_

#include <avr/io.h>

#define TOUCHCONFIGPORT PORTC.DIR = (1<<7);
#define TOUCHTIMES 3
#define TOUCHPORT PORTC

#define TOUCHTOGGLEHIGH PORTC.OUTSET = (1<<7);
#define TOUCHTOGGLELOW  PORTC.OUTCLR = (1<<7);

void touch_init(void);
uint8_t touchGetPad(uint8_t pin);

#define TOUCHCALINIT {98,95,68,68,74}

#endif /* TOUCHPAD_H_ */
