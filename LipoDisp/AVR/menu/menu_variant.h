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
//********          Generated with Treebuilder.exe       *********

 #define	MENUESIZE	56	// number of menu itmes (array size)
 #define	MAX_ITEM_NAME_CHARLENGTH	20	// max name length
 #define	MENUGENUID	9889	// Generation UID
// Enum definitions
typedef enum
{
	Cells,
	mA,
	mAh,
	min,
	ms,
	mV,
} eParameterType_t;

// Parameter externals
typedef struct myPar_tag
{
      Parameter_t parCurrent;	// stepsize -1 for Ulis dirty hack
      Parameter_t parChVoltSET;	// 
      Parameter_t parBalActVoltSET;	// 
      Parameter_t parMaxcap;	// 
      Parameter_t parMaxtime;	// 
      Parameter_t parChVoltLiPo1;	// 
      Parameter_t parBalActVoltLiPo1;	// 
      Parameter_t parChVoltLiPo2;	// 
      Parameter_t parBalActVoltLiPo2;	// 
      Parameter_t parVoltLiIo;	// 
      Parameter_t parBalActVoltLiIo;	// 
      Parameter_t parVoltLiFe;	// 
      Parameter_t parBalActVoltLiFe;	// 
      Parameter_t parChtVoltStorage1;	// 
      Parameter_t parBalActVoltStor1;	// 
      Parameter_t parChtVoltStorage2;	// 
      Parameter_t parBalActVoltStor2;	// 
      Parameter_t parRefreshPeriod;	// 
      Parameter_t parMinSupplyVolt;	// 
} myPar_t;

extern myPar_t myPar;

 #define	MENUE_PARCOUNT	19	// number of parameters

// Action Prototypes
void ActionChargeModeStorage1 (void);	// 
void ActionChargeModeLiPo1 (void);	// 
void ActionChargeModeStorage2 (void);	// 
void ActionChargeModeLiFe (void);	// 
void ActionChargeModeLiIo (void);	// 
void ActionChargeModeLiPo2 (void);	// 
void savePreset (void);	// 
void savePreset (void);	// 
void savePreset (void);	// 
void savePreset (void);	// 
void savePreset (void);	// 
void savePreset (void);	// 
void actCalH (void);	// 
void actCalL (void);	// 
void ActionShowLast (void);	// 

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
char	txtSTORAGEHIGH[] 	PROGMEM="Storage high"; \
char	txtCHARGELIFE[] 	PROGMEM="Charge LiFe"; \
char	txtCHARGELIIO[] 	PROGMEM="Charge LiIo"; \
char	txtCHARGELIPOLOW[] 	PROGMEM="Charge LiPo low"; \
char	txtBACK[] 	PROGMEM="(back)"; \
char	txtPRESETSETUP[] 	PROGMEM="Preset Setup"; \
char	txtPWM[] 	PROGMEM="Pwm"; \
char	txtMINSUPPLY[] 	PROGMEM="Min Supply"; \
char	txtCALIBRATIONH[] 	PROGMEM="Calibration H"; \
char	txtCALIBRATIONL[] 	PROGMEM="Calibration L"; \
char	txtLITHIUMPOLYMER[] 	PROGMEM="Lithium Polymer"; \
char	txtLITHIUMPOLYMERLOW[] 	PROGMEM="Lithium Polymer low"; \
char	txtLITHIUMION[] 	PROGMEM="Lithium Ion"; \
char	txtLIFEPO[] 	PROGMEM="LiFePo4"; \
char	txtBACKTOMAIN[] 	PROGMEM="(back to Main)"; \
char	txtLIPOVOLTAGE[] 	PROGMEM="LiPo Voltage"; \
char	txtBACKANDSAVE[] 	PROGMEM="back and save"; \
char	txtLIPOVOLTAGELOW[] 	PROGMEM="LiPo Voltage low"; \
char	txtLIIONVOLTAGE[] 	PROGMEM="LiIon Voltage"; \
char	txtLIFEPOVOLT[] 	PROGMEM="LiFePo4 Volt."; \
char	txtSTORAGEVOLTAGE[] 	PROGMEM="Storage Voltage"; \
char	txtSTORAGEVOLTAGEHIGH[] 	PROGMEM="Storage Voltage high"; \
char	txtREFRESHPERIOD[] 	PROGMEM="Refresh-period"; \

