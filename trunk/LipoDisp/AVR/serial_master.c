/*
 * serial.c
 *
 *  Created on: 25.05.2010
 *      Author: Fabian
 */
#include "OS/FabOS.h"
#include "serial.h"
#include "usart.h"
#include "string.h"

Battery_Info_t g_tBattery_Info;
Command_t g_tCommand;
uint8_t glCommError=0;

#define SLAVEDERIALID 55

UCIFrame_t g_tUCIRXFrame;
uint8_t    g_ucRXLength;

ISR(USARTE0_RXC_vect)
{
	uint8_t* p = (uint8_t*)&g_tUCIRXFrame;
	if(g_ucRXLength < sizeof(UCIFrame_t)) // avoid over-write of the frame (too long)
	{
		p[g_ucRXLength] = USARTE0.DATA;
		g_ucRXLength++;

		OS_SetAlarm(OSALMCommandTimeout,5); // reset Alarm, if stuff arrives
	}
	else
		glCommError = 1;

	if(g_tUCIRXFrame.len == g_ucRXLength)
	{
		OS_SetEvent(OSTSKCommand,OSEVTDataRecvd);
	}
}

void HandleSerial(UCIFrame_t *_RXFrame)
{
	if(
		(_RXFrame->ID == SLAVEDERIALID) &&
		(UCIGetCRC(&g_tUCIRXFrame) == g_tUCIRXFrame.crc) )
	{
		switch(_RXFrame->UCI)
		{
		case UCI_GET_CMDs:
			OS_MutexGet(OSMTXCommand);
			memcpy((uint8_t*)&g_tCommand, _RXFrame->values, sizeof(g_tCommand));
			OS_MutexRelease(OSMTXCommand);
			break;
		case UCI_GET_INTs:

			break;
		case UCI_GET_OPVs:
			OS_MutexGet(OSMTXBattInfo);
			glCommError =0;
			memcpy((uint8_t*)&g_tBattery_Info, _RXFrame->values, sizeof(g_tBattery_Info));
			OS_MutexRelease(OSMTXBattInfo);
			break;
		case UCI_SET_CMDs:
			// not likely to be answered by the slave ;-) yet // fixme
			break;
		default:
			break;
		}

	}
	else
	{
		glCommError = 3; // CRC wrong
	}

}

void UCISendBlockCrc( UCIFrame_t* pU)
{
	pU->crc = 0;
	pU->crc = CRC8x((uint8_t*)pU ,pU->len);
	USARTSendBlockDMA(&DMA.CH1,(uint8_t*)pU ,pU->len);
}

uint8_t UCIGetCRC( UCIFrame_t* pU)
{
	uint8_t tempCRC = pU->crc;
	pU->crc = 0;
	uint8_t res = CRC8x((uint8_t*)pU,pU->len);
	pU->crc = tempCRC;
	return res;
}

