#include "menu_variant.h" 
#include "menu.h"
#include "string.h"


//extern vars / functions
extern uint8_t gucSelectedItem;
extern void HandOverValueToUI(uint16_t value, uint16_t upper, uint16_t lower);
extern uint16_t GetvalueFromUI(void);
extern void GetSubMenuCount(uint8_t *Size, uint8_t *StartIndex);
extern void start_injection(uint8_t program);	//The injection execute function
//extern bool MAN_TRIG_ACTIVE;
uint16_t guintUBoost=0;
uint16_t guintIBoost=30;//3,0 A
uint16_t guintTBoost=50;
uint16_t guintIHold =15;//1,5A
uint16_t guintTHold =50;
uint8_t	 gcProfileNr = 0;
//global vars
					
//menue strings
//FLASH
char txtmainmenu[] 			PROGMEM="Main Menu";
char txtback[] 				PROGMEM="(back)";
char txtbla[] 				PROGMEM="BlaBlubb";



//Current Profile Parameter
Parameter_t CPPar[1] = 	{
		{1000,0,3000}
						};

MenuItem_t m_items[MENUESIZE] = 	{//	strName			pAction			pParam			ucJumpTrg		ucParent	ucSettings
								/* 0*/{	txtmainmenu, 	0, 				0,				0,				0,			FLASH	},
								//main menu
								/* 1*/{	txtbla,			0, 				0, 				0,				0, 			FLASH	},

								/*83*/{	txtback,		0,				0,				0,				0,			FLASH	},
									};

void menu_draw_header(char *menu_header)
{
	//delete old menu header
		//injektortester
		//	lcd_write_string("               ",1,0) ;
		//nokia lcd
		lcd_draw_filled_box(BLACK, 0, 0, 320, 40);
	//draw new menu header
		//injektortester
		//lcd_write_string(menu_header,1,0);
		//nokia lcd
		lcd_print(WHITE, BLACK, 2, 0, 0, menu_header);
}

void menu_draw_unselected_items(char *item_name, uint16_t par_exists, int16_t parameter, char *unit, uint8_t lcd_pos)
{
	//delet old value
		//lcd_write_string("                    ",2+lcd_pos,0);
		lcd_draw_filled_box(BLACK, 20, 40+lcd_pos*20, 300, 20);
	//draw menu item
		//lcd_write_string(item_name,2+lcd_pos,1) ;
		lcd_print(GREEN, BLACK, 1, 20, 40+lcd_pos*20,item_name);
	if (par_exists)  
	{	
		//delete old val
			//lcd_write_string("     ",2+lcd_pos, 9);
		//write new val
			//lcd_write_var("%i1%s", (uint16_t)parameter, unit, 2+lcd_pos, 9); //write new val
			//lcd_write_string(unit,2+lcd_pos, 14);
			lcd_print(WHITE, BLACK, 1, 200, 40+lcd_pos*20, "%i",parameter);
	}
	
	
	
}

extern void menu_draw_selected_item(char *item_name, uint16_t par_exists, int16_t parameter, char *unit, uint8_t lcd_pos)
{
	//delet old value
		//lcd_write_string("                    ",2+lcd_pos,0);
		//lcd_write_string("          ",2+lcd_pos,10);
		lcd_draw_filled_box(BLACK, 200, 40+lcd_pos*20, 100, 20);
	//draw menu item
		//lcd_write_string(item_name,2+lcd_pos,1) ;
		//lcd_write_string("*",2+lcd_pos,19) ;
		lcd_print(WHITE, BLACK, 1, 20, 40+lcd_pos*20,item_name);
		lcd_print(WHITE, BLACK, 1, 300, 40+lcd_pos*20, "<=");

	if (par_exists)  
	{	
		//delete old val
			//lcd_write_string("     ",2+lcd_pos, 9);
		//write new val
			//lcd_write_var("%i1%s", (uint16_t)parameter, unit, 2+lcd_pos, 9); //write new val
			//lcd_write_string(unit,2+lcd_pos, 14);
			lcd_print(WHITE, BLACK, 1, 200, 40+lcd_pos*20, "%i",parameter);
	}
	
}

