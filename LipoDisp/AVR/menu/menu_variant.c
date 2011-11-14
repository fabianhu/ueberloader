#include "../OS/FabOS.h"
#include "menu.h"
#include "menu_variant.h" 
#include "string.h"
#include "../comm/serial.h"
#include <avr/eeprom.h>

typedef enum
{
	eBattTypeLiPo, eBattTypeLiFe, eBattTypeLiIon
} eBattType_t;

// global vars
eBattType_t g_eBattType;

//SRAM
//user defined names
uint8_t eeNames[3][MAX_SRAM_ITEM_NAME_LENGHT + 1];
char ud_name[][MAX_SRAM_ITEM_NAME_LENGHT + 1] =
	{//Menu item name
			{ "CURRENT-TEST" },
			{ "USER DEFINED 02" },
			{ "USER DEFINED 03" }, };

//extern vars
extern Menue_t MyMenue;
extern Command_t g_tCommand; // command.c
extern uint8_t g_bMenuActive;
extern uint8_t g_Trig_SavePars;
extern ChargerMode_t g_Tansfer_Action;
extern uint8_t gCommandsKnown; // command.c

//prototypes

void menuUp(void);
extern void GetSubMenuCount(uint8_t *Size, uint8_t *StartIndex);
void entermenu(void);
void HandOverValueToUI(uint16_t value, uint16_t upper, uint16_t lower, uint16_t stepsize);





//******** START OF AUTO-GENERATED CODE DO NOT EDIT!!! *********
// Text definitions
char	txtMAINMENU[] 	PROGMEM="Main Menu";
char	txtCURRENTSETPOINT[] 	PROGMEM="Current Setpoint";
char	txtVOLTAGESETPOINT[] 	PROGMEM="Voltage Setpoint";
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
char	txtCALIBRATIONH[] 	PROGMEM="Calibration H";
char	txtCALIBRATIONL[] 	PROGMEM="Calibration L";
char	txtBALACTIVVOLTAGE[] 	PROGMEM="Bal. Activ. Voltage";
char	txtCHARGEVOLTAGE[] 	PROGMEM="Charge Voltage";
char	txtSTORAGEVOLTAGE[] 	PROGMEM="Storage Voltage";
char	txtDISCHARGEVOLTAGE[] 	PROGMEM="Discharge Voltage";
char	txtLIFEPOVOLT[] 	PROGMEM="LiFePo4 Volt.";
char	txtLIIONVOLTAGE[] 	PROGMEM="LiIon Voltage";
char	txtCAPACITY[] 	PROGMEM="Capacity";
char	txtTIME[] 	PROGMEM="Time";
char	txtFEQUENCY[] 	PROGMEM="Fequency";
char	txtREFRESHPERIOD[] 	PROGMEM="Refresh-period";

// Parameter definitions
Parameter_t parCurrent = {	0, 100, 10000, 100, mA};
Parameter_t parVoltage = {	0, 2500, 4200, 10, mV};
Parameter_t parCellCount = {	0, 0, 6, 1, Cells};
Parameter_t parLiPoChVolt = {	0, 3000, 4200, 10, mV};
Parameter_t parLiPoStVolt = {	0, 2000, 4100, 100, mV};
Parameter_t parLiPoDisVolt = {	0, 3000, 4200, 10, mV};
Parameter_t parLiFeVolt = {	0, 2000, 3700, 10, mV};
Parameter_t parLiIonVolt = {	0, 3500, 4100, 10, mV};
Parameter_t parBalActVolt = {	0, 3000, 4200, 100, mV};
Parameter_t parMaxcap = {	0, 0, 10000, 100, mAh};
Parameter_t parMaxtime = {	0, 1, 6000, 1, min};
Parameter_t parPWMfrequency = {	0, 10, 100, 10, kHz};
Parameter_t parRefreshPeriod = {	0, 1, 10000, 10, ms};

			//Name	Act	Par	Jmp	Parent	Memory
