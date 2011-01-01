#include "touchpad.h"
#include "../OS/FabOS.h"
#include "../lcd/lcd.h" // fixme remove


typedef enum eGestures_tag
{
	eGNothing = 0b000,
	eGPlus = 0b001,
	eGMitte = 0b010,
	eGMittePlus = 0b011,
	eGMinus = 0b100,
	eGSplit = 0b101,
	eGMitteMinus = 0b110,
	eGFullHouse = 0b111
} eGestures_t;


uint16_t GetvalueFromUI(void);
uint8_t touchGetSpeed(int16_t* speed);
extern void menu_select(void);
int16_t touchGetSchwerpunkt(void);
void sFilter(int16_t* o, int16_t* n);
void svFilter(int16_t* o, int16_t* n, uint8_t x);
eGestures_t getGesture(void);
uint8_t bitcount3(uint8_t b);


uint16_t touchcalbytes[5] =
TOUCHCALINIT;
int32_t touchValue;
int32_t touchValueUpper;
int32_t touchValueLower;
//uint16_t touchPosition;

uint8_t guctype;

particle_t myP =
	{ 0, 0, 0, 99 };

extern uint8_t debug,debug2;


/*

 Particle needs:
 Position (= value)
 Velocity (= speed and direction)
 Acceleration (the more the user slides, the more speed.)


 1. You need to measure the velocity of your cursor (either mouse cursor or finger.
 2. Implement a simple particle physics loop. information about how to do that here
 3. give your particle "bounds" using math derived from the width of your scrolling plane, and the width of your viewport
 4. continuously Add the the difference between the mouse velocity and the particle velocity, to the particle's velocity, so the particle's velocity "matches" the mouse's velocity for as long as it's moving.
 5. Stop doing step 4 as soon as the user lifts their finger. The physics loop takes care of inertia.
 6. Add your personal flourishes such as "bumper" margins, and smooth scrolling "anchor" points that operate on zeno's paradox for calculating motion.
 7. I nearly forgot: Take the coordinates derived from above, and use it as the location of your scrolling plane.


 */

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

uint8_t touchGetPad5(uint8_t pin)
{
	uint8_t cnt, i;
	uint8_t mask = (1 << pin);

	cnt = 0;
	OS_DISABLEALLINTERRUPTS;// absolutely no interrupts allowed here!
	for (i = 0; i < TOUCHREPCNT; i++)
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


	return cnt - (touchcalbytes[pin] / 100);
}

uint8_t touchGetPad3(uint8_t pin)
{
	uint8_t cnt, i;
	uint8_t mask;

	if (pin == 0)
		mask = 1;
	else
		if (pin == 1)
			mask = 1 << 2;
		else
			mask = 1 << 4;

	cnt = 0;
	OS_DISABLEALLINTERRUPTS;// absolutely no interrupts allowed here!
	for (i = 0; i < TOUCHREPCNT; i++)
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


	return cnt - (touchcalbytes[pin] / 100);
}

void process_touch_digital(void) // Notlösung f. 3 Tasten
{
	uint8_t i, t[5];

	for (i = 0; i < 5; ++i)
	{
		t[i] = touchGetPad5(i);
		//OS_WaitTicks(OSALTouchPause,1);
	}

	if (t[0] > 20)
	{
		if (touchValue < touchValueUpper)
		{
			touchValue++;
		}
	}

	if (t[4] > 20)
	{
		if (touchValue > touchValueLower)
		{
			touchValue--;
		}
	}

	if (t[2] > 20)
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
		touchpads[i] = touchGetPad3(i);
	}
}

int16_t touchGetSchwerpunkt(void)
{

	uint8_t i;
	uint16_t sum;
	uint16_t Moment;

	touchGetPads(); // fixme measure here?

	Moment = 0;
	sum = 0;
	for (i = 0; i < TOUCHCOUNT; ++i)
	{

		Moment += (i + 1) * 250 * touchpads[i];
		sum += touchpads[i];
	}
	if (sum > TOUCHMINSIGNAL) //fixme make self learning
		return (Moment / sum) - 250;
	else
		return -1;

}

void sFilter(int16_t* o, int16_t* n)
{
	int32_t temp;
	if (*o == 0)
		*o = *n;
	else
	{
		temp = *o * 3 + *n;
		*o = (temp + 2) / 4; // "halbes dazu, wg Rundungsfehler"
	}
}

void svFilter(int16_t* o, int16_t* n, uint8_t x)
{
	int32_t temp;
	if (*o == 0)
		*o = *n;
	else
	{
		temp = *o * (x - 1) + *n;
		*o = (temp + x / 2) / x; // "halbes dazu, wg Rundungsfehler"
	}
}

