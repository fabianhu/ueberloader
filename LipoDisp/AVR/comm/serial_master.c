/*
 * serial.c
 *
 *  Created on: 25.05.2010
 *      Author: Fabian
 */
#include "../OS/FabOS.h"
#include "serial.h"
#include "usart.h"
#include "string.h"

Battery_Info_t g_tBattery_Info;
Command_t g_tCommand;
uint8_t glCommError=0;

#if OS_TRACE_ON == 1
	extern uint8_t OS_Tracebuffer[OS_TRACESIZE];
	#if OS_TRACESIZE <= 0xff
	extern uint8_t OS_TraceIdx;
	#else
	extern uint16_t OS_TraceIdx;
	#endif
#endif

#define SLAVEDERIALID 55

UCIFrame_t g_tUCIRXFrame;
uint8_t    g_ucRXLength;

ISR(USARTE0_RXC_vect)
{
	OS_TRACE(101);
	
	uint8_t* p = (uint8_t*)&g_tUCIRXFrame;
	if((USARTE0_STATUS & USART_FERR_bm) || (USARTE0_STATUS & USART_BUFOVF_bm))
	{
		glCommError = 45;
		g_ucRXLength = 0; // reset received data length
		g_tUCIRXFrame.len = UCIHEADERLEN; // reset header length in recd. data
	}

	if(g_ucRXLength < sizeof(UCIFrame_t)) // avoid over-write of the frame (too long)
	{
		p[g_ucRXLength] = USARTE0_DATA;
		g_ucRXLength++;

		//OS_SetAlarm(OSALMCommandTimeout,5); // reset Alarm, if stuff arrives
		if(g_tUCIRXFrame.len == g_ucRXLength)
		{
			OS_SetEvent(OSTSKCommand,OSEVTDataRecvd);
		}
	}
	else
	{
		glCommError = 1;
		g_ucRXLength = 0; // reset received data length
		g_tUCIRXFrame.len = UCIHEADERLEN; // reset header length in recd. data
	}
	OS_TRACE(102);
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
			// fixme not likely to be answered by the slave ;-) yet
			break;
		default:
			break;
		}
	}
	else
	{
		glCommError = 3; // CRC wrong
	}

	memset(_RXFrame,0xff,95);

}

// ATTENTION, the data given with the Pointer IS MODIFIED HERE!!!!!
void UCISendBlockCrc( UCIFrame_t* pU) // if the master sends a block, it is to be assumed, that the RX data is useless, therefore it is discarded here.
{
	// prepare RX for answer BEFORE sending, as we could be interrupted afterwards.

	g_ucRXLength = 0; // reset received data length
	g_tUCIRXFrame.len = UCIHEADERLEN; // reset header length in recd. data
	pU->crc = 0; 
	pU->crc = CRC8x((uint8_t*)pU ,pU->len);
	if(USARTSendBlockDMA(&DMA.CH1,(uint8_t*)pU ,pU->len)) glCommError = 4;
}

uint8_t UCIGetCRC( UCIFrame_t* pU)
{
	uint8_t tempCRC = pU->crc;
	pU->crc = 0;
	uint8_t res = CRC8x((uint8_t*)pU,pU->len);
	pU->crc = tempCRC;
	return res;
}

