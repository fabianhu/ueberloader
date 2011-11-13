#include "touchpad.h"
#include "../OS/FabOS.h"
#include "../lcd/lcd.h" // fixme remove
#include "../comm/serial.h"
#include "../menu/menu.h"
#include "../menu/menu_variant.h"

// Prototypes:
void ProcessTouch(void);
int16_t GetvalueFromUI(void);
uint8_t touchGetSpeed(int16_t* speed, int32_t *Schwerpunkt);
void HandOverValueToUI(uint16_t value, uint16_t upper, uint16_t lower,
		uint16_t stepsize);
int32_t touchGetSchwerpunkt(void);
void sFilter(int16_t* o, int16_t* n);
void svFilter(int16_t* o, int16_t* n, uint8_t x);
eGestures_t getGesture(void);
eGestures_t getGestureSkip(void);
uint8_t bitcount3(uint8_t b);

extern void GetSubMenuCount(uint8_t *Size, uint8_t *StartIndex);
extern eBatteryStatus_t GetBattStatus(void);
extern ChargerMode_t g_Tansfer_Action;

// Globals:
extern uint8_t g_debug, g_debug2, g_debug3;
extern int16_t g_debug4;
extern uint8_t g_bMenuActive;
extern Command_t g_tCommand; // only for adjusting the current while charging

int16_t g_ausTouchCalValues[5] =
TOUCHCALINIT;

int16_t g_aucTouchpads[TOUCHCOUNT];

particle_t myP =
	{ 0, 0, 0, 99, 0, 32000 };

void TaskTouch()
{
	OS_SetAlarm( OSALTouchRepeat, 10 );

#define TOUCHSENSELEVEL 15

	TOUCHCONFIGPORT; // configure port

	while(1)
	{

		OS_WaitAlarm( OSALTouchRepeat );
		OS_SetAlarm( OSALTouchRepeat, 10 ); // every 10ms

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
	uint8_t mask = ( 1 << pin );

	value = 0;
	OS_DISABLEALLINTERRUPTS;// absolutely no interrupts allowed here!
	for(i = 0; i < TOUCHREPCNT ; i++)
	{
		TOUCHTOGGLEHIGH;
		while(!( TOUCHPORT.IN & mask ))
		{
			value++;
		}
		TOUCHTOGGLELOW;
		while(TOUCHPORT.IN & mask)
		{
			value++;
		}
	}
	OS_ENABLEALLINTERRUPTS;

	// limit re-calibration
	if(value * 100 < g_ausTouchCalValues[pin]) // wenns denn schneller war
	{
		if(value * 100 < g_ausTouchCalValues[pin] - 500) //ups das war aber zu flott
		{
			// lassen wie's war bzw. nur leicht weniger
			g_ausTouchCalValues[pin] -= 20;
		}
		else
		{
			// mitnehmen
			g_ausTouchCalValues[pin] = value * 100;
		}
	}

	g_ausTouchCalValues[pin]++; // every some time, correct the calibration bytes.. even, if a touch is recognized... -> provides self healing..


	return value - ( g_ausTouchCalValues[pin] / 100 );
}

#define MOMENTMULTIPLIER 100ULL

int32_t touchGetSchwerpunkt(void)
{
	int32_t sum;
	int32_t Moment;
	int32_t ret;
	uint8_t i;

	for(i = 0; i < TOUCHCOUNT ; i++)
	{
		g_aucTouchpads[i] = touchGetPad5( i );
	}

	Moment = 0;
	sum = 0;
	for(i = 0; i < TOUCHCOUNT ; i++)
	{

		Moment += ( i + 1 ) * MOMENTMULTIPLIER * g_aucTouchpads[i];
		sum += g_aucTouchpads[i];
	}
	if(sum > ( SLIDEMINSIGNAL )) //fixme make self learning
	{
		ret = ( Moment / sum ) - MOMENTMULTIPLIER;

		// fixme debug:
		if(ret < 0)
		{
			asm("break");
		}
		return ret;
	}
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
		*o = ( temp + 2 ) / 4; // "halbes dazu, wg Rundungsfehler"
	}
}

