#include "menu_variant.h" 
#include "menu.h"
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
					
//menue strings
//FLASH
char txtmainmenu[] 			PROGMEM="Main Menu";
char txtInject[] 			PROGMEM="Injection";
char txtConfig[] 			PROGMEM="Configuration";
char txtPowerOff[] 			PROGMEM="Power Off";
char txtYes[] 				PROGMEM="Yes";
char txtNo[] 				PROGMEM="Cancel";
char txtSingle[] 			PROGMEM="Single";
char txtSequence[] 			PROGMEM="Sequence";
char txtManTrigger[] 		PROGMEM="Man Trigger";
char txtExtTrigger[] 		PROGMEM="Ext Trigger";
char txtStart[]				PROGMEM="Start";
char txtChaProfile[]		PROGMEM="Current Profiles";
char txtback[] 				PROGMEM="(back)";
char txtNrInjections[] 		PROGMEM="Inj-Count";
char txtPauseTime[] 		PROGMEM="Pausetime";
char txtMANInj_01[] 		PROGMEM="SOGAV 250";
char txtMANInj_02[] 		PROGMEM="DF Pilot";
char txtMANInj_03[] 		PROGMEM="CR";
char txtUBoost[] 			PROGMEM="Boost U";
char txtIBoost[] 			PROGMEM="Boost I";
char txtTBoost[] 			PROGMEM="Boost Dur";
char txtFBoost[] 			PROGMEM="Boost Trig";
char txtPBoost[] 			PROGMEM="Boost CUT";
char txtIHold[] 			PROGMEM="Hold  I";
char txtTHold[] 			PROGMEM="Hold  Dur";
char txtFHold[] 			PROGMEM="Hold  Trig";
char txtselect[] 			PROGMEM="(Select)";
char txtChangeName[]		PROGMEM="Change Name";
char txtTriggerMode[]		PROGMEM="Trigger Mode";
//SRAM
//user defined names
uint8_t eeNames[3][MAX_SRAM_ITEM_NAME_LENGHT+1];
char ud_name[][MAX_SRAM_ITEM_NAME_LENGHT+1]	={//Menu item name
													{"CURRENT-TEST"},
													{"USER DEFINED 02"},
													{"USER DEFINED 03"},
											};
//function Prototypes


//Parameter types			examples
#define BOOSTVOLTAGE	1	//240-1100 	= 24,0V	-110,0	V
#define BOOSTCURRENT	2	//0-300		= 0,0 	-30,0	A
#define BOOSTTIME		3	//0-65500	= 0		-65500	us
#define BOOSTFREQUENCY	4	//20-500	= 20	-500	us
#define HOLDCURRENT		5	//0-300		= 0,0 	-30,0	A
#define HOLDTIME		6	//0-60000	= 0		-60000	us
#define HOLDFREQUENCY	7	//20-500	= 20	-500	us
#define INTERVALLTIME	8	//0-60000	= 0		-60000  ms

//Variables in EEPROM
uint16_t eeParameter[(NROFPROFILES*8*4)+2] EEMEM;	//variable in eeprom for storing the parameters
uint8_t eeVarsValid EEMEM;							//Marker if a valid dataset is stored in eeprom

//Current Profile Parameter
Parameter_t CPPar[NROFPROFILES*8+2] = 	{//	uBoost			iBoost 				tBoost 					fBoost				PBoost				iHold 				tHold					fHold
		   /*MAN SOGAV 250 			 */	{1100,1100,1100,1},	{158,158,158,2},	{3100,3100,3100,3}, 	{100,100,100,4},	{170,170,170,7},	{23,23,23,5},		{38100,38100,38100,6},	{100,100,100,7},	//(default, min , max, type)
		   /*MAN DF Pilot 			 */ {1000,240,1100,1},	{180,180,180,2},	{1000,1000,1000,3}, 	{50,20,500,4},		{0,0,500,7},		{100,100,100,5},	{16000,16000,16000,6},	{100,100,100,7},	//(default, min , max, type)
		   /*MAN CR 				 */	{1100,1100,1100,1},	{180,180,180,2},	{1000,1000,1000,3}, 	{50,50,50,4},		{10,10,10,7},		{120,120,120,5},	{13000,13000,13000,6},	{50,50,50,7},		//(default, min , max, type)
		   /*USER injector Profile 01*/	{300,240,1100,1},	{7,0,250,2},		{200,0,5000,3}, 		{50,20,500,4},		{0,0,500,7},		{0,0,150,5},		{0,0,60000,6},			{20,20,500,7},		//(default, min , max, type)
		   /*USER injector Profile 02*/	{1000,240,1100,1},	{250,0,250,2},		{5000,0,5000,3}, 		{20,20,500,4},		{20,0,500,7},		{150,0,150,5},		{60000,0,60000,6},		{20,20,500,7},		//(default, min , max, type)
		   /*USER injector Profile 03*/	{1000,240,1100,1},	{0,0,250,2},		{0,0,5000,3}, 			{20,20,500,4},		{0,0,500,7},		{0,0,150,5},		{0,0,60000,6},			{20,20,500,7},		//(default, min , max, type)
		  								{10,1,1000,0},	//Number of Injections
										{1000,100,60000,8},//Time between Injections
									};

