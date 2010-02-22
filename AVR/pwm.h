/*
 * PWM settings for the Üeberloader
 *
 * */

#include <avr/io.h>

// Prototypes
void initPWM(void);
void setPWM(uint16_t usPower, uint16_t usStartstep);
void SetEnableBuck(uint16_t usStartstep); // 1000-0 scaled; 0= fully started
void SetEnableBoost(uint16_t usStartstep); // 1000-0 scaled; 0= fully started

// Defines
#define ENABLE_A_OFF	TCC0.CCBBUF = 0;
#define ENABLE_B_OFF	TCD0.CCBBUF = TCD0.PERBUF +4; // inverted

#define PERIOD_DIV 10ul
#define PERIOD_H 2560ul // 1280 = 100kHz
//define PERIOD_L PERIOD_H + (PERIOD_H +4)* PERIOD_DIV // common divider...
#define PERIOD_L PERIOD_H + (PERIOD_H )* PERIOD_DIV // odd divider...
#define PERIOD_MAX 0xffff
#define MINSWITCHOFFPWM 300

#define STARTMAX 1000ul;
