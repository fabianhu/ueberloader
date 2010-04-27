
/*! Offset addresses for production signature row on which is not in current GCC
 *  header files
 * \name GCC compatibility macros
 */
//@{
#ifndef ADCACAL0_offset

#define ADCACAL0_offset 0x20
#define ADCACAL1_offset 0x21
#define ADCBCAL0_offset 0x24
#define ADCBCAL1_offset 0x25
#endif
//@}

typedef struct Calibration_tag
{
	int16_t sADCOffset;
	uint16_t usCPUTemp85C;
	int16_t sADCRef_mV;
}Calibration_t;



uint16_t ADCinit(void); //(returns offset)

void ADC_ActivateHiCurrentMeas(void);
void ADC_ActivateLoCurrentMeas(void);

void ADC_StartConvCh3Pin(uint8_t c);
void ADC_StartConvInt(uint8_t c);
uint16_t ADC_ScaleCell_mV(uint16_t in);

