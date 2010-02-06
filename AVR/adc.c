#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "adc.h"
#include "OS/FabOS.h"
#include "adc_driver.h"

volatile uint16_t g_usADCvalues[8];

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
	//ADC_ConvMode_and_Resolution_Config(&ADCA, ADC_ConvMode_Signed, ADC_RESOLUTION_12BIT_gc);
	ADCA.CTRLA = 0b00000000;
	ADCA.CTRLB = ADC_CONMODE_bm ;//| ADC_FREERUN_bm; // unsigned

	/* Set sample rate */
	//ADC_Prescaler_Config(&ADCA, ADC_PRESCALER_DIV64_gc); // resulting in 2MHz ADC clock
	ADCA.PRESCALER = ADC_PRESCALER_DIV64_gc;

	/* Set reference voltage on ADC A to be VCC/1.6 V.*/
	//ADC_Reference_Config(&ADCA, ADC_REFSEL_VCC_gc);
	ADCA.REFCTRL = ADC_REFSEL_VCC_gc | ADC_TEMPREF_bm | ADC_BANDGAP_bm;

	ADCA.EVCTRL = ADC_SWEEP_0123_gc | ADC_EVSEL_7_gc | ADC_EVACT_SWEEP_gc;

	/* Get offset value for ADC A. */
	/*ADC_Ch_InputMode_and_Gain_Config(&ADCA.CH0,
									 ADC_CH_INPUTMODE_DIFF_gc,
									 ADC_DRIVER_CH_GAIN_NONE);*/
	ADCA.CH0.CTRL = ADC_CH_INPUTMODE0_bm; // external
	ADCA.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN0_gc;
	ADCA.CH1.CTRL = ADC_CH_INPUTMODE0_bm; // external
	ADCA.CH1.MUXCTRL = ADC_CH_MUXPOS_PIN1_gc;
	ADCA.CH2.CTRL = ADC_CH_INPUTMODE0_bm; // external
	ADCA.CH2.MUXCTRL = ADC_CH_MUXPOS_PIN2_gc;
	ADCA.CH3.CTRL = ADC_CH_INPUTMODE0_bm; // external
	ADCA.CH3.MUXCTRL = ADC_CH_MUXPOS_PIN3_gc;

/*
	ADC_Ch_InputMux_Config(&ADCA.CH0, ADC_CH_MUXPOS_PIN1_gc, ADC_CH_MUXNEG_PIN1_gc);
	ADC_Ch_InputMux_Config(&ADCA.CH1, ADC_CH_MUXPOS_PIN2_gc, ADC_CH_MUXNEG_PIN1_gc);
	ADC_Ch_InputMux_Config(&ADCA.CH2, ADC_CH_MUXPOS_PIN3_gc, ADC_CH_MUXNEG_PIN1_gc);
	ADC_Ch_InputMux_Config(&ADCA.CH3, ADC_CH_MUXPOS_PIN4_gc, ADC_CH_MUXNEG_PIN1_gc);
*/
	ADCA.CTRLA |= ADC_ENABLE_bm;
	ADCA.CTRLA |= ADC_DMASEL_CH0123_gc;
	
	ADCA.CH0.CTRL = ADC_CH_START_bm;
	/* Wait until common mode voltage is stable. */
	do{
		/* If the conversion on the ADCA channel 0 never is
		 * complete this will be a deadlock. */
	}while(!ADC_Ch_Conversion_Complete(&ADCA.CH0));
	ADCA.INTFLAGS = 0b00001111;



	//DMA.INTFLAGS = DMA_CH_TRNINTLVL_HI_gc;
	DMA.CH0.SRCADDR0 = (int)(&ADCA.CH0RES) & 0xff;
	DMA.CH0.SRCADDR1 = ((int)(&ADCA.CH0RES) & 0xff00)>>8;
	DMA.CH0.SRCADDR2 = ((int)(&ADCA.CH0RES) & 0xff0000)>>16;
	DMA.CH0.DESTADDR0 = (int)(&g_usADCvalues[0]) & 0xff;
	DMA.CH0.DESTADDR1 = ((int)(&g_usADCvalues[0]) & 0xff00)>>8;
	DMA.CH0.DESTADDR2 = ((int)(&g_usADCvalues[0]) & 0xff0000)>>16;
	DMA.CH0.CTRLA = DMA_CH_BURSTLEN_8BYTE_gc; //| DMA_CH_SINGLE_bm;//0b00000001;
	DMA.CH0.CTRLB = DMA_CH_TRNINTLVL_HI_gc; // 0b00000010; // medium isr for complete
	DMA.CH0.ADDRCTRL = DMA_CH_SRCRELOAD_BLOCK_gc | DMA_CH_SRCDIR_INC_gc | DMA_CH_DESTRELOAD_TRANSACTION_gc | DMA_CH_DESTDIR_INC_gc;//0b01011101;
	DMA.CH0.TRIGSRC = DMA_CH_TRIGSRC_EVSYS_CH0_gc;//DMA_CH_TRIGSRC_ADCA_CH4_gc;////;0x10+0x4; // ADCA / CH4
	DMA.CH0.TRFCNT = 8; // 4 ADC-values with 16 bit
	DMA.CH0.REPCNT = 1;
	DMA.CH0.CTRLA |= DMA_CH_ENABLE_bm;

	DMA.CTRL |= DMA_ENABLE_bm;
	//ADCA.CTRLA |= 0b00111100; // start conversion for 4 channels





	// Event 7 triggers ADC sweep

	// Event 0 triggers DMA transfer from ADC to array
	EVSYS.CH0CTRL = 0;
	EVSYS.CH0MUX  = EVSYS_CHMUX_ADCA_CH3_gc;

	EVSYS.STROBE = (1<<7);
	gusTimer = TCC1.CNT;

}

ISR(DMA_CH0_vect)
{
	uint8_t i;

	for(i=0;i<4;i++)
	{
	g_usADCvalues[i+4]= g_usADCvalues[i];
	}
	
	uint16_t d = TCC1.CNT - gusTimer;

// it takes 986 peripheral ck from activation to here. that is 246 ck. that is again 15,4 ADC-Clock (2MHz), which is quite ok.
	
	DMA.CH0.CTRLB |= DMA_CH_TRNIF_bm;// clear interrupt flag
	DMA.CH0.CTRLA |= DMA_CH_ENABLE_bm;// re-enable DMA
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