MenuItem_t m_items[MENUESIZE] = 	{//	strName			pAction			pParam			ucJumpTrg		ucParent	ucSettings
								/* 0*/{	txtmainmenu, 	0, 				0,				0,				0,			FLASH	},
								//main menu
								/* 1*/{	txtInject,		0, 				0, 				4,				0, 			FLASH	},
								/* 2*/{	txtConfig,		0,				0,				17,				0,			FLASH	},
								/* 3*/{	txtPowerOff,	0,				0,				10,				0,			FLASH	},
								//Injection menu
								/* 4*/{	txtSingle,		0,				0,				7,				1,			FLASH	},
								/* 5*/{	txtSequence,	0,				0,				12,				1,			FLASH	},
								/* 6*/{	txtback,		0,				0,				1,				1,			FLASH	},
								//Single Injection menu
								/* 7*/{	txtStart,		0,				0,				0,				4,			FLASH	},
								/* 8*/{	txtChaProfile,	0,				0,				0,				4,			FLASH	},
								/* 9*/{	txtback,		0,				0,				4,				4,			FLASH	},
								//PowerOff menu
								/*10*/{	txtYes,			0,				0,				0,				3,			FLASH	},
								/*11*/{	txtNo,			0,				0,				3,				3,			FLASH	},
								//Sequence Injection menu
								/*12*/{	txtStart	,	0, 				0,				0,				5,			FLASH	},
								/*13*/{	txtNrInjections,0,				&CPPar[48],		0,				5,			FLASH	},
								/*14*/{	txtPauseTime,	0,				&CPPar[49],		0,				5,			FLASH	},
								/*15*/{	txtChaProfile,	0,				0,				0,				5,			FLASH	},
								/*16*/{	txtback,		0,				0,				5,				5,			FLASH	},
								//Config Menu
								/*17*/{	txtTriggerMode,	0,				0,				90,				2,			FLASH	},
								/*18*/{	txtChaProfile,	0,				0,				20,				2,			FLASH	},
								/*19*/{	txtback,		0,				0,				2,				2,			FLASH	},					
								//Profile Menu menu
								/*20*/{	txtMANInj_01,	0,				0,				27,				18,			FLASH	},
								/*21*/{	txtMANInj_02,	0,				0,				37,				18,			FLASH	},
								/*22*/{	txtMANInj_03,	0,				0,				47,				18,			FLASH	},
								/*23*/{	ud_name[0],		0,				0,				57,				18,			SRAM	},
								/*24*/{	ud_name[1],		0,				0,				68,				18,			SRAM	},
								/*25*/{	ud_name[2],		0,				0,				79,				18,			SRAM	},
								/*26*/{	txtback,		0,				0,				18,				18,			FLASH	},
								//MAN Injector 01 Parameter
								/*27*/{	txtselect,		0,				0,				0,				20,			FLASH	},
								/*28*/{	txtUBoost,		0,				&CPPar[0],		0,				20,			FLASH	},
								/*29*/{	txtIBoost,		0,				&CPPar[1],		0,				20,			FLASH	},
								/*30*/{	txtTBoost,		0,				&CPPar[2],		0,				20,			FLASH	},
								/*31*/{	txtFBoost,		0,				&CPPar[3],		0,				20,			FLASH	},
								/*32*/{	txtPBoost,		0,				&CPPar[4],		0,				20,			FLASH	},
								/*33*/{	txtIHold,		0,				&CPPar[5],		0,				20,			FLASH	},
								/*34*/{	txtTHold,		0,				&CPPar[6],		0,				20,			FLASH	},
								/*35*/{	txtFHold,		0,				&CPPar[7],		0,				20,			FLASH	},
								/*36*/{	txtback,		0,				0,				20,				20,			FLASH	},
								//MAN Injector 02 Parameter
								/*37*/{	txtselect,		0,				0,				0,				21,			FLASH	},
								/*38*/{	txtUBoost,		0,				&CPPar[8],		0,				21,			FLASH	},
								/*39*/{	txtIBoost,		0,				&CPPar[9],		0,				21,			FLASH	},
								/*40*/{	txtTBoost,		0,				&CPPar[10],		0,				21,			FLASH	},
								/*41*/{	txtFBoost,		0,				&CPPar[11],		0,				21,			FLASH	},
								/*42*/{	txtPBoost,		0,				&CPPar[12],		0,				21,			FLASH	},
								/*43*/{	txtIHold,		0,				&CPPar[13],		0,				21,			FLASH	},
								/*44*/{	txtTHold,		0,				&CPPar[14],		0,				21,			FLASH	},
								/*45*/{	txtFHold,		0,				&CPPar[15],		0,				21,			FLASH	},
								/*46*/{	txtback,		0,				0,				21,				21,			FLASH	},
								//MAN Injector 03 Parameter
								/*47*/{	txtselect,		0,				0,				0,				22,			FLASH	},
								/*48*/{	txtUBoost,		0,				&CPPar[16],		0,				22,			FLASH	},
								/*49*/{	txtIBoost,		0,				&CPPar[17],		0,				22,			FLASH	},
								/*50*/{	txtTBoost,		0,				&CPPar[18],		0,				22,			FLASH	},
								/*51*/{	txtFBoost,		0,				&CPPar[19],		0,				22,			FLASH	},
								/*52*/{	txtPBoost,		0,				&CPPar[20],		0,				22,			FLASH	},
								/*53*/{	txtIHold,		0,				&CPPar[21],		0,				22,			FLASH	},
								/*54*/{	txtTHold,		0,				&CPPar[22],		0,				22,			FLASH	},
								/*55*/{	txtFHold,		0,				&CPPar[23],		0,				22,			FLASH	},
								/*56*/{	txtback,		0,				0,				22,				22,			FLASH	},
								//USER Injector 01 Parameter
								/*57*/{	txtselect,		0,				0,				0,				23,			FLASH	},
								/*58*/{	txtChangeName,	0,				0,				0,				23,			FLASH	},
								/*59*/{	txtUBoost,		0,				&CPPar[24],		0,				23,			FLASH	},
								/*60*/{	txtIBoost,		0,				&CPPar[25],		0,				23,			FLASH	},
								/*61*/{	txtTBoost,		0,				&CPPar[26],		0,				23,			FLASH	},
								/*62*/{	txtFBoost,		0,				&CPPar[27],		0,				23,			FLASH	},
								/*63*/{	txtPBoost,		0,				&CPPar[28],		0,				23,			FLASH	},
								/*64*/{	txtIHold,		0,				&CPPar[29],		0,				23,			FLASH	},
								/*65*/{	txtTHold,		0,				&CPPar[30],		0,				23,			FLASH	},
								/*66*/{	txtFHold,		0,				&CPPar[31],		0,				23,			FLASH	},
								/*67*/{	txtback,		0,				0,				23,				23,			FLASH	},
								//USER Injector 02 Parameter
								/*68*/{	txtselect,		0,				0,				0,				24,			FLASH	},
								/*69*/{	txtChangeName,	0,				0,				0,				24,			FLASH	},
								/*70*/{	txtUBoost,		0,				&CPPar[32],		0,				24,			FLASH	},
								/*71*/{	txtIBoost,		0,				&CPPar[33],		0,				24,			FLASH	},
								/*72*/{	txtTBoost,		0,				&CPPar[34],		0,				24,			FLASH	},
								/*73*/{	txtFBoost,		0,				&CPPar[35],		0,				24,			FLASH	},
								/*74*/{	txtPBoost,		0,				&CPPar[36],		0,				24,			FLASH	},
								/*75*/{	txtIHold,		0,				&CPPar[37],		0,				24,			FLASH	},
								/*76*/{	txtTHold,		0,				&CPPar[38],		0,				24,			FLASH	},
								/*77*/{	txtFHold,		0,				&CPPar[39],		0,				24,			FLASH	},
								/*78*/{	txtback,		0,				0,				24,				24,			FLASH	},
								//USER Injector 03 Parameter
								/*79*/{	txtselect,		0,					0,				0,				25,			FLASH	},
								/*80*/{	txtChangeName,	0,					0,				0,				25,			FLASH	},
								/*81*/{	txtUBoost,		0,				&CPPar[40],		0,				25,			FLASH	},
								/*82*/{	txtIBoost,		0,				&CPPar[41],		0,				25,			FLASH	},
								/*83*/{	txtTBoost,		0,				&CPPar[42],		0,				25,			FLASH	},
								/*84*/{	txtFBoost,		0,				&CPPar[43],		0,				25,			FLASH	},
								/*85*/{	txtPBoost,		0,				&CPPar[44],		0,				25,			FLASH	},
								/*86*/{	txtIHold,		0,				&CPPar[45],		0,				25,			FLASH	},
								/*87*/{	txtTHold,		0,				&CPPar[46],		0,				25,			FLASH	},
								/*88*/{	txtFHold,		0,				&CPPar[47],		0,				25,			FLASH	},
								/*89*/{	txtback,		0,				0,				25,				25,			FLASH	},
								//Trigger Menu
								/*90*/{	txtManTrigger,	0,				0,				0,				17,			FLASH	},
								/*91*/{	txtExtTrigger,	0,				0,				0,				17,			FLASH	},
								};

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