//int16_t touch(void)
//{
//	static uint8_t bwasInvalid;
//	static uint16_t gradientOldPos;
//	static int16_t gradient = 0;
//	static int16_t gradientFlt = 0;
//	int16_t posRaw;
//	static int16_t posFlt;
//
//	// here do all touch processing
//
//	// get values
//	posRaw = touchGetSchwerpunkt();
//
//	if (posRaw >= 0)
//	{
//		sFilter(&posFlt, &posRaw);
//	}
//	else
//	{
//		bwasInvalid = 1;
//	}
//
//	// override filter, if new touch
//	if (posRaw >= 0 && bwasInvalid)
//	{
//		// first touch
//		bwasInvalid = 0;
//		gradient = 0;
//		gradientFlt = 0;
//		gradientOldPos = posRaw;
//		posFlt = posRaw;
//	}
//
//	// calculate position
//
//	// calculate derivative
//
//	// check for tap(s)
//	// "Tap"
//
//	// 1. Schwerpunkt muss -1 sein.
//	// Schwerpunkt muss f. gewisses Intervall( min / max) mittig BLEIBEN + Gradient nahe 0 sein
//
//
//	static uint8_t TapState = 0;
//	static uint8_t TapTime = 0;
//
//#define TAPMINPOS 400
//#define TAPMAXPOS 600
//#define TAPMINTIME 1
//#define TAPMAXTIME 6
//
//	switch (TapState)
//	{
//		case 0:
//			TapTime = 0;
//			if (posRaw == -1)
//			{
//				TapState = 1;
//			}
//			break;
//		case 1:
//			if (posRaw == -1)
//			{
//				// nix
//			}
//			else
//			{
//				TapTime = 0;
//				if (posRaw > TAPMINPOS && posRaw < TAPMAXPOS)
//					TapState = 2;
//			}
//			break;
//		case 2:
//			if (posRaw > TAPMINPOS && posRaw < TAPMAXPOS)
//			{
//				TapTime++;
//
//			}
//			else
//				if (posRaw == -1)
//				{
//					if (TapTime > TAPMINTIME && TapTime < TAPMAXTIME)
//					{
//						// yeah!
//						//fixme LCD_LIGHT_TGL; <- doesn't work with my hardware, best regards Joe
//						TapState = 0;
//					}
//				}
//				else
//				{
//					TapState = 0;
//				}
//
//			break;
//
//		default:
//			TapState = 0;
//			break;
//	}
//
//	// calculate changed value
//	// TAP blocks the value change until cleared by "customer" fixme ist das so?
//	if (posRaw >= 0)
//	{
//		gradient = posFlt - gradientOldPos;
//		gradientOldPos = posFlt;
//		sFilter(&gradientFlt, &gradient);
//		touchValue
//				= limit((touchValue-gradientFlt), touchValueLower, touchValueUpper);
//
//	}
//	else
//	{
//		if (abs(gradientFlt) > 30) // let run...
//		{
//			touchValue
//					= limit((touchValue-gradientFlt), touchValueLower, touchValueUpper);
//		}
//	}
//
//	return posFlt;//gradientFlt+500;
//}

#define TOUCHREDUCEFACTOR 8

void HandOverValueToUI(uint16_t value, uint16_t upper, uint16_t lower) // fixme umziehen ins menu_variant
{
	touchValue = value * TOUCHREDUCEFACTOR; // fixme particle rein
	touchValueLower = lower * TOUCHREDUCEFACTOR;
	touchValueUpper = upper * TOUCHREDUCEFACTOR;
}

uint16_t GetvalueFromUI(void)
{
	return touchValue / TOUCHREDUCEFACTOR;
}

void touchGetValue(int32_t* pValue) // read txtback the (changed) value Mutex?
{
	//OS_ENTERCRITICAL
	OS_PREVENTSCHEDULING;
	*pValue = touchValue / TOUCHREDUCEFACTOR;
	OS_ALLOWSCHEDULING;
	//OS_LEAVECRITICAL
}

uint8_t touchGetTap(void) // get tap "event" and clear it 1== normal tap, 2 = double tap
{
	return 0;
}

int16_t touchGetPosition(void) // read the pos
{
	return 0;
}

uint8_t touchGetSpeed(int16_t* speed)
{
	static int16_t opos=-1, fpos=-1;
	int16_t pos;
	pos = touchGetSchwerpunkt();
	if (pos != -1)
	{
		if (fpos == -1)
			fpos = pos; // filter überschreiben, wenn gerade gültig geworden.
		else
			svFilter(&fpos, &pos, 10); // sonst filtern
	}
	else
	{
		// invalid (no touch)
		fpos = -1;
		opos = -1;
		return 1; // no speed!
	}

	if (opos == -1)
	{
		// invalid (no touch)
		opos = fpos;
		*speed = 0;
		return 0; // speed!
	}
	else
	{
		*speed = fpos - opos;
		opos = fpos;
		return 0;
	}

}

