#include "menu_variant.h" 
#include "string.h"
#include <avr/eeprom.h>


//extern vars / functions
extern uint8_t gucSelectedItem;
extern void HandOverValueToUI(uint16_t value, uint16_t upper, uint16_t lower, uint8_t type);
extern uint16_t GetvalueFromUI(void);
extern void GetSubMenuCount(uint8_t *Size, uint8_t *StartIndex);
extern void start_injection(uint8_t program);	//The injection execute function
//extern bool MAN_TRIG_ACTIVE;	examples
uint16_t guintUBoost=0;			//800	=80,0V
uint16_t guintIBoost=0;			//35	=3,5 A
uint16_t guintTBoost=0;			//5000	=5000us
uint16_t guintFBoost=0;			//20	=20us
uint16_t guintPBoost=0;			//20	=20us		
uint16_t guintIHold =0;			//15	=1,5A
uint16_t guintTHold =0;			//10000	=10000us
uint16_t guintFHold =0;			//20	=20us
uint16_t guintInjCNT = 0;
uint16_t guintPauseTime=0;
uint8_t	 gcProfileNr = 0;		//1		=Profile1 MAN SOGAV 250 (profile 1-6)
uint8_t gucTriggerSource=1;		//TriggerSource 1= Manual, 2=External
uint8_t gucJumpTarget=0;		//
char *ProfileName;
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
	/* 3*/	{txtSETUP,	 0,	 0,	14,	0,	FLASH},
	/* 4*/	{txtBACK,	 leavenmenu,	 0,	0,	0,	FLASH},
	/* 5*/	{txtMODE,	 0,	 0,	8,	1,	FLASH},
	/* 6*/	{txtCHARGEMETHOD,	 0,	 0,	10,	1,	FLASH},
	/* 7*/	{txtCHARGELIMIT,	 0,	 0,	12,	1,	FLASH},
	/* 8*/	{txtAUTO,	 ActionChargeModeAuto,	 0,	0,	5,	FLASH},
	/* 9*/	{txtMANUAL,	 ActionChargeModeMaual,	 0,	0,	5,	FLASH},
	/* 10*/	{txtFULL,	 ActionChargeMethodFull,	 0,	0,	6,	FLASH},
	/* 11*/	{txtSTORAGE,	 ActionChargeMethodStorage,	 0,	0,	6,	FLASH},
	/* 12*/	{txtMANUAL,	 ActionChargeMethodManual,	 0,	0,	6,	FLASH},
	/* 13*/	{txtBACK,	 0,	 0,	10,	6,	FLASH},
	/* 12*/	{txtCAPACITY,	 0,	 &maxcap,	0,	7,	FLASH},
	/* 13*/	{txtTIME,	 0,	 &maxtime,	0,	7,	FLASH},
	/* 16*/	{txtBACK,	 0,	 0,	12,	7,	FLASH},
	/* 14*/	{txtBALANCER,	 0,	 0,	16,	3,	FLASH},
	/* 15*/	{txtPWM,	 0,	 0,	17,	3,	FLASH},
	/* 16*/	{txtACTIVATIONVOLTAGE,	 0,	 &balactvolt,	0,	14,	FLASH},
	/* 20*/	{txtBACK,	 0,	 0,	16,	14,	FLASH},
	/* 17*/	{txtFEQUENCY,	 0,	 &PWMfrequency,	0,	15,	FLASH},
	/* 18*/	{txtREFRESHPERIOD,	 0,	 &RefreshPeriod,	0,	15,	FLASH},
	/* 23*/	{txtBACK,	 0,	 0,	17,	15,	FLASH},
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




