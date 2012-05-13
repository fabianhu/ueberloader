/*
	This is the Üeberloader SHARED USART

	(c) 2010 Fabian Huslik

*/

#include "usart.h"
#include "usart_variant.h"

void USARTinit(void)
{
	//	1. Set the TxD pin value high.
	PORTE_OUTSET = (1<<3);
	//	2. Set the TxD pin as output.
	PORTE_DIRSET = (1<<3);
	//	3. Set the baud rate and frame format.

	USARTE0_BAUDCTRLA = BSEL & 0xFF; // BSEL 7:0
	USARTE0_BAUDCTRLB = BSCALE << 4 | (BSEL & 0xF00)>>8; // BSCALE, BSEL 11:8
	//	4. Set mode of operation.
	USARTE0_CTRLA = USART_RXCINTLVL_LO_gc; // RX isr low prio, because the Scheduler must never be interrupted.
	USARTE0_CTRLC = 0b011; // asyncronous, no parity, one stop bit, 8 bits
	//	5. Enable the Transmitter or the Receiver depending on the usage.
	USARTE0_CTRLB =  USART_RXEN_bm |USART_TXEN_bm; //

	// generally enable low level ISR
	PMIC_CTRL |= PMIC_LOLVLEN_bm;

	DMA_CH1_DESTADDR0 = (int)(&USARTE0.DATA) & 0xff;
	DMA_CH1_DESTADDR1 = ((int)(&USARTE0.DATA) & 0xff00)>>8;
	DMA_CH1_DESTADDR2 = ((int)(&USARTE0.DATA) & 0xff0000)>>16;
	DMA_CH1_CTRLA = DMA_CH_BURSTLEN_1BYTE_gc | DMA_CH_SINGLE_bm;
	DMA_CH1_CTRLB = 0;//DMA_CH_TRNINTLVL_LO_gc; // Lo isr for complete
	DMA_CH1_ADDRCTRL = DMA_CH_SRCRELOAD_TRANSACTION_gc | DMA_CH_SRCDIR_INC_gc | DMA_CH_DESTRELOAD_NONE_gc | DMA_CH_DESTDIR_FIXED_gc;
	DMA_CH1_TRIGSRC = DMA_CH_TRIGSRC_USARTE0_DRE_gc; // Channel 2 triggers DMA. (attention: channel 4 does not exist!!!)
	//DMA_CH1.REPCNT = 1;
//
//	DMA_CH2.SRCADDR0 = (int)(&USARTE0.DATA) & 0xff;
//	DMA_CH2.SRCADDR1 = ((int)(&USARTE0.DATA) & 0xff00)>>8;
//	DMA_CH2.SRCADDR2 = ((int)(&USARTE0.DATA) & 0xff0000)>>16;
//	DMA_CH2.CTRLA = DMA_CH_BURSTLEN_1BYTE_gc | DMA_CH_SINGLE_bm;//0b00000001;
//	DMA_CH2.CTRLB = DMA_CH_TRNINTLVL_HI_gc; // Hi isr for complete
//	DMA_CH2.ADDRCTRL = DMA_CH_SRCRELOAD_NONE_gc | DMA_CH_SRCDIR_FIXED_gc | DMA_CH_DESTRELOAD_TRANSACTION_gc | DMA_CH_DESTDIR_INC_gc;
//	DMA_CH2.TRIGSRC = DMA_CH_TRIGSRC_USARTE0_RXC_gc;
//	DMA_CH2.REPCNT = 1;

	// enable TX pin
	PORTE_DIRSET = (1<<3); // PE3

	DMA_CTRL |= DMA_ENABLE_bm;
}

//ISR(DMA_CH2_vect)
//{
//	uint16_t i;
//
//	i= GetRecvdBytes(&DMA_CH2);
//}


uint8_t USARTSendBlockDMA(DMA_CH_t* DMAch, uint8_t* pArray, uint8_t Len) // return 1 on busy
{
	if((DMAch->CTRLB & DMA_CH_CHBUSY_bm) || (DMAch->CTRLB & DMA_CH_CHPEND_bm))
	{
		// Error, Busy.
		return 1;
	}
	else
	{
		DMAch->SRCADDR0 = (int)(pArray) & 0xff;
		DMAch->SRCADDR1 = ((int)(pArray) & 0xff00)>>8;
		DMAch->SRCADDR2 = ((int)(pArray) & 0xff0000)>>16;
		DMAch->TRFCNT = Len; // the whole array
		DMAch->CTRLA |= DMA_CH_ENABLE_bm;
		return 0;
	}
}

//
//void RecvBlockDMA(DMA_CH_t* DMAch, uint8_t* pArray, uint8_t Len) // Len = max rx length
//{
//	DMAch->DESTADDR0 = (int)(pArray) & 0xff;
//	DMAch->DESTADDR1 = ((int)(pArray) & 0xff00)>>8;
//	DMAch->DESTADDR2 = ((int)(pArray) & 0xff0000)>>16;
//	DMAch->TRFCNT = Len; // the whole array
//	DMAch->CTRLA |= DMA_CH_ENABLE_bm;
//}
//
//uint16_t GetRecvdBytes(DMA_CH_t* DMAch)
//{
//	return DMAch->TRFCNT;
//}

