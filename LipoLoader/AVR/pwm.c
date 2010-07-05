/*
 * PWM generation for the Üeberloader
 *
 * */

#include "pwm.h"
#include "serial.h"
#include "OS/FabOS.h"
#include "ueberloader.h"

extern Battery_Info_t g_tBattery_Info;

void vPWM_Init(void)
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
	TCD0.PERBUF = PERIOD_H;// 0+9*4; // 10khz (9 Überläufe, 4 wg. hires)
	TCD0.CCABUF = PERIOD_H-MINSWITCHOFFPWM;
	TCD0.CCBBUF = 0;

	HIRESD.CTRL = HIRES_HREN_TC0_gc;

	// sync timers via event 0
	EVSYS.STROBE = 0b00000001;
}


void vPWM_Set(uint16_t usPower, uint16_t usStartstep)
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

		// rechts ENABLE läuft mit:
		TCD0.CCBBUF = u;//PERIOD_H*2 - power;//(invertiert!)
	}
	else
	{
		//rechts ENABLE
		TCD0.CCBBUF = 0; //(invertiert!)
	}
}

uint16_t gTest;

void vGovernor(
		uint16_t _I_Set_mA,
		uint16_t _U_Set_mV,
		uint16_t _I_Act_mA,
		uint16_t _U_Act_mV,
		uint16_t _U_Supp_mV)
{
	static uint16_t usPower = 0, usStartstep = STARTMAX;
	static uint8_t cn = 0;
	static int16_t I_Set_mA_Ramped = 0;
		int32_t nConverterPower_W = ((_I_Act_mA) * (_U_Act_mV - _U_Supp_mV));

	if(		_I_Act_mA > (_I_Set_mA+(_I_Set_mA/10)) ||
			_U_Act_mV > (_U_Set_mV+(_U_Set_mV/10))
		)
	{
		// overshoot prevention
		//_I_Set_mA = 0; // fixme does not work!!!!!!
	}

	if(_I_Set_mA <= 0)
	{
		usPower = 0;
		ENABLE_A_OFF;ENABLE_B_OFF;
		usStartstep = STARTMAX;
		vPWM_Set(usPower,usStartstep);
		PID(0,0,0,0,0,0,0,1);
		I_Set_mA_Ramped = 0;
	}
	else
	{

		if(_I_Set_mA > STARTUPLEVEL_mA)
		{
			if(usStartstep > 0)
			{
				_I_Set_mA = STARTUPLEVEL_mA;
			}
		}



// calculate Current setpoint
		if(_U_Act_mV < _U_Set_mV /*&& sConverterPower < MAXCONVERTERPOWER_W*/)
		{
			if (I_Set_mA_Ramped < _I_Set_mA)
				I_Set_mA_Ramped++;
		}
		else
		{
			if (I_Set_mA_Ramped > 0)
				I_Set_mA_Ramped--;
		}


		usPower = PID(_I_Act_mA, I_Set_mA_Ramped, 0, 1, 0, 0, PERIOD_H*17/10, 0);


		// fixme
		OS_MutexGet(OSMTXBattInfo);
		//pwm und pwmdings
		g_tBattery_Info.usPWM = usPower;
		g_tBattery_Info.usPWMStep = usStartstep;
		g_tBattery_Info.usConverterPower_W = (uint16_t)nConverterPower_W;
		g_tBattery_Info.sISetpoint = I_Set_mA_Ramped;
		g_tBattery_Info.sDiff = _I_Act_mA - I_Set_mA_Ramped;
		OS_MutexRelease(OSMTXBattInfo);

		if(abs(_I_Act_mA - _I_Set_mA) < _I_Set_mA/20 && _I_Set_mA > STARTUPLEVEL_mA/2)
		{
			if (++cn == 3)
			{
				cn=0;
				if (usStartstep >0)
					usStartstep--; // muss null werden.
			}
		}
		else
		{
			//usStartstep = STARTMAX;
		}

		vPWM_Set(usPower,usStartstep);
		gTest = usPower;
	}
}

uint16_t PID(int16_t actual, int16_t set, uint16_t kP, int16_t kI, uint16_t kD, int16_t lowerLimit, int16_t upperLimit, uint8_t zero)
{
	int32_t res;
	int16_t P,D;
	static int32_t I;
	static int16_t old_actual;
	int16_t diff;
	int16_t der;

	diff = set-actual;

	der = actual-old_actual;
	old_actual= actual;

	P = (kP * diff)/255;
	I = I + (kI*diff);
	I = limit(I,(uint32_t)lowerLimit*255,(uint32_t)upperLimit*255); // wind-up protection
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

	res = P+I/255+D;

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
