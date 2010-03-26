/*
	This is the Üeberloader ADC

	(c) 2010 Fabian Huslik

*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "adc.h"
#include "OS/FabOS.h"

volatile int16_t g_sADCvalues[3];

#include <avr/pgmspace.h>

/*! \brief Function for GCC to read out calibration byte.
 *
 *  \note For IAR support, include the adc_driver_asm.S90 file in your project.
 *
 *  \param index The index to the calibration byte.
 *
 *  \return Calibration byte.
 */
uint8_t SP_ReadCalibrationByte( uint8_t index )
{
	uint8_t result;

	/* Load the NVM Command register to read the calibration row. */
	NVM_CMD = NVM_CMD_READ_CALIB_ROW_gc;
 	result = pgm_read_byte(index);

	/* Clean up NVM Command register. */
 	NVM_CMD = NVM_CMD_NO_OPERATION_gc;

	return result;
}


/*! \brief This function get the calibration data from the production calibration.
 *
 *  The calibration data is loaded from flash and stored in the calibration
 *  register. The calibration data reduces the non-linearity error in the adc.
 *
 *  \param  adc          Pointer to ADC module register section.
 */
void ADC_CalibrationValues_Load(ADC_t * adc)
{
	if(&ADCA == adc){
		 /* Get ADCACAL0 from production signature . */
		adc->CALL = SP_ReadCalibrationByte( PROD_SIGNATURES_START + ADCACAL0_offset );
		adc->CALH = SP_ReadCalibrationByte( PROD_SIGNATURES_START + ADCACAL1_offset );
	}else {
		/* Get ADCBCAL0 from production signature  */
		adc->CALL = SP_ReadCalibrationByte( PROD_SIGNATURES_START + ADCBCAL0_offset );
		adc->CALH = SP_ReadCalibrationByte( PROD_SIGNATURES_START + ADCBCAL1_offset );
	}
}

uint16_t gusTimer =0;


uint16_t ADCinit(void)
{
	// ADC concept new:
	/* CH0..2 : event triggered conversion of input and output voltage and Current - resulting in DMA0 ISR.
	 * CH3 : VCC_mVolt / zero offset / temperature internal / temperature external and Balancer channels -- cyclic
	 * */

	/* Move stored calibration values to ADC A. */
	ADC_CalibrationValues_Load(&ADCA);

	/* Set up ADC A to have unsigned conversion mode and 12 bit resolution. */
	ADCA.CTRLA = 0b00000000;
	ADCA.CTRLB = 0; // all unsigned !

	/* Set sample rate */
	ADCA.PRESCALER = ADC_PRESCALER_DIV16_gc;// resulting in 2MHz ADC clock

	/* Set reference voltage on ADC A to be VCC_mVolt/1.6 V.*/
	ADCA.REFCTRL = ADC_REFSEL_VCC_gc | ADC_TEMPREF_bm | ADC_BANDGAP_bm; // VCC_mVolt/1.6 reference

	ADCA.EVCTRL = ADC_SWEEP_012_gc | ADC_EVSEL_7_gc | ADC_EVACT_SWEEP_gc; // Event 7 releases sweep over ch 0-2



	ADCA.CH0.CTRL = ADC_CH_INPUTMODE_SINGLEENDED_gc; // external
	ADCA.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN0_gc;
	ADCA.CH0.INTCTRL = 0; // no ISR
	ADCA.CH1.CTRL = ADC_CH_INPUTMODE_SINGLEENDED_gc; // external
	ADCA.CH1.MUXCTRL = ADC_CH_MUXPOS_PIN1_gc;
	ADCA.CH1.INTCTRL = 0; // no ISR
	ADCA.CH2.CTRL = ADC_CH_INPUTMODE_SINGLEENDED_gc; // external
	ADCA.CH2.MUXCTRL = ADC_CH_MUXPOS_PIN2_gc;
	ADCA.CH2.INTCTRL = 0; // no ISR
	ADCA.CH3.CTRL = ADC_CH_INPUTMODE_SINGLEENDED_gc; // external
	ADCA.CH3.MUXCTRL = ADC_CH_MUXPOS_PIN3_gc;
	ADCA.CH3.INTCTRL = 0; // no ISR


	ADCA.CTRLA |= ADC_ENABLE_bm;
	//ADCA.CTRLA |= ADC_DMASEL_CH0123_gc; // check, if necessary: it is not.
	
	ADCA.CH0.CTRL = ADC_CH_START_bm;
	/* Wait until common mode voltage is stable. */
	do{
		/* If the conversion on the ADCA channel 0 never is
		 * complete this will be a deadlock. */
	}while(!(ADCA.CH0.INTFLAGS & 1));
	ADCA.INTFLAGS = 0b00001111;

 // setup DMA to transfer 3 successive conversions of ch 0-2 to the g_sADCvalues array.

	//DMA.INTFLAGS = DMA_CH_TRNINTLVL_HI_gc;
	DMA.CH0.SRCADDR0 = (int)(&ADCA.CH0RES) & 0xff;
	DMA.CH0.SRCADDR1 = ((int)(&ADCA.CH0RES) & 0xff00)>>8;
	DMA.CH0.SRCADDR2 = ((int)(&ADCA.CH0RES) & 0xff0000)>>16;
	DMA.CH0.DESTADDR0 = (int)(&g_sADCvalues[0]) & 0xff;
	DMA.CH0.DESTADDR1 = ((int)(&g_sADCvalues[0]) & 0xff00)>>8;
	DMA.CH0.DESTADDR2 = ((int)(&g_sADCvalues[0]) & 0xff0000)>>16;
	DMA.CH0.CTRLA = DMA_CH_BURSTLEN_8BYTE_gc | DMA_CH_SINGLE_bm;//0b00000001;
	DMA.CH0.CTRLB = DMA_CH_TRNINTLVL_HI_gc; // Hi isr for complete
	DMA.CH0.ADDRCTRL = DMA_CH_SRCRELOAD_BLOCK_gc | DMA_CH_SRCDIR_INC_gc | DMA_CH_DESTRELOAD_TRANSACTION_gc | DMA_CH_DESTDIR_INC_gc;//0b01011101;
	DMA.CH0.TRIGSRC = DMA_CH_TRIGSRC_ADCA_CH2_gc; // Channel 2 triggers DMA. (attention: channel 4 does not exist!!!)
	DMA.CH0.TRFCNT = 6; // 4 ADC-values with 16 bit
	DMA.CH0.REPCNT = 1;
	DMA.CH0.CTRLA |= DMA_CH_ENABLE_bm;

	DMA.CTRL |= DMA_ENABLE_bm;
	//ADCA.CTRLA |= 0b00111100; // start conversion for 4 channels


	// Event 7 triggers ADC sweep

	gusTimer = TCC1.CNT;

	EVSYS.STROBE = (1<<7);

	// fixme Get offset value for ADC A.
	return 140;
}

