/*
 * usart_variant.h for Loader
 *
 *  Created on: 29.06.2010
 *      Author: Besitzer
 */

#ifndef USART_VARIANT_H_
#define USART_VARIANT_H_


// values for 9600 baud and F_CPU == 32000000
#if F_CPU != 32000000
#error "please correct the baud rate"
#endif

#define BSEL 3317
#define BSCALE 0b1100

#if BSEL > 4096
#error "too much BSEL!!"
#endif

#endif /* USART_VARIANT_H_ */
