/* definitions for menu */
#define F_CPU 16000000UL  // 16 MHz

#include <avr/io.h>
#include <avr/pgmspace.h>
#include "menu_variant.h"
#include <util/delay.h>
//#include "display.h"	//injektortester
#include "lcd.h"		//nokia
#include "OS/FabOS.h"


typedef struct Parameter_tag
{
	int16_t		sParam;
	int16_t		sLowerLimit;
	int16_t		sUpperLimit;
	char		sUnit[4];
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
	Parameter_t*	pParamID;		//Parameter to change
	uint8_t			ucJumpTrg; 		//jump target
	uint8_t			ucParent;		//Parent item (0 = root)
	uint8_t 		ucSettings;		//menut item settings 
									//0bxxxx abcc 
									//		 ||	|_ 	cc 	menu item str location Flash(0), RAM(1) or EEPROM(2)
									//		 ||___	b 	menu item is selected
									//		 |____  a	paramter edit mode active (maybe a== b ?)
}
MenuItem_t;

//Errorcode
#define INFINITE_LOOP 	1			//A infinite loop exists in the menu structure
#define WRONG_SUB_UCID 	2			//The ID of the sub menu item is wrong
#define WRONG_PREV_UCID 3			//The ID of the previous menu item is wrong
#define WRONG_NEXT_UCID	4			//The ID of the next menu item is wrong

//bitmasks
#define BMSK_SELECTED 		(1<<2)
//#define BMSK_PAR_ED_MODE 	(1<<3)
#define BMSK_NAME_LOCATION	ucSettings&0x03
	

//extern Prototypes
extern void menu_init(void);
extern void menu_show(void);
extern void menu_next(unsigned char step);
extern void menu_prev(unsigned char step);
extern void menu_select(void);
extern uint8_t ParameterModeActive(void);

