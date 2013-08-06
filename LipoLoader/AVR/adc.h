
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


void ADCinit(void); //(returns offset)

void ADC_ActivateHiCurrentMeas(void);
void ADC_ActivateLoCurrentMeas(void);

void ADC_StartConvCh3Pin(uint8_t c);
void ADC_StartConvInt(uint8_t c);
int16_t ADC_ScaleCell_mV(int16_t raw);
int16_t ADC_ScaleVolt_mV(int16_t raw);
int16_t ADC_ScaleLowAmp_mA(int16_t raw);
int16_t ADC_ScaleHighAmp_mA(int16_t raw, int16_t zero);


int16_t ADC_GetISRValue(uint8_t ch);