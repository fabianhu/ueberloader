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
#include <string.h>

extern Battery_Info_t g_tBattery_Info;
extern ADC_Values_t g_tADCValues;
extern Command_t g_tCommand;

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

#define MYSERIALID 55



UCIFrame_t g_tUCIRXFrame;
uint8_t    g_ucRXLength;
UCIFrame_t g_tUCITXFrame;
extern uint16_t gTest;

void HandleSerial(UCIFrame_t *_RXFrame)
{
	uint8_t len=0; // byte! length of values

	g_tUCITXFrame.ID = MYSERIALID;
	g_tUCITXFrame.UCI = _RXFrame->UCI; // Prepare answer

	if(_RXFrame->ID == MYSERIALID)
	{
		switch(_RXFrame->UCI)
		{
		case UCI_GET_CMDs:
			OS_MutexGet(OSMTXCommand);
			memcpy(g_tUCITXFrame.values, (uint8_t*)&g_tCommand, sizeof(g_tCommand));
			len = sizeof(g_tCommand);
			OS_MutexRelease(OSMTXCommand);
			break;
		case UCI_GET_INTs:

			break;
		case UCI_GET_OPVs:
			OS_MutexGet(OSMTXBattInfo);
			memcpy(g_tUCITXFrame.values, (uint8_t*)&g_tBattery_Info, sizeof(g_tBattery_Info));
			OS_MutexRelease(OSMTXBattInfo);
			len = sizeof(g_tBattery_Info);
			break;
		case UCI_SET_CMDs:
			OS_MutexGet(OSMTXCommand);
			memcpy((uint8_t*)&g_tCommand, g_tUCIRXFrame.values, sizeof(g_tCommand));
			OS_MutexRelease(OSMTXCommand);
			len = 0; // no answer expected.
			break;
		default:
			len = 0;
		}

		if(len > 0)
			{
			g_tUCITXFrame.len = len+UCIHEADERLEN;
			UCISendBlockCrc(&g_tUCITXFrame);
			}
	}

}


ISR(USARTE0_RXC_vect)
{
	uint8_t* p = (uint8_t*)&g_tUCIRXFrame;
	if(g_ucRXLength < sizeof(UCIFrame_t)) // avoid over-write of the frame (too long)
	{
		p[g_ucRXLength] = USARTE0.DATA;
		g_ucRXLength++;

		OS_SetAlarm(OSALMCommTimeout,5); // reset Alarm, if stuff arrives
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

	OS_WaitTicks(OSALMCommTimeout,1000); // wait for ADC init
	uint8_t ret;

	while(1)
	{
		ret = OS_WaitEventTimeout(OSEVTDataRecvd,5,OSALMCommTimeout);
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
		g_tUCIRXFrame.len = 0x0;


//			OS_ENTERCRITICAL;
//			g_tCommand.usCurrentSetpoint = 500;
//			g_tCommand.usMinBalanceVolt_mV = 3000;
//			g_tCommand.usVoltageSetpoint_mV = 4150;
//			g_tCommand.eChargerMode = eModeAuto;
//			OS_LEAVECRITICAL;


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
