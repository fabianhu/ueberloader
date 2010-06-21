/* definitions for menu */
#ifndef MENU_H
	#define MENU_H

#include <avr/io.h>
#include <avr/pgmspace.h>
#include "lcd.h"
//#include "OS/FabOS.h"//get unused stack

typedef struct Parameter_tag
{
	int16_t		sParam;
	int16_t		sLowerLimit;
	int16_t		sUpperLimit;
}Parameter_t;

typedef struct MenuItem_tag
{
	//must be initialized by user
	uint8_t 		ucID;			//Menu ID
	char* 			strName;		//Name
	void(*			pAction)(void);	//Function to execute
	Parameter_t*	pParamID;		//Parameter to change
	uint8_t			ucSubMenu; 		//Linked submenu
	uint8_t			ucPrev;			//Previous menu
	uint8_t			ucNext;			//Next menu
	//initialized by init_menu()
	uint8_t 		ucTop;			//Top Level menu
	uint8_t 		ucNr_grp_items;	//Number of items in the group
	uint8_t 		ucItem_nr;		//Position of the item in the group
	uint8_t 		ucItem_sel;		//Menue item is selected
	uint8_t 		ucParam_en;		//Parameter is enabled (enable editing)
}
MenuItem_t;

extern void init_menu(void);
extern void show_menu(void);
extern void menu_next(unsigned char step);
extern void menu_prev(unsigned char step);
extern void menu_select(void);
#endif //menu

