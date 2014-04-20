#include "touchpad.h"
#include "../OS/FabOS.h"
#include "../comm/serial.h"
#include "../menu/menu.h"
#include "../menu/menu_variant.h"

// Prototypes:
void ProcessTouch(void);
int16_t callback_menu_GetvalueFromUI(void);
uint8_t touchGetSpeed(int16_t* speed, int32_t *Schwerpunkt);
void callback_menu_HandOverValueToUI(uint16_t value, uint16_t upper, uint16_t lower,
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
		
		if(GetBattStatus() == eBattWaiting)
		{
			ProcessTouch();//change value
		}
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

// erfasst welche Tasten gedr�ckt sind und gibt eine Prozentzahl zur�ck, um wieviel Prozent die Tastitensit�t h�her liegt als der Langzeitdurchschnitt
int16_t touchGetPad5(uint8_t pin)	
{
	int16_t value, i;
	uint8_t mask = ( 1 << pin );
	static uint8_t first = 0;
	static int16_t meanvalue[5];

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
	
	/* Info: 
	Bei nicht gedr�ckten Tasten liegen die Pin values in folgenden Bereichen (bei meinem 3-Tastigen):
	Tasten nicht gedr�ckt
	T1: 145 bis 151 nicht gedr�ckt / gedr�ckt 231 bis 241
	T2: (nix angeschlossen) 140 bis 141 / gedr�ckt 144 bis 145
	T3: 111 bis 122 nicht gedr�ckt / gedr�ckt 205 bis 219
	T4: (nix angeschlossen) 99 bis 101 nicht gedr�ckt / gedr�ckt 102
	T5: 170 bis 184 nicht gedr�ckt / gedr�ckt  251 bis 279                                                          -----> uint8 war zu klein!
	--> es liegen mindestens 50% value zwischen nicht gedr�ckt und gedr�ckt  --> es soll ein Tastendruck erkannt 
	werden, wenn der value �ber 30% gegen�ber dem Langzeitmittel ansteigt. 
 	
	// debug um alte values zu sehen
	static uint16_t storage[5][50];
	static uint8_t indexStorage;
	storage[pin][indexStorage] = value;
	if (pin > 3) // indexStorage muss richtig ver�ndert werden
	{
		indexStorage++;
		if (indexStorage == 50){ indexStorage = 0;}
	} */
		
	meanvalue[pin] = ((meanvalue[pin]*99) + value)/100;	// Mittelwert �ber 100 Werte, nicht �ndern!! nur bei diesem Setup laufen die Werte, wenn man eine Taste h�lt, wei� aber nicht warum
	
	if (first < 250)
	{
		if (first < 10) meanvalue[pin] = value;			// der 1. Wert wird verworfen, der 2. genommen und �berschreibt meanvalue[pin]
		first++;
		return 0;
	}
	else
	{
		return ((value * 100)/ meanvalue[pin])-100;		// gibt die Prozentzahl zur�ck, die der Value h�her liegt als der Durchschnitt der letzten 100 Werte											
	}													// derzeit wird eine gedr�ckte Taste bei > 32% erkannt
}

// fixme, wird das Ding noch gebraucht ?
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
			//asm("break"); bin grad da drin h�ngen geblieben (ret =-74) habs rausgeschmissen (Uli)
		}
		return ret;
	}
	else
	return -1;

}
// fixme, wird das Ding noch gebraucht ?
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
// fixme, wird das Ding noch gebraucht ?
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

void callback_menu_HandOverValueToUI(uint16_t value, uint16_t upper, uint16_t lower,
uint16_t stepsize)
{
	myP.upscale = (uint32_t)SLIDERUPSCALE
	/ ( (uint32_t)upper - (uint32_t)lower );

	myP.position = (uint32_t)value * myP.upscale;
	myP.stepsize = (uint32_t)stepsize * myP.upscale;
	myP.min = (uint32_t)lower * myP.upscale;
	myP.max = (uint32_t)upper * myP.upscale;
}

