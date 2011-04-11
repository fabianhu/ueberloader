/*
 * serial.c
 *
 *  Created on: 25.05.2010
 *      Author: Fabian
 */
#include "OS/FabOS.h"
#include "serial.h"
#include "ueberloader.h"
#include "usart.h"
#include "pwm.h"
#include <string.h>
#include <avr/eeprom.h>

// prototypes:

uint8_t HandleSerial(UCIFrame_t *_RXFrame);
uint8_t UCIGetCRC( UCIFrame_t* pU);
void UCISendBlockCrc( UCIFrame_t* pU);

uint16_t calcCRC16(uint16_t* c, uint16_t len);
uint16_t calcCRC16S ( uint8_t* c, uint8_t len );

// globals
uint8_t g_ParReady =0; 


extern Battery_Info_t g_tBattery_Info;
extern ADC_Values_t g_tADCValues;
extern Command_t g_tCommand ;
extern ChargerMode_t g_eChargerMode;

#define MYSERIALID 55

UCIFrame_t g_tUCIRXFrame;
uint8_t    g_ucRXLength;
UCIFrame_t g_tUCITXFrame;
extern uint16_t gTest;

uint8_t gCommErr = 0; // fixme remove
uint16_t gCommErrCnt = 0;

ISR(USARTE0_RXC_vect)
{
	uint8_t* p = (uint8_t*)&g_tUCIRXFrame;
	if((USARTE0.STATUS & USART_FERR_bm) || (USARTE0.STATUS & USART_BUFOVF_bm))
	{
		gCommErr = 45;
		g_ucRXLength = 0; // reset received data length
		g_tUCIRXFrame.len = UCIHEADERLEN; // reset header length in recd. data
	}

	if(g_ucRXLength < sizeof(UCIFrame_t)) // avoid over-write of the frame (too long)
	{
		p[g_ucRXLength] = USARTE0.DATA;
		g_ucRXLength++;

		OS_SetAlarm(OSALMCommTimeout,5); // reset Alarm, if stuff arrives
		if(g_tUCIRXFrame.len == g_ucRXLength)
		{
			OS_SetEvent/*fromISR fixme*/(OSTSKCommRX,OSEVTDataRecvd);
		}
	}
	else
	{
		gCommErr = 1;
		g_ucRXLength = 0; // reset received data length
		g_tUCIRXFrame.len = UCIHEADERLEN; // reset header length in recd. data
	}
}

#define TIMEOUTRXMINORCYCLE 10

void TaskCommRX(void)
{
	g_tUCIRXFrame.len = 0xff;

	OS_WaitTicks(OSALMCommTimeout,1000); // wait for ADC init
	uint8_t ret;

	while(1)
	{
		ret = OS_WaitEventTimeout(OSEVTDataRecvd,OSALMCommTimeout,TIMEOUTRXMINORCYCLE);
		if(ret == OSEVTDataRecvd)
		{
			HandleSerial(&g_tUCIRXFrame);
		}
		else
		{
			//timeout
	
		}

		// re-init for new frame
		g_ucRXLength = 0;
		g_tUCIRXFrame.len = UCIHEADERLEN;

	}

}

uint16_t crc=0;

uint8_t HandleSerial(UCIFrame_t *_RXFrame)
{
	uint8_t len=0; // byte! length of values

	g_tUCITXFrame.ID = MYSERIALID;
	g_tUCITXFrame.UCI = _RXFrame->UCI; // Prepare answer

	if(
		(_RXFrame->ID == MYSERIALID) &&
		(UCIGetCRC(&g_tUCIRXFrame) == g_tUCIRXFrame.crc) && 
		g_ParReady
	  )
	{
		switch(_RXFrame->UCI)
		{
		case UCI_GET_CMDs:
			OS_MutexGet(OSMTXCommand);
			memcpy(g_tUCITXFrame.values, (uint8_t*)&g_tCommand, sizeof(g_tCommand));
			OS_MutexRelease(OSMTXCommand);
			len = sizeof(g_tCommand);
			break;
		case UCI_GET_OPVs:
			OS_MutexGet(OSMTXBattInfo);
			memcpy(g_tUCITXFrame.values, (uint8_t*)&g_tBattery_Info, sizeof(g_tBattery_Info));
			OS_MutexRelease(OSMTXBattInfo);
			len = sizeof(g_tBattery_Info);
			break;
		case UCI_SET_CMDs:
			OS_MutexGet(OSMTXCommand);
			memcpy((uint8_t*)&g_tCommand, g_tUCIRXFrame.values, sizeof(Command_t));
			OS_MutexRelease(OSMTXCommand);
			// prepare ok answer:
			g_tUCITXFrame.values[0] = 1;
			len = 1;
			break;
		case UCI_WRITE_EEPROM:
			eeprom_update_block((uint8_t*)&g_tCommand, EEPROM_START, sizeof(Command_t));
			// add crc check!!!
			crc = calcCRC16S((uint8_t*)&g_tCommand,sizeof(Command_t));
			eeprom_update_block((uint8_t*)&crc, (void*)(EEPROM_START + sizeof(Command_t)), 2);

			PWM_Setfrequency(g_tCommand.basefrequency); // in kHz!!!
			// prepare ok answer:
			g_tUCITXFrame.values[0] = 1;
			len = 1;
			break;
		case UCI_ACTION:
			if((ChargerMode_t)g_tUCIRXFrame.values[0] != eModeNoChange)
			{
				g_eChargerMode = (ChargerMode_t)g_tUCIRXFrame.values[0];
			}
			break;
		default:
			len = 0;
		}


		if(len > 0) // sending of answer necessary?
		{
			g_tUCITXFrame.len = len+UCIHEADERLEN;
			UCISendBlockCrc(&g_tUCITXFrame);
		}
	}
	else
	{
		// comm error
		if(_RXFrame->ID != MYSERIALID)
		{
			gCommErr = 3;
			gCommErrCnt++;
		}
		else
		{
			gCommErr = 4;
			gCommErrCnt++;
		}

	}
	return 0;
}

// Attention, the pointer content is CHANGED!!!
void UCISendBlockCrc( UCIFrame_t* pU)
{
	// prepare RX for answer BEFORE sending, as we could be interrupted afterwards.

	g_ucRXLength = 0; // reset received data length
	g_tUCIRXFrame.len = UCIHEADERLEN; // reset header length in recd. data
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

