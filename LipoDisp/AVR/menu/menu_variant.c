#include "../OS/FabOS.h"
#include "menu.h"
#include "menu_variant.h" 
#include "string.h"
#include "../comm/serial.h"
#include <avr/eeprom.h>

typedef enum
{
	eBattModeLiPo, eBattModeLiFe, eBattModeLiIon, eBattModeStorage, eBattModeDisch
} eBattMode_t;

// global vars
eBattMode_t g_eBattMode; // am Lader eingestellter Mode

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
char	txtBACK[] 	PROGMEM="(back)";
char	txtCHARGEMODE[] 	PROGMEM="Charge Mode";
char	txtCURRENTSETPOINT[] 	PROGMEM="Current Setpoint";
char	txtMODESETTINGS[] 	PROGMEM="Mode Settings";
char	txtSETUP[] 	PROGMEM="Setup";
char	txtCHARGELIPONOW[] 	PROGMEM="Charge LiPo NOW";
char	txtCHARGELIIONOW[] 	PROGMEM="Charge LiIo NOW";
char	txtCHARGELIFENOW[] 	PROGMEM="Charge LiFe NOW";
char	txtSTORAGENOW[] 	PROGMEM="Storage NOW";
char	txtDISCHARGENOW[] 	PROGMEM="Discharge NOW";
char	txtLITHIUMPOLYMER[] 	PROGMEM="Lithium Polymer";
char	txtLITHIUMION[] 	PROGMEM="Lithium Ion";
char	txtLIFEPO[] 	PROGMEM="LiFePo4";
char	txtSTORAGE[] 	PROGMEM="Storage";
char	txtDISCHARGE[] 	PROGMEM="Discharge";
char	txtBACKTOMAIN[] 	PROGMEM="(back to Main)";
char	txtLIPOVOLTAGE[] 	PROGMEM="LiPo Voltage";
char	txtBALACTIVVOLTAGE[] 	PROGMEM="Bal. Activ. Voltage";
char	txtBALANCER[] 	PROGMEM="Balancer";
char	txtLIIONVOLTAGE[] 	PROGMEM="LiIon Voltage";
char	txtLIFEPOVOLT[] 	PROGMEM="LiFePo4 Volt.";
char	txtSTORAGEVOLTAGE[] 	PROGMEM="Storage Voltage";
char	txtDISCHARGEVOLTAGE[] 	PROGMEM="Discharge Voltage";
char	txtCHARGELIMITS[] 	PROGMEM="Charge Limits";
char	txtPWM[] 	PROGMEM="Pwm";
char	txtMANCELLCOUNT[] 	PROGMEM="Man Cell Count";
char	txtCALIBRATIONH[] 	PROGMEM="Calibration H";
char	txtCALIBRATIONL[] 	PROGMEM="Calibration L";
char	txtCAPACITY[] 	PROGMEM="Capacity";
char	txtTIME[] 	PROGMEM="Time";
char	txtFEQUENCY[] 	PROGMEM="Fequency";
char	txtREFRESHPERIOD[] 	PROGMEM="Refresh-period";

// Parameter definitions
Parameter_t parCurrent = {	0, 100, 10000, 100, mA};
Parameter_t parChVoltLiPo = {	0, 3000, 4200, 10, mV};
Parameter_t parBalActVoltLiPo = {	0, 3000, 4200, 100, mV};
Parameter_t parBalOnLiPo = {	0, 0, 1, 1, onoff};
Parameter_t parVoltLiIo = {	0, 3500, 4100, 10, mV};
Parameter_t parBalActVoltLiIo = {	0, 3000, 4200, 100, mV};
Parameter_t parBalOnLiIo = {	0, 0, 1, 1, onoff};
Parameter_t parVoltLiFe = {	0, 2000, 3700, 10, mV};
Parameter_t parBalActVoltLiFe = {	0, 3000, 4200, 100, mV};
Parameter_t parBalOnLiFe = {	0, 0, 1, 1, onoff};
Parameter_t parChtVoltStorage = {	0, 2000, 4100, 100, mV};
Parameter_t parBalActVoltStor = {	0, 3000, 4200, 100, mV};
Parameter_t parBalOnStor = {	0, 0, 1, 1, onoff};
Parameter_t parVoltDisch = {	0, 3000, 4200, 10, mV};
Parameter_t parBalOnDisch = {	0, 0, 1, 1, onoff};
Parameter_t parMaxcap = {	0, 0, 10000, 100, mAh};
Parameter_t parMaxtime = {	0, 1, 6000, 1, min};
Parameter_t parPWMfrequency = {	0, 10, 100, 10, kHz};
Parameter_t parRefreshPeriod = {	0, 1, 10000, 10, ms};
Parameter_t parCellCount = {	0, 1, 6, 1, Cells};

			//Name	Act	Par	Jmp	Parent	Memory
