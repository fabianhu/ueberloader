
#include "../OS/FabOS.h"
#include "../comm/serial.h"
#include "../comm/usart_variant.h"
#include "../comm/usart.h"
#include <string.h>

// Globals:
UCIFrame_t 		g_tUCITXBlock; // used in DMA
uint8_t 		g_NewComand =0; // indicates new command to be sent
uint8_t 		g_GotNewComand =0; // indicates stored commands I got from slave
Battery_Info_t 	g_tBattery_Info;
Command_t 		g_tCommand;
UCIFrame_t 		g_tUCIRXFrame; // serial receive data
uint8_t    		g_ucRXLength;

uint8_t 		g_Trig_SavePars; // triger save parameter command
ChargerMode_t 	g_Tansfer_Action;
uint8_t 		g_OPVsValid = 0; // opvs have been received.


// Prototypes:
uint8_t vWaitForResult(uint8_t);
uint8_t HandleSerial(UCIFrame_t *_RXFrame, uint8_t content);
uint8_t UCIGetCRC( UCIFrame_t* pU);
void UCISendBlockCrc( UCIFrame_t* pU);
eBatteryStatus_t GetBattStatus(void);

void handleCommError(uint8_t errNo);
uint8_t getLastCommError(void);
uint8_t gCommandsKnown=0;


void TaskCommand(void)
{

	uint8_t ret;

	OS_SetAlarm(OSALMCommandRepeat,1000);
	while(1)
	{
		OS_WaitAlarm(OSALMCommandRepeat);
		OS_SetAlarm(OSALMCommandRepeat,1000);


		if(gCommandsKnown == 0)
		{
			g_tUCITXBlock.ID = 55;
			g_tUCITXBlock.UCI = UCI_GET_CMDs;
			g_tUCITXBlock.len = UCIHEADERLEN;
			UCISendBlockCrc(&g_tUCITXBlock);
			ret = vWaitForResult(UCI_GET_CMDs);
			handleCommError(ret);
			if(ret == 0)
			{
				gCommandsKnown = 1;
			}
		}


		g_tUCITXBlock.ID = 55;
		g_tUCITXBlock.UCI = UCI_GET_OPVs;
		g_tUCITXBlock.len = UCIHEADERLEN;
		UCISendBlockCrc(&g_tUCITXBlock);
		ret = vWaitForResult(UCI_GET_OPVs);
		handleCommError(ret);
	    if (ret == 0)
	    { 	g_OPVsValid = 1; }
	    else
	    {  	g_OPVsValid = 0;  }

	    if(gCommandsKnown == 1)
	    {
	    	g_tUCITXBlock.ID = 55;
			g_tUCITXBlock.UCI = UCI_SET_CMDs;
			g_tUCITXBlock.len = UCIHEADERLEN+sizeof(Command_t);
	    	OS_MutexGet(OSMTXCommand);
				memcpy(g_tUCITXBlock.values,&g_tCommand,sizeof(Command_t));
	    	OS_MutexRelease(OSMTXCommand);
	    	UCISendBlockCrc(&g_tUCITXBlock);

			ret = vWaitForResult(UCI_SET_CMDs);
			handleCommError(ret);
	    	g_NewComand = 0;


			if(g_Trig_SavePars == 1)
			{
				g_Trig_SavePars = 0; // reset
				g_tUCITXBlock.ID = 55;
				g_tUCITXBlock.UCI = UCI_WRITE_EEPROM;
				g_tUCITXBlock.len = UCIHEADERLEN;
				UCISendBlockCrc(&g_tUCITXBlock);
				ret = vWaitForResult(UCI_WRITE_EEPROM);
				handleCommError(ret);
			}

			if(g_Tansfer_Action != eModeNoChange)
			{
				g_tUCITXBlock.ID = 55;
				g_tUCITXBlock.UCI = UCI_ACTION;
				g_tUCITXBlock.len = UCIHEADERLEN+1;
				g_tUCITXBlock.values[0] = (uint8_t)g_Tansfer_Action;
				UCISendBlockCrc(&g_tUCITXBlock);

				ret = vWaitForResult(UCI_ACTION);
				handleCommError(ret);
				if (ret == 0) g_Tansfer_Action = eModeNoChange; // reset on success

			}
	    }

	}
}


