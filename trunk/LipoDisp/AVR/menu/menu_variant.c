#include "../OS/FabOS.h"
#include "menu.h"
#include "menu_variant.h" 
#include "string.h"
#include "../comm/serial.h"
#include "../comm/eeprom.h"


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
extern ChargerMode_t g_Tansfer_Action;
extern uint8_t gCommandsKnown; // command.c

//prototypes

void menuUp(void);
extern void GetSubMenuCount(uint8_t *Size, uint8_t *StartIndex);
void callback_menu_HandOverValueToUI(uint16_t value, uint16_t upper, uint16_t lower, uint16_t stepsize);

void savePersistent(void);
void loadPersistent(void);


//******** INSERT INTO C FILE *********
// Text definitions
	MENUE_TEXT_VARDEF
// Parameter definitions
	MENUE_PARAM_VARDEF
// Menue definitions
	MENUE_MENUE_VARDEF

//******** INSERT INTO C FILE *********





void UpdateCommandsFromParam(void)
{
	OS_MutexGet( OSMTXCommand );

	g_tCommand.sCurrentSetpoint = myPar.parCurrent.sValue;
	g_tCommand.usVoltageSetpoint_mV = myPar.parChVoltSET.sValue;	
	g_tCommand.usMinBalanceVolt_mV = myPar.parBalActVoltSET.sValue;
	g_tCommand.unQ_max_mAh = myPar.parMaxcap.sValue;
	g_tCommand.usT_max_s = myPar.parMaxtime.sValue;
	g_tCommand.refreshrate = myPar.parRefreshPeriod.sValue;
	g_tCommand.ucUserCellCount = 0; // does not exist in menue fixme
	g_tCommand.SuppMin_mV = myPar.parMinSupplyVolt.sValue;

	OS_MutexRelease( OSMTXCommand );

}

// ****** Action functions *********

void ActionChargeModeLiPo(void)
{
	myPar.parChVoltSET.sValue = myPar.parChVoltLiPo.sValue;
	myPar.parBalActVoltSET.sValue = myPar.parBalActVoltLiPo.sValue;

	menuUp();
	UpdateCommandsFromParam();
		
}

void ActionChargeModeLiIo(void)
{
	myPar.parChVoltSET.sValue = myPar.parVoltLiIo.sValue;
	myPar.parBalActVoltSET.sValue = myPar.parBalActVoltLiIo.sValue;

	menuUp();
	UpdateCommandsFromParam();
}

void ActionChargeModeLiFe(void)
{
	myPar.parChVoltSET.sValue = myPar.parVoltLiFe.sValue;
	myPar.parBalActVoltSET.sValue = myPar.parBalActVoltLiFe.sValue;

	menuUp();
	UpdateCommandsFromParam();
}

void ActionChargeModeStorage(void)
{
	myPar.parChVoltSET.sValue = myPar.parChtVoltStorage.sValue;
	myPar.parBalActVoltSET.sValue = myPar.parBalActVoltStor.sValue;

	menuUp();
	UpdateCommandsFromParam();
}

void ActionChargeModeDischarge(void)
{
	// fixme notimplemented
	menuUp();
}

void ActionShowLast (void)
{
	// fixme todo	
}

void actCalH(void)
{
	g_Tansfer_Action = eActCalHigh;

}

void actCalL(void)
{
	g_Tansfer_Action = eActCalLow;
}


void menuUp(void)
{
	uint8_t SubMenuGroupSize=0, StartIndex=0;
	MyMenue.gucSelectedItem = m_items[MyMenue.gucSelectedItem].ucParent;
	//get array start index of the submenu items
	GetSubMenuCount(&SubMenuGroupSize, &StartIndex);
	//send values to UI
	callback_menu_HandOverValueToUI(SubMenuGroupSize-(MyMenue.gucSelectedItem-StartIndex), SubMenuGroupSize, 1, 1);
}

void menuUpnSave(void)
{
	savePersistent();
	menuUp();
}


void savePersistent(void)
{	
	int16_t buf[8];
	
	buf[0] = myPar.parChVoltLiPo.sValue	;
	buf[1] = myPar.parBalActVoltLiPo.sValue	;

	buf[2] = myPar.parVoltLiIo.sValue;
	buf[3] = myPar.parBalActVoltLiIo.sValue;

	buf[4] = myPar.parVoltLiFe.sValue;
	buf[5] = myPar.parBalActVoltLiFe.sValue;

	buf[6] = myPar.parChtVoltStorage.sValue;
	buf[7] = myPar.parBalActVoltStor.sValue;
	
	eeprom_WriteBlockWCRC((uint8_t*)&buf, EEPROM_PAR_START, sizeof(buf));
	
}

void loadPersistent(void)
{
	int16_t buf[8];
	
	if(eeprom_ReadBlockWCRC( (uint8_t*)&buf,
	(void*)( EEPROM_PAR_START ), sizeof(buf) ))
	{
		// crc fail
	}
	else
	{
		myPar.parChVoltLiPo.sValue = buf[0];
		myPar.parBalActVoltLiPo.sValue = buf[1];

		myPar.parVoltLiIo.sValue = buf[2];
		myPar.parBalActVoltLiIo.sValue = buf[3];

		myPar.parVoltLiFe.sValue = buf[4];
		myPar.parBalActVoltLiFe.sValue = buf[5];

		myPar.parChtVoltStorage.sValue = buf[6];
		myPar.parBalActVoltStor.sValue = buf[7];
	}	
	
}


// ************* LCD interface

void menu_draw_header(char *menu_header)
{
	static uint8_t lf=0;
	if (lf == 0)
	{
		loadPersistent();
		lf = 1;
	}
	
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
			lcd_print(WHITE, BLACK, 1, 200, 40+lcd_pos*20, "%d",parameter);
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
			lcd_print(WHITE, BLACK, 1, 200, 40+lcd_pos*20, "%d",parameter);
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
			lcd_print(RED, BLACK, 1, 200, 40+lcd_pos*20, "%d",parameter);
		}

void menu_del_menuitems(void)//menu plane is changed
{
	//delete menu items
	lcd_draw_filled_box( BLACK, 20, 40, 300, 200);
}

void menu_draw_groupposition(uint8_t itemnr, uint8_t groupitems)
{
	lcd_print( WHITE, BLACK, 1, 280, 10, "%dv%d",itemnr,groupitems);
}

void callback_menu_ParChanged(void)
{
	UpdateCommandsFromParam();
}