/* definitions for menu */

#include <avr/io.h>


void ResetLastBatteryInfo(void);



#define LED_ON  PORTD.OUTSET = (1<<3);
#define LED_OFF PORTD.OUTCLR = (1<<3);


// special macro for binary constants in the form 0b10010101 is then B(10010101) 
#define B(x) \
0##x >>  0 & 0x01 | \
0##x >>  2 & 0x02 | \
0##x >>  4 & 0x04 | \
0##x >>  6 & 0x08 | \
0##x >>  8 & 0x10 | \
0##x >> 10 & 0x20 | \
0##x >> 12 & 0x40 | \
0##x >> 14 & 0x80