#endif

// Parameter definitions
#ifndef MENUE_PARAM_VARDEF
#define MENUE_PARAM_VARDEF \
myPar_t myPar = { \
/*parCurrent*/ {	0, 100, 10000, -1, mA}, \
/*parChVoltSET*/ {	0, 3300, 4250, 10, mV}, \
/*parBalActVoltSET*/ {	0, 3200, 4300, 100, mV}, \
/*parMaxcap*/ {	0, 250, 30000, 250, mAh}, \
/*parMaxtime*/ {	0, 0, 9000, 20, min}, \
/*parChVoltLiPo1*/ {	0, 3300, 4250, 10, mV}, \
/*parBalActVoltLiPo1*/ {	0, 3200, 4300, 100, mV}, \
/*parChVoltLiPo2*/ {	0, 3300, 4250, 10, mV}, \
/*parBalActVoltLiPo2*/ {	0, 3200, 4300, 100, mV}, \
/*parVoltLiIo*/ {	0, 3300, 4250, 10, mV}, \
/*parBalActVoltLiIo*/ {	0, 3200, 4300, 100, mV}, \
/*parVoltLiFe*/ {	0, 3300, 4250, 10, mV}, \
/*parBalActVoltLiFe*/ {	0, 3200, 4300, 100, mV}, \
/*parChtVoltStorage1*/ {	0, 3300, 4250, 10, mV}, \
/*parBalActVoltStor1*/ {	0, 3200, 4300, 100, mV}, \
/*parChtVoltStorage2*/ {	0, 3300, 4250, 10, mV}, \
/*parBalActVoltStor2*/ {	0, 3200, 4300, 100, mV}, \
/*parRefreshPeriod*/ {	0, 1, 10000, 10, ms}, \
/*parMinSupplyVolt*/ {	0, 8000, 20000, 500, mV}, \
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
	/* 7*/	{txtSETUP,	 0,	 0,	16,	0,	FLASH}, \
	/* 8*/	{txtSHOWLASTVALUES,	 ActionShowLast,	 0,	0,	0,	FLASH}, \
	/* 9*/	{txtSTORAGE,	 ActionChargeModeStorage1,	 0,	0,	1,	FLASH}, \
	/* 10*/	{txtCHARGELIPO,	 ActionChargeModeLiPo1,	 0,	0,	1,	FLASH}, \
	/* 11*/	{txtSTORAGEHIGH,	 ActionChargeModeStorage2,	 0,	0,	1,	FLASH}, \
	/* 12*/	{txtCHARGELIFE,	 ActionChargeModeLiFe,	 0,	0,	1,	FLASH}, \
	/* 13*/	{txtCHARGELIIO,	 ActionChargeModeLiIo,	 0,	0,	1,	FLASH}, \
	/* 14*/	{txtCHARGELIPOLOW,	 ActionChargeModeLiPo2,	 0,	0,	1,	FLASH}, \
	/* 15*/	{txtBACK,	 0,	 0,	1,	1,	FLASH}, \
	/* 16*/	{txtBACK,	 0,	 0,	7,	7,	FLASH}, \
	/* 17*/	{txtPRESETSETUP,	 0,	 0,	22,	7,	FLASH}, \
	/* 18*/	{txtPWM,	 0,	 0,	54,	7,	FLASH}, \
	/* 19*/	{txtMINSUPPLY,	 0,	 &myPar.parMinSupplyVolt,	0,	7,	FLASH}, \
	/* 20*/	{txtCALIBRATIONH,	 actCalH,	 0,	0,	7,	FLASH}, \
	/* 21*/	{txtCALIBRATIONL,	 actCalL,	 0,	0,	7,	FLASH}, \
	/* 22*/	{txtBACK,	 0,	 0,	17,	17,	FLASH}, \
	/* 23*/	{txtLITHIUMPOLYMER,	 0,	 0,	30,	17,	FLASH}, \
	/* 24*/	{txtLITHIUMPOLYMERLOW,	 0,	 0,	34,	17,	FLASH}, \
	/* 25*/	{txtLITHIUMION,	 0,	 0,	38,	17,	FLASH}, \
	/* 26*/	{txtLIFEPO,	 0,	 0,	42,	17,	FLASH}, \
	/* 27*/	{txtSTORAGE,	 0,	 0,	46,	17,	FLASH}, \
	/* 28*/	{txtSTORAGEHIGH,	 0,	 0,	50,	17,	FLASH}, \
	/* 29*/	{txtBACKTOMAIN,	 0,	 0,	1,	17,	FLASH}, \
	/* 30*/	{txtBACK,	 0,	 0,	23,	23,	FLASH}, \
	/* 31*/	{txtLIPOVOLTAGE,	 0,	 &myPar.parChVoltLiPo1,	0,	23,	FLASH}, \
	/* 32*/	{txtBALACTIVVOLTAGE,	 0,	 &myPar.parBalActVoltLiPo1,	0,	23,	FLASH}, \
	/* 33*/	{txtBACKANDSAVE,	 savePreset,	 0,	0,	23,	FLASH}, \
	/* 34*/	{txtBACK,	 0,	 0,	24,	24,	FLASH}, \
	/* 35*/	{txtLIPOVOLTAGELOW,	 0,	 &myPar.parChVoltLiPo2,	0,	24,	FLASH}, \
	/* 36*/	{txtBALACTIVVOLTAGE,	 0,	 &myPar.parBalActVoltLiPo2,	0,	24,	FLASH}, \
	/* 37*/	{txtBACKANDSAVE,	 savePreset,	 0,	0,	24,	FLASH}, \
	/* 38*/	{txtBACK,	 0,	 0,	25,	25,	FLASH}, \
	/* 39*/	{txtLIIONVOLTAGE,	 0,	 &myPar.parVoltLiIo,	0,	25,	FLASH}, \
	/* 40*/	{txtBALACTIVVOLTAGE,	 0,	 &myPar.parBalActVoltLiIo,	0,	25,	FLASH}, \
	/* 41*/	{txtBACKANDSAVE,	 savePreset,	 0,	0,	25,	FLASH}, \
	/* 42*/	{txtBACK,	 0,	 0,	26,	26,	FLASH}, \
	/* 43*/	{txtLIFEPOVOLT,	 0,	 &myPar.parVoltLiFe,	0,	26,	FLASH}, \
	/* 44*/	{txtBALACTIVVOLTAGE,	 0,	 &myPar.parBalActVoltLiFe,	0,	26,	FLASH}, \
	/* 45*/	{txtBACKANDSAVE,	 savePreset,	 0,	0,	26,	FLASH}, \
	/* 46*/	{txtBACK,	 0,	 0,	27,	27,	FLASH}, \
	/* 47*/	{txtSTORAGEVOLTAGE,	 0,	 &myPar.parChtVoltStorage1,	0,	27,	FLASH}, \
	/* 48*/	{txtBALACTIVVOLTAGE,	 0,	 &myPar.parBalActVoltStor1,	0,	27,	FLASH}, \
	/* 49*/	{txtBACKANDSAVE,	 savePreset,	 0,	0,	27,	FLASH}, \
	/* 50*/	{txtBACK,	 0,	 0,	28,	28,	FLASH}, \
	/* 51*/	{txtSTORAGEVOLTAGEHIGH,	 0,	 &myPar.parChtVoltStorage2,	0,	28,	FLASH}, \
	/* 52*/	{txtBALACTIVVOLTAGE,	 0,	 &myPar.parBalActVoltStor2,	0,	28,	FLASH}, \
	/* 53*/	{txtBACKANDSAVE,	 savePreset,	 0,	0,	28,	FLASH}, \
	/* 54*/	{txtBACK,	 0,	 0,	18,	18,	FLASH}, \
	/* 55*/	{txtREFRESHPERIOD,	 0,	 &myPar.parRefreshPeriod,	0,	18,	FLASH}, \
};
#endif

//******** END OF AUTO-GENERATED HEADER DO NOT EDIT!!! *********