ISR(DMA_CH0_vect)
{

	
//volatile uint16_t d = TCC1.CNT - gusTimer;
// it takes 986 clk from activation to here.
	
	DMA.CH0.CTRLB |= DMA_CH_TRNIF_bm;// clear interrupt flag
	DMA.CH0.CTRLA |= DMA_CH_ENABLE_bm;// re-enable DMA

	//OS_SetEvent(0,1);
}

/*
 * Start conversion on ADC Channel 3, which is not cyclically triggered.
 *
 * Parameter c = Pin Number.
 *
 * */
void ADCStartConvCh(uint8_t c)
{
	/* M32 ADMUX = 0b111 & c;
	ADCSRA |= (1<<ADSC);*/

	ADCA.CH3.CTRL = ADC_CH_INPUTMODE_SINGLEENDED_gc; // external
	ADCA.CH3.MUXCTRL = (c << 3 ) & 0b01111000;

	ADCA.CH3.CTRL = ADC_CH_START_bm;
}

/*
 * Start internal conversion
 *
 * 0 = int temperature
 * 1 = Bandgap voltage
 * 2 = Scaled VCC_mVolt (/10)
 * 3 = DAC
 *
 * */
void ADCStartConvInt(uint8_t c)
{
	ADCA.CH3.CTRL = ADC_CH_INPUTMODE_INTERNAL_gc; // external
	ADCA.CH3.MUXCTRL = (c << 3 ) & 0b01111000;

	ADCA.CH3.CTRL = ADC_CH_START_bm;
}

void ADCStartConvAll(void)
{
	/* M32 ADMUX = 0b111 & 0;
	ADCSRA |= (1<<ADSC);*/
}

/* M32ISR(SIG_ADC)
{
	 // copy ADC
	g_sADCvalues[ADMUX] = ADC;
	

	// start new conversion
	if( ADMUX < 7)
	{
		ADMUX++;
		ADCSRA |= (1<<ADSC);
	}
	else
	OS_SetEvent(0,1);
}*/
