/* definitions for menu */

#include <avr/io.h>

typedef struct ADC_Values_tag
{
	uint16_t		CellVolt[6];
	uint16_t		VCC;
	//uint16_t 		ADC_Zero_Offset;
	uint16_t		TempCPU;
	uint16_t		TempInt[2];
}ADC_Values_t;