void menu_draw_selected_parameter(char *item_name, int16_t parameter, char *unit, uint8_t lcd_pos)
{
	//delet old value
		lcd_draw_filled_box(BLACK, 20, 40+lcd_pos*20, 300, 20);
	//draw menu item
		lcd_print(RED, BLACK, 1, 20, 40+lcd_pos*20,item_name);
		lcd_print(RED, BLACK, 1, 280, 40+lcd_pos*20, "edit");
	//delete old val
		//lcd_write_string("*     ",2+lcd_pos, 8);
	//write new val
			//lcd_write_var("%i1%s", (uint16_t)parameter, unit, 2+lcd_pos, 9); //write new val
			//lcd_write_string(unit,2+lcd_pos, 14);
		lcd_print(RED, BLACK, 1, 200, 40+lcd_pos*20, "%i",parameter);
}

void menu_del_menuitems(void)//menu plane is changed
{
	//delete menu items
	lcd_draw_filled_box(BLACK, 20, 40, 300, 200);
	//injektortester
		//lcd_write_string("                    ",2,0) ;
		//lcd_write_string("                    ",3,0) ;
		//lcd_write_string("                    ",4,0) ;
	
	//nokia lcd
	lcd_draw_filled_box(BLACK, 40, 40, 280, 200);
}

void menu_draw_groupposition(uint8_t itemnr, uint8_t groupitems)
{
	lcd_print(WHITE, BLACK, 1, 280, 10, "%iv%i",itemnr,groupitems);
}

/////////////////////////////////////////////////////////////////////////////////////////
// U S E R  F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////

extern uint8_t enter_pushed(void); //returns 1 if rot button is pressed


