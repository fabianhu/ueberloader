/* definitions for menu */

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




//******** START OF AUTO-GENERATED HEADER DO NOT EDIT!!! *********

 #define	MENUESIZE	45	// number of menu itmes (array size)
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
} eParameterType_t;

// Parameter externals
typedef struct myPar_tag
{
      Parameter_t parCurrent;
      Parameter_t parChVoltSET;
      Parameter_t parBalActVoltSET;
      Parameter_t parMaxcap;
      Parameter_t parMaxtime;
      Parameter_t parChVoltLiPo;
      Parameter_t parBalActVoltLiPo;
      Parameter_t parVoltLiIo;
      Parameter_t parBalActVoltLiIo;
      Parameter_t parVoltLiFe;
      Parameter_t parBalActVoltLiFe;
      Parameter_t parChtVoltStorage;
      Parameter_t parBalActVoltStor;
      Parameter_t parVoltDisch;
      Parameter_t parPWMfrequency;
      Parameter_t parRefreshPeriod;
      Parameter_t parMinSupplyVolt;
	uint8_t crc8;
} myPar_t;

extern myPar_t myPar;

// Action Prototypes
void ActionChargeModeStorage (void);
void ActionChargeModeLiPo (void);
void ActionChargeModeLiIo (void);
void ActionChargeModeLiFe (void);
void ActionChargeModeDischarge (void);
void actCalH (void);
void actCalL (void);
void ActionShowLast (void);

// Text definitions
#ifndef MENUE_TEXT_VARDEF
#define MENUE_TEXT_VARDEF \
char	txtREADY[] 	PROGMEM="Ready..."; \
char	txtPRESETS[] 	PROGMEM="Presets"; \
char	txtCURRENTSETPOINT[] 	PROGMEM="Current Setpoint"; \
char	txtCELLVOLTAGESET[] 	PROGMEM="Cell Voltage SET"; \
char	txtBALACTIVVOLTAGE[] 	PROGMEM="Bal. Activ. Voltage"; \
char	txtMAXCAPACITY[] 	PROGMEM="Max Capacity"; \
char	txtMAXTIME[] 	PROGMEM="Max Time"; \
char	txtSETUP[] 	PROGMEM="Setup"; \
char	txtSHOWLASTVALUES[] 	PROGMEM="Show Last Values"; \
char	txtSTORAGE[] 	PROGMEM="Storage"; \
char	txtCHARGELIPO[] 	PROGMEM="Charge LiPo"; \
char	txtCHARGELIIO[] 	PROGMEM="Charge LiIo"; \
char	txtCHARGELIFE[] 	PROGMEM="Charge LiFe"; \
char	txtDISCHARGE[] 	PROGMEM="Discharge"; \
char	txtBACK[] 	PROGMEM="(back)"; \
char	txtPRESETSETUP[] 	PROGMEM="Preset Setup"; \
char	txtPWM[] 	PROGMEM="Pwm"; \
char	txtMINSUPPLY[] 	PROGMEM="Min Supply"; \
char	txtCALIBRATIONH[] 	PROGMEM="Calibration H"; \
char	txtCALIBRATIONL[] 	PROGMEM="Calibration L"; \
char	txtLITHIUMPOLYMER[] 	PROGMEM="Lithium Polymer"; \
char	txtLITHIUMION[] 	PROGMEM="Lithium Ion"; \
char	txtLIFEPO[] 	PROGMEM="LiFePo4"; \
char	txtBACKTOMAIN[] 	PROGMEM="(back to Main)"; \
char	txtLIPOVOLTAGE[] 	PROGMEM="LiPo Voltage"; \
char	txtLIIONVOLTAGE[] 	PROGMEM="LiIon Voltage"; \
char	txtLIFEPOVOLT[] 	PROGMEM="LiFePo4 Volt."; \
char	txtSTORAGEVOLTAGE[] 	PROGMEM="Storage Voltage"; \
char	txtDISCHARGEVOLTAGE[] 	PROGMEM="Discharge Voltage"; \
char	txtFEQUENCY[] 	PROGMEM="Fequency"; \
char	txtREFRESHPERIOD[] 	PROGMEM="Refresh-period"; \

