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
#define TOUCHREPCNT 1
#define TOUCHPORT PORTC

#define TOUCHTOGGLEHIGH PORTC.OUTSET = (1<<7);
#define TOUCHTOGGLELOW  PORTC.OUTCLR = (1<<7);

void touch_init(void);
uint8_t touchGetPad(uint8_t pin);
int16_t touchGetSchwerpunkt(void);

#define TOUCHCALINIT {0xffff,0xffff,0xffff,0xffff,0xffff}

#define abs(X) ((X)<0?-(X):(X))
#define min(x,y) (((x)<(y))?(x):(y))
#define max(x,y) (((x)>(y))?(x):(y))
#define limit(v,l,u) (((v)<(l))?(l):(((v)>(u))?(u):(v)))


#endif /* TOUCHPAD_H_ */