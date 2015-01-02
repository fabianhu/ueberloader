#include "touchpad.h"
#include "../OS/FabOS.h"
#include "../comm/serial.h"
#include "../menu/menu.h"
#include "../menu/menu_variant.h"

// Prototypes:
void ProcessTouch(void);
int16_t callback_menu_GetvalueFromUI(void);
//uint8_t touchGetSpeed(int16_t* speed, int32_t *Schwerpunkt);
void callback_menu_HandOverValueToUI(uint16_t value, uint16_t upper, uint16_t lower,
uint16_t stepsize);
//int32_t touchGetSchwerpunkt(void);
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

// erfasst welche Tasten gedrückt sind und gibt eine Prozentzahl zurück, um wieviel Prozent die Tastitensität höher liegt als der Langzeitdurchschnitt
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
	Bei nicht gedrückten Tasten liegen die Pin values in folgenden Bereichen (bei meinem 3-Tastigen):
	Tasten nicht gedrückt
	T1: 145 bis 151 nicht gedrückt / gedrückt 231 bis 241
	T2: (nix angeschlossen) 140 bis 141 / gedrückt 144 bis 145
	T3: 111 bis 122 nicht gedrückt / gedrückt 205 bis 219
	T4: (nix angeschlossen) 99 bis 101 nicht gedrückt / gedrückt 102
	T5: 170 bis 184 nicht gedrückt / gedrückt  251 bis 279                                                          -----> uint8 war zu klein!
	--> es liegen mindestens 50% value zwischen nicht gedrückt und gedrückt  --> es soll ein Tastendruck erkannt 
	werden, wenn der value über 30% gegenüber dem Langzeitmittel ansteigt. 
 	
	// debug um alte values zu sehen
	static uint16_t storage[5][50];
	static uint8_t indexStorage;
	storage[pin][indexStorage] = value;
	if (pin > 3) // indexStorage muss richtig verändert werden
	{
		indexStorage++;
		if (indexStorage == 50){ indexStorage = 0;}
	} */
		
	meanvalue[pin] = ((meanvalue[pin]*99) + value)/100;	// Mittelwert über 100 Werte, nicht ändern!! nur bei diesem Setup laufen die Werte, wenn man eine Taste hält, weiß aber nicht warum
	
	if (first < 250)
	{
		if (first < 10) meanvalue[pin] = value;			// der 1. Wert wird verworfen, der 2. genommen und überschreibt meanvalue[pin]
		first++;
		return 0;
	}
	else
	{
		return ((value * 100)/ meanvalue[pin])-100;		// gibt die Prozentzahl zurück, die der Value höher liegt als der Durchschnitt der letzten 100 Werte											
	}													// derzeit wird eine gedrückte Taste bei > 32% erkannt
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

eTouchstate_t eTouchstate = eTSIdle;
uint8_t info = 0;


int32_t Schwerpunkt;
void ProcessTouch(void)
{
	eGestures_t eActualGesture, newGesture;
	static eGestures_t s_ucOldGesture;
	int16_t calculator;
	static uint8_t gesturecount = 0;
	
	// hier wird die aktuell gültige Geste ermittelt = eActualGesture
	eActualGesture = eGNothing;					// Ausgangslage, nix gedrückt, keine Aktion
	newGesture = getGestureSkip();				// ermittelt welche Taste gedrückt ist
	if (newGesture == s_ucOldGesture)			// wenn alt gleich neu	
	{
		eActualGesture = newGesture;			// aktuelle Geste merken
	}
	else
	{
		eActualGesture = eGNothing;				// nix tun
	}
	s_ucOldGesture = newGesture;				// aktuelle Geste merken
												// weils so schön war und um das schneller zu machen gleich nochmal
	newGesture = getGestureSkip();				// ermittelt welche Taste gedrückt ist
	if (eActualGesture == eGNothing)			// wenn vorher noch keine Geste erkannt wurde
	{
		if (newGesture == s_ucOldGesture)		// wenn neue gleich alter Geste UND noch keine Geste erkannt wurde
		{
			eActualGesture = newGesture;		// aktuelle Geste merken
		}
		else
		{
			eActualGesture = eGNothing;			// nix tun
		}
	}
	s_ucOldGesture = newGesture;				// neue Geste merken


if(eActualGesture != eGNothing && gesturecount == 0 )   // Tastenentprellung - unterdrückt mehrere Gesten hintereinender 
{
	gesturecount = 20;	
}
else
{
	if (gesturecount > 0)
	{
		gesturecount -= 1;
		eActualGesture = eGNothing;
	}
}

// welcome to Ulis dirtiest hack ever!
// es geht immer noch dreckiger :) Grüße Uli
// verschiedene Schrittweiten für die Ladestromeinstellung
	int16_t stepsize_used = myP.stepsize;
	uint8_t plus=0,minus=0;
	
		if(eActualGesture == eGPlus)
		plus =1;
		if(eActualGesture == eGMinus)
		minus =1;

	if(myP.stepsize == -1*myP.upscale && (plus || minus))			// wird nur aufgerufen wenn gerade Ladestromverstellung ausgewählt ist	
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
				// menue bestätigung
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
}


// Ermittelt welche der fünf!! Tasten gedrückt sind. Eine Taste gilt als gedrückt, wenn der gelieferte Zahlenwert größer als der Wert von TOUCHMINSIGNAL (32) ist.
// Rückgabewert ist Taste Oben =00001, Taste Mitte = 00100, Taste unten = 10000 oder eine Kombi davon und Zwischenwerte.
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

