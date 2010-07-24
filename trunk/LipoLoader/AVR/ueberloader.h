/* definitions for menu */

#include <avr/io.h>


void ResetLastBatteryInfo(void);


typedef struct ADC_Values_tag
{
	int16_t		TempInt;
	int16_t		VCC_mVolt;
	int16_t 	Bandgap;
	int16_t		TempCPU;
}ADC_Values_t;


#define LED_ON  PORTD.OUTSET = (1<<3);
#define LED_OFF PORTD.OUTCLR = (1<<3);


#define abs(X) (((X)<0)?-(X):(X))


