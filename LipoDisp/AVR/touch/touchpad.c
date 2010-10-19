

#include "touchpad.h"
#include "../OS/FabOS.h"
#include "../lcd/lcd.h" // fixme remove

uint16_t touchcalbytes[5] = TOUCHCALINIT;

uint16_t GetvalueFromUI(void);

uint16_t guivalue, guiupper, guilower;
uint8_t guctype;
extern void menu_select(void);

uint16_t GetvalueFromUI(void)
{	
	return guivalue;

	 
}

void HandOverValueToUI(uint16_t value, uint16_t upper, uint16_t lower, uint8_t type)
{
	// fixme mutex?
	guivalue = value;
	guiupper = upper;
	guilower = lower;
	guctype = type;
}

/*uint8_t touchGetPad(uint8_t pin)
{
	uint8_t cnt,i;
	uint8_t mask;

	if(pin == 0) mask = (1<<0);
	if(pin == 1) mask = (1<<2);
	if(pin == 2) mask = (1<<4);

	cnt = 0;
	OS_DISABLEALLINTERRUPTS  // no interrupts allowed here!
	for(i=0;i<TOUCHREPCNT;i++)
	{
		TOUCHTOGGLEHIGH;
		while (!(TOUCHPORT.IN & mask))
		{
			cnt++;
		}
		TOUCHTOGGLELOW;
		while (TOUCHPORT.IN & mask)
		{
			cnt++;
		}
	}
	OS_ENABLEALLINTERRUPTS

	touchcalbytes[pin] = min(touchcalbytes[pin],cnt*100);

	touchcalbytes[pin]++; // every some time, correct the calibration bytes.. even, if a touch is recognized... -> provides self healing..


	return cnt - (touchcalbytes[pin]/100);
}*/

uint8_t touchGetPad(uint8_t pin)
{
	uint8_t cnt,i;
	uint8_t mask = (1<<pin);

	cnt = 0;
	OS_DISABLEALLINTERRUPTS;// absolutely no interrupts allowed here!
	for(i=0;i<TOUCHREPCNT;i++)
	{
		TOUCHTOGGLEHIGH;
		while (!(TOUCHPORT.IN & mask))
		{
			cnt++;
		}
		TOUCHTOGGLELOW;
		while (TOUCHPORT.IN & mask)
		{
			cnt++;
		}
	}
	OS_ENABLEALLINTERRUPTS;

	touchcalbytes[pin] = min(touchcalbytes[pin],cnt*100);

	touchcalbytes[pin]++; // every some time, correct the calibration bytes.. even, if a touch is recognized... -> provides self healing..


	return cnt - (touchcalbytes[pin]/100);
}

void process_touch(void)
{
	uint8_t i,t[5];

	for (i = 0; i < 5; ++i)
	{
		t[i] = touchGetPad(i);
		//OS_WaitTicks(OSALTouchPause,1);
	}


	if(t[0]>20)
	{
		if(guivalue<guiupper)
		{
			guivalue++;	
		}
	}

	if(t[4]>20)
	{
		if(guivalue>guilower)
		{
			guivalue--;	
		}
	}

	if(t[2]>20)
	{
		menu_select();
		OS_WaitTicks(OSALTouchPause,100);
	}

}

void touch_init(void)
{
	TOUCHCONFIGPORT;
}


uint16_t touchpads[TOUCHCOUNT];

void touchGetPads(void)
{
	uint8_t i;
	for (i = 0; i < TOUCHCOUNT; ++i)
	{
		touchpads[i] = touchGetPad(i);
	}
}

int16_t touchGetSchwerpunkt(void)
{

	uint8_t i;
	uint16_t sum;
	uint16_t dings;

	touchGetPads(); // fixme measure here?
	
	dings = 0;
	sum = 0;
	for (i = 0; i < TOUCHCOUNT; ++i)
	{

		dings += (i+1) * 250 * touchpads[i];
		sum += touchpads[i];
	}
	if (sum > TOUCHMINSIGNAL) //fixme make self learning
		return (dings/sum)-250;
	else
		return -1;

}

