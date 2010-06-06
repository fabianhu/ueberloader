/*
 * serial.h
 *
 *  Created on: 25.05.2010
 *      Author: Fabian
 */

#ifndef SERIAL_H_
#define SERIAL_H_

#include <avr/io.h>

#define UCI_SET_STATE 			1
#define UCI_SET_CURRENT_mA 		2
#define UCI_SET_CELL_VOLT		3
#define UCI_SET_CELLCNT 		4
#define UCI_GET_STATE 			5
#define UCI_GET_SET_CURRENT		6
#define UCI_GET_SET_CELL_VOLT	7
#define UCI_GET_SET_CELLCNT		8
#define UCI_GET_ACT_VOLT		9
#define UCI_GET_ACT_CURRENT		10
#define UCI_GET_ACT_CELL_VOLTS	11


typedef struct UCIFrame_tag
{
	uint8_t ID;
	uint8_t UCI;
	uint8_t crc;
	uint8_t len;
	union
	{
		uint8_t values8[12];
		uint16_t values16[6];
	} V;
}UCIFrame_t;

#define UCIHEADERLEN 4

void UCISendBlockCrc( UCIFrame_t* pU);
uint8_t UCIGetCRC( UCIFrame_t* pU);



#endif /* SERIAL_H_ */