/*
void fun_selp(void)
{
	uint8_t SubMenuGroupSize=0, StartIndex=0;
	//set index of Profile parent
	gucSelectedItem = m_items[gucSelectedItem].ucParent;
	//get group size & start index
	GetSubMenuCount(&SubMenuGroupSize, &StartIndex);
	//get profile number
	gcProfileNr = gucSelectedItem-StartIndex;
	//save new parameter //offset = 5 (5 parameter for each profile)
	guintUBoost=CPPar[5*gcProfileNr+0].sParam;
	guintIBoost=CPPar[5*gcProfileNr+1].sParam;
	guintTBoost=CPPar[5*gcProfileNr+2].sParam;
	guintIHold =CPPar[5*gcProfileNr+3].sParam;
	guintTHold =CPPar[5*gcProfileNr+4].sParam;
	//jump to Injection Menu 
	gucSelectedItem = INJECTIONMENUID;
	//get array start index of jump target
	GetSubMenuCount(&SubMenuGroupSize, &StartIndex);
	//send values to UI
	HandOverValueToUI(SubMenuGroupSize-(gucSelectedItem-StartIndex), SubMenuGroupSize, 1);
}

void user_info_nameedit(void)
{	
	char strtmp[21];
	lcd_write_string(strcpy_P(strtmp,PSTR("I< OK         Del >I")),2,0) ;
	lcd_write_string(strcpy_P(strtmp,PSTR("<> Change Char Pos  ")),3,0) ;
	lcd_write_string(strcpy_P(strtmp,PSTR("() Select Char      ")),4,0) ;
}

void user_info_charedit(void)
{
	char strtmp[21];
	lcd_write_string(strcpy_P(strtmp,PSTR("<> Change Char      ")),3,0) ;
	lcd_write_string(strcpy_P(strtmp,PSTR("() Enter            ")),4,0) ;
}

void user_info_delete(void)
{
	char strtmp[21];
	lcd_write_string(strcpy_P(strtmp,PSTR("                    ")),2,0);
	lcd_write_string(strcpy_P(strtmp,PSTR("Press Select to     ")),3,0);
	lcd_write_string(strcpy_P(strtmp,PSTR("clear name		   ")),4,0);
}

void user_info_select(void)
{
	char strtmp[21];
	lcd_write_string(strcpy_P(strtmp,PSTR("                    ")),2,0);
	lcd_write_string(strcpy_P(strtmp,PSTR("Press Select to     ")),3,0);
	lcd_write_string(strcpy_P(strtmp,PSTR("accept new name	   ")),4,0);	
}

void fun_cpn(void)
{
	char newname[MAX_SRAM_ITEM_NAME_LENGHT+1]="";
	int8_t active_pos = 1, old_pos=255, edit_active=1;
	char active_char, old_char, *ptrname;
	uint8_t i;
	
	lcd_clear();
	//copy profile name
	strcpy(newname,m_items[m_items[gucSelectedItem].ucParent].strName);
	ptrname=m_items[m_items[gucSelectedItem].ucParent].strName;
	//print user info
	lcd_write_string(newname,1,0) ;
	user_info_nameedit();
	HandOverValueToUI(1, MAX_SRAM_ITEM_NAME_LENGHT+2, 0);
	while(edit_active)
	{
		active_pos=(uint8_t)GetvalueFromUI();
		if(active_pos!=old_pos) //select char to change
		{	
			//store new value
			old_pos=active_pos;
			if(active_pos>MAX_SRAM_ITEM_NAME_LENGHT+1) //delete item selected
			{	
				//print user info
				user_info_delete();
				while(active_pos==old_pos)
				{
					active_pos=(uint8_t)GetvalueFromUI();
					if(enter_pushed()) 
					{
						for(i=0;i<MAX_SRAM_ITEM_NAME_LENGHT;i++)//del name
						{
							newname[i]=' ';
						}
						newname[MAX_SRAM_ITEM_NAME_LENGHT+1]='\0';//terminate string
						HandOverValueToUI(1, MAX_SRAM_ITEM_NAME_LENGHT+2, 0);
					}
				}
				lcd_write_string(newname,1,0) ;
				user_info_nameedit();
			}
			else if(active_pos==0)//ok
			{
				//print user info
				user_info_select();	
				while(active_pos==old_pos && edit_active)
				{
					active_pos=(uint8_t)GetvalueFromUI();
					if(enter_pushed()) 
					{
						//strcpy(m_items[m_items[gucSelectedItem].ucParent].strName,newname);
						for(i=0;i<MAX_SRAM_ITEM_NAME_LENGHT;i++)//del name
						{
							*ptrname++=newname[i];
						}
						*ptrname='\0';//terminate string
						edit_active=0;

					}
				}
				if(edit_active)
				{
					lcd_write_string(newname,1,0) ;
					user_info_nameedit();
				}
			}
			else
			{
				//enable cursor
				lcd_go_to_position(1,active_pos-1);
				lcd_cursor_blink();
			}
		}
		if(enter_pushed()) //edit selected char
		{	
			//print user info
			user_info_charedit();
			//get active char
			active_char = newname[active_pos-1];
			if(active_char==' ') active_char='A';//space then start with A
			old_char = active_char;
			//set limits
			if(31 < active_char && active_char < 58) 	HandOverValueToUI(active_char, 58,  31);	//Chars " "-9
			if(64 < active_char && active_char < 91) 	HandOverValueToUI(active_char, 91,  64);		//Chars Z-A 
			if(96 < active_char && active_char < 123) 	HandOverValueToUI(active_char, 123, 96);	//Chars z-a
			while(!enter_pushed())
			{	
				//get act char
				active_char = (char)GetvalueFromUI();
				if(active_char!=old_char)
				{	
					old_char = active_char;
					//set new limits
					switch(active_char)
					{
						case 31:
					    	HandOverValueToUI(122, 123, 96);	//Chars z-a
							active_char=122;
					  		break;
				
						case 58:
					    	HandOverValueToUI(65, 91, 64);		//Chars Z-A 
							active_char=65;
					  		break;

						case 64:
					    	HandOverValueToUI(57, 58, 31);		//Chars " "-9
							active_char=57;
					  		break;

						case 91:
					    	HandOverValueToUI(97, 123, 96);		//Chars z-a
							active_char=97;
					  		break;
						case 96:
					    	HandOverValueToUI(90, 91, 64);		//Chars Z-A 
							active_char=90;
					  		break;

						case 123:
					    	HandOverValueToUI(32, 58, 31);		//Chars " "-9
							active_char=32;
					  		break;
					}
					//change char in name
					newname[active_pos-1]=active_char;
					//print new name
					lcd_write_string(newname,1,0);
					//enable cursor
					lcd_go_to_position(1,active_pos-1);
					lcd_cursor_blink();
				}
			}
			//char disselected
			HandOverValueToUI(active_pos, MAX_SRAM_ITEM_NAME_LENGHT+2, 0);
			lcd_write_string(newname,1,0) ;
			user_info_nameedit();
			//enable cursor
			lcd_go_to_position(1,active_pos-1);
			lcd_cursor_blink();
		}
	}	
}
	*/
