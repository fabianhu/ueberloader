/*
 * touchpad.h
 *
 *  Created on: 23.06.2010
 *      Author: Besitzer
 */

#ifndef TOUCHPAD_H_
#define TOUCHPAD_H_

#include <avr/io.h>

#define TOUCHCOUNT 3

#define TOUCHCONFIGPORT PORTC.DIR = (1<<7);
#define TOUCHPORT PORTC
#define TOUCHTOGGLEHIGH PORTC.OUTSET = (1<<7);
#define TOUCHTOGGLELOW  PORTC.OUTCLR = (1<<7);

#define TOUCHREPCNT 5

#define TOUCHMINSIGNAL 22
#define MINSLIDESPEED 8

#define MINGESTURETIME 6


void touch_init(void);
//uint8_t touchGetPad(uint8_t pin);
//int16_t touchGetSchwerpunkt(void);
int16_t touch(void);
void touchGetValue(int32_t* pValue);
//void HandOverValueToUI(uint16_t value, uint16_t upper, uint16_t lower);

#define TOUCHCALINIT {0xffff,0xffff,0xffff,0xffff,0xffff}

#define abs(X) ((X)<0?-(X):(X))
#define min(x,y) (((x)<(y))?(x):(y))
#define max(x,y) (((x)>(y))?(x):(y))
#define limit(v,l,u) (((v)<(l))?(l):(((v)>(u))?(u):(v)))


typedef struct paticle_tag
{
	int32_t velocity;
	int32_t position;
	int32_t force;
	int32_t friction; //(factor)
}particle_t;

typedef enum eGestures_tag
{
	eGNothing 	= 0,
	eGPlus 		= 1,
	eGMitte 	= 2,
	eGMittePlus = 3,
	eGMinus 	= 4,
	eGSplit 	= 5,
	eGMitteMinus = 6,
	eGFullHouse = 7
} eGestures_t;


typedef enum eTouchstate_tag
{
	eTSIdle 		= 0,
	eTSTouched 		= 1,
	eTSMoving 		= 2,
	eTSGesture	 	= 3,
	eTSBlocked 		= 4
} eTouchstate_t;


#endif /* TOUCHPAD_H_ */
