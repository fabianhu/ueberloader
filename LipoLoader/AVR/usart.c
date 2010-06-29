/*
	This is the Üeberloader SHARED USART

	(c) 2010 Fabian Huslik

*/

#include "usart.h"
#include "usart_variant.h"
#include "OS/FabOS.h"


void USARTinit(void)
{
	//	1. Set the TxD pin value high.
	PORTE.OUTSET = (1<<3);
	//	2. Set the TxD pin as output.
	PORTE.DIRSET = (1<<3);
	//	3. Set the baud rate and frame format.

	USARTE0.BAUDCTRLA = BSEL & 0xFF; // BSEL 7:0
	USARTE0.BAUDCTRLB = BSCALE << 4 | (BSEL & 0xF00)>>8; // BSCALE, BSEL 11:8
	//	4. Set mode of operation.
	USARTE0.CTRLA = USART_RXCINTLVL0_bm; // RX isr low prio
	USARTE0.CTRLC = 0b011; // asyncronous, no parity, one stop bit, 8 bits
	//	5. Enable the Transmitter or the Receiver depending on the usage.
	USARTE0.CTRLB =  USART_RXEN_bm |USART_TXEN_bm; //

	DMA.CH1.DESTADDR0 = (int)(&USARTE0.DATA) & 0xff;
	DMA.CH1.DESTADDR1 = ((int)(&USARTE0.DATA) & 0xff00)>>8;
	DMA.CH1.DESTADDR2 = ((int)(&USARTE0.DATA) & 0xff0000)>>16;
	DMA.CH1.CTRLA = DMA_CH_BURSTLEN_1BYTE_gc | DMA_CH_SINGLE_bm;//0b00000001;
	DMA.CH1.CTRLB = 0;//DMA_CH_TRNINTLVL_HI_gc; // Hi isr for complete
	DMA.CH1.ADDRCTRL = DMA_CH_SRCRELOAD_TRANSACTION_gc | DMA_CH_SRCDIR_INC_gc | DMA_CH_DESTRELOAD_NONE_gc | DMA_CH_DESTDIR_FIXED_gc;
	DMA.CH1.TRIGSRC = DMA_CH_TRIGSRC_USARTE0_DRE_gc; // Channel 2 triggers DMA. (attention: channel 4 does not exist!!!)
	//DMA.CH1.REPCNT = 1;
//
//	DMA.CH2.SRCADDR0 = (int)(&USARTE0.DATA) & 0xff;
//	DMA.CH2.SRCADDR1 = ((int)(&USARTE0.DATA) & 0xff00)>>8;
//	DMA.CH2.SRCADDR2 = ((int)(&USARTE0.DATA) & 0xff0000)>>16;
//	DMA.CH2.CTRLA = DMA_CH_BURSTLEN_1BYTE_gc | DMA_CH_SINGLE_bm;//0b00000001;
//	DMA.CH2.CTRLB = DMA_CH_TRNINTLVL_HI_gc; // Hi isr for complete
//	DMA.CH2.ADDRCTRL = DMA_CH_SRCRELOAD_NONE_gc | DMA_CH_SRCDIR_FIXED_gc | DMA_CH_DESTRELOAD_TRANSACTION_gc | DMA_CH_DESTDIR_INC_gc;
//	DMA.CH2.TRIGSRC = DMA_CH_TRIGSRC_USARTE0_RXC_gc;
//	DMA.CH2.REPCNT = 1;

	// enable TX pin
	PORTE.DIRSET = (1<<3); // PE3

	DMA.CTRL |= DMA_ENABLE_bm;
}

//ISR(DMA_CH2_vect)
//{
//	uint16_t i;
//
//	i= GetRecvdBytes(&DMA.CH2);
//}


uint8_t USARTSendBlockDMA(DMA_CH_t* DMAch, uint8_t* pArray, uint8_t Len) // return 1 on busy
{
	if(DMAch->CTRLB & DMA_CH_CHBUSY_bm || DMAch->CTRLB & DMA_CH_CHPEND_bm)
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


/* C## code
/// <summary>
/// CRC16S
///
/// The S stands for "Simple", because we use no lookup table here.
/// calculates the CRC16 Checksum of a given byte array
/// using the x^16+x^15+x^2+1 generator polynom (0xA001)
/// processing each byte without using a lookup table
/// </summary>
/// <param name="Data">a byte array of data</param>
/// <returns>2 byte CRC Checksum</returns>
public static byte[] CRC16S ( byte[] Data )
{
  ushort Polynom = 0xA001;
  ushort Register = 0xFFFF;
  ushort temp = 0x00;

  // loop through the entire array of bytes
  for ( int i = 0; i < Data.Length; i++ )
  {
    temp = Data[i];

    // shift all 8 data bits once
    for ( int y = 0; y < 8; y++ )
    {
      if ( ( ( Register ^ temp ) & 0x01 ) == 0x01 )
      {
        Register >>= 1;
        Register ^= Polynom;
      }
      else
      {
        Register >>= 1;
      }
      temp >>= 1; // shift data 1 bit right, dividing it by 2

    } // end of inner for loop (bit shift loop)
  } // end of outer for loop (data loop)

  // now we have got our overall 2-byte CRC "Checksum" number
  return new byte[2] { (byte) ( Register / 256 ), (byte) ( Register %
256 ) };
} // end of CRC16S Method
*/

#define CRC16INIT 0xA001

uint16_t calcCRC16r(uint16_t crc, uint16_t c)
{ // CCITT 16 bit (X^16 + X^12 + X^5 + 1).
  crc  = (unsigned char)(crc >> 8) | (crc << 8);
  crc ^= c;
  crc ^= (unsigned char)(crc & 0xff) >> 4;
  crc ^= (crc << 8) << 4;
  crc ^= ((crc & 0xff) << 4) << 1;

  return (crc);
}

uint16_t calcCRC16(uint16_t* c, uint16_t len)
{
	uint16_t i;
	uint16_t crc=CRC16INIT;
	for(i=0;i<len;i++)
	{
		crc = calcCRC16r(crc,c[i]);
	}
	return crc;
}

uint8_t CRC8c(uint8_t input, uint8_t seed)
{
    uint8_t i, feedback;

    for (i=0; i<8; i++)
    {
        feedback = ((seed ^ input) & 0x01);
        if (!feedback) seed >>= 1;
        else
        {
            seed ^= 0x18;
            seed >>= 1;
            seed |= 0x80;
        }
        input >>= 1;
    }

    return seed;
}

uint8_t CRC8x(uint8_t *input,uint8_t len)
{
    uint8_t i, check;

    check=0;

    for (i=0; i<len; i++)
    {
        check = CRC8c(input[i], check);
    }

    return check;
}
