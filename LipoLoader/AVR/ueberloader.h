/* definitions for menu */

#include <avr/io.h>





typedef struct ADC_Values_tag
{
	int16_t		TempInt;
	int16_t		VCC_mVolt;
	int16_t 	Bandgap;
	int16_t		TempCPU;
}ADC_Values_t;





#define abs(X) (((X)<0)?-(X):(X))