typedef enum eTouchstate_tag
{
	eTSIdle = 0, eTSTouched = 1, eTSMoved = 2, eTSDoubleTouch = 3, eTSBlocked = 4
} eTouchstate_t;

#define MINSLIDESPEED 5

eTouchstate_t eTouchstate = eTSIdle;

void ProcessTouch(void)
{
	int16_t sSpeed;
	uint8_t ucPos;
	static int16_t s_sSpeedFiltered = 0;
	static uint8_t s_ucOldGesture;
	uint8_t ret;

	ret = touchGetSpeed(&sSpeed);
	svFilter(&s_sSpeedFiltered, &sSpeed, 4);

	switch (eTouchstate)
	{
		case eTSIdle:
			s_sSpeedFiltered = 0;
			myP.force = 0;
			if (ret == 0)
			{
			/*	if (myP.velocity == 0)
				{*/
					eTouchstate = eTSTouched;
				/*}
				else
				{
					eTouchstate = eTSMoved;
				}'*/
			}
			break;

		case eTSTouched:
			if (ret == 0)
			{
				if(abs(s_sSpeedFiltered) > MINSLIDESPEED)
				{
					eTouchstate = eTSMoved;
				}
				else
				{
					// merken, wo
					s_ucOldGesture = getGesture();
					if(bitcount3(s_ucOldGesture) > 1)
					{
						eTouchstate = eTSDoubleTouch;
					}
					break;
				}
				// no break, drop through to moved
			}
			else
				if (ret == 1)
				{
					// hier gemerkten single t auswerten.
					if(s_ucOldGesture == eGPlus)
					{
						// fixme DO tap up
						debug2 = eGPlus;
					}
					else if(s_ucOldGesture == eGMinus)
					{
						// fixme DO tap down
						debug2 = eGMinus;
					}
					else if(s_ucOldGesture == eGMitte)
					{
						menu_select();
						// fixme DO tap mid
						debug2 = eGMitte;
					}

					eTouchstate = eTSIdle;
					OS_WaitTicks(OSALTouchPause,10);
					break;

				}
				// no break!!

		case eTSMoved:
			if (ret == 1)
			{
				eTouchstate = eTSIdle;
				break;
			}
			if (abs(s_sSpeedFiltered) < MINSLIDESPEED)
			{
				myP.force = 0;
				myP.velocity = 0;// myP.velocity*9 /10;
			}
			else
			{
				myP.force = s_sSpeedFiltered;
			}

			myP.velocity = myP.velocity + myP.force;
			break;

		case eTSDoubleTouch:
			if (ret == 1)
			{
				switch (s_ucOldGesture)
				{
					case eGNothing:
						// never used
						break;
					case eGPlus:
						// never used
						break;
					case eGMitte:
						// never used
						break;
					case eGMittePlus:
						debug2 = eGMittePlus;
						break;
					case eGMinus:
						// never used
						break;
					case eGSplit:
						// fixme lock / unlock
						debug2 = eGSplit;
						break;
					case eGMitteMinus:
						debug2 = eGMitteMinus;
						break;
					case eGFullHouse:
						// never used
						break;
					default:
						break;
				}
				eTouchstate = eTSIdle;
			}
			else
			{
				// nicht losgelassen,
				if(getGesture()== eGFullHouse)
				{
					// fixme do here the full house emergency stuff
					debug2 = eGFullHouse;

					eTouchstate = eTSBlocked;
				}
			}

			break;

		case eTSBlocked:
			if (ret == 1) // warte bis komplett losgelassen
			{
				eTouchstate = eTSIdle;
			}

			break;

		default:
			break;
	}

	static uint8_t n;
	if (n++ == 10)
	{
		n = 0;
		myP.velocity = (myP.velocity * myP.friction) / 100;
	}

	myP.position = myP.position + myP.velocity;

	debug = eTouchstate;

}

eGestures_t getGesture(void)
{
	uint8_t ret = 0;

	uint8_t i;

	for (i = 0; i < TOUCHCOUNT; i++)
	{
		if (touchpads[i] > TOUCHMINSIGNAL)
			ret |= (1 << i);
	}

	return ret; // scho feddisch
}

uint8_t bitcount(uint8_t b)
{
	uint8_t r=0,i;

	for (i = 0; i < 8; ++i)
	{
		if(b & (1<<i))
			r++;
	}
	return r;
}

uint8_t bitcount3(uint8_t b)
{
	uint8_t r=0,i;

	for (i = 0; i < 3; ++i)
	{
		if(b & (1<<i))
			r++;
	}
	return r;
}

/*
switch (getGesture())
{
	case eGNothing:

		break;
	case eGPlus:

		break;
	case eGMitte:

		break;
	case eGMittePlus:

		break;
	case eGMinus:

		break;
	case eGSplit:

		break;
	case eGMitteMinus:

		break;
	case eGFullHouse:

		break;
	default:
		break;
}
*/
