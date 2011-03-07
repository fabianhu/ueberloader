
#include "menu.h"
#include "menu_variant.h" 
#include "string.h"
#include "../comm/serial.h"
#include <avr/eeprom.h>


//extern vars / functions
extern Menue_t MyMenue;
extern Command_t 		g_tCommand; // command.c

extern void GetSubMenuCount(uint8_t *Size, uint8_t *StartIndex);
//global vars
					
void leavemenu(void);

void menuUp(void);





//******** START OF AUTO-GENERATED CODE DO NOT EDIT!!! *********
// Text definitions
char	txtMAINMENU[] 	PROGMEM="Main Menu";
char	txtCURRENTSETPOINT[] 	PROGMEM="Current Setpoint";
char	txtACTION[] 	PROGMEM="Action";
char	txtBATTTYPESELECT[] 	PROGMEM="Batt Type Select";
char	txtSETUP[] 	PROGMEM="Setup";
char	txtBACK[] 	PROGMEM="(back)";
char	txtCHARGEFULL[] 	PROGMEM="Charge Full";
char	txtCHARGESTORAGE[] 	PROGMEM="Charge Storage";
char	txtDISCHARGE[] 	PROGMEM="Discharge";
char	txtLIPO[] 	PROGMEM="LiPo";
char	txtLIFEPO[] 	PROGMEM="LiFePo4";
char	txtLIION[] 	PROGMEM="LiIon";
char	txtCELLCOUNT[] 	PROGMEM="CellCount";
char	txtBATTSETTINGS[] 	PROGMEM="Batt Settings";
char	txtCHARGELIMITS[] 	PROGMEM="Charge Limits";
char	txtPWM[] 	PROGMEM="Pwm";
char	txtCHARGEVOLTAGE[] 	PROGMEM="Charge Voltage";
char	txtSTORAGEVOLTAGE[] 	PROGMEM="Storage Voltage";
char	txtDISCHARGEVOLTAGE[] 	PROGMEM="Discharge Voltage";
char	txtBALACTIVVOLTAGE[] 	PROGMEM="Bal. Activ. Voltage";
char	txtLIFEPOVOLT[] 	PROGMEM="LiFePo4 Volt.";
char	txtLIIONVOLTAGE[] 	PROGMEM="LiIon Voltage";
char	txtACTIVATIONVOLTAGE[] 	PROGMEM="Activation Voltage";
char	txtCAPACITY[] 	PROGMEM="Capacity";
char	txtTIME[] 	PROGMEM="Time";
char	txtFEQUENCY[] 	PROGMEM="Fequency";
char	txtREFRESHPERIOD[] 	PROGMEM="Refresh-period";

// Parameter definitions
Parameter_t parCurrent = {	0, 100, 10000, 100, mA};
Parameter_t parCellCount = {	0, 0, 6, 1, Cells};
Parameter_t parLiPoChVolt = {	0, 3000, 4200, 10, mV};
Parameter_t parLiPoStVolt = {	0, 2000, 4100, 100, mV};
Parameter_t parLiPoDisVolt = {	0, 3000, 4200, 10, mV};
Parameter_t parLiPoBalActVolt = {	0, 2000, 4100, 100, mV};
Parameter_t parLiFeVolt = {	0, 2000, 3700, 10, mV};
Parameter_t parBalActVolt = {	0, 2000, 4100, 100, V};
Parameter_t parLiIonVolt = {	0, 3500, 4100, 10, mV};
Parameter_t parBalActVolt2 = {	0, 2000, 4100, 100, V};
Parameter_t parMaxcap = {	0, 0, 10000, 100, mAh};
Parameter_t parMaxtime = {	0, 1, 6000, 1, min};
Parameter_t parPWMfrequency = {	0, 10, 100, 10, kHz};
Parameter_t parRefreshPeriod = {	0, 1, 10000, 10, ms};

			//Name	Act	Par	Jmp	Parent	Memory