#endif

// Parameter definitions
#ifndef MENUE_PARAM_VARDEF
#define MENUE_PARAM_VARDEF \
myPar_t myPar = { \
/*parCurrent*/ {	0, 100, 10000, -1, mA}, \
/*parChVoltSET*/ {	0, 3000, 4200, 10, mV}, \
/*parBalActVoltSET*/ {	0, 3000, 4200, 100, mV}, \
/*parMaxcap*/ {	0, 0, 10000, 100, mAh}, \
/*parMaxtime*/ {	0, 1, 6000, 1, min}, \
/*parChVoltLiPo*/ {	0, 3000, 4200, 10, mV}, \
/*parBalActVoltLiPo*/ {	0, 3000, 4200, 100, mV}, \
/*parVoltLiIo*/ {	0, 3500, 4200, 10, mV}, \
/*parBalActVoltLiIo*/ {	0, 3000, 4200, 100, mV}, \
/*parVoltLiFe*/ {	0, 2000, 3700, 10, mV}, \
/*parBalActVoltLiFe*/ {	0, 3000, 4200, 100, mV}, \
/*parChtVoltStorage*/ {	0, 2000, 4100, 10, mV}, \
/*parBalActVoltStor*/ {	0, 3000, 4200, 100, mV}, \
/*parVoltDisch*/ {	0, 3000, 4200, 10, mV}, \
/*parPWMfrequency*/ {	0, 10, 100, 10, kHz}, \
/*parRefreshPeriod*/ {	0, 1, 10000, 10, ms}, \
/*parMinSupplyVolt*/ {	0, 8000, 20000, 500, mV}, \
/*CRC*/	0 \
};
#endif

			//Name	Act	Par	Jmp	Parent	Memory