void svFilter(int16_t* o, int16_t* n, uint8_t x)
{
	int32_t temp;
	if(*o == 0)
		*o = *n;
	else
	{
		temp = *o * ( x - 1 ) + *n;
		*o = ( temp + x / 2 ) / x; // "halbes dazu, wg Rundungsfehler"
	}
}

#define SLIDERUPSCALE 32000ul

void HandOverValueToUI(uint16_t value, uint16_t upper, uint16_t lower,
		uint16_t stepsize)
{
	myP.upscale = (uint32_t)SLIDERUPSCALE
			/ ( (uint32_t)upper - (uint32_t)lower );

	myP.position = (uint32_t)value * myP.upscale;
	myP.stepsize = (uint32_t)stepsize * myP.upscale;
	myP.min = (uint32_t)lower * myP.upscale;
	myP.max = (uint32_t)upper * myP.upscale;
}

int16_t GetvalueFromUI(void)
{
	return myP.position / myP.upscale;
}

void touchGetValue(int16_t* pValue) // read txtback the (changed) value Mutex?
{
	//OS_ENTERCRITICAL
	OS_PREVENTSCHEDULING;
	*pValue = myP.position / myP.upscale;
	OS_ALLOWSCHEDULING;
	//OS_LEAVECRITICAL
}

//uint8_t touchGetTap(void) // get tap "event" and clear it 1== normal tap, 2 = double tap
//{
//	return 0;
//}
//
//int16_t touchGetPosition(void) // read the pos
//{
//	return 0;
//}

// returns 1 if speed is OK
uint8_t touchGetSpeed(int16_t* speed, int32_t *Schwerpunkt)
{

	// fixme mach rein TOUCHMINDIST

	static int16_t opos = -1, fpos = -1;
	int16_t pos;
	int16_t speed_raw;

	*Schwerpunkt = pos = touchGetSchwerpunkt();
	if(pos != -1)
	{
		if(fpos == -1)
			fpos = pos; // filter überschreiben, wenn gerade gültig geworden.
		else
			svFilter( &fpos, &pos, 8 ); // sonst filtern
	}
	else
	{
		// invalid (no touch)
		fpos = -1;
		opos = -1;
		*speed = 0;
		return 0; // no speed!
	}

	if(opos == -1) // gerade gültig geworden?
	{

		opos = fpos;
		*speed = 0;
		return 0; // no speed, neue Messung.
	}
	else
	{
		speed_raw = fpos - opos;
		svFilter( speed, &speed_raw, 4 );

		if(abs(*speed) > MAXSLIDESPEED || abs(*speed) < MINSLIDESPEED)
		{
			opos = -1;
			fpos = -1;
			*speed = 0;
			return 0;
		}
		else
		{
			opos = fpos;
			return 1;
		}
	}

}

eTouchstate_t eTouchstate = eTSIdle;
uint8_t s_ucKeyLock = 0;
uint8_t info = 0;

