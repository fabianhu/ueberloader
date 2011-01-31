#include "menu_variant.h" 
#include "string.h"
#include <avr/eeprom.h>


//extern vars / functions
extern uint8_t gucSelectedItem;
//extern void HandOverValueToUI(uint16_t value, uint16_t upper, uint16_t lower, uint8_t type);
//extern uint16_t GetvalueFromUI(void);
extern void GetSubMenuCount(uint8_t *Size, uint8_t *StartIndex);
//global vars
					




//******** START OF AUTO-GENERATED CODE DO NOT EDIT!!! *********
// Text definitions
char	txtMAINMENU[] 	PROGMEM="Main Menu";
char	txtCHARGE[] 	PROGMEM="Charge";
char	txtDISCHARGE[] 	PROGMEM="Discharge";
char	txtSETUP[] 	PROGMEM="Setup";
char	txtBACK[] 	PROGMEM="(back)";
char	txtMODE[] 	PROGMEM="Mode";
char	txtCHARGEMETHOD[] 	PROGMEM="Charge Method";
char	txtCHARGELIMIT[] 	PROGMEM="Charge Limit";
char	txtAUTO[] 	PROGMEM="Auto";
char	txtMANUAL[] 	PROGMEM="Manual";
char	txtFULL[] 	PROGMEM="Full";
char	txtSTORAGE[] 	PROGMEM="Storage";
char	txtCAPACITY[] 	PROGMEM="Capacity";
char	txtTIME[] 	PROGMEM="Time";
char	txtBALANCER[] 	PROGMEM="Balancer";
char	txtPWM[] 	PROGMEM="Pwm";
char	txtACTIVATIONVOLTAGE[] 	PROGMEM="Activation Voltage";
char	txtFEQUENCY[] 	PROGMEM="Fequency";
char	txtREFRESHPERIOD[] 	PROGMEM="Refresh-period";

// Parameter definitions
Parameter_t maxcap = {	0, 0, 10000, mAh};
Parameter_t maxtime = {	60, 1, 6000, min};
Parameter_t balactvolt = {	3600, 2000, 4100, V};
Parameter_t PWMfrequency = {	50, 10, 100, kHz};
Parameter_t RefreshPeriod = {	10, 1, 10000, ms};

			//Name	Act	Par	Jmp	Parent	Memory
MenuItem_t m_items[MENUESIZE] = {
	/* 0*/	{txtMAINMENU,	 0,	 0,	1,	0,	FLASH},
	/* 1*/	{txtCHARGE,	 0,	 0,	5,	0,	FLASH},
	/* 2*/	{txtDISCHARGE,	 0,	 0,	0,	0,	FLASH},
	/* 3*/	{txtSETUP,	 0,	 0,	18,	0,	FLASH},
	/* 4*/	{txtBACK,	 leavenmenu,	 0,	0,	0,	FLASH},
	/* 5*/	{txtMODE,	 0,	 0,	9,	1,	FLASH},
	/* 6*/	{txtCHARGEMETHOD,	 0,	 0,	11,	1,	FLASH},
	/* 7*/	{txtCHARGELIMIT,	 0,	 0,	15,	1,	FLASH},
	/* 8*/	{txtBACK,	 0,	 0,	1,	1,	FLASH},
	/* 9*/	{txtAUTO,	 ActionChargeModeAuto,	 0,	0,	5,	FLASH},
	/* 10*/	{txtMANUAL,	 ActionChargeModeMaual,	 0,	0,	5,	FLASH},
	/* 11*/	{txtFULL,	 ActionChargeMethodFull,	 0,	0,	6,	FLASH},
	/* 12*/	{txtSTORAGE,	 ActionChargeMethodStorage,	 0,	0,	6,	FLASH},
	/* 13*/	{txtMANUAL,	 ActionChargeMethodManual,	 0,	0,	6,	FLASH},
	/* 14*/	{txtBACK,	 0,	 0,	6,	6,	FLASH},
	/* 15*/	{txtCAPACITY,	 0,	 &maxcap,	0,	7,	FLASH},
	/* 16*/	{txtTIME,	 0,	 &maxtime,	0,	7,	FLASH},
	/* 17*/	{txtBACK,	 0,	 0,	7,	7,	FLASH},
	/* 18*/	{txtBALANCER,	 0,	 0,	21,	3,	FLASH},
	/* 19*/	{txtPWM,	 0,	 0,	23,	3,	FLASH},
	/* 20*/	{txtBACK,	 0,	 0,	3,	3,	FLASH},
	/* 21*/	{txtACTIVATIONVOLTAGE,	 0,	 &balactvolt,	0,	18,	FLASH},
	/* 22*/	{txtBACK,	 0,	 0,	18,	18,	FLASH},
	/* 23*/	{txtFEQUENCY,	 0,	 &PWMfrequency,	0,	19,	FLASH},
	/* 24*/	{txtREFRESHPERIOD,	 0,	 &RefreshPeriod,	0,	19,	FLASH},
	/* 25*/	{txtBACK,	 0,	 0,	19,	19,	FLASH},
};