MenuItem_t m_items[MENUESIZE] = {
	/* 0*/	{txtMAINMENU,	 0,	 0,	1,	0,	FLASH},
	/* 1*/	{txtBACK,	 leavemenu,	 0,	0,	0,	FLASH},
	/* 2*/	{txtCHARGEMODE,	 0,	 0,	6,	0,	FLASH},
	/* 3*/	{txtCURRENTSETPOINT,	 0,	 &parCurrent,	0,	0,	FLASH},
	/* 4*/	{txtMODESETTINGS,	 0,	 0,	11,	0,	FLASH},
	/* 5*/	{txtSETUP,	 0,	 0,	37,	0,	FLASH},
	/* 6*/	{txtCHARGELIPONOW,	 ActionChargeModeLiPo,	 0,	0,	2,	FLASH},
	/* 7*/	{txtCHARGELIIONOW,	 ActionChargeModeLiIo,	 0,	0,	2,	FLASH},
	/* 8*/	{txtCHARGELIFENOW,	 ActionChargeModeLiFe,	 0,	0,	2,	FLASH},
	/* 9*/	{txtSTORAGENOW,	 ActionChargeModeStorage,	 0,	0,	2,	FLASH},
	/* 10*/	{txtDISCHARGENOW,	 ActionChargeModeDischarge,	 0,	0,	2,	FLASH},
	/* 11*/	{txtBACK,	 0,	 0,	4,	4,	FLASH},
	/* 12*/	{txtLITHIUMPOLYMER,	 0,	 0,	18,	4,	FLASH},
	/* 13*/	{txtLITHIUMION,	 0,	 0,	22,	4,	FLASH},
	/* 14*/	{txtLIFEPO,	 0,	 0,	26,	4,	FLASH},
	/* 15*/	{txtSTORAGE,	 0,	 0,	30,	4,	FLASH},
	/* 16*/	{txtDISCHARGE,	 0,	 0,	34,	4,	FLASH},
	/* 17*/	{txtBACKTOMAIN,	 0,	 0,	1,	4,	FLASH},
	/* 18*/	{txtBACK,	 0,	 0,	12,	12,	FLASH},
	/* 19*/	{txtLIPOVOLTAGE,	 0,	 &parChVoltLiPo,	0,	12,	FLASH},
	/* 20*/	{txtBALACTIVVOLTAGE,	 0,	 &parBalActVoltLiPo,	0,	12,	FLASH},
	/* 21*/	{txtBALANCER,	 0,	 &parBalOnLiPo,	0,	12,	FLASH},
	/* 22*/	{txtBACK,	 0,	 0,	13,	13,	FLASH},
	/* 23*/	{txtLIIONVOLTAGE,	 0,	 &parVoltLiIo,	0,	13,	FLASH},
	/* 24*/	{txtBALACTIVVOLTAGE,	 0,	 &parBalActVoltLiIo,	0,	13,	FLASH},
	/* 25*/	{txtBALANCER,	 0,	 &parBalOnLiIo,	0,	13,	FLASH},
	/* 26*/	{txtBACK,	 0,	 0,	14,	14,	FLASH},
	/* 27*/	{txtLIFEPOVOLT,	 0,	 &parVoltLiFe,	0,	14,	FLASH},
	/* 28*/	{txtBALACTIVVOLTAGE,	 0,	 &parBalActVoltLiFe,	0,	14,	FLASH},
	/* 29*/	{txtBALANCER,	 0,	 &parBalOnLiFe,	0,	14,	FLASH},
	/* 30*/	{txtBACK,	 0,	 0,	15,	15,	FLASH},
	/* 31*/	{txtSTORAGEVOLTAGE,	 0,	 &parChtVoltStorage,	0,	15,	FLASH},
	/* 32*/	{txtBALACTIVVOLTAGE,	 0,	 &parBalActVoltStor,	0,	15,	FLASH},
	/* 33*/	{txtBALANCER,	 0,	 &parBalOnStor,	0,	15,	FLASH},
	/* 34*/	{txtBACK,	 0,	 0,	16,	16,	FLASH},
	/* 35*/	{txtDISCHARGEVOLTAGE,	 0,	 &parVoltDisch,	0,	16,	FLASH},
	/* 36*/	{txtBALANCER,	 0,	 &parBalOnDisch,	0,	16,	FLASH},
	/* 37*/	{txtBACK,	 0,	 0,	5,	5,	FLASH},
	/* 38*/	{txtCHARGELIMITS,	 0,	 0,	43,	5,	FLASH},
	/* 39*/	{txtPWM,	 0,	 0,	46,	5,	FLASH},
	/* 40*/	{txtMANCELLCOUNT,	 0,	 &parCellCount,	0,	5,	FLASH},
	/* 41*/	{txtCALIBRATIONH,	 actCalH,	 0,	0,	5,	FLASH},
	/* 42*/	{txtCALIBRATIONL,	 actCalL,	 0,	0,	5,	FLASH},
	/* 43*/	{txtBACK,	 0,	 0,	38,	38,	FLASH},
	/* 44*/	{txtCAPACITY,	 0,	 &parMaxcap,	0,	38,	FLASH},
	/* 45*/	{txtTIME,	 0,	 &parMaxtime,	0,	38,	FLASH},
	/* 46*/	{txtBACK,	 0,	 0,	39,	39,	FLASH},
	/* 47*/	{txtFEQUENCY,	 0,	 &parPWMfrequency,	0,	39,	FLASH},
	/* 48*/	{txtREFRESHPERIOD,	 0,	 &parRefreshPeriod,	0,	39,	FLASH},
};