MenuItem_t m_items[MENUESIZE] = {
	/* 0*/	{txtMAINMENU,	 0,	 0,	1,	0,	FLASH},
	/* 1*/	{txtCURRENTSETPOINT,	 0,	 &parCurrent,	0,	0,	FLASH},
	/* 2*/	{txtACTION,	 0,	 0,	6,	0,	FLASH},
	/* 3*/	{txtBATTTYPESELECT,	 0,	 0,	9,	0,	FLASH},
	/* 4*/	{txtSETUP,	 0,	 0,	12,	0,	FLASH},
	/* 5*/	{txtBACK,	 leavemenu,	 0,	0,	0,	FLASH},
	/* 6*/	{txtCHARGEFULL,	 ActionChargeMethodFull,	 0,	0,	2,	FLASH},
	/* 7*/	{txtCHARGESTORAGE,	 ActionChargeMethodStorage,	 0,	0,	2,	FLASH},
	/* 8*/	{txtDISCHARGE,	 ActionChargeMethodDischarge,	 0,	0,	2,	FLASH},
	/* 9*/	{txtLIPO,	 actSelTypeLiPo,	 0,	0,	3,	FLASH},
	/* 10*/	{txtLIFEPO,	 actSelTypeLiFe,	 0,	0,	3,	FLASH},
	/* 11*/	{txtLIION,	 actSelTypeLiIon,	 0,	0,	3,	FLASH},
	/* 12*/	{txtCELLCOUNT,	 0,	 &parCellCount,	0,	4,	FLASH},
	/* 13*/	{txtBATTSETTINGS,	 0,	 0,	17,	4,	FLASH},
	/* 14*/	{txtCHARGELIMITS,	 0,	 0,	32,	4,	FLASH},
	/* 15*/	{txtPWM,	 0,	 0,	35,	4,	FLASH},
	/* 16*/	{txtBACK,	 0,	 0,	4,	4,	FLASH},
	/* 17*/	{txtLIPO,	 0,	 0,	21,	13,	FLASH},
	/* 18*/	{txtLIFEPO,	 0,	 0,	26,	13,	FLASH},
	/* 19*/	{txtLIION,	 0,	 0,	29,	13,	FLASH},
	/* 20*/	{txtBACK,	 0,	 0,	13,	13,	FLASH},
	/* 21*/	{txtCHARGEVOLTAGE,	 0,	 &parLiPoChVolt,	0,	17,	FLASH},
	/* 22*/	{txtSTORAGEVOLTAGE,	 0,	 &parLiPoStVolt,	0,	17,	FLASH},
	/* 23*/	{txtDISCHARGEVOLTAGE,	 0,	 &parLiPoDisVolt,	0,	17,	FLASH},
	/* 24*/	{txtBALACTIVVOLTAGE,	 0,	 &parLiPoBalActVolt,	0,	17,	FLASH},
	/* 25*/	{txtBACK,	 0,	 0,	17,	17,	FLASH},
	/* 26*/	{txtLIFEPOVOLT,	 0,	 &parLiFeVolt,	0,	18,	FLASH},
	/* 27*/	{txtBALACTIVVOLTAGE,	 0,	 &parBalActVolt,	0,	18,	FLASH},
	/* 28*/	{txtBACK,	 0,	 0,	18,	18,	FLASH},
	/* 29*/	{txtLIIONVOLTAGE,	 0,	 &parLiIonVolt,	0,	19,	FLASH},
	/* 30*/	{txtACTIVATIONVOLTAGE,	 0,	 &parBalActVolt2,	0,	19,	FLASH},
	/* 31*/	{txtBACK,	 0,	 0,	19,	19,	FLASH},
	/* 32*/	{txtCAPACITY,	 0,	 &parMaxcap,	0,	14,	FLASH},
	/* 33*/	{txtTIME,	 0,	 &parMaxtime,	0,	14,	FLASH},
	/* 34*/	{txtBACK,	 0,	 0,	14,	14,	FLASH},
	/* 35*/	{txtFEQUENCY,	 0,	 &parPWMfrequency,	0,	15,	FLASH},
	/* 36*/	{txtREFRESHPERIOD,	 0,	 &parRefreshPeriod,	0,	15,	FLASH},
	/* 37*/	{txtBACK,	 0,	 0,	15,	15,	FLASH},
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


extern uint8_t g_bMenuActive;

typedef enum
{
	eBattTypeLiPo,
	eBattTypeLiFe,
	eBattTypeLiIon
} eBattType_t;



eBattType_t g_eBattType;



// Action functions

void ActionChargeMethodFull (void)
{
	switch (g_eBattType)
	{
		case eBattTypeLiPo:
			g_tCommand.usVoltageSetpoint_mV = parLiPoChVolt.sValue;
			g_tCommand.usMinBalanceVolt_mV = parLiPoBalActVolt.sValue;
			break;
		case eBattTypeLiFe:
			break;
		case eBattTypeLiIon:
			break;
		default:
			break;
	}
	menuUp();
}

void ActionChargeMethodStorage (void)
{
	switch (g_eBattType)
	{
		case eBattTypeLiPo:
			g_tCommand.usVoltageSetpoint_mV = parLiPoStVolt.sValue;
			g_tCommand.usMinBalanceVolt_mV = parLiPoBalActVolt.sValue;
			break;
		case eBattTypeLiFe:
			break;
		case eBattTypeLiIon:
			break;
		default:
			break;
	}
	menuUp();
}
void ActionChargeMethodDischarge (void)
{
	switch (g_eBattType)
	{
		case eBattTypeLiPo:
			g_tCommand.usVoltageSetpoint_mV = parLiPoDisVolt.sValue;
			g_tCommand.usMinBalanceVolt_mV = parLiPoBalActVolt.sValue; // fixme balancieren beim Entladen aus.
			break;
		case eBattTypeLiFe:
			break;
		case eBattTypeLiIon:
			break;
		default:
			break;
	}
	menuUp();
}
void actSelTypeLiPo (void){g_eBattType = eBattTypeLiPo; menuUp();}
void actSelTypeLiFe (void){g_eBattType = eBattTypeLiFe; menuUp();}
void actSelTypeLiIon (void){g_eBattType = eBattTypeLiIon; menuUp();}

void leavemenu(void)
{
	g_bMenuActive = 0;
}

void menuUp(void)
{
	MyMenue.gucSelectedItem = m_items[MyMenue.gucSelectedItem].ucParent;
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




