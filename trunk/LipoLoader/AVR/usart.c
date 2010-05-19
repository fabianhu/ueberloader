/*
	This is the Üeberloader USART

	(c) 2010 Fabian Huslik

*/

#include "usart.h"
#include "OS/FabOS.h"

uint8_t USART_TX_Array[100];

void USARTinit(void)
{
	//	1. Set the TxD pin value high.
	PORTE.OUTSET = (1<<3);
	//	2. Set the TxD pin as output.
	PORTE.DIRSET = (1<<3);
	//	3. Set the baud rate and frame format.
#define BSEL 3301
#define BSCALE -5

	USARTE0.BAUDCTRLA = BSEL & 0xFF; // BSEL 7:0
	USARTE0.BAUDCTRLB = BSCALE << 4 | (BSEL & 0xF00)>>8; // BSCALE, BSEL 11:8
	//	4. Set mode of operation.
	USARTE0.CTRLA = 0; // no interrupts
	USARTE0.CTRLC = 0b011; // asyncronous, no parity, one stop bit, 8 bits
	//	5. Enable the Transmitter or the Receiver depending on the usage.
	USARTE0.CTRLB =  USART_TXEN_bm; //USART_RXEN_bm |

	DMA.CH1.DESTADDR0 = (int)(&USARTE0.DATA) & 0xff;
	DMA.CH1.DESTADDR1 = ((int)(&USARTE0.DATA) & 0xff00)>>8;
	DMA.CH1.DESTADDR2 = ((int)(&USARTE0.DATA) & 0xff0000)>>16;
	DMA.CH1.CTRLA = DMA_CH_BURSTLEN_1BYTE_gc; // | DMA_CH_SINGLE_bm;//0b00000001;
	DMA.CH1.CTRLB = 0;//DMA_CH_TRNINTLVL_HI_gc; // Hi isr for complete
	DMA.CH1.ADDRCTRL = DMA_CH_SRCRELOAD_BLOCK_gc | DMA_CH_SRCDIR_INC_gc | DMA_CH_DESTRELOAD_NONE_gc | DMA_CH_DESTDIR_FIXED_gc;//0b01011101;
	DMA.CH1.TRIGSRC = DMA_CH_TRIGSRC_USARTE0_DRE_gc; // Channel 2 triggers DMA. (attention: channel 4 does not exist!!!)

	DMA.CH1.REPCNT = 1;


	DMA.CTRL |= DMA_ENABLE_bm;

}

void USART_TX_Test(void)
{
	uint8_t i;
	for(i=0;i<100;i++)
	{
		USART_TX_Array[i] = i;
	}

	SendBlock(USART_TX_Array,100);

}

uint8_t SendBlock(uint8_t* pArray, uint8_t Len) // return 1 on busy
{
	if(DMA.STATUS & DMA_CH1BUSY_bm || DMA.STATUS & DMA_CH1PEND_bm)
	{
		// Error, Busy.
		return 1;
	}
	else
	{
		DMA.CH1.SRCADDR0 = (int)(pArray) & 0xff;
		DMA.CH1.SRCADDR1 = ((int)(pArray) & 0xff00)>>8;
		DMA.CH1.SRCADDR2 = ((int)(pArray) & 0xff0000)>>16;
		DMA.CH1.TRFCNT = Len; // the whole array
		DMA.CH1.CTRLA |= DMA_CH_ENABLE_bm;
		return 0;
	}

}
