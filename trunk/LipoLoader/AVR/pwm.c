/*
 * PWM generation for the Üeberloader
 *
 * */

#include "pwm.h"
#include "serial.h"
#include "OS/FabOS.h"
#include "filter.h"

extern Battery_Info_t g_tBattery_Info;
int32_t propscale(int32_t _value, int32_t _minin, int32_t _maxin, int32_t _minout, int32_t _maxout);

uint16_t pwm_us_period_H = 2560ul; // 1280 = 100kHz // main frequency
uint16_t pwm_us_period_div = 10ul; // main / x = slow(refresh) frq.
uint16_t pwm_us_period_L; // init !!! = (pwm_us_period_H + (pwm_us_period_H )* pwm_us_period_div );

void PWM_Setfrequency(int16_t f) // in kHz!!!
{
	pwm_us_period_H = ( 12800 / f ) * 10;
	pwm_us_period_L = ( pwm_us_period_H
			+ ( pwm_us_period_H ) * pwm_us_period_div ); // fixme 11?
}

void PWM_SetRatio(uint16_t r)
{
	pwm_us_period_div = r;
}

void vPWM_Init(void)
{
	pwm_us_period_L = ( pwm_us_period_H
			+ ( pwm_us_period_H ) * pwm_us_period_div );

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
	TCD0.PERBUF = pwm_us_period_H; // 0+9*4; // 10khz (9 Überläufe, 4 wg. hires)
	TCD0.CCABUF = pwm_us_period_H - MINSWITCHOFFPWM;
	TCD0.CCBBUF = 0;

	HIRESD.CTRL = HIRES_HREN_TC0_gc;

	// sync timers via event 0
	EVSYS.STROBE = 0b00000001;

	vPWM_Set( 0, STARTMAX );
}

// alt: usPower = 0.. 2*pwm_us_period_H

/*
Power Bereiche neu?:

Power skaliert in:
0..  2*pwm_us_period_H + 2*CHANGEOVER - 2*MINSWITCHOFFPWM 


1. 0 .. pwm_us_period_H - MINSWITCHOFFPWM
2. pwm_us_period_H - MINSWITCHOFFPWM + 1 .. pwm_us_period_H - MINSWITCHOFFPWM + CHANGEOVER
3. pwm_us_period_H - MINSWITCHOFFPWM + CHANGEOVER + 1 .. pwm_us_period_H - MINSWITCHOFFPWM + 2*CHANGEOVER
4. pwm_us_period_H - MINSWITCHOFFPWM + 2*CHANGEOVER + 1 .. 2*pwm_us_period_H - 2*MINSWITCHOFFPWM + 2*CHANGEOVER


		links	rechts			links	rechts
1. PWM 0..99%	99%			Per	High	Low 
2. PWM 99%		99%			Per	H..L	L
3. PWM 99%		99%			Per	L		L..H 
4. PWM 99%		99..0%		Per	Low		High



*/
// usStartstep = 1000..0