MenuItem_t m_items[MENUESIZE] = {
	/* 0*/	{txtMAINMENU,	 0,	 0,	1,	0,	FLASH},
	/* 1*/	{txtCURRENTSETPOINT,	 0,	 &parCurrent,	0,	0,	FLASH},
	/* 2*/	{txtVOLTAGESETPOINT,	 0,	 &parVoltage,	0,	0,	FLASH},
	/* 3*/	{txtACTION,	 0,	 0,	7,	0,	FLASH},
	/* 4*/	{txtBATTTYPESELECT,	 0,	 0,	10,	0,	FLASH},
	/* 5*/	{txtSETUP,	 0,	 0,	13,	0,	FLASH},
	/* 6*/	{txtBACK,	 leavemenu,	 0,	0,	0,	FLASH},
	/* 7*/	{txtCHARGEFULL,	 ActionChargeMethodFull,	 0,	0,	3,	FLASH},
	/* 8*/	{txtCHARGESTORAGE,	 ActionChargeMethodStorage,	 0,	0,	3,	FLASH},
	/* 9*/	{txtDISCHARGE,	 ActionChargeMethodDischarge,	 0,	0,	3,	FLASH},
	/* 10*/	{txtLIPO,	 actSelTypeLiPo,	 0,	0,	4,	FLASH},
	/* 11*/	{txtLIFEPO,	 actSelTypeLiFe,	 0,	0,	4,	FLASH},
	/* 12*/	{txtLIION,	 actSelTypeLiIon,	 0,	0,	4,	FLASH},
	/* 13*/	{txtCELLCOUNT,	 0,	 &parCellCount,	0,	5,	FLASH},
	/* 14*/	{txtBATTSETTINGS,	 0,	 0,	20,	5,	FLASH},
	/* 15*/	{txtCHARGELIMITS,	 0,	 0,	33,	5,	FLASH},
	/* 16*/	{txtPWM,	 0,	 0,	36,	5,	FLASH},
	/* 17*/	{txtCALIBRATIONH,	 actCalH,	 0,	0,	5,	FLASH},
	/* 18*/	{txtCALIBRATIONL,	 actCalL,	 0,	0,	5,	FLASH},
	/* 19*/	{txtBACK,	 0,	 0,	5,	5,	FLASH},
	/* 20*/	{txtLIPO,	 0,	 0,	25,	14,	FLASH},
	/* 21*/	{txtLIFEPO,	 0,	 0,	29,	14,	FLASH},
	/* 22*/	{txtLIION,	 0,	 0,	31,	14,	FLASH},
	/* 23*/	{txtBALACTIVVOLTAGE,	 0,	 &parBalActVolt,	0,	14,	FLASH},
	/* 24*/	{txtBACK,	 0,	 0,	14,	14,	FLASH},
	/* 25*/	{txtCHARGEVOLTAGE,	 0,	 &parLiPoChVolt,	0,	20,	FLASH},
	/* 26*/	{txtSTORAGEVOLTAGE,	 0,	 &parLiPoStVolt,	0,	20,	FLASH},
	/* 27*/	{txtDISCHARGEVOLTAGE,	 0,	 &parLiPoDisVolt,	0,	20,	FLASH},
	/* 28*/	{txtBACK,	 0,	 0,	20,	20,	FLASH},
	/* 29*/	{txtLIFEPOVOLT,	 0,	 &parLiFeVolt,	0,	21,	FLASH},
	/* 30*/	{txtBACK,	 0,	 0,	21,	21,	FLASH},
	/* 31*/	{txtLIIONVOLTAGE,	 0,	 &parLiIonVolt,	0,	22,	FLASH},
	/* 32*/	{txtBACK,	 0,	 0,	22,	22,	FLASH},
	/* 33*/	{txtCAPACITY,	 0,	 &parMaxcap,	0,	15,	FLASH},
	/* 34*/	{txtTIME,	 0,	 &parMaxtime,	0,	15,	FLASH},
	/* 35*/	{txtBACK,	 0,	 0,	15,	15,	FLASH},
	/* 36*/	{txtFEQUENCY,	 0,	 &parPWMfrequency,	0,	16,	FLASH},
	/* 37*/	{txtREFRESHPERIOD,	 0,	 &parRefreshPeriod,	0,	16,	FLASH},
	/* 38*/	{txtBACK,	 0,	 0,	16,	16,	FLASH},
};

//******** END OF AUTO-GENERATED CODE DO NOT EDIT!!! *********







void UpdateCommandsFromParam(void)
{
	OS_MutexGet( OSMTXCommand );

	g_tCommand.sCurrentSetpoint = parCurrent.sValue;
	g_tCommand.usMinBalanceVolt_mV = parBalActVolt.sValue;
	g_tCommand.unQ_max_mAs = parMaxcap.sValue;
	g_tCommand.usT_max_s = parMaxtime.sValue;
	g_tCommand.basefrequency = parPWMfrequency.sValue;
	g_tCommand.refreshrate = parRefreshPeriod.sValue;
	g_tCommand.ucUserCellCount = parCellCount.sValue;
	g_tCommand.usVoltageSetpoint_mV = parVoltage.sValue;

	OS_MutexRelease( OSMTXCommand );

}

// ****** Action functions *********