// Ermittelt welche der drei!! Tasten gedrückt sind. Eine Taste gilt als gedrückt, wenn der gelieferte Zahlenwert größer als der Wert von TouchMinSignal (32) ist.
// Rückgabewert ist Taste Oben =001, Taste Mitte = 010, Taste uben = 100 oder eine Kombi davon.
eGestures_t getGestureSkip(void) // delayed by one cycle
{
	uint8_t result , ret = 0;
	static uint8_t oldresult, buttonReleased, gesture, gesturerecognized, moveup, movedown, movedownrecognized, moveuprecognized, movetime, count = 0;
	uint8_t i, j, k;
	
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
	// Das Ding entscheidet, ob es sich um eine Geste, einen Move oder um einen einfachen Tastendruck handelt
	// Falls es eine Geste oder ein Move ist, wird diese(r) abgespeichert. Erst beim loslassen der Taste(n) wird reagiert und 2mal das Ergebnis ausgegeben
	if (buttonReleased < 1)							// wenn alle Tasten ausgelassen wurden
	{
		if (result < oldresult && result == eGNothing)		// wenn die letzte Taste losgelassen wurde wird entschieden ob es eine Geste, ein Move oder ein normaler Tastendruck war
		{
			if (movedown == 3 || moveup == 3)		// wenn move vorliegt
			{
				if (movedown == 3) movedownrecognized = 1;
				if (moveup == 3) moveuprecognized = 1;
				ret = eGNothing;							
				gesturerecognized = 0;					// wenn Move erkannt wurde, Move sticht Geste
				movedown = 0;							// alles wieder zurücksetzen
				moveup = 0;
				oldresult = result;						// sonst funktioniert es mit der permanenten Ausgabe nicht, da old noch nicht = eGNothing ist
				
			}
			else
			{
				ret = oldresult;						// einfachen Tastendruck zum 1. mal ausgeben
				buttonReleased = 1;						// Knopf wurde losgelassen, das merken wir uns
				movedown = 0;
				moveup = 0;
			}
			if (gesturerecognized == 1)					// wenn Geste erkannt wurde
			{
				ret = gesture;							// Geste zum 1. mal ausgeben
				buttonReleased = 1;						// Knopf wurde losgelassen, das merken wir uns	 
			}	
		}
		else
		{												// wird immer abgearbeitet, wenn nicht gerade die letzte Taste losgelassen wird, hier erfolgt die Entscheidung, ob es sich um eine Geste handelt oder nicht.
			ret = eGNothing;							// nix zu melden
			buttonReleased = 0;							// zurücksetzten
			//---------- Gestenerkennung									
			if (result != eGNothing && result != eGPlus	// Wenn Geste erkannt wurde
					&& result != eGMitte && result != eGMinus)	
			{
				gesture = result;						// Geste speichern
				gesturerecognized = 1;
			}
			//---------- Move down
			if (result == eGPlus && oldresult == eGNothing)	// könnte ein Move von oben nach unten werden
			{
				movedown = 1;
				movetime = 0;
			}
			if (movedown == 1 && result == eGMitte)		// jetzt schon bei der Mitte
			{
				movedown = 2;
			}
			if (movedown == 2 && result == eGMinus)		// unten angekommem --> gültiger Move down
			{
				movedown = 3;
			}
			// ---------------------
			//---------- Move up
			if (result == eGMinus && oldresult == eGNothing)	// könnte ein Move von unten noch oben werden
			{
				moveup = 1;
				movetime = 0;
			}
			if (moveup == 1 && result == eGMitte)		// jetzt schon bei der Mitte
			{
				moveup = 2;
			}
			if (moveup == 2 && result == eGPlus)		// oben angekommem --> gültiger Move up
			{
				moveup = 3;
			}
			// ---------------------
			
			oldresult = result;							// aktuelle Tastenauswertung merken				
		}
	}
	else
	{
		if (result == eGNothing)						// wenn das Result immernoch NULL ist
		{
			if (gesturerecognized == 1)					// wenn Geste erkannt
			{
				ret = gesture;							// Geste zum 2. mal ausgeben 
				gesturerecognized = 0;					// zurücksetzten
			}
			else
			{
				ret = oldresult;						// einfachen Tastendruck zum 2. mal ausgeben
			}		
		}
		
		buttonReleased = 0;								// zurücksetzten
		oldresult = result;								// Ergebnis merken
	}
	
	
	// zur Ermittlung wie lange der Move gedauert hat
	if (moveup > 0 || movedown > 0)
	{
		if (movetime < MOVEMAXTIME) movetime++;			// Zeit des moves messen --> langsammer move ändert dann die Werte auch langsammer
	}

	
	//----- Gibt Move aus, bis eine Taste gedrückt wird 
	if ((movedownrecognized == 1 || moveuprecognized == 1) && ret == eGNothing)
	{
		if (movetime < MOVEMINTIME) movetime = MOVEMINTIME;
		if (count > movetime)						// je größer moovetime desto langsammer ändern sich dann die Werte
		{
			if (movedownrecognized == 1) { ret = eGMinus; }
			else { ret = eGPlus; }						// Move wird auch immer 2 mal ausgeben, hoch = 1 , runter = 4
			if (count > movetime +1 ) count = 0;			
		}
		count++;							
	}
	else
	{
		if (movedownrecognized == 1 || moveuprecognized == 1) // unterdrückt den ersten Tastendruck nach dem move und stoppt Ihn
		{ 
			ret = eGNothing; 
			buttonReleased = 0; 
			oldresult = eGNothing;
			movedown = 0; 
			moveup =0;
		}	
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

