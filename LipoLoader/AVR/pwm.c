/*
 * PWM generation for the Üeberloader
 *
 * */

#include "pwm.h"
#include "serial.h"
#include "OS/FabOS.h"
#include "ueberloader.h"

extern Battery_Info_t g_tBattery_Info;

uint16_t pwm_us_period_H = 2560ul; // 1280 = 100kHz // main frequency
uint16_t pwm_us_period_div = 10ul;					// main / x = slow(refresh) frq.
uint16_t pwm_us_period_L; // init !!! = (pwm_us_period_H + (pwm_us_period_H )* pwm_us_period_div );

void PWM_Setfrequency(int16_t f) // in kHz!!!
{
	pwm_us_period_H = (12800/f)*10;
	pwm_us_period_L = (pwm_us_period_H + (pwm_us_period_H )* pwm_us_period_div );
}

void PWM_SetRatio(uint16_t r)
{
	pwm_us_period_div = r;
}


void vPWM_Init(void)
{
	pwm_us_period_L = (pwm_us_period_H + (pwm_us_period_H )* pwm_us_period_div );

	TCC0.CTRLA = TC_CLKSEL_DIV1_gc;
	TCC0.CTRLB = 0b00110000 | TC_WGMODE_SS_gc; // single slope
	TCC0.CTRLC = 0; // not used: manually activate output compare
	TCC0.CTRLD = TC_EVACT_RESTART_gc | TC_EVSEL_CH0_gc; // event0 does restart of the timer.
	TCC0.CTRLE = 0; // not used: last bit: Byte-mode
	TCC0.PERBUF = pwm_us_period_H;
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
	TCD0.PERBUF = pwm_us_period_H;// 0+9*4; // 10khz (9 Überläufe, 4 wg. hires)
	TCD0.CCABUF = pwm_us_period_H-MINSWITCHOFFPWM;
	TCD0.CCBBUF = 0;

	HIRESD.CTRL = HIRES_HREN_TC0_gc;

	// sync timers via event 0
	EVSYS.STROBE = 0b00000001;

	vPWM_Set(0,STARTMAX);
}


void vPWM_Set(uint16_t usPower, uint16_t usStartstep)
{
	if ( usPower <= pwm_us_period_H - MINSWITCHOFFPWM )
	{
		//buck
		// links IN
		TCC0.CCABUF = usPower;
		TCC0.PERBUF = pwm_us_period_H;

		// rechts IN
		TCD0.CCABUF = pwm_us_period_L - MINSWITCHOFFPWM;
		TCD0.PERBUF = pwm_us_period_L;

		SetEnableBuck(usStartstep);
	}
	else if(usPower <= pwm_us_period_H)
	{
		uint32_t u;
		u = MINSWITCHOFFPWM - (pwm_us_period_H - (uint32_t)usPower);
		u = u*100*pwm_us_period_div  / MINSWITCHOFFPWM; // result = 1..10 (*100)
		u = pwm_us_period_H * u /100;
		TCC0.PERBUF = pwm_us_period_H + u;
		TCC0.CCABUF = TCC0.PERBUF - MINSWITCHOFFPWM;

		TCD0.PERBUF = pwm_us_period_L;
		TCD0.CCABUF = pwm_us_period_L - MINSWITCHOFFPWM;

		SetEnableBuck(usStartstep);
		//EVSYS.STROBE = (1<<0); // sync timers
	}
	else if(usPower <= pwm_us_period_H + MINSWITCHOFFPWM)
	{
		uint32_t u;
		u = MINSWITCHOFFPWM - (usPower - pwm_us_period_H);
		u = u*100*pwm_us_period_div / MINSWITCHOFFPWM; // result = 1..10 (*100)
		u = pwm_us_period_H * u / 100;
		TCD0.PERBUF = pwm_us_period_H + u;
		TCD0.CCABUF = TCD0.PERBUF - MINSWITCHOFFPWM;

		TCC0.PERBUF = pwm_us_period_L;
		TCC0.CCABUF = TCC0.PERBUF - MINSWITCHOFFPWM;

		SetEnableBoost(usStartstep);
		//EVSYS.STROBE = (1<<0); // sync timers
	}
	else
	{
		//boost
		// links IN
		TCC0.PERBUF = pwm_us_period_L;
		TCC0.CCABUF = TCC0.PERBUF - MINSWITCHOFFPWM;

		// rechts IN
		TCD0.PERBUF = pwm_us_period_H;
		TCD0.CCABUF = pwm_us_period_H*2 - usPower;//pwm_us_period_H*2 - power - 2* MINSWITCHOFFPWM ;

		// links ENABLE immer an:
		TCC0.CCBBUF = PERIOD_MAX;

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

		// rechts ENABLE läuft mit:
		TCD0.CCBBUF = u;//pwm_us_period_H*2 - power;//(invertiert!)
	}
	else
	{
		//rechts ENABLE
		TCD0.CCBBUF = 0; //(invertiert!)
	}
}