//******** END OF AUTO-GENERATED CODE DO NOT EDIT!!! *********





//SRAM
//user defined names
uint8_t eeNames[3][MAX_SRAM_ITEM_NAME_LENGHT+1];
char ud_name[][MAX_SRAM_ITEM_NAME_LENGHT+1]	={//Menu item name
													{"CURRENT-TEST"},
													{"USER DEFINED 02"},
													{"USER DEFINED 03"},
											};

//Variables in EEPROM
//uint16_t eeParameter[(100*8*4)+2] EEMEM;	//variable in eeprom for storing the parameters
//uint8_t eeVarsValid EEMEM;							//Marker if a valid dataset is stored in eeprom



// Action functions
void ActionChargeModeAuto (void){;}
void ActionChargeModeMaual (void){;}
void ActionChargeMethodFull (void){;}
void ActionChargeMethodStorage (void){;}
void ActionChargeMethodManual (void){;}
extern uint8_t g_bMenuActive;
void leavenmenu (void)
{
	g_bMenuActive = 0;
}






// ************* LCD interface

void menu_draw_header(char *menu_header)
{
	//delete old menu header
		lcd_draw_filled_box(BLACK, 0, 0, 320, 40);
	//draw new menu header
		//injektortester
		lcd_print(WHITE, BLACK, 2, 0, 0, menu_header);
}

void menu_draw_unselected_items(char *item_name, uint16_t par_exists, int16_t parameter, uint8_t type, uint8_t lcd_pos)
{
	//delet old value
		lcd_draw_filled_box(BLACK, 20, 40+lcd_pos*20, 300, 20);
	//draw menu item
		lcd_print(GREEN, BLACK, 1, 20, 40+lcd_pos*20,item_name);
	if (par_exists)  
	{	
		//delete old val
			
		//write new val
			lcd_print(WHITE, BLACK, 1, 200, 40+lcd_pos*20, "%i",parameter);
	}
	
	
	
}

extern void menu_draw_selected_item(char *item_name, uint16_t par_exists, int16_t parameter, uint8_t type, uint8_t lcd_pos)
{
	//delet old value
		lcd_draw_filled_box(BLACK, 200, 40+lcd_pos*20, 100, 20);
	//draw menu item
		lcd_print(WHITE, BLACK, 1, 20, 40+lcd_pos*20,item_name);
		lcd_print(WHITE, BLACK, 1, 300, 40+lcd_pos*20, "<=");

	if (par_exists)  
	{	
		//delete old val

		//write new val
		lcd_print(WHITE, BLACK, 1, 200, 40+lcd_pos*20, "%i",parameter);
	}
	
}

void menu_draw_selected_parameter(char *item_name, int16_t parameter, uint8_t type, uint8_t lcd_pos)
{
	//delet old value
		lcd_draw_filled_box(BLACK, 20, 40+lcd_pos*20, 300, 20);
	//draw menu item
		lcd_print(RED, BLACK, 1, 20, 40+lcd_pos*20,item_name);
		lcd_print(RED, BLACK, 1, 280, 40+lcd_pos*20, "edit");
	//write new val
		lcd_print(RED, BLACK, 1, 200, 40+lcd_pos*20, "%i",parameter);	
}

void menu_del_menuitems(void)//menu plane is changed
{
	//delete menu items
	lcd_draw_filled_box(BLACK, 20, 40, 300, 200);
}

void menu_draw_groupposition(uint8_t itemnr, uint8_t groupitems)
{
	lcd_print(WHITE, BLACK, 1, 280, 10, "%iv%i",itemnr,groupitems);
}

/////////////////////////////////////////////////////////////////////////////////////////
// U S E R  F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////




