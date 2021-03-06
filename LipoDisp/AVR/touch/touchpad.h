/*
 * touchpad.h
 *
 *  Created on: 23.06.2010
 *      Author: Besitzer
 */

#ifndef TOUCHPAD_H_
#define TOUCHPAD_H_

#include <avr/io.h>

#define TOUCHCOUNT 5

#define TOUCHCONFIGPORT PORTC.DIR = (1<<7);
#define TOUCHPORT PORTC
#define TOUCHTOGGLEHIGH PORTC.OUTSET = (1<<7);
#define TOUCHTOGGLELOW  PORTC.OUTCLR = (1<<7);

#define TOUCHREPCNT 5

#define TOUCHMINSIGNAL 20
#define MOVEMAXTIME 150
#define MOVEMINTIME 20

//#define SLIDEMINSIGNAL 43
//#define TOUCHMINDIST 400 // fixme geht nicht

//#define MINSLIDESPEED 7

//#define MAXSLIDESPEED 100


#define TOUCHDISABLESLIDE 1

void touch_init(void);
//uint8_t touchGetPad(uint8_t pin);
//int16_t touchGetSchwerpunkt(void);
int16_t touch(void);
void touchGetValue(int16_t* pValue);

#define TOUCHCALINIT {15800,15600,11300,11300,12900} //fixme automatisch machen

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
	int32_t min;
	int32_t max;
	int16_t stepsize;
	int16_t upscale;
}particle_t;

typedef enum eGestures_tag // for only 3 pads !!!!!!!
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
	eTSTouching 	= 1,
	eTSGesture	 	= 2,
	eTSBlocked 		= 3
} eTouchstate_t;


#endif /* TOUCHPAD_H_ */
