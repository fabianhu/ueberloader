
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
uint8_t 		glCommError=0;


// Prototypes:
uint8_t vWaitForResult(void);
void HandleSerial(UCIFrame_t *_RXFrame);

void TaskCommand(void)
{
	OS_SetAlarm(OSALMCommandRepeat,1000);
	while(1)
	{
		OS_WaitAlarm(OSALMCommandRepeat);
		OS_SetAlarm(OSALMCommandRepeat,300);


		if(1)
		{
			g_tUCITXBlock.ID = 55;
			g_tUCITXBlock.UCI = UCI_GET_CMDs;
			g_tUCITXBlock.len = UCIHEADERLEN;
			UCISendBlockCrc(&g_tUCITXBlock);
			glCommError = vWaitForResult();
		}




		g_tUCITXBlock.ID = 55;
		g_tUCITXBlock.UCI = UCI_GET_OPVs;
		g_tUCITXBlock.len = UCIHEADERLEN;
		UCISendBlockCrc(&g_tUCITXBlock);
	    glCommError = vWaitForResult();

	    if(g_NewComand)
	    {
	    	g_tUCITXBlock.ID = 55;
			g_tUCITXBlock.UCI = UCI_SET_CMDs;
			g_tUCITXBlock.len = UCIHEADERLEN+sizeof(Command_t);
	    	OS_MutexGet(OSMTXCommand);
				memcpy(g_tUCITXBlock.values,&g_tCommand,sizeof(Command_t));
	    	OS_MutexRelease(OSMTXCommand);
	    	UCISendBlockCrc(&g_tUCITXBlock);

	    	//OS_WaitTicks(OSALMCommandWait,20);
			//glCommError = vWaitForResult();
	    	g_NewComand = 0;
	    }

	    /*g_tUCITXBlock.ID = 55;
	    g_tUCITXBlock.UCI = UCI_GET_CMDs;
	    g_tUCITXBlock.len = UCIHEADERLEN;
	    UCISendBlockCrc(&g_tUCITXBlock);
	    glCommError = vWaitForResult();*/
	}
}


uint8_t vWaitForResult(void)
{
	uint8_t ret;
	uint8_t commerror = 0;

	ret = OS_WaitEventTimeout(OSEVTDataRecvd,OSALMCommandTimeout, 150); // at 9600 baud it takes 105 ms to transfer the stuff.
    if(ret == OSEVTDataRecvd)
	{
		HandleSerial(&g_tUCIRXFrame);
	}
	else
	{
		commerror = 100;
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
			g_GotNewComand = 1;
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

