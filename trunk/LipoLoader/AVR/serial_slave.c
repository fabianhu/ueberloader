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

extern Battery_Balancer_t g_tBattery_Balancer;
extern Battery_Governor_t g_tBattery_Governor;
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
	uint8_t len,i; // byte! length of values

	g_tUCITXFrame.ID = MYSERIALID;
	g_tUCITXFrame.UCI = _RXFrame->UCI;

	if(_RXFrame->ID == MYSERIALID)
	{
		switch(_RXFrame->UCI)
		{
		case UCI_SET_STATE:

			break;
		case UCI_SET_CURRENT_mA:

			break;
		case UCI_SET_CELL_VOLT:

			break;
		case UCI_SET_CELLCNT:

			break;
		case UCI_GET_STATE:
			g_tUCITXFrame.V.values8[0] = g_tBattery_Balancer.eState;
			len = 1;
			break;
		case UCI_GET_SET_CURRENT:

			break;
		case UCI_GET_SET_CELL_VOLT:

			break;
		case UCI_GET_SET_CELLCNT:

			break;
		case UCI_GET_ACT_VOLT:
			g_tUCITXFrame.V.values16[0] = g_tBattery_Governor.usVoltage_mV;
			g_tUCITXFrame.V.values16[1] = g_tADCValues.VCC_mVolt;
			len = 4;
			break;
		case UCI_GET_ACT_CURRENT:
			g_tUCITXFrame.V.values16[0] = g_tBattery_Governor.sCurrent_mA;
			g_tUCITXFrame.V.values16[1] = gTest;
			len = 4;
			break;
		case UCI_GET_ACT_CELL_VOLTS:
			for(i=0;i<6;i++)
			{
				g_tUCITXFrame.V.values16[i] = g_tBattery_Balancer.Cells[i].usVoltage_mV;
			}
			len = 12;
			break;
		default:
			len = 0;
		}

		if(len > 0)
			{
			g_tUCITXFrame.len = len+3;
			USARTSendBlockDMA(&DMA.CH1,(uint8_t*)&g_tUCITXFrame, g_tUCITXFrame.len); // add header length
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
	}

//	if(g_ucRXLength == 3)
//	{	// update rest of bytes to wait
//		g_tUCIRXFrame.len = g_tUCIRXFrame.len;
//	}

	if(g_tUCIRXFrame.len == g_ucRXLength)
	{
		OS_SetEvent(2,1);
	}
}

void TaskComm(void)
{
	g_tUCIRXFrame.len = 0xff;

	OS_WaitTicks(1000); // wait for ADC init
	uint8_t ret;

	while(1)
	{
		ret = OS_WaitEventTimeout(1,50);
		if(ret == 1)
		{
			//real event
			HandleSerial(&g_tUCIRXFrame);
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