//******** END OF AUTO-GENERATED CODE DO NOT EDIT!!! *********







void UpdateCommandsFromParam(void)
{
	OS_MutexGet( OSMTXCommand );

	/// UIUIUIUI  hier nach Mode selektieren FIXME


	g_tCommand.sCurrentSetpoint = parCurrent.sValue;
	g_tCommand.usMinBalanceVolt_mV = parBalActVoltLiPo.sValue;
	g_tCommand.unQ_max_mAs = parMaxcap.sValue;
	g_tCommand.usT_max_s = parMaxtime.sValue;
	g_tCommand.basefrequency = parPWMfrequency.sValue;
	g_tCommand.refreshrate = parRefreshPeriod.sValue;
	g_tCommand.ucUserCellCount = parCellCount.sValue;
	g_tCommand.usVoltageSetpoint_mV = parChVoltLiPo.sValue;

	OS_MutexRelease( OSMTXCommand );

}

// ****** Action functions *********

void ActionChargeModeLiPo(void)
{
	g_eBattMode = eBattModeLiPo;

	OS_MutexGet( OSMTXCommand );
	g_tCommand.usVoltageSetpoint_mV = parChVoltLiPo.sValue;
	OS_MutexRelease( OSMTXCommand );

	g_Tansfer_Action = eActModeAuto; // trigger

	menuUp();
	leavemenu();
}

void ActionChargeModeLiIo(void)
{
	menuUp();
}

void ActionChargeModeLiFe(void)
{
	menuUp();
}

void ActionChargeModeStorage(void)
{
	menuUp();
}

void ActionChargeModeDischarge(void)
{
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

