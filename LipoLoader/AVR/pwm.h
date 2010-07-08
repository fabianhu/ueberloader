/*
 * PWM settings for the �eberloader
 *
 * */

#include <avr/io.h>

// Prototypes
void vPWM_Init(void);
void vPWM_Set(uint16_t usPower, uint16_t usStartstep);
void SetEnableBuck(uint16_t usStartstep); // 1000-0 scaled; 0= fully started
void SetEnableBoost(uint16_t usStartstep); // 1000-0 scaled; 0= fully started

void vGovernor(	uint16_t _I_Set_mA,	uint16_t _I_Act_mA);

uint16_t PID(int16_t actual, int16_t set, uint16_t kP, int16_t kI, uint16_t kD, int16_t lowerLimit, int16_t upperLimit, uint8_t zero);
void RampUpDn(uint16_t* ramped, uint16_t target, uint16_t upper,uint16_t lower);
void RampUp(uint16_t* ramped, uint16_t target);
void RampDn(uint16_t* ramped, uint16_t target);



// Defines
#define ENABLE_A_OFF	TCC0.CCBBUF = 0;
#define ENABLE_B_OFF	TCD0.CCBBUF = TCD0.PERBUF +4; // inverted

#define PERIOD_DIV 10ul
#define PERIOD_H 2560ul // 1280 = 100kHz
//define PERIOD_L PERIOD_H + (PERIOD_H +4)* PERIOD_DIV // common divider...
#define PERIOD_L PERIOD_H + (PERIOD_H )* PERIOD_DIV // odd divider...
#define PERIOD_MAX 0xffff
#define MINSWITCHOFFPWM 300

#define STARTMAX 1000ul

#define STARTUPLEVEL_mA 300 // minimum current for syncronous op.

#define MAXCONVERTERPOWER_W 13 // maximum converter power


#define min(x,y) (((x)<(y))?(x):(y))
#define max(x,y) (((x)>(y))?(x):(y))
#define limit(v,l,u) (((v)<(l))?(l):(((v)>(u))?(u):(v)))