#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "adc.h"
#include "OS/FabOS.h"
#include "adc_driver.h"

volatile uint16_t g_usADCvalues[8];

void ADCinit(void)
{
	/* M32 ADCSRA |= (1<<ADEN)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(0<<ADPS0);
	ADMUX = 0;*/

	/*// load ADC calibration registers
	NVM_CMD = NVM_CMD_READ_CALIB_ROW_gc; //Load the NVM Command register to read the calibration row.
	ADCA.CALL = pgm_read_byte( PROD_SIGNATURES_START + ADCACAL0_offset );
	ADCA.CALH = pgm_read_byte( PROD_SIGNATURES_START + ADCACAL1_offset );
	NVM_CMD = NVM_CMD_NO_OPERATION_gc; // Clean up NVM Command register.*/

	/* Move stored calibration values to ADC A. */
	ADC_CalibrationValues_Load(&ADCA);

	/* Set up ADC A to have signed conversion mode and 12 bit resolution. */
	ADC_ConvMode_and_Resolution_Config(&ADCA, ADC_ConvMode_Signed, ADC_RESOLUTION_12BIT_gc);

	/* Set sample rate */
	ADC_Prescaler_Config(&ADCA, ADC_PRESCALER_DIV64_gc); // resulting in 2MHz ADC clock

	/* Set reference voltage on ADC A to be VCC/1.6 V.*/
	ADC_Reference_Config(&ADCA, ADC_REFSEL_VCC_gc);

	/* Get offset value for ADC A. */
	ADC_Ch_InputMode_and_Gain_Config(&ADCA.CH0,
									 ADC_CH_INPUTMODE_DIFF_gc,
									 ADC_DRIVER_CH_GAIN_NONE);

	ADC_Ch_InputMux_Config(&ADCA.CH0, ADC_CH_MUXPOS_PIN1_gc, ADC_CH_MUXNEG_PIN1_gc);
	ADC_Ch_InputMux_Config(&ADCA.CH1, ADC_CH_MUXPOS_PIN2_gc, ADC_CH_MUXNEG_PIN1_gc);
	ADC_Ch_InputMux_Config(&ADCA.CH2, ADC_CH_MUXPOS_PIN3_gc, ADC_CH_MUXNEG_PIN1_gc);
	ADC_Ch_InputMux_Config(&ADCA.CH3, ADC_CH_MUXPOS_PIN4_gc, ADC_CH_MUXNEG_PIN1_gc);

	ADC_Enable(&ADCA);

	/* Wait until common mode voltage is stable. */
	do{
		/* If the conversion on the ADCA channel 0 never is
		 * complete this will be a deadlock. */
	}while(!ADC_Ch_Conversion_Complete(&ADCA.CH0));

	DMA.CTRL =DMA_ENABLE_bm;
	DMA.CH0.SRCADDR0 = (int)(&ADCA.CH0RES) & 0xff;
	DMA.CH0.SRCADDR1 = ((int)(&ADCA.CH0RES) & 0xff00)>>8;
	DMA.CH0.SRCADDR2 = ((int)(&ADCA.CH0RES) & 0xff0000)>>16;
	DMA.CH0.DESTADDR0 = (int)(&g_usADCvalues[0]) & 0xff;
	DMA.CH0.DESTADDR1 = ((int)(&g_usADCvalues[0]) & 0xff00)>>8;
	DMA.CH0.DESTADDR2 = ((int)(&g_usADCvalues[0]) & 0xff0000)>>16;
	DMA.CH0.CTRLA = 0b10000001;
	DMA.CH0.CTRLB = 0b00000010; // medium isr for complete
	DMA.CH0.ADDRCTRL = 0b01011101;
	DMA.CH0.TRIGSRC = 0x10+0x4; // ADCA / CH4
	DMA.CH0.TRFCNT = 4; // 4 values
	DMA.CH0.REPCNT = 1;

	ADCA.CTRLA |= 0b00111100; // start conversion for 4 channels

}

ISR(ADCA_CH0_vect)
{
	g_usADCvalues[0]++;

}

void ADCStartConvCh(uint8_t c)
{
	/* M32 ADMUX = 0b111 & c;
	ADCSRA |= (1<<ADSC);*/
}

void ADCStartConvAll(void)
{
	/* M32 ADMUX = 0b111 & 0;
	ADCSRA |= (1<<ADSC);*/
}

/* M32ISR(SIG_ADC)
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
}*/
