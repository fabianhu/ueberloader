/* definitions for menu */


//******** START OF AUTO-GENERATED HEADER DO NOT EDIT!!! *********

 #define	MENUESIZE	29	// number of menu itmes (array size)
 #define	MAX_ITEM_NAME_CHARLENGTH	18	// number of menu itmes (array size)
// Enum definitions
typedef enum
{
	Cells,
	kHz,
	mA,
	mAh,
	min,
	ms,
	mV,
	V,
} eParameterType_t;

// Parameter externals
extern Parameter_t parMaxcap;
extern Parameter_t parMaxtime;
extern Parameter_t parBalActVolt;
extern Parameter_t parPWMfrequency;
extern Parameter_t parRefreshPeriod;
extern Parameter_t parCurrent;
extern Parameter_t parVoltSetpoint;
extern Parameter_t parCellCount;

// Action Prototypes
void ActionChargeModeAuto (void);
void ActionChargeModeMaual (void);
void ActionChargeMethodFull (void);
void ActionChargeMethodStorage (void);
void ActionChargeMethodManual (void);
void leavenmenu (void);

//******** END OF AUTO-GENERATED HEADER DO NOT EDIT!!! *********




//Errorcode
#define INFINITE_LOOP 			1			//A infinite loop exists in the menu structure
#define WRONG_SUB_UCID 			2			//The ID of the sub menu item is wrong
#define WRONG_PREV_UCID 		3			//The ID of the previous menu item is wrong
#define WRONG_NEXT_UCID			4			//The ID of the next menu item is wrong

//Menusettings
#define ITEMS_PER_PAGE 			6				//nr of items per menu page
#define MAX_SRAM_ITEM_NAME_LENGHT 	16	//max nr of chars for SRAM item names
#define SCROLL_LINE_DISTANCE		2			

//get menu item name location
#define FLASH	0
#define SRAM	1
#define EEPROM	2

//extern Prototypes
extern void menu_draw_header(char *menu_header);
extern void menu_del_menuitems(void);
extern void menu_draw_unselected_items(char *item_name, uint16_t par_exists, int16_t parameter, uint8_t type, uint8_t lcd_pos);
extern void menu_draw_selected_item(char *item_name, uint16_t par_exists, int16_t parameter, uint8_t type, uint8_t lcd_pos);
extern void menu_draw_selected_parameter(char *item_name, int16_t parameter, uint8_t type, uint8_t lcd_pos);
extern void menu_draw_groupposition(uint8_t itemnr, uint8_t groupitems);
