

#include "touchpad.h"
#include "OS/FabOS.h"

uint16_t touchcalbytes[5] = TOUCHCALINIT;


uint8_t touchGetPad(uint8_t pin)
{
	uint8_t cnt,i;
	uint8_t mask = (1<<pin);

	cnt = 0;
	OS_ENTERCRITICAL  // no interrupts allowed here!
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
	OS_LEAVECRITICAL

	touchcalbytes[pin] = min(touchcalbytes[pin],cnt*100);

	touchcalbytes[pin]++; // every some time, correct the calibration bytes.. even, if a touch is recognized... -> provides self healing..


	return cnt - (touchcalbytes[pin]/100);
}


void touch_init(void)
{
	TOUCHCONFIGPORT;
}
#define TOUCHCOUNT 5

uint16_t touchpads[TOUCHCOUNT];

void touch(void)
{
	uint8_t i;
	for (i = 0; i < TOUCHCOUNT; ++i)
	{
		touchpads[i] = touchGetPad(i);
	}
}

int16_t touchGetSchwerpunkt(void)
{
	uint16_t force;

	uint16_t maxVal1=0,maxVal2=0;
	int8_t maxIdx1=0,maxIdx2=0,i;

	touch(); // fixme ?
	
	for (i = 0; i < TOUCHCOUNT; ++i)
	{
		if(touchpads[i] >= maxVal1)
		{
			maxVal2 = maxVal1; // eins schieben
			maxVal1 = touchpads[i];
			maxIdx2 = maxIdx1;
			maxIdx1 = i;
		}
		else if (touchpads[i] >= maxVal2)
		{
			maxVal2 = touchpads[i];
			maxIdx2 =  i;
		}
	}

	force = maxVal1;//(maxVal1 + maxVal2) /2;

	if (abs(maxIdx2-maxIdx1) <= 1 && force > 6) // fixme adapt
	{
		// only one or no difference.
		if(maxIdx1 > maxIdx2)
		{
			return maxIdx2*250 + ((maxVal1*250 / (maxVal1+maxVal2)));
		}
		else
		{
			return maxIdx1*250 + ((maxVal2*250 / (maxVal1+maxVal2)));
		}



	}
	else
	{
		// double touch!

	}



	return -1;
}