uint8_t vWaitForResult(uint8_t content)
{
	uint8_t ret;
	uint8_t commerror = 0;

	ret = OS_WaitEventTimeout(OSEVTDataRecvd,OSALMCommandTimeout, 300); // at 9600 baud it takes 105 ms to transfer the stuff.
    if(ret & OSEVTDataRecvd)
	{
    	commerror = HandleSerial(&g_tUCIRXFrame, content);
	}
	else
	{
		commerror = 100;// + "content";
	}
    g_ucRXLength = 0;
    g_tUCIRXFrame.len = UCIHEADERLEN;
    return commerror;
}

ISR(USARTE0_RXC_vect)
{
	OS_TRACE(101);

	uint8_t* p = (uint8_t*)&g_tUCIRXFrame;
	if((USARTE0_STATUS & USART_FERR_bm) || (USARTE0_STATUS & USART_BUFOVF_bm))
	{
		handleCommError(45);
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
		handleCommError(1);
		g_ucRXLength = 0; // reset received data length
		g_tUCIRXFrame.len = UCIHEADERLEN; // reset header length in recd. data
	}
	OS_TRACE(102);
}

uint8_t HandleSerial(UCIFrame_t *_RXFrame, uint8_t content)
{
	uint8_t ret = 99;

	if(
		(_RXFrame->ID == SLAVEDERIALID) &&
		(UCIGetCRC(&g_tUCIRXFrame) == g_tUCIRXFrame.crc) )
	{
		if(_RXFrame->UCI != content) return 98;

		switch(_RXFrame->UCI)
		{
		case UCI_GET_CMDs:
			OS_MutexGet(OSMTXCommand);
			memcpy((uint8_t*)&g_tCommand, _RXFrame->values, sizeof(g_tCommand));
			g_GotNewComand = 1;
			OS_MutexRelease(OSMTXCommand);
			ret =0; // ok, packet received!
			break;
		case UCI_GET_OPVs:
			OS_MutexGet(OSMTXBattInfo);
			memcpy((uint8_t*)&g_tBattery_Info, _RXFrame->values, sizeof(g_tBattery_Info));

			OS_MutexRelease(OSMTXBattInfo);
			ret =0; // ok, packet received!
			break;
		case UCI_SET_CMDs:
			ret =0; // ok, packet received!
			break;
		case UCI_WRITE_EEPROM:
			ret =0; // ok, packet received!
			break;
		case UCI_ACTION:
			ret =0; // ok, packet received!
			break;
		default:
			break;
		}
	}
	else
	{
		ret = 3; // CRC wrong
	}

	memset(_RXFrame,0xff,sizeof(UCIFrame_t)); // clear frame buffer

	return ret;

}

// ATTENTION, the data given with the Pointer IS MODIFIED HERE!!!!!
void UCISendBlockCrc( UCIFrame_t* pU) // if the master sends a block, it is to be assumed, that the RX data is useless, therefore it is discarded here.
{
	// prepare RX for answer BEFORE sending, as we could be interrupted afterwards.

	g_ucRXLength = 0; // reset received data length
	g_tUCIRXFrame.len = UCIHEADERLEN; // reset header length in recd. data
	pU->crc = 0;
	pU->crc = CRC8x((uint8_t*)pU ,pU->len);
	if(USARTSendBlockDMA(&DMA.CH1,(uint8_t*)pU ,pU->len) != 0 )
	{
		handleCommError(4);
	}
}

uint8_t UCIGetCRC( UCIFrame_t* pU)
{
	uint8_t tempCRC = pU->crc;
	pU->crc = 0;
	uint8_t res = CRC8x((uint8_t*)pU,pU->len);
	pU->crc = tempCRC;
	return res;
}

eBatteryStatus_t GetBattStatus(void)
{
	if(g_OPVsValid == 1)
		return g_tBattery_Info.eState;
	else
		return eBattUnknown;
}


uint8_t LastCommError =0;
#define COMMERRARRSIZE 5 // !! watch buffer size!!!
uint8_t CommErrArr[COMMERRARRSIZE]; // fixme debug only!!!
uint8_t CommErrArrIdx = 0;

void handleCommError(uint8_t errNo)
{
	//gCommandsKnown = 0; // fixme
	OS_PREVENTSCHEDULING
	if(CommErrArrIdx < COMMERRARRSIZE && errNo != 0)
	{
		CommErrArr[CommErrArrIdx] = errNo;
		CommErrArrIdx++;
	}

	LastCommError = errNo;
OS_ALLOWSCHEDULING
}


uint8_t getLastCommError(void)
{
	return LastCommError;
}

