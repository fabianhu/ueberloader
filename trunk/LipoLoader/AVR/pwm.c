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
		TCC0.PERBUF = pwm_us_period_L;
		TCC0.CCABUF = TCC0.PERBUF - MINSWITCHOFFPWM;

		uint32_t u;
		u = MINSWITCHOFFPWM - (usPower - pwm_us_period_H);
		u = u*100*pwm_us_period_div / MINSWITCHOFFPWM; // result = 1..10 (*100)
		u = pwm_us_period_H * u / 100;

		TCD0.PERBUF = pwm_us_period_H + u;
		TCD0.CCABUF = TCD0.PERBUF - MINSWITCHOFFPWM;

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

void CalcStartStepAndLimitI(int16_t* _I_Set_mA, uint16_t* usStartstep, int16_t _I_Act_mA)
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
		if(Limit < *_I_Set_mA  )
		{
			Limit++;
			*_I_Set_mA = Limit;
		}
		else
			Limit = 10000; // don't do anything with the setpoint
	}

    // calculate startstep
    if(abs(_I_Act_mA - *_I_Set_mA) < *_I_Set_mA/20 && *_I_Set_mA > STARTUPLEVEL_mA/2)
		{
			if (++ucStartCnt == 3) // every third cycle
			{
				ucStartCnt=0;
				if (*usStartstep >0)
					(*usStartstep)--; // muss null werden.
			}
		}
 }

// FIXME take care of negative current (discharge)

#define KP 0  // FIXME festlegen
#define KI 300
#define KD 0

uint16_t kP = KP;
uint16_t kI = KI;
uint16_t kD = KD;


void vGovernor(	int16_t _I_Set_mA,	int16_t _I_Act_mA )
{
#define SCALEMULTIPLE_U 10000L
#define SCALEMULTIPLE_I 10000L
//	int32_t nAct_I_diff_pct, nAct_U_diff_pct;
	//static int32_t nDerOld_pct;
	//int32_t nAct_pct, diff;
	int32_t nDer_pct;

	uint16_t usPower = 0;
	static uint16_t usStartstep = STARTMAX;

	CalcStartStepAndLimitI(&_I_Set_mA, &usStartstep, _I_Act_mA); // *********************** Do before calculating with I setpoint !!!

//	// scale diffs to 0..10000; (that means, that the setpoint IS now SCALEMULTIPLE for BOTH)
//	nAct_U_diff_pct = SCALEMULTIPLE_U - ((int32_t)_U_Act_mV * SCALEMULTIPLE_U) / (int32_t)_U_Set_mV; 	// ex: 10000 - 4150 * 10000 / 4200 = 20
//																				// ex: 10000 - 4201 * 10000 / 4200 = -2
//																				// ex: 10000 - 24200 * 10000 / 24300 = 41
//
//	nAct_I_diff_pct = SCALEMULTIPLE_I - ((int32_t)_I_Act_mA * SCALEMULTIPLE_I) / (int32_t)_I_Set_mA; 	// ex: 10000 - 990  * 10000 / 1000 = 1000
//																				// ex: 10000 - 1001 * 10000 / 1000 = -10
//																				// ex: 10000 - 1 * 10000 / 300 = 9966
// 	diff_pct = min(nAct_U_diff_pct, nAct_I_diff_pct);


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

	static int32_t I_Integrator=0;

	if(_I_Set_mA <= 0 ) // ramp down!!! fixme! or discharge into battery!! supply volt goes high!!!
	{
		ENABLE_A_OFF;ENABLE_B_OFF;
		usStartstep = STARTMAX; // reset, because of static !!
		vPWM_Set(0,usStartstep);
		I_Integrator = 0; // "Zero the I"
		usPower = 0; // reset for display.
	}
	else
	{
		usPower = PID(diff, &I_Integrator, kP, kI, kD, 0, pwm_us_period_H*17/10); // fixme max value!
		vPWM_Set(usPower,usStartstep);
	}

	// fixme additional info to be removed ?
	OS_MutexGet(OSMTXBattInfo);
	//pwm und pwmdings
	g_tBattery_Info.usPWM = usPower;
	g_tBattery_Info.usPWMStep = usStartstep;
	g_tBattery_Info.sISetpoint = _I_Set_mA;
	g_tBattery_Info.sDiff = diff;
	g_tBattery_Info.usConverterPower_W = nDer_pct;
	OS_MutexRelease(OSMTXBattInfo);
}


uint16_t PID(int32_t diff,  int32_t* I, uint16_t kP, int16_t kI, uint16_t kD, int16_t lowerLimit, int16_t upperLimit)
{
#define I_REDUCTION 255 // to get smoother I, it calculates in multiples of this.

	int32_t res;
	int32_t P,D;

	P = ((int32_t)kP * diff)/1024L; // reduce effect for bigger parameters
	*I = *I + ((int32_t)kI*diff)/1024L;
	*I = limit(*I,(uint32_t)lowerLimit*I_REDUCTION,(uint32_t)upperLimit*I_REDUCTION); // wind-up protection
	D = 0;//(der*(int32_t)kD)/1024L;

	res = P + *I/I_REDUCTION + D;

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