void ActionChargeMethodFull(void)
{
	switch(g_eBattType)
	{
		case eBattTypeLiPo:
			OS_MutexGet( OSMTXCommand );
			g_tCommand.usVoltageSetpoint_mV = parLiPoChVolt.sValue;
			OS_MutexRelease( OSMTXCommand );
			break;
		case eBattTypeLiFe:
			break;
		case eBattTypeLiIon:
			break;
		default:
			break;
	}
	g_Tansfer_Action = eActModeAuto; // trigger

	menuUp();
}

void ActionChargeMethodStorage(void)
{
	switch(g_eBattType)
	{
		case eBattTypeLiPo:
			OS_MutexGet( OSMTXCommand );
			g_tCommand.usVoltageSetpoint_mV = parLiPoStVolt.sValue;
			OS_MutexRelease( OSMTXCommand );
			break;
		case eBattTypeLiFe:
			break;
		case eBattTypeLiIon:
			break;
		default:
			break;
	}
	g_Tansfer_Action = eActModeAuto; // trigger
	menuUp();
}
void ActionChargeMethodDischarge(void)
{
	switch(g_eBattType)
	{
		case eBattTypeLiPo:
			OS_MutexGet( OSMTXCommand );
			g_tCommand.usVoltageSetpoint_mV = parLiPoDisVolt.sValue;
			OS_MutexRelease( OSMTXCommand );
			break;
		case eBattTypeLiFe:
			break;
		case eBattTypeLiIon:
			break;
		default:
			break;
	}
	g_Tansfer_Action = eActModeAuto; // trigger
	menuUp();
}
void actSelTypeLiPo(void)
{
	g_eBattType = eBattTypeLiPo;
	menuUp();
}
void actSelTypeLiFe(void)
{
	g_eBattType = eBattTypeLiFe;
	menuUp();
}
void actSelTypeLiIon(void)
{
	g_eBattType = eBattTypeLiIon;
	menuUp();
}

void actCalH(void)
{
	g_Tansfer_Action = eActCalHigh;

}

void actCalL(void)
{
	g_Tansfer_Action = eActCalLow;
}

void leavemenu(void)
{
	g_bMenuActive = 0;
	g_Trig_SavePars = 1;
	UpdateCommandsFromParam();
	g_Tansfer_Action = eActModeAuto; // fixme merken ???? / Funktion!!

	// Ladestrom editierbar machen: // fixme
	//send parameter values and limits to UI
	HandOverValueToUI(	parCurrent.sValue,
						parCurrent.sUpperLimit,
						parCurrent.sLowerLimit,
						parCurrent.sStepSize
						);
}

void entermenu(void)
{
	g_Tansfer_Action = eActModeStop;
}

void menuUp(void)
{
	MyMenue.gucSelectedItem = m_items[MyMenue.gucSelectedItem].ucParent;
}

// ************* LCD interface

void menu_draw_header(char *menu_header)
{
	//delete old menu header
	lcd_draw_filled_box( BLACK, 0, 0, 320, 40);
	//draw new menu header
			//injektortester
			lcd_print(WHITE, BLACK, 2, 0, 0, menu_header);
		}

void menu_draw_unselected_items(char *item_name, uint16_t par_exists,
		int16_t parameter, uint8_t type, uint8_t lcd_pos)
{
	//delet old value
	lcd_draw_filled_box( BLACK, 20, 40+lcd_pos*20, 300, 20);
	//draw menu item
			lcd_print(GREEN, BLACK, 1, 20, 40+lcd_pos*20,item_name);
			if (par_exists)
			{
				//delete old val

			//write new val
			lcd_print(WHITE, BLACK, 1, 200, 40+lcd_pos*20, "%i",parameter);
		}
	}

extern void menu_draw_selected_item(char *item_name, uint16_t par_exists,
		int16_t parameter, uint8_t type, uint8_t lcd_pos)
{
	//delet old value
	lcd_draw_filled_box( BLACK, 200, 40+lcd_pos*20, 100, 20);
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

void menu_draw_selected_parameter(char *item_name, int16_t parameter,
		uint8_t type, uint8_t lcd_pos)
{
	//delet old value
	lcd_draw_filled_box( BLACK, 20, 40+lcd_pos*20, 300, 20);
	//draw menu item
			lcd_print(RED, BLACK, 1, 20, 40+lcd_pos*20,item_name);
			lcd_print(RED, BLACK, 1, 280, 40+lcd_pos*20, "edit");
			//write new val
			lcd_print(RED, BLACK, 1, 200, 40+lcd_pos*20, "%i",parameter);
		}

void menu_del_menuitems(void)//menu plane is changed
{
	//delete menu items
	lcd_draw_filled_box( BLACK, 20, 40, 300, 200);
}

void menu_draw_groupposition(uint8_t itemnr, uint8_t groupitems)
{
	lcd_print( WHITE, BLACK, 1, 280, 10, "%iv%i",itemnr,groupitems);
}