volatile uint8_t test = 0;

void vGovernor(	uint16_t _I_Set_mA,	uint16_t _I_Act_mA)
{
	uint16_t usPower = 0;
	static uint16_t usStartstep = STARTMAX;
	static uint8_t cn = 0;


	if(_I_Set_mA <= 0 )
	{
		ENABLE_A_OFF;ENABLE_B_OFF;
		usStartstep = STARTMAX; // reset, because of static !!
		vPWM_Set(0,usStartstep);
		PID(0,0,0,0,0,0,0,1);
		usPower = 0; // reset for display.
	}
	else
	{

		if(_I_Set_mA > STARTUPLEVEL_mA && usStartstep > 0)
		{
				_I_Set_mA = STARTUPLEVEL_mA;
		}

// calculate startstep
		if(abs(_I_Act_mA - _I_Set_mA) < _I_Set_mA/20 && _I_Set_mA > STARTUPLEVEL_mA/2)
		{
			if (++cn == 3)
			{
				cn=0;
				if (usStartstep >0)
					usStartstep--; // muss null werden.
			}
		}

		usPower = PID(_I_Act_mA, _I_Set_mA, 0, 1, 0, 0, pwm_us_period_H*17/10, 0);

		vPWM_Set(usPower,usStartstep);
	}

	// fixme additional info to be removed ?
	OS_MutexGet(OSMTXBattInfo);
	//pwm und pwmdings
	g_tBattery_Info.usPWM = usPower;
	g_tBattery_Info.usPWMStep = usStartstep;
	g_tBattery_Info.sISetpoint = _I_Set_mA;
	g_tBattery_Info.sDiff = _I_Act_mA - _I_Set_mA;
	OS_MutexRelease(OSMTXBattInfo);
}

/*
void RampUpDn(uint16_t* ramped, uint16_t target)
{
	if(*ramped < target)
		(*ramped)++;
	else if(*ramped > target)
		(*ramped)--;
}

void RampUp(uint16_t* ramped, uint16_t target)
{
	if(*ramped < target)
		(*ramped)++;
	else if(*ramped > target)
		(*ramped)= target;
}

void RampDn(uint16_t* ramped, uint16_t target)
{
	if(*ramped < target)
		(*ramped)= target;
	else if(*ramped > target)
		(*ramped)--;
}*/

uint16_t PID(int16_t actual, int16_t set, uint16_t kP, int16_t kI, uint16_t kD, int16_t lowerLimit, int16_t upperLimit, uint8_t zero)
{

#define I_REDUCTION 64

	int32_t res;
	int16_t P,D;
	//static uint8_t r=0;
	static int32_t I=0;
	static int16_t old_actual;
	int16_t diff;
	int16_t der;

	diff = set-actual;

	der = actual-old_actual;
	old_actual= actual;

	P = (kP * diff)/8192;
	I = I + (kI*diff);
	I = limit(I,(uint32_t)lowerLimit*I_REDUCTION,(uint32_t)upperLimit*I_REDUCTION); // wind-up protection
	D = der*kD;

	// reset integrator
	if (zero)
	{
		P = 0;
		I = 0;
		D = 0;
	}
	else
	{
		asm("nop"); // fixme breakpoint only
	}

/*

	r++;
	if (r == 10)
	{
		if(actual < set)
		{
			I+=255;
		}
		else if (actual > set)
		{
			I-=255;
		}
		r=0;
	}
	P=0;D=0;

	I = limit(I,(uint32_t)lowerLimit*255,(uint32_t)upperLimit*255);

	if (zero) I = 0;
	*/

	res = P + I/I_REDUCTION + D;

	return limit(res,lowerLimit,upperLimit);
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
