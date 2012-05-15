/* definitions for menu */

#include <avr/io.h>


void ResetLastBatteryInfo(void);



#define LED_ON  PORTD.OUTSET = (1<<3);
#define LED_OFF PORTD.OUTCLR = (1<<3);



