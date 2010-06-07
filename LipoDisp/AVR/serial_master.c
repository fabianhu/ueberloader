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
/*

Serial Protocol:

ID:UCI:Value(optional)

ID: byte 0..255, where 0 is broadcast
broadcast expects no answer.

Masters Commands:
 Set State ((Sets also to manual mode))
 Set Current
 Set Cell Voltage
 Set Cell Count

 Get State
 Get Current setting
 Get Cell Voltage setting
 Get Cell Count setting

 Get actual Voltage (Battery, Supply)
 Get Cell Voltages (all Cell voltages 0..5)
 Get actual Current mA

...
 Get Balancer PWM (one byte per cell)
 Get actual Capacity mAh
 Get actual Capacity mWh

 Set Max Charge (mAh)

 */

#define SLAVEDERIALID 55

void HandleSerial(UCIFrame_t *_RXFrame)
{
	if(_RXFrame->ID == SLAVEDERIALID)
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

}

UCIFrame_t g_tUCIRXFrame;
uint8_t    g_ucRXLength;

ISR(USARTE0_RXC_vect)
{
	uint8_t* p = (uint8_t*)&g_tUCIRXFrame;
	if(g_ucRXLength < sizeof(UCIFrame_t)) // avoid over-write of the frame (too long)
	{
		p[g_ucRXLength] = USARTE0.DATA;
		g_ucRXLength++;

		OS_SetAlarm(OSTaskCommRX,5); // reset Alarm, if stuff arrives
	}

//	if(g_ucRXLength == 3)
//	{	// update rest of bytes to wait
//		g_tUCIRXFrame.len = g_tUCIRXFrame.len;
//	}

	if(g_tUCIRXFrame.len == g_ucRXLength)
	{
		OS_SetEvent(OSTaskCommRX,OSEVTDataRecvd);
	}
}

void TaskCommRX(void)
{
	g_tUCIRXFrame.len = 0xff;

	OS_WaitTicks(2000); // wait for Slave init
	uint8_t ret;

	while(1)
	{
		ret = OS_WaitEventTimeout(OSEVTDataRecvd,5);
		if(ret == OSEVTDataRecvd)
		{
			//real event
			if(UCIGetCRC(&g_tUCIRXFrame) == g_tUCIRXFrame.crc)
			{
				// CRC is OK:
				HandleSerial(&g_tUCIRXFrame);
			}
		}
		else
		{
			//timeout
		}

		// re-init for new frame
		g_ucRXLength = 0;
		g_tUCIRXFrame.len = 0xff;
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