void sFilter(int16_t* o, int16_t* n)
{
	int32_t temp;
	if(*o == 0)
		*o = *n;
	else
	{
		temp = *o * 3 + *n;
		*o = (temp+2) / 4; // "halbes dazu, wg Rundungsfehler"
	}
}


int32_t touchValue;
int32_t touchValueUpper;
int32_t touchValueLower;
//uint16_t touchPosition;

int16_t touch(void)
{
	static uint8_t bwasInvalid;
	static uint16_t gradientOldPos;
	static int16_t gradient =0;
	static int16_t gradientFlt =0;
	int16_t posRaw;
	static int16_t posFlt;

	// here do all touch processing

	// get values
	posRaw = touchGetSchwerpunkt();

	if(posRaw >=0)
	{
		sFilter(&posFlt,&posRaw);
	}
	else
	{
		bwasInvalid = 1;
	}

	// override filter, if new touch
	if(posRaw >= 0 && bwasInvalid)
	{
		// first touch
		bwasInvalid =0;
		gradient = 0;
		gradientFlt = 0;
		gradientOldPos = posRaw;
		posFlt = posRaw;
	}

	// calculate position

	// calculate derivative

	// check for tap(s)
	// "Tap"

	// 1. Schwerpunkt muss -1 sein.
	// Schwerpunkt muss f. gewisses Intervall( min / max) mittig BLEIBEN + Gradient nahe 0 sein


	static uint8_t TapState =0;
	static uint8_t TapTime =0;

#define TAPMINPOS 400
#define TAPMAXPOS 600
#define TAPMINTIME 1
#define TAPMAXTIME 6

	switch (TapState)
	{
		case 0:
			TapTime =0;
			if (posRaw == -1)
			{
				TapState = 1;
			}
			break;
		case 1:
			if (posRaw == -1)
			{
				// nix
			}
			else
			{
				TapTime =0;
				if(posRaw >TAPMINPOS && posRaw <TAPMAXPOS)
					TapState = 2;
			}
			break;
		case 2:
			if(posRaw >TAPMINPOS && posRaw <TAPMAXPOS)
			{
				TapTime++;

			}
			else if (posRaw == -1)
			{
				if(TapTime > TAPMINTIME && TapTime < TAPMAXTIME)
				{
					// yeah!
					//fixme LCD_LIGHT_TGL; <- doesn't work with my hardware, best regards Joe 
					TapState = 0;
				}
			}
			else
			{
				TapState =0;
			}

			break;

		default:
			TapState = 0;
			break;
	}


	// calculate changed value
		// TAP blocks the value change until cleared by "customer" fixme ist das so?
	if(posRaw >= 0)
	{
		gradient = posFlt - gradientOldPos;
		gradientOldPos = posFlt;
		sFilter(&gradientFlt,&gradient);
		touchValue = limit((touchValue-gradientFlt), touchValueLower, touchValueUpper);

	}
	else
	{
		if (abs(gradientFlt)>30) // let run...
		{
			touchValue = limit((touchValue-gradientFlt), touchValueLower, touchValueUpper);
		}
	}

	return posFlt;//gradientFlt+500;
}

#define TOUCHREDUCEFACTOR 32

//void HandOverValueToUI(uint16_t value, uint16_t upper, uint16_t lower)
//{
//	touchValue = value*TOUCHREDUCEFACTOR;
//	touchValueLower = lower*TOUCHREDUCEFACTOR;
//	touchValueUpper = upper*TOUCHREDUCEFACTOR;
//}


//uint16_t GetvalueFromUI(void)
//{
//	return touchValue/TOUCHREDUCEFACTOR;
//}


void touchGetValue(int32_t* pValue, uint8_t Mutex) // read txtback the (changed) value Mutex?
{
	OS_PREVENTSCHEDULING;
	*pValue = touchValue/TOUCHREDUCEFACTOR;
	OS_ALLOWSCHEDULING;
}

uint8_t touchGetTap(void) // get tap "event" and clear it 1== normal tap, 2 = double tap
{
	return 0;
}

int16_t touchGetPosition(void) // read the pos
{
	return 0;
}