int32_t Schwerpunkt;
void ProcessTouch(void)
{

	eGestures_t eActualGesture;
	static int16_t s_sSpeedFiltered = 0;
	static eGestures_t s_ucOldGesture;
	uint8_t bMoved;

	

	static int32_t OldSchwerpunkt;

	bMoved	= touchGetSpeed( &s_sSpeedFiltered, &Schwerpunkt/*, &TouchBitfield*/);
	// fixme  beim integrieren den Schwerpunkt / speed  nur rechnen, wenn nur ein bzw. zwei benachbarte gedrückt.
	eActualGesture = getGestureSkip();

	g_debug4 = s_sSpeedFiltered;

	static uint16_t TimeDiff = 0;

	TimeDiff++;

	switch(eTouchstate)
	{
		case eTSIdle:
			//s_sSpeedFiltered = 0;
			myP.force = 0;
			if(bMoved == 0 && eActualGesture == 0)
			{
				// not touched
				break;
			}
			else
			{
				TimeDiff = 0;
				OldSchwerpunkt = Schwerpunkt;
				eTouchstate = eTSTouching;
			}
			// no break;
		case eTSTouching:

			if(bMoved == 1) // touched / moved
			{
				if(s_ucKeyLock)
				break;
				
				// adapt speed

				myP.force = -s_sSpeedFiltered;
				info = 2;

				myP.velocity = myP.velocity + myP.force;

				/*static uint8_t n;  // bremsen!!!
				 if (n++ == 10)
				 {
				 n = 0;
				 myP.velocity = (myP.velocity * myP.friction) / 100;
				 }*/
			}
			else
			{
				if(myP.velocity == 0)
				{
					eTouchstate = eTSGesture;
					info = 3;
				}
				else
				{
					// do nothing and let particle move on
					info = 5;
					if(eActualGesture != 0)
					{
						// bremsen
						myP.velocity = 0;
						info = 6;
					}
				}

			}

#if TOUCHDISABLESLIDE == 1
			myP.velocity = 0;
#else
			myP.position = limit(myP.position + myP.velocity , myP.min , myP.max);
#endif
			break;

		case eTSGesture:
			info = 7;
			switch(s_ucOldGesture)
			// können wir nehmen, weil lag lange genug an.
			{
				case eGNothing:

					break;
				case eGPlus:
					if(s_ucKeyLock)
						break;
					if(g_bMenuActive)
					{
						if(myP.position + myP.stepsize < myP.max)
							myP.position += myP.stepsize;
						else
							myP.position = myP.max;
					}
					else
					{
						OS_MutexGet( OSMTXCommand );
						if(g_tCommand.sCurrentSetpoint + 100
								< parCurrent.sUpperLimit)
							g_tCommand.sCurrentSetpoint += 100;
						OS_MutexRelease( OSMTXCommand );
					}
					break;
				case eGMitte:
					if(s_ucKeyLock)
						break;
					// menue bestätigung
					if(g_bMenuActive)
					{
						menu_select();
					}
					else
					{
						if(GetBattStatus() == eBattWaiting)
						{
							// fixme duplicate weil der doppeltatsch net geht.

							// block charger
							g_Tansfer_Action = eActModeStop;

							uint8_t SubMenuGroupSize, StartIndex;

							GetSubMenuCount( &SubMenuGroupSize, &StartIndex );

							HandOverValueToUI( SubMenuGroupSize,
									SubMenuGroupSize, 1, 1 );

							g_bMenuActive = 1;
						}
						else
							g_Tansfer_Action = eActModeStop; // fixme charger has to take it also in charging mode.

					}
					break;
				case eGMittePlus:

					break;
				case eGMinus:
					if(s_ucKeyLock)
						break;
					if(g_bMenuActive)
					{
						if(myP.position - myP.stepsize > myP.min)
							myP.position -= myP.stepsize;
						else
							myP.position = myP.min;
					}
					else
					{
						OS_MutexGet( OSMTXCommand );
						if(g_tCommand.sCurrentSetpoint - 100
								> parCurrent.sLowerLimit)
							g_tCommand.sCurrentSetpoint -= 100;
						OS_MutexRelease( OSMTXCommand );
					}
					break;
				case eGSplit:
					// unlock
					if(s_ucKeyLock)
					{
						s_ucKeyLock = 0;
					}
					else
					{
						s_ucKeyLock = 1;
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
			TimeDiff = 0;

			//OS_WaitTicks(OSALTouchPause,10);
			break;

		case eTSBlocked:
			if(bMoved == 0 && TimeDiff > 20 && eActualGesture == 0) // warte bis komplett losgelassen
			{
				eTouchstate = eTSIdle;
			}

			break;

		default:
			break;
	}

	s_ucOldGesture = eActualGesture;

	g_debug = eTouchstate;

	g_debug3 = eActualGesture;

}

eGestures_t getGesture(void) // delayed by one cycle
{
	uint8_t ret = 0;
	uint8_t i;

	for(i = 0; i < TOUCHCOUNT ; i++)
	{
		if(g_aucTouchpads[i] > TOUCHMINSIGNAL)
			ret |= ( 1 << i );
	}

	return ret; // scho feddisch
}

eGestures_t getGestureSkip(void) // delayed by one cycle
{
	uint8_t ret = 0;
	uint8_t i, j;

	for(i = 0 , j = 0; i < TOUCHCOUNT ; i += 2 , j++) // nur die obere mittlere und untere zulassen.
	{
		if(g_aucTouchpads[i] > TOUCHMINSIGNAL)
			ret |= ( 1 << j );
	}

	return ret;
}

uint8_t bitcount(uint8_t b)
{
	uint8_t r = 0, i;

	for(i = 0; i < 8 ; ++i)
	{
		if(b & ( 1 << i ))
			r++;
	}
	return r;
}

uint8_t bitcount3(uint8_t b)
{
	uint8_t r = 0, i;

	for(i = 0; i < 3 ; ++i)
	{
		if(b & ( 1 << i ))
			r++;
	}
	return r;
}

void touchtest(void)
{

	myP.max = 32000;
	myP.min = 0;

	//			for (i = 0; i < 3; i++)
	//			{
	//				lcd_print(WHITE, BLACK, 1, 0, 16*i,"val/t%i/tIdx/t%i      " ,maxVal[i],maxIdx[i]);
	//			}

	//				static int16_t oypos;
	//				int16_t nypos1;
	//
	//
	//				//touchGetValue(&nypos1);
	//
	//
	//				lcd_print(WHITE, BLACK, FONTSIZE, 0, 16,"VAL/t%i/tt      " ,nypos1);


	//				lcd_draw_box( 255,255,255,315,nypos1,5,5);
	//				if(nypos1 >0)
	//				{
	//
	//					lcd_draw_box(255,255,255,315,nypos1/4,5,5);
	//					oypos = nypos1/4;
	//				}

	static uint16_t g;

	g++;

	lcd_draw_pixel( RED,g,  (-myP.position/0xff)+160);
	//lcd_draw_pixel(GREEN,g,myP.velocity/0xff+128);
			//lcd_draw_pixel(YELLOW,g,myP.force/0xff+160);
			lcd_draw_pixel(WHITE,g,30-g_debug*5);
			if (g == 320)
			{
				lcd_clear();
				lcd_draw_line(YELLOW,0,30,320,30);
				g =0;
			}

			lcd_print(WHITE, BLACK, 1, 0, 32,"State: %i  " ,(uint16_t)g_debug);
			lcd_print(WHITE, BLACK, 1, 0, 64,"Gesture: %i  " ,(uint16_t)g_debug2);
	lcd_print(WHITE, BLACK, 1, 0, 96,"Particle: %i , %i   " ,(uint16_t)myP.position,(uint16_t)myP.velocity);
	lcd_print(WHITE, BLACK, 1, 0, 128,"info: %i  " ,(uint16_t)info);

	lcd_print(WHITE, BLACK, 1, 280, 20,"%i  " ,g_aucTouchpads[0]);
	lcd_print(WHITE, BLACK, 1, 280, 60,"%i  " ,g_aucTouchpads[1]);
	lcd_print(WHITE, BLACK, 1, 280, 120,"%i  " ,g_aucTouchpads[2]);
	lcd_print(WHITE, BLACK, 1, 280, 180,"%i  " ,g_aucTouchpads[3]);
	lcd_print(WHITE, BLACK, 1, 280, 220,"%i  " ,g_aucTouchpads[4]);

	lcd_print(RED, BLACK, 1, 160, 120,"%i  " ,Schwerpunkt);

	/*lcd_print(WHITE, BLACK, 1, 200, 20,"%i  " ,g_aucTouchpads[0]);
	lcd_print(WHITE, BLACK, 1, 200, 60,"%i  " ,g_aucTouchpads[1]);
	lcd_print(WHITE, BLACK, 1, 200, 120,"%i  " ,g_aucTouchpads[2]);
	lcd_print(WHITE, BLACK, 1, 200, 180,"%i  " ,g_aucTouchpads[3]);
	lcd_print(WHITE, BLACK, 1, 200, 220,"%i  " ,g_aucTouchpads[4]);*/


}

