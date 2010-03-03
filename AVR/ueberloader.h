/* definitions for menu */

#include <avr/io.h>

typedef struct ADC_Values_tag
{
	uint16_t		CellVolt[6];
	uint16_t		TempInt[2];
	uint16_t		VCC;
	//uint16_t 		IntRef;
	uint16_t		TempCPU;
}ADC_Values_t;

