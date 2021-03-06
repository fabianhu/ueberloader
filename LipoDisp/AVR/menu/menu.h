/* definitions for menu */

#include <avr/io.h>
#include <avr/pgmspace.h>
//#include "menu_variant.h"
#include "../lcd/lcd.h"		//nokia

typedef struct Menue_tag
{
	uint8_t gucSelectedItem;
	uint8_t ShowParameter;
	uint8_t ShowMenu;
	uint8_t MenuMode;
}Menue_t;

#define MENUEINIT {1,0,0,1}


typedef struct Parameter_tag // do not change types or order, generator uses this type!
{
	int16_t		sValue;			//Parameter value
	int16_t		sLowerLimit;	//Parameter Lower Limit
	int16_t		sUpperLimit;	//Parameter Upper Limit
	int16_t		sStepSize;		//Parameter Step size
	uint8_t		sType;		//Parameter type (Voltage, Current...)	
}Parameter_t;


typedef struct MenuItemName_tag
{
	//must be initialized by user
	char* 			strName;		//Name
}
MenuItemName_t;

typedef struct MenuItem_tag
{
	char* 			strName;		//Name
	void(*			pAction)(void);	//Function to execute
	Parameter_t*	pParam;		//Parameter to change
	uint8_t			ucJumpTrg; 		//jump target
	uint8_t			ucParent;		//Parent item (0 = root)
	uint8_t 		ucSettings;		//menut item settings 
									//0bxxxx abcc 
									//		 ||	|_ 	cc 	menu item str location Flash(0), RAM(1) or EEPROM(2)
									//		 ||___	b 	menu item is selected
									//		 |____  a	paramter edit mode active (maybe a== b ?)
}
MenuItem_t;

//Parameter types			examples
#define MENUITEM		0   //...just incr/decr. with 1
#define BOOSTVOLTAGE	1	//guintUBoost=800;	//80,0V
#define BOOSTCURRENT	2	//guintIBoost=30;	//3,0A
#define BOOSTTIME		3	//guintTBoost=5000;	//5,0ms
#define BOOSTFREQUENCY	4	//guintFBoost=20;	//20us
#define HOLDCURRENT		5	//guintIHold =15;	//1,5A
#define HOLDTIME		6	//guintTHold =10000;//10,0ms
#define HOLDFREQUENCY	7	//guintFHold =20;	//20us

//Errorcode
#define INFINITE_LOOP 	1			//A infinite loop exists in the menu structure
#define WRONG_SUB_UCID 	2			//The ID of the sub menu item is wrong
#define WRONG_PREV_UCID 3			//The ID of the previous menu item is wrong
#define WRONG_NEXT_UCID	4			//The ID of the next menu item is wrong


//extern Prototypes
extern void menu_init(void);
extern void menu_show(void);
extern void menu_next(unsigned char step);
extern void menu_prev(unsigned char step);
extern void menu_select(void);
extern uint8_t ParameterModeActive(void);
extern void menue_forcedraw(void);

