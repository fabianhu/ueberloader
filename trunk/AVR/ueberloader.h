/* definitions for menu */

#include <avr/io.h>

typedef struct ADC_Values_tag
{
	uint16_t		Cell_mVolt[6];
	uint16_t		TempInt[2];
	uint16_t		VCC_mVolt;
	//uint16_t 		IntRef;
	uint16_t		TempCPU;
}ADC_Values_t;

typedef struct Calibration_tag
{
	uint16_t usADCOffset;
}Calibration_t;