void vPWM_Set(uint16_t usPower, uint16_t usStartstep)
{
	uint32_t u;
	if(usPower <= pwm_us_period_H - MINSWITCHOFFPWM) // ok.
	{
		//step down mode
		// left fast PWM
		TCC0.CCABUF = usPower; 
		TCC0.PERBUF = pwm_us_period_H;

		// right in slo-mo
		TCD0.CCABUF = pwm_us_period_L - MINSWITCHOFFPWM;
		TCD0.PERBUF = pwm_us_period_L;

		SetEnableBuck( usStartstep );
	}
	else if(usPower <= pwm_us_period_H - MINSWITCHOFFPWM + CHANGEOVER) // ok.
	{
		u = propscale(usPower-(pwm_us_period_H-MINSWITCHOFFPWM),0,CHANGEOVER,pwm_us_period_H,pwm_us_period_L);
		
		// left fast PWM, but slowing down (longer period)
		TCC0.CCABUF = u - MINSWITCHOFFPWM;
		TCC0.PERBUF = u;

		// right in slo-mo
		TCD0.CCABUF = pwm_us_period_L - MINSWITCHOFFPWM;
		TCD0.PERBUF = pwm_us_period_L;

		SetEnableBuck( usStartstep );
		//EVSYS.STROBE = (1<<0); // sync timers
	}
	else if(usPower <= pwm_us_period_H - MINSWITCHOFFPWM + 2*CHANGEOVER) // ok.
	{
		u = propscale(usPower-(pwm_us_period_H-MINSWITCHOFFPWM+CHANGEOVER),0,CHANGEOVER,pwm_us_period_L,pwm_us_period_H);
		
		// left in slo-mo
		TCC0.CCABUF = pwm_us_period_L - MINSWITCHOFFPWM;
		TCC0.PERBUF = pwm_us_period_L;

		// right from slow to fast
		TCD0.CCABUF = u - MINSWITCHOFFPWM;
		TCD0.PERBUF = u;

		SetEnableBoost( usStartstep );
		//EVSYS.STROBE = (1<<0); // sync timers
	}
	else // the boost phase
	{
		//boost
		//  | max possible PWM              |    Power   | all previous cases                             |
		u = (pwm_us_period_H-MINSWITCHOFFPWM) - (usPower-(pwm_us_period_H - MINSWITCHOFFPWM + 2*CHANGEOVER));
		
		// left in slo-mo
		TCC0.CCABUF = pwm_us_period_L - MINSWITCHOFFPWM;
		TCC0.PERBUF = pwm_us_period_L;

		// right fast PWM
		TCD0.CCABUF = u;
		TCD0.PERBUF = pwm_us_period_H;

		// links ENABLE immer an:
		TCC0.CCBBUF = PERIOD_MAX;

		SetEnableBoost( usStartstep );
	}
}

void SetEnableBuck(uint16_t usStartstep) // 1000-0 scaled; 0= fully started
{
	uint32_t u;
	// links ENABLE immer an:
	TCC0.CCBBUF = PERIOD_MAX;

	if(usStartstep)
	{
		u = ( (uint32_t)TCD0.PERBUF + 4ul ) * (uint32_t)usStartstep / STARTMAX; // +4 gegen Überlappung (4 wg. 2 blinde Bits)

		// rechts ENABLE falls start immer aus:
		TCD0.CCBBUF = u; //(invertiert!)
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
		u = (uint32_t)TCD0.CCABUF * (uint32_t)usStartstep / STARTMAX;

		// rechts ENABLE läuft mit:
		TCD0.CCBBUF = u; //pwm_us_period_H*2 - power;//(invertiert!)
	}
	else
	{
		//rechts ENABLE
		TCD0.CCBBUF = 0; //(invertiert!)
	}
}

void CalcStartStepAndLimitI(int16_t* _I_Set_mA, uint16_t* usStartstep,
		int16_t _I_Act_mA)
{
	static uint8_t ucStartCnt;
	static int16_t Limit;

	if(*_I_Set_mA > STARTUPLEVEL_mA && *usStartstep > 0) // reduce current setpoint, if starting
	{
		*_I_Set_mA = STARTUPLEVEL_mA;
		Limit = STARTUPLEVEL_mA;
	}

	if(*_I_Set_mA > STARTUPLEVEL_mA && *usStartstep == 0) // if starting is over -> un-limit I (but slow!)
	{
		if(Limit < *_I_Set_mA)
		{
			Limit++;
			*_I_Set_mA = Limit;
		}
		else
			Limit = 10000; // don't do anything with the setpoint
	}

	// calculate startstep
	if(abs(_I_Act_mA - *_I_Set_mA) < *_I_Set_mA / 20
			&& *_I_Set_mA >= STARTUPLEVEL_mA / 2)
	{
		if(++ucStartCnt == 3) // every third cycle
		{
			ucStartCnt = 0;
			if(*usStartstep > 0)
				( *usStartstep )--; // muss null werden.
		}
	}
	else
		if(*_I_Set_mA < STARTUPLEVEL_mA / 2)
		{
			if(++ucStartCnt == 3) // every third cycle
			{
				ucStartCnt = 0;
				if(*usStartstep < STARTMAX)
					( *usStartstep )++; // raise at low currents to prevent "Ting" at Stop..
			}

		}
}

