
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

void ADCinit(void);

void ADCStartConvCh(uint8_t c);

void ADCStartConvAll(void);
