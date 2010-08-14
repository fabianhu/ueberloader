/* definitions for menu */


#include <avr/io.h>
#include <avr/pgmspace.h>
#include "lcd.h"


typedef struct Parameter_tag
{
	int16_t		sParam;
	int16_t		sLowerLimit;
	int16_t		sUpperLimit;
}Parameter_t;

typedef struct MenuItemName_tag
{
	//must be initialized by user
	char* 			strName;		//Name
}
MenuItemName_t;

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
	//initialized by menu_init()
	uint8_t 		ucTop;			//Top Level menu
	uint8_t 		ucNr_grp_items;	//Number of items in the group
	uint8_t 		ucItem_nr;		//Position of the item in the group
	uint8_t 		ucItem_sel;		//Menue item is selected
	uint8_t 		ucParam_en;		//Parameter is enabled (enable editing)
}
MenuItem_t;

//Errorcode
#define INFINITE_LOOP 	1			//A infinite loop exists in the menu structure
#define WRONG_SUB_UCID 	2			//The ID of the sub menu item is wrong
#define WRONG_PREV_UCID 3			//The ID of the previous menu item is wrong
#define WRONG_NEXT_UCID	4			//The ID of the next menu item is wrong

//Menusettings
#define ITEMSCOUNT_PAGE 3			


//extern Prototypes
extern void menu_init(void);
extern void menu_show(void);
extern void menu_next(unsigned char step);
extern void menu_prev(unsigned char step);
extern void menu_select(void);


