
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
	uint16_t usADCOffset;
	uint16_t usCPUTemp85C;
	uint16_t usADCRef_mV;
}Calibration_t;



uint16_t ADCinit(void); //(returns offset)

void vActivateHiCurrentMeas(void);
void vActivateLoCurrentMeas(void);

void ADCStartConvCh(uint8_t c);
void ADCStartConvInt(uint8_t c);

void ADCStartConvAll(void);
