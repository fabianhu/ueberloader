/*
 * USART
 */
#include <avr/io.h>


void USARTinit(void);
uint8_t USARTSendBlockDMA(DMA_CH_t* DMAch, uint8_t* pArray, uint8_t Len);
void RecvBlockDMA(DMA_CH_t* DMAch, uint8_t* pArray, uint8_t Len);
uint16_t GetRecvdBytes(DMA_CH_t* DMAch);
void USART_TX_Testxx(void);