#define KP 0
#define KI 75
#define KD 0

uint16_t kP = KP;
uint16_t kI = KI;
uint16_t kD = KD;

void vGovernor(int16_t _I_Set_mA, int16_t _I_Act_mA)
{
#define SCALEMULTIPLE_U 10000L
#define SCALEMULTIPLE_I 10000L
//	int32_t nAct_I_diff_pct, nAct_U_diff_pct;
	//static int32_t nDerOld_pct;
	//int32_t nAct_pct, diff;
	int32_t nDer_pct;

	uint16_t usPower = 0;
	static uint16_t usStartstep = STARTMAX;

	CalcStartStepAndLimitI( &_I_Set_mA, &usStartstep, _I_Act_mA ); // *********************** Do before calculating with I setpoint !!!

	int32_t diff = _I_Set_mA - _I_Act_mA; // positive = missing value

	// wenn positiv, dannn gas
	// wenn beide positiv, dann der kleinere
	// wenn beide negativ, dann der kleinere
	// wenn Spannung negativ, dann die.
	//
	// the bigger wins - so the governor limits the bigger one. (!) change at negative Currents!

	/*
	 * Nochmal in Text, was das werden soll:
	 * Wenn die Spannung unter dem Sollwert ist, regeln wir den Strom.
	 * Wenn sie Spannung erreicht ist, regeln wir die.
	 *
	 * Entweder brauchen wir 2 PID sets oder eine passende Skalierung.
	 * */

	nDer_pct = 0;
	//nDerOld_pct= nAct_pct;

	static int32_t I_Integrator = 0;

	if(_I_Set_mA <= 0) // ramp down!!! todo! or discharge into battery!! supply volt goes high!!!
	{
		ENABLE_A_OFF;
		ENABLE_B_OFF;
		usStartstep = STARTMAX; // reset, because of static !!
		vPWM_Set( 0, usStartstep );
		I_Integrator = 0; // "Zero the I"
		usPower = 0; // reset for display.
	}
	else
	{
		usPower = PID( diff, &I_Integrator, kP, kI, kD, 0,
				pwm_us_period_H * 17 / 10 ); // todo max value fixieren
		vPWM_Set( usPower, usStartstep );
	}

	// todo additional info to be removed ?
	OS_MutexGet( OSMTXBattInfo );
	//pwm und pwmdings
	g_tBattery_Info.usPWM = usPower;
//	g_tBattery_Info.usPWMStep = usStartstep;
	g_tBattery_Info.sISetpoint = _I_Set_mA;
//	g_tBattery_Info.sDiff = diff;
	g_tBattery_Info.usConverterPower_W = nDer_pct;
	OS_MutexRelease( OSMTXBattInfo );
}

int16_t PID(int32_t diff, int32_t* I, uint16_t kP, uint16_t kI, uint16_t kD, int16_t lowerLimit, int16_t upperLimit) 
{
#define I_REDUCTION 255 // to get smoother I, it calculates in multiples of this.
#define P_REDUCTION 32 // to get smoother I, it calculates in multiples of this.
	int32_t res;
	int32_t P, D;

	P = ( (int32_t)kP * diff ) / P_REDUCTION; // reduce effect for bigger parameters
	*I = *I + ( (int32_t)kI * diff ) /128;
	*I = limit(*I,(int32_t)lowerLimit*I_REDUCTION,(int32_t)upperLimit*I_REDUCTION); // wind-up protection
	D = 0; //(der*(int32_t)kD)/1024L;

	res = P + *I / I_REDUCTION + D;

	return limit(res,lowerLimit,upperLimit);
}

/// BELOW CODE FRAGMENTS

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


int32_t propscale(int32_t _value, int32_t _minin, int32_t _maxin, int32_t _minout, int32_t _maxout)
{
	int32_t rangedvalue;
	int32_t inrange = _maxin-_minin;
	int32_t outrange = _maxout-_minout;
	if(inrange== 0) return 0; // divide by zero protection
	
	_value = limit(_value,_minin,_maxin); // do not extrapolate
	
	rangedvalue = (_value-_minin)*outrange/inrange;

	return rangedvalue + _minout;
}