#ifndef MENUE_MENUE_VARDEF
#define MENUE_MENUE_VARDEF \
MenuItem_t m_items[MENUESIZE] = { \
	/* 0*/	{txtREADY,	 0,	 0,	1,	0,	FLASH}, \
	/* 1*/	{txtPRESETS,	 0,	 0,	9,	0,	FLASH}, \
	/* 2*/	{txtCURRENTSETPOINT,	 0,	 &myPar.parCurrent,	0,	0,	FLASH}, \
	/* 3*/	{txtCELLVOLTAGESET,	 0,	 &myPar.parChVoltSET,	0,	0,	FLASH}, \
	/* 4*/	{txtBALACTIVVOLTAGE,	 0,	 &myPar.parBalActVoltSET,	0,	0,	FLASH}, \
	/* 5*/	{txtMAXCAPACITY,	 0,	 &myPar.parMaxcap,	0,	0,	FLASH}, \
	/* 6*/	{txtMAXTIME,	 0,	 &myPar.parMaxtime,	0,	0,	FLASH}, \
	/* 7*/	{txtSETUP,	 0,	 0,	15,	0,	FLASH}, \
	/* 8*/	{txtSHOWLASTVALUES,	 ActionShowLast,	 0,	0,	0,	FLASH}, \
	/* 9*/	{txtSTORAGE,	 ActionChargeModeStorage,	 0,	0,	1,	FLASH}, \
	/* 10*/	{txtCHARGELIPO,	 ActionChargeModeLiPo,	 0,	0,	1,	FLASH}, \
	/* 11*/	{txtCHARGELIIO,	 ActionChargeModeLiIo,	 0,	0,	1,	FLASH}, \
	/* 12*/	{txtCHARGELIFE,	 ActionChargeModeLiFe,	 0,	0,	1,	FLASH}, \
	/* 13*/	{txtDISCHARGE,	 ActionChargeModeDischarge,	 0,	0,	1,	FLASH}, \
	/* 14*/	{txtBACK,	 0,	 0,	1,	1,	FLASH}, \
	/* 15*/	{txtBACK,	 0,	 0,	7,	7,	FLASH}, \
	/* 16*/	{txtPRESETSETUP,	 0,	 0,	21,	7,	FLASH}, \
	/* 17*/	{txtPWM,	 0,	 0,	42,	7,	FLASH}, \
	/* 18*/	{txtMINSUPPLY,	 0,	 &myPar.parMinSupplyVolt,	0,	7,	FLASH}, \
	/* 19*/	{txtCALIBRATIONH,	 actCalH,	 0,	0,	7,	FLASH}, \
	/* 20*/	{txtCALIBRATIONL,	 actCalL,	 0,	0,	7,	FLASH}, \
	/* 21*/	{txtBACK,	 0,	 0,	16,	16,	FLASH}, \
	/* 22*/	{txtLITHIUMPOLYMER,	 0,	 0,	28,	16,	FLASH}, \
	/* 23*/	{txtLITHIUMION,	 0,	 0,	31,	16,	FLASH}, \
	/* 24*/	{txtLIFEPO,	 0,	 0,	34,	16,	FLASH}, \
	/* 25*/	{txtSTORAGE,	 0,	 0,	37,	16,	FLASH}, \
	/* 26*/	{txtDISCHARGE,	 0,	 0,	40,	16,	FLASH}, \
	/* 27*/	{txtBACKTOMAIN,	 0,	 0,	1,	16,	FLASH}, \
	/* 28*/	{txtBACK,	 0,	 0,	22,	22,	FLASH}, \
	/* 29*/	{txtLIPOVOLTAGE,	 0,	 &myPar.parChVoltLiPo,	0,	22,	FLASH}, \
	/* 30*/	{txtBALACTIVVOLTAGE,	 0,	 &myPar.parBalActVoltLiPo,	0,	22,	FLASH}, \
	/* 31*/	{txtBACK,	 0,	 0,	23,	23,	FLASH}, \
	/* 32*/	{txtLIIONVOLTAGE,	 0,	 &myPar.parVoltLiIo,	0,	23,	FLASH}, \
	/* 33*/	{txtBALACTIVVOLTAGE,	 0,	 &myPar.parBalActVoltLiIo,	0,	23,	FLASH}, \
	/* 34*/	{txtBACK,	 0,	 0,	24,	24,	FLASH}, \
	/* 35*/	{txtLIFEPOVOLT,	 0,	 &myPar.parVoltLiFe,	0,	24,	FLASH}, \
	/* 36*/	{txtBALACTIVVOLTAGE,	 0,	 &myPar.parBalActVoltLiFe,	0,	24,	FLASH}, \
	/* 37*/	{txtBACK,	 0,	 0,	25,	25,	FLASH}, \
	/* 38*/	{txtSTORAGEVOLTAGE,	 0,	 &myPar.parChtVoltStorage,	0,	25,	FLASH}, \
	/* 39*/	{txtBALACTIVVOLTAGE,	 0,	 &myPar.parBalActVoltStor,	0,	25,	FLASH}, \
	/* 40*/	{txtBACK,	 0,	 0,	26,	26,	FLASH}, \
	/* 41*/	{txtDISCHARGEVOLTAGE,	 0,	 &myPar.parVoltDisch,	0,	26,	FLASH}, \
	/* 42*/	{txtBACK,	 0,	 0,	17,	17,	FLASH}, \
	/* 43*/	{txtFEQUENCY,	 0,	 &myPar.parPWMfrequency,	0,	17,	FLASH}, \
	/* 44*/	{txtREFRESHPERIOD,	 0,	 &myPar.parRefreshPeriod,	0,	17,	FLASH}, \
};
#endif

//******** END OF AUTO-GENERATED HEADER DO NOT EDIT!!! *********







