/*
 * usart_variant.h for Display
 *
 *  Created on: 29.06.2010
 *      Author: Fabian
 */

#ifndef USART_VARIANT_H_
#define USART_VARIANT_H_

// values for 9600 baud and F_CPU == 16000000
#if F_CPU != 16000000
#error "please correct the baud rate"
#endif

#define BSEL 3301
#define BSCALE 0b1011

#if BSEL > 4096
#error "too much BSEL!!"
#endif

#endif /* USART_VARIANT_H_ */
