#include <avr/io.h>
#include <avr/interrupt.h>

#include "adc.h"
#include "OS/FabOS.h"

volatile uint16_t g_usADCvalues[8];

void ADCinit(void)
{
	ADCSRA |= (1<<ADEN)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(0<<ADPS0);
	ADMUX = 0;
}

void ADCStartConvCh(uint8_t c)
{
	ADMUX = 0b111 & c;
	ADCSRA |= (1<<ADSC);
}

void ADCStartConvAll(void)
{
	ADMUX = 0b111 & 0;
	ADCSRA |= (1<<ADSC);
}

ISR(SIG_ADC)
{
	// copy ADC
	g_usADCvalues[ADMUX] = ADC;
	

	// start new conversion
	if( ADMUX < 7)
	{
		ADMUX++;
		ADCSRA |= (1<<ADSC);
	}
	else
	OS_SetEvent(0,1);
}