int16_t callback_menu_GetvalueFromUI(void)
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
		fpos = pos; // filter �berschreiben, wenn gerade g�ltig geworden.
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

	if(opos == -1) // gerade g�ltig geworden?
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
uint8_t info = 0;

int32_t Schwerpunkt;
void ProcessTouch(void)
{
	eGestures_t eActualGesture, newGesture;
	static eGestures_t s_ucOldGesture;
	static int16_t s_sSpeedFiltered = 0;
	uint8_t bMoved;
	static int32_t OldSchwerpunkt;
	int16_t calculator;
	
	// hier wird die aktuell g�ltige Geste ermittelt = eActualGesture
	eActualGesture = eGNothing;					// Ausgangslage, nix gedr�ckt, keine Aktion
	//bMoved	= touchGetSpeed( &s_sSpeedFiltered, &Schwerpunkt/*, &TouchBitfield*/);	// fixme  beim integrieren den Schwerpunkt / speed  nur rechnen, wenn nur ein bzw. zwei benachbarte gedr�ckt.
	newGesture = getGestureSkip();				// ermittelt welche Taste gedr�ckt ist
	if (newGesture == s_ucOldGesture)			// wenn alt gleich neu	
	{
		eActualGesture = newGesture;			// Geste ausf�hren
	}
	else
	{
		eActualGesture = eGNothing;				// nix tun
	}
	s_ucOldGesture = newGesture;				// aktuelle Geste merken
												// weils so sch�n war und um das schneller zu machen gleich nochmal
	//bMoved	= touchGetSpeed( &s_sSpeedFiltered, &Schwerpunkt/*, &TouchBitfield*/);	// fixme  beim integrieren den Schwerpunkt / speed  nur rechnen, wenn nur ein bzw. zwei benachbarte gedr�ckt.
	newGesture = getGestureSkip();				// ermittelt welche Taste gedr�ckt ist
	if (eActualGesture == eGNothing)			// wenn vorher noch keine Geste erkannt wurde
	{
		if (newGesture == s_ucOldGesture)	// wenn neue gleich alter Geste UND noch keine Geste erkannt wurde
		{
			eActualGesture = newGesture;			// Geste ausf�hren
		}
		else
		{
			eActualGesture = eGNothing;			// nix tun
		}
	}
	s_ucOldGesture = newGesture;				// neue Geste merken

//	static uint16_t TimeDiff = 0;
//	TimeDiff++;

// welcome to Ulis dirtiest hack ever!
// es geht immer noch dreckiger :) Gr��e Uli
// verschiedene Schrittweiten f�r die Ladestromeinstellung
	int16_t stepsize_used = myP.stepsize;
	uint8_t plus=0,minus=0;
	
		if(eActualGesture == eGPlus)
		plus =1;
		if(eActualGesture == eGMinus)
		minus =1;

	if(myP.stepsize == -1*myP.upscale && (plus || minus))			// wird nur aufgerufen wenn gerade Ladestromverstellung ausgew�hlt ist	
	{			
		if(plus)
		{
			if(myP.position < 500*myP.upscale)						// setzt verschiedene Schrittweiten, je nach aktuell eingestelltem Ladestrom
				stepsize_used = 100*myP.upscale;		
			if(myP.position >= 500*myP.upscale)
				stepsize_used = 250*myP.upscale;		
			if(myP.position >= 2000*myP.upscale)
				stepsize_used = 500*myP.upscale;
			if(myP.position >= 6000*myP.upscale)
				stepsize_used = 1000*myP.upscale;
		}
		if(minus)
		{
			if(myP.position <= 10000*myP.upscale)					// setzt verschiedene Schrittweiten, je nach aktuell eingestelltem Ladestrom
				stepsize_used = 1000*myP.upscale;
			if(myP.position <= 6000*myP.upscale)
				stepsize_used = 500*myP.upscale;		
			if(myP.position <= 2000*myP.upscale)
				stepsize_used = 250*myP.upscale;				
			if(myP.position <= 500*myP.upscale)
				stepsize_used = 100*myP.upscale;
		}
	}
// nearly end of dirty hack

/*	
	switch(eTouchstate)	// Wertet die gedr�ckten Tasten aus
	{
		case eTSIdle:
			//s_sSpeedFiltered = 0;
			myP.force = 0;
			if(bMoved == 0 && eActualGesture == 0)
			{		// not touched	
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
			{	// adapt speed	
				myP.force = -s_sSpeedFiltered;
				info = 2;
				myP.velocity = myP.velocity + myP.force; */
				/*static uint8_t n;  // bremsen!!!
				if (n++ == 10)
				{
				n = 0;
				myP.velocity = (myP.velocity * myP.friction) / 100;
				}*/ /*
			}
			else
			{
				if(myP.velocity == 0)
				{
					eTouchstate = eTSGesture;
					info = 3;
				}
				else
				{			// do nothing and let particle move on	
					info = 5;
					if(eActualGesture != 0)
					{
						myP.velocity = 0;			// bremsen
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
			info = 7; */
			switch(eActualGesture)   //Je nach ermittelter Geste wird hier reagiert
			{
				case eGNothing:
					break;
				case eGPlus:
					if(myP.position + stepsize_used < myP.max)
					myP.position += stepsize_used;
					else
					myP.position = myP.max;
					break;
				case eGMitte:
					// menue best�tigung
					menu_select();
					break;
				case eGMittePlus:			// Wert aufs maximum gesetzt
					myP.position = myP.max;
					break;
				case eGMinus:
					if(myP.position - stepsize_used > myP.min)
					myP.position -= stepsize_used;
					else
					myP.position = myP.min;
					break;
				case eGSplit:				// Wert in die Mitte des Wertebereichs gesetzt
						if (myP.stepsize == -1*myP.upscale)		// special Uli Hack, damit der Ladestrom auf 5A gesetzt wird
						{
							myP.position = 5000 * myP.upscale;				// entspricht 5A
						}
						else
						{
							calculator = ((myP.max - myP.min) / stepsize_used) / 2;	
							myP.position = myP.min + (calculator * stepsize_used);	
						}
					break;
					case eGMitteMinus:		// Wert aufs miinimum gesetzt
						myP.position = myP.min;
					break;
				case eGFullHouse:
					break;
				default:
					break;
			}
			//eTouchstate = eTSIdle;  // auf eTSBlocked zu gehen macht es nur langsam
			//eTouchstate = eTSBlocked;
			/*
			TimeDiff = 0;
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
	g_debug = eTouchstate;
	g_debug3 = eActualGesture;*/
}


