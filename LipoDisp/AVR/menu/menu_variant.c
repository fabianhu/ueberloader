
#include "menu.h"
#include "menu_variant.h" 
#include "string.h"
#include "../comm/serial.h"
#include <avr/eeprom.h>


//extern vars / functions
extern uint8_t gucSelectedItem;
extern Command_t 		g_tCommand; // command.c

extern void GetSubMenuCount(uint8_t *Size, uint8_t *StartIndex);
//global vars
					







//******** START OF AUTO-GENERATED CODE DO NOT EDIT!!! *********
// Text definitions
char	txtMAINMENU[] 	PROGMEM="Main Menu";
char	txtCHARGE[] 	PROGMEM="Charge";
char	txtDISCHARGE[] 	PROGMEM="Discharge";
char	txtSETUP[] 	PROGMEM="Setup";
char	txtCURRENTSETPOINT[] 	PROGMEM="Current Setpoint";
char	txtVOLTSETPOINT[] 	PROGMEM="Volt Setpoint";
char	txtCELLCOUNT[] 	PROGMEM="CellCount";
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
Parameter_t parMaxcap = {	1000, 0, 10000, 100, mAh};
Parameter_t parMaxtime = {	60, 1, 6000, 1, min};
Parameter_t parBalActVolt = {	3600, 2000, 4100, 100, V};
Parameter_t parPWMfrequency = {	50, 10, 100, 10, kHz};
Parameter_t parRefreshPeriod = {	10, 1, 10000, 10, ms};
Parameter_t parCurrent = {	1000, 100, 10000, 100, mA};
Parameter_t parVoltSetpoint = {	4150, 2300, 4200, 50, mV};
Parameter_t parCellCount = {	0, 0, 6, 1, Cells};

			//Name	Act	Par	Jmp	Parent	Memory
MenuItem_t m_items[MENUESIZE] = {
	/* 0*/	{txtMAINMENU,	 0,	 0,	1,	0,	FLASH},
	/* 1*/	{txtCHARGE,	 0,	 0,	8,	0,	FLASH},
	/* 2*/	{txtDISCHARGE,	 0,	 0,	0,	0,	FLASH},
	/* 3*/	{txtSETUP,	 0,	 0,	21,	0,	FLASH},
	/* 4*/	{txtCURRENTSETPOINT,	 0,	 &parCurrent,	0,	0,	FLASH},
	/* 5*/	{txtVOLTSETPOINT,	 0,	 &parVoltSetpoint,	0,	0,	FLASH},
	/* 6*/	{txtCELLCOUNT,	 0,	 &parCellCount,	0,	0,	FLASH},
	/* 7*/	{txtBACK,	 leavenmenu,	 0,	0,	0,	FLASH},
	/* 8*/	{txtMODE,	 0,	 0,	12,	1,	FLASH},
	/* 9*/	{txtCHARGEMETHOD,	 0,	 0,	14,	1,	FLASH},
	/* 10*/	{txtCHARGELIMIT,	 0,	 0,	18,	1,	FLASH},
	/* 11*/	{txtBACK,	 0,	 0,	1,	1,	FLASH},
	/* 12*/	{txtAUTO,	 ActionChargeModeAuto,	 0,	0,	8,	FLASH},
	/* 13*/	{txtMANUAL,	 ActionChargeModeMaual,	 0,	0,	8,	FLASH},
	/* 14*/	{txtFULL,	 ActionChargeMethodFull,	 0,	0,	9,	FLASH},
	/* 15*/	{txtSTORAGE,	 ActionChargeMethodStorage,	 0,	0,	9,	FLASH},
	/* 16*/	{txtMANUAL,	 ActionChargeMethodManual,	 0,	0,	9,	FLASH},
	/* 17*/	{txtBACK,	 0,	 0,	9,	9,	FLASH},
	/* 18*/	{txtCAPACITY,	 0,	 &parMaxcap,	0,	10,	FLASH},
	/* 19*/	{txtTIME,	 0,	 &parMaxtime,	0,	10,	FLASH},
	/* 20*/	{txtBACK,	 0,	 0,	10,	10,	FLASH},
	/* 21*/	{txtBALANCER,	 0,	 0,	24,	3,	FLASH},
	/* 22*/	{txtPWM,	 0,	 0,	26,	3,	FLASH},
	/* 23*/	{txtBACK,	 0,	 0,	3,	3,	FLASH},
	/* 24*/	{txtACTIVATIONVOLTAGE,	 0,	 &parBalActVolt,	0,	21,	FLASH},
	/* 25*/	{txtBACK,	 0,	 0,	21,	21,	FLASH},
	/* 26*/	{txtFEQUENCY,	 0,	 &parPWMfrequency,	0,	22,	FLASH},
	/* 27*/	{txtREFRESHPERIOD,	 0,	 &parRefreshPeriod,	0,	22,	FLASH},
	/* 28*/	{txtBACK,	 0,	 0,	22,	22,	FLASH},
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
void ActionChargeModeAuto (void)
{
	g_tCommand.eChargerMode = eModeAuto;
	// fixme up!
}
void ActionChargeModeMaual (void)
{
	g_tCommand.eChargerMode = eModeManual;
	// fixme up!
}
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




