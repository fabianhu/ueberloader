/*
 * PWM generation for the �eberloader
 *
 * */

#include "pwm.h"

void initPWM(void)
{
	TCC0.CTRLA = TC_CLKSEL_DIV1_gc;
	TCC0.CTRLB = 0b00110000 | TC_WGMODE_SS_gc; // single slope
	TCC0.CTRLC = 0; // not used: manually activate output compare
	TCC0.CTRLD = TC_EVACT_RESTART_gc | TC_EVSEL_CH0_gc; // event0 does restart of the timer.
	TCC0.CTRLE = 0; // not used: last bit: Byte-mode
	TCC0.PERBUF = PERIOD_H; // 100khz
	TCC0.CCABUF = 0;
	TCC0.CCBBUF = 0;

	HIRESC.CTRL = HIRES_HREN_TC0_gc;

	TCC0.INTCTRLA = 0; // no error/overflow interrupt
	//TCC0.INTCTRLB = TC_CCDINTLVL_HI_gc;

	TCD0.CTRLA = TC_CLKSEL_DIV1_gc;
	TCD0.CTRLB = 0b00110000 | TC_WGMODE_SS_gc; // single slope
	TCD0.CTRLC = 0; // not used: manually activate output compare
	TCD0.CTRLD = TC_EVACT_RESTART_gc | TC_EVSEL_CH0_gc; // event0 does restart of the timer.
	TCD0.CTRLE = 0; // not used: last bit: Byte-mode
	TCD0.PERBUF = PERIOD_H;// 0+9*4; // 10khz (9 �berl�ufe, 4 wg. hires)
	TCD0.CCABUF = PERIOD_H-MINSWITCHOFFPWM;
	TCD0.CCBBUF = 0;

	HIRESD.CTRL = HIRES_HREN_TC0_gc;

	// sync timers via event 0
	EVSYS.STROBE = 0b00000001;
}


void setPWM(uint16_t usPower, uint16_t usStartstep)
{
	if ( usPower <= PERIOD_H - MINSWITCHOFFPWM )
	{
		//buck
		// links IN
		TCC0.CCABUF = usPower;
		TCC0.PERBUF = PERIOD_H;

		// rechts IN
		TCD0.CCABUF = PERIOD_L - MINSWITCHOFFPWM;
		TCD0.PERBUF = PERIOD_L;

		SetEnableBuck(usStartstep);
	}
	else if(usPower <= PERIOD_H)
	{
		uint32_t u;
		u = MINSWITCHOFFPWM - (PERIOD_H - (uint32_t)usPower);
		u = u*100*PERIOD_DIV  / MINSWITCHOFFPWM; // result = 1..10 (*100)
		u = PERIOD_H * u /100;
		TCC0.PERBUF = PERIOD_H + u;
		TCC0.CCABUF = TCC0.PERBUF - MINSWITCHOFFPWM;

		TCD0.PERBUF = PERIOD_L;
		TCD0.CCABUF = PERIOD_L - MINSWITCHOFFPWM;

		SetEnableBuck(usStartstep);
		//EVSYS.STROBE = (1<<0); // sync timers
	}
	else if(usPower <= PERIOD_H + MINSWITCHOFFPWM)
	{
		uint32_t u;
		u = MINSWITCHOFFPWM - (usPower - PERIOD_H);
		u = u*100*PERIOD_DIV / MINSWITCHOFFPWM; // result = 1..10 (*100)
		u = PERIOD_H * u / 100;
		TCD0.PERBUF = PERIOD_H + u;
		TCD0.CCABUF = TCD0.PERBUF - MINSWITCHOFFPWM;

		TCC0.PERBUF = PERIOD_L;
		TCC0.CCABUF = TCC0.PERBUF - MINSWITCHOFFPWM;

		SetEnableBoost(usStartstep);
		//EVSYS.STROBE = (1<<0); // sync timers
	}
	else
	{
		//boost
		// links IN
		TCC0.CCABUF = TCC0.PERBUF - MINSWITCHOFFPWM;

		// rechts IN
		TCD0.CCABUF = PERIOD_H*2 - usPower;//PERIOD_H*2 - power - 2* MINSWITCHOFFPWM ;

		// links ENABLE immer an:
		TCC0.CCBBUF = PERIOD_MAX;


		TCC0.PERBUF = PERIOD_L;
		TCD0.PERBUF = PERIOD_H;

		SetEnableBoost(usStartstep);
	}
}


void SetEnableBuck(uint16_t usStartstep) // 1000-0 scaled; 0= fully started
{
	uint32_t u;
	// links ENABLE immer an:
	TCC0.CCBBUF = PERIOD_MAX;

	if(usStartstep)
	{
		u= ((uint32_t)TCD0.PERBUF+4ul) * (uint32_t)usStartstep / STARTMAX;

		// rechts ENABLE falls start immer aus:
		TCD0.CCBBUF = u;//(invertiert!)
	}
	else
	{
		TCD0.CCBBUF = 0;
	}
}

void SetEnableBoost(uint16_t usStartstep) // 1000-0 scaled; 0= fully started
{
	uint32_t u;

	// links ENABLE immer an:
	TCC0.CCBBUF = PERIOD_MAX;

	if(usStartstep)
	{
		u= (uint32_t)TCD0.CCABUF * (uint32_t)usStartstep / STARTMAX;

		// rechts ENABLE l�uft mit:
		TCD0.CCBBUF = u;//PERIOD_H*2 - power;//(invertiert!)
	}
	else
	{
		//rechts ENABLE
		TCD0.CCBBUF = 0; //(invertiert!)
	}
}


/// BELOW CODE FRAGMENTS

// sync timers
/*if (TCC0.PERBUF > TCD0.PERBUF)
{
	// TCC is the resetter
	TCC0.CCDBUF = TCC0.PER;
	EVSYS.CH0MUX = 0b11000111; // select compare match TCC - D to create reset of both timers.

}
else
{	// TCD is the resetter
	TCD0.CCDBUF = TCD0.PER;
	EVSYS.CH0MUX = 0b11010111; // select compare match TCD - D to create reset of both timers.
}*/