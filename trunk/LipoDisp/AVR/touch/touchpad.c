#include "touchpad.h"
#include "../OS/FabOS.h"
#include "../lcd/lcd.h" // fixme remove


// Prototypes:
void ProcessTouch(void);
int16_t GetvalueFromUI(void);
uint8_t touchGetSpeed(int16_t* speed, int32_t *Schwerpunkt);
void HandOverValueToUI(uint16_t value, uint16_t upper, uint16_t lower, uint16_t stepsize);
int32_t touchGetSchwerpunkt(void);
void sFilter(int16_t* o, int16_t* n);
void svFilter(int16_t* o, int16_t* n, uint8_t x);
eGestures_t getGesture(void);
uint8_t bitcount3(uint8_t b);
extern void menu_select(void);
extern void GetSubMenuCount(uint8_t *Size, uint8_t *StartIndex);


// Globals:
extern uint8_t g_debug, g_debug2, g_debug3;
extern int16_t g_debug4;
extern uint8_t g_bMenuActive;

int16_t g_ausTouchCalValues[5] =
TOUCHCALINIT;

int16_t g_aucTouchpads[TOUCHCOUNT];
int16_t g_ausTouchpadsRAW[TOUCHCOUNT];


particle_t myP =
	{ 0, 0, 0, 99 ,0,32000};



void TaskTouch()
{
	OS_SetAlarm(OSALTouchRepeat,10);

#define TOUCHSENSELEVEL 15

	TOUCHCONFIGPORT; // configure port

	while(1)
	{

		OS_WaitAlarm(OSALTouchRepeat);
		OS_SetAlarm(OSALTouchRepeat,10); // every 10ms

		ProcessTouch();//change value
	}
}







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

uint8_t touchGetPad5(uint8_t pin)
{
	uint8_t value, i;
	uint8_t mask = (1 << pin);

	value = 0;
	OS_DISABLEALLINTERRUPTS;// absolutely no interrupts allowed here!
	for (i = 0; i < TOUCHREPCNT; i++)
	{
		TOUCHTOGGLEHIGH;
		while (!(TOUCHPORT.IN & mask))
		{
			value++;
		}
		TOUCHTOGGLELOW;
		while (TOUCHPORT.IN & mask)
		{
			value++;
		}
	}
	OS_ENABLEALLINTERRUPTS;

	g_ausTouchpadsRAW[pin] = (value*2 + 	g_ausTouchpadsRAW[pin])/3 ;

	// limit re-calibration
	if(value*100 < g_ausTouchCalValues[pin]) // wenns denn schneller war
	{
		if(value*100 < g_ausTouchCalValues[pin] - 500) //ups das war aber zu flott
		{
			// lassen wie's war bzw. nur leicht weniger
			g_ausTouchCalValues[pin] -= 100;  // fixme zu flott !!!!!
		}
		else
		{
			// mitnehmen
			g_ausTouchCalValues[pin] = value*100;
		}
	}

	g_ausTouchCalValues[pin]++; // every some time, correct the calibration bytes.. even, if a touch is recognized... -> provides self healing..



	return value - (g_ausTouchCalValues[pin] / 100);
}




#define MOMENTMULTIPLIER 100ULL

int32_t touchGetSchwerpunkt(void)
{
int32_t sum;
int32_t Moment;
int32_t ret;
	uint8_t i;

	for (i = 0; i < TOUCHCOUNT; i++)
	{
		g_aucTouchpads[i] = touchGetPad5(i);
	}

	Moment = 0;
	sum = 0;
	for (i = 0; i < TOUCHCOUNT; i++)
	{

		Moment += (i + 1) * MOMENTMULTIPLIER * g_aucTouchpads[i];
		sum += g_aucTouchpads[i];
	}
	if (sum > (SLIDEMINSIGNAL)) //fixme make self learning
	{
		ret = (Moment / sum) - MOMENTMULTIPLIER;

		// fixme debug:
		if(ret < 0)
		{
			asm("break");
		}
		return ret ;
	}
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

#define SLIDERUPSCALE 32000

void HandOverValueToUI(uint16_t value, uint16_t upper, uint16_t lower, uint16_t stepsize) 
{
	myP.upscale = SLIDERUPSCALE / (upper-lower);


	myP.position = value * myP.upscale; // fixme particle rein
	myP.stepsize = stepsize * myP.upscale;
	myP.min = lower * myP.upscale;
	myP.max = upper * myP.upscale;
}

int16_t GetvalueFromUI(void)
{
	return  myP.position / myP.upscale;
}

void touchGetValue(int16_t* pValue) // read txtback the (changed) value Mutex?
{
	//OS_ENTERCRITICAL
	OS_PREVENTSCHEDULING;
	*pValue = myP.position / myP.upscale;
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

// returns 1 if speed is OK
uint8_t touchGetSpeed(int16_t* speed, int32_t *Schwerpunkt)
{
	static int16_t opos=-1, fpos=-1;
	int16_t pos;
	int16_t speed_raw;

	*Schwerpunkt = pos = touchGetSchwerpunkt();
	if (pos != -1)
	{
		if (fpos == -1)
			fpos = pos; // filter überschreiben, wenn gerade gültig geworden.
		else
			svFilter(&fpos, &pos, 8); // sonst filtern
	}
	else
	{
		// invalid (no touch)
		fpos = -1;
		opos = -1;
		*speed = 0;
		return 0; // no speed!
	}

	if (opos == -1) // gerade gültig geworden?
	{
		
		opos = fpos;
		*speed = 0;
		return 0; // no speed, neue Messung.
	}
	else
	{
		if(abs(fpos - opos) > MAXSLIDESPEED)
		{
			opos = -1;
			fpos = -1;
			*speed = 0;
			return 0;
		}
		else
		{
			speed_raw = fpos - opos;
			svFilter(speed, &speed_raw, 4);
			opos = fpos;
			return 1;
		}
	}

}


eTouchstate_t eTouchstate = eTSIdle;



uint8_t SubMenuGroupSize, StartIndex; // fixme debug only


void ProcessTouch(void)
{
	int32_t Schwerpunkt;
	uint8_t ucActualGesture;
	static int16_t s_sSpeedFiltered = 0;
	static uint8_t s_ucOldGesture;
	uint8_t bMoved;


	static int32_t OldSchwerpunkt;


	bMoved = touchGetSpeed(&s_sSpeedFiltered, &Schwerpunkt/*, &TouchBitfield*/); // fixme  beim integrieren den Schwerpunkt / speed  nur rechnen, wenn nur ein bzw. zwei benachbarte gedrückt.
	ucActualGesture = getGesture();

	g_debug4 = s_sSpeedFiltered;

	static uint16_t TimeDiff=0;

	TimeDiff++;

	static uint8_t magic;

	switch (eTouchstate)
	{
		case eTSIdle:
			//s_sSpeedFiltered = 0;
			myP.force = 0;
			if (bMoved == 0 && ucActualGesture == 0)
			{
				// not touched
				break;
			}
			else
			{
				TimeDiff =0;
				OldSchwerpunkt = Schwerpunkt;
				eTouchstate = eTSTouching;
			}
			// no break;
		case eTSTouching:
			if (bMoved == 1) // touched / moved
			{
				magic = 10;
				// adapt speed
				if (abs(s_sSpeedFiltered) < MINSLIDESPEED)
				{
					myP.force = 0;
					myP.velocity = 0;
				}
				else
				{
					myP.force = - s_sSpeedFiltered;
				}

				/*static uint8_t n;  // bremsen!!!
				if (n++ == 10)
				{
					n = 0;
					myP.velocity = (myP.velocity * myP.friction) / 100;
				}*/

				myP.velocity = myP.velocity + myP.force;
			}
			else
			{	
				if(magic >0) magic--;

				if (myP.velocity == 0 /*&& magic > 0*/)
				{
					if(abs(OldSchwerpunkt-Schwerpunkt)< TOUCHMINDIST)
					eTouchstate = eTSGesture;
					else
					eTouchstate = eTSIdle;
				}
				else
				{
					// do nothing and let particle move on // evtl bremsen fixme?
				}

			}


			myP.position = limit(myP.position + myP.velocity , myP.min , myP.max);
			break;

		case eTSGesture:
			switch (s_ucOldGesture) // können wir nehmen, weil lag lange genug an.
			{
				case eGNothing:

					break;
				case eGPlus:
					if(myP.position + myP.stepsize < myP.max)
						myP.position += myP.stepsize;
					else
						myP.position = myP.max;
					break;
				case eGMitte:
					// menue bestätigung
					if(g_bMenuActive)
					{
						menu_select();
					}
					else
					{
						// fixme duplicate weil der doppeltatsch net geht.
						GetSubMenuCount(&SubMenuGroupSize, &StartIndex);

						HandOverValueToUI(SubMenuGroupSize, SubMenuGroupSize, 1, 1);

						g_bMenuActive = 1;

					}
					break;
				case eGMittePlus:

					break;
				case eGMinus:
					if(myP.position - myP.stepsize > myP.min)
						myP.position -= myP.stepsize;
					else
						myP.position = myP.min;
					break;
				case eGSplit:
					// unlock
					if(g_bMenuActive)
					{
						// nix, weil im menu gibt's nen "back"
					}
					else
					{
						
						GetSubMenuCount(&SubMenuGroupSize, &StartIndex);

						HandOverValueToUI(SubMenuGroupSize, SubMenuGroupSize, 1, 1);

						g_bMenuActive = 1;
					}

					break;
				case eGMitteMinus:

					break;
				case eGFullHouse:

					break;
				default:
					break;
			}
			g_debug2 = s_ucOldGesture;

			eTouchstate = eTSBlocked;
			TimeDiff =0;

			//OS_WaitTicks(OSALTouchPause,10);
			break;

		case eTSBlocked:
			if (bMoved == 0 && TimeDiff > 20 && ucActualGesture == 0) // warte bis komplett losgelassen
			{
				eTouchstate = eTSIdle;
			}

			break;

		default:
			break;
	}

	s_ucOldGesture = ucActualGesture;

	g_debug = eTouchstate;

	g_debug3 = ucActualGesture;

}

eGestures_t getGesture(void) // delayed by one cycle
{
	uint8_t ret = 0;
	uint8_t i,j;
	
	for (i=0, j=0; i < TOUCHCOUNT; i+=2,j++)
	{
		if (g_aucTouchpads[i] > TOUCHMINSIGNAL)
			ret |= (1 << j);
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
