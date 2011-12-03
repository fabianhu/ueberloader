/* definitions for menu */




//******** START OF AUTO-GENERATED HEADER DO NOT EDIT!!! *********

 #define	MENUESIZE	49	// number of menu itmes (array size)
 #define	MAX_ITEM_NAME_CHARLENGTH	19	// number of menu itmes (array size)
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
	onoff,
} eParameterType_t;

// Parameter externals
extern Parameter_t parCurrent;
extern Parameter_t parChVoltLiPo;
extern Parameter_t parBalActVoltLiPo;
extern Parameter_t parBalOnLiPo;
extern Parameter_t parVoltLiIo;
extern Parameter_t parBalActVoltLiIo;
extern Parameter_t parBalOnLiIo;
extern Parameter_t parVoltLiFe;
extern Parameter_t parBalActVoltLiFe;
extern Parameter_t parBalOnLiFe;
extern Parameter_t parChtVoltStorage;
extern Parameter_t parBalActVoltStor;
extern Parameter_t parBalOnStor;
extern Parameter_t parVoltDisch;
extern Parameter_t parBalOnDisch;
extern Parameter_t parMaxcap;
extern Parameter_t parMaxtime;
extern Parameter_t parPWMfrequency;
extern Parameter_t parRefreshPeriod;
extern Parameter_t parCellCount;

// Action Prototypes
void leavemenu (void);
void ActionChargeModeLiPo (void);
void ActionChargeModeLiIo (void);
void ActionChargeModeLiFe (void);
void ActionChargeModeStorage (void);
void ActionChargeModeDischarge (void);
void actCalH (void);
void actCalL (void);

//******** END OF AUTO-GENERATED HEADER DO NOT EDIT!!! *********







//Menusettings
#define ITEMS_PER_PAGE 			8				//nr of items per menu page
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
