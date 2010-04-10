/*
 * This is the Menu
 *
 * */

#include "menu.h"

const char txBlubb[] PROGMEM="MyText";

MenuItem_t me[8] = {
		{ 1 , txBlubb, 0, 0, 0, 0, 0},
		{ 2 , txBlubb, 0, 0, 0, 0, 0},
		{ 3 , txBlubb, 0, 0, 0, 0, 0},
		{ 4 , txBlubb, 0, 0, 0, 0, 0},
		{ 5 , txBlubb, 0, 0, 0, 0, 0},
		{ 6 , txBlubb, 0, 0, 0, 0, 0},
		{ 7 , txBlubb, 0, 0, 0, 0, 0},
};

Parameter_t pe[3]; // pe[0] is the actual menu position


MenuItem_t* gpsActualItem;
MenuItem_t* gpsSelectedItem;

void menuSelect(MenuItem_t mi)
{
	gpsActualItem = &mi;

	if(mi.pAction) mi.pAction();

	if(mi.ucSubMenu)
	{
		gpsActualItem = &me[mi.ucSubMenu]; // branch into submenu
	}
	else if(mi.pParamID)
	{
		gpsSelectedItem = gpsActualItem;
		 // und wieder raus...

	}

}


void menuUp(MenuItem_t mi)
{
	if(mi.pParamID)
	{
		// change parameter
		if((*mi.pParamID).sParam < (*mi.pParamID).sUpperLimit)
		{
			(*mi.pParamID).sParam++;
		}
	}
	else
	{
		// switch to next item
		gpsSelectedItem = &me[mi.ucNext];
	}

}

void menuDn(MenuItem_t mi)
{
	if(mi.pParamID)
	{
		// change parameter
		if((*mi.pParamID).sParam > (*mi.pParamID).sLowerLimit)
		{
			(*mi.pParamID).sParam--;
		}
	}
	else
	{
		// switch to prev item
		gpsSelectedItem = &me[mi.ucPrev];
	}
}

/*
Konzept:

Struct merkt sich, wo wir sind.
Wenn wir in einem Parameter stehen, wird beim drehen der Parameter geändert.
Wenn wir im Menü sind, wird beim Drehen der akt. Menüpunkt geändert.

--> Dreh-Drück struct wird mit aktuellem Menü bzw. Parameter vorbedatet.

Dreh-drück bekommt: Pointer zu Aktuellem Wert, Oberer und unterer Grenze.




*/
