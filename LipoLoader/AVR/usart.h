/*
 * USART
 */
#include <avr/io.h>


void USARTinit(void);
uint8_t SendBlock(uint8_t* pArray, uint8_t Len);
void USART_TX_Test(void);