// Ermittelt welche der f�nf!! Tasten gedr�ckt sind. Eine Taste gilt als gedr�ckt, wenn der gelieferte Zahlenwert gr��er als der Wert von TOUCHMINSIGNAL (32) ist.
// R�ckgabewert ist Taste Oben =00001, Taste Mitte = 00100, Taste unten = 10000 oder eine Kombi davon und Zwischenwerte.
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

// Ermittelt welche der drei!! Tasten gedr�ckt sind. Eine Taste gilt als gedr�ckt, wenn der gelieferte Zahlenwert gr��er als der Wert von TouchMinSignal (32) ist.
// R�ckgabewert ist Taste Oben =001, Taste Mitte = 010, Taste uben = 100 oder eine Kombi davon.
eGestures_t getGestureSkip(void) // delayed by one cycle
{
	uint8_t result , ret = 0;
	static uint8_t oldresult, buttonReleased, gesture, gesturerecognized, moveup, movedown, movedownrecognized, moveuprecognized, count = 0;
	uint8_t i, j, k, l;
	
	for(k = 0; k < TOUCHCOUNT ; k++)
	{
		g_aucTouchpads[k] = touchGetPad5(k);		// holt die Tastenwerte
	}
	
	result = 0;
	for(i = 0 , j = 0; i < TOUCHCOUNT ; i += 2 , j++) // nur die obere mittlere und untere Taste zulassen.
	{
		if(g_aucTouchpads[i] > TOUCHMINSIGNAL)
		result |= ( 1 << j );
	}
	
	// ich habe ein Monster erschaffen:
	// Das Ding entscheidet, ob es sich um eine Geste, einen move oder um einen einfachen Tastendruck
	// Falls es eine Geste ist, wird diese abgespeichert. Erst beim loslassen der Taste(n) wird reagiert und 2mal das Ergebnis ausgegeben
	if (buttonReleased < 1)							// wenn alle Tasten ausgelassen wurden
	{
		if (result < oldresult && result == 0)		// wenn die letzte Taste losgelassen wurde wird entschieden ob es eine Geste, ein Move oder ein normaler Tastendruck war
		{
			if (movedown == 3 || moveup == 3)		// wenn move vorliegt
			{
				if (movedown == 3) movedownrecognized = 1;
				if (moveup == 3) moveuprecognized = 1;
				ret = 0;							
				gesturerecognized = 0;				// wenn Move erkannt wurde, Move sticht Geste
				movedown = 0;						// alles wieder zur�cksetzen
				moveup = 0;
				oldresult = result;					// sonst funktioniert es mit der permanenten Ausgabe nicht
				
			}
			else
			{
				ret = oldresult;					// einfachen Tastendruck zum 1. mal ausgeben
				buttonReleased = 1;						// Knopf wurde losgelassen, das merken wir uns
			}
			if (gesturerecognized == 1)				// wenn Geste erkannt wurde
			{
				ret = gesture;						// Geste zum 1. mal ausgeben
				buttonReleased = 1;						// Knopf wurde losgelassen, das merken wir uns	 
			}	
		}
		else
		{											// wird immer abgearbeitet, wenn nicht gerade die letzte Taste losgelassen wird, hier erfolgt die Entscheidung, ob es sich um eine Geste handelt oder nicht.
			ret = 0;								// nix zu melden
			buttonReleased = 0;						// zur�cksetzten
			//---------- Gestenerkennung									
			if (result != 0 && result != 1			// Wenn Geste erkannt wurde
					&& result != 2 && result != 4)	
			{
				gesture = result;					// Geste speichern
				gesturerecognized = 1;
			}
			//---------- Move down
			if (result == 1 && oldresult == 0)			// k�nnte ein Move von oben nach unten werden
			{
				movedown = 1;
			}
			if (movedown == 1 && result == 2)			// jetzt schon bei der Mitte
			{
				movedown = 2;
			}
			if (movedown == 2 && result == 4)			// unten angekommem --> g�ltiger Move down
			{
				movedown = 3;
			}
			// ---------------------
			//---------- Move up
			if (result == 4 && oldresult == 0)			// k�nnte ein Move von unten noch oben werden
			{
				moveup = 1;
			}
			if (moveup == 1 && result == 2)				// jetzt schon bei der Mitte
			{
				moveup = 2;
			}
			if (moveup == 2 && result == 1)				// oben angekommem --> g�ltiger Move up
			{
				moveup = 3;
			}
			// ---------------------
			
			oldresult = result;						// aktuelle Tastenauswertung merken				
		}
	}
	else
	{
		if (result == 0)							// wenn das Result immernoch NULL ist
		{
			if (gesturerecognized == 1)				// wenn Geste erkannt
			{
				ret = gesture;						// Geste zum 2. mal ausgeben 
				gesturerecognized = 0;				// zur�cksetzten
			}
			else
			{
				ret = oldresult;					// einfachen Tastendruck zum 2. mal ausgeben
			}		
		}
		
		buttonReleased = 0;							// zur�cksetzten
		oldresult = result;							// Ergebnis merken
	}
	
	//----- Gibt Move aus, bis eine Taste gedr�ckt wird 
	if ((movedownrecognized == 1 || moveuprecognized == 1) && ret == 0)
	{
		if (count > 29)
		{
			if (movedownrecognized == 1) { ret = 4; }
			else { ret = 1; }						// Move wird auch immer 2 mal ausgeben, hoch = 1 , runter = 4
			if (count >30) count = 0;			
		}
		count++;							
	}
	else
	{
		if (movedownrecognized == 1 || moveuprecognized == 1) ret = 0;	// unterdr�ckt den ersten Tastendruck nach dem move und stoppt ihn
		count = 0;
		movedownrecognized = 0;
		moveuprecognized = 0;
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

