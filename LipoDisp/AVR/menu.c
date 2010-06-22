 /*
 * This is the Menu
 *
 * */

#include "menu.h"
#include "OS/FabOS.h"

//function prototypes
void system_info (void);

//menue strings
char txtmainmenu[] PROGMEM="Hauptmenue";
char back[] PROGMEM="(back)";

char txtmenu1[] PROGMEM="Sample submenues";
	char txtsubmenu11[] PROGMEM="SSubmenue11";
	char txtsubmenu12[] PROGMEM="SSubmenue12";
	char txtsubmenu13[] PROGMEM="SSubmenue13";
	char txtsubmenu14[] PROGMEM="SSubmenue14";
char txtmenu2[] PROGMEM="Sample parameter";
	char txtsubmenu21[] PROGMEM="SParameter1";
	char txtsubmenu22[] PROGMEM="SParameter2";
char txtmenu3[] PROGMEM="Sample function";
	char txtsubmenu31[] PROGMEM="SFunction";


Parameter_t p_item[] = {//sParam	sLowerLimit		sUpperLimit;
						{0,			0,				100},
						{50,		50,				60},
						}; 

MenuItem_t m_item[] = {//ucID	strName			pAction			pParam		ucSubMenu		ucPrev		ucNext
						{ 255 ,	txtmainmenu, 		0, 				0, 			1, 			255,		255},
						{ 1 , 	txtmenu1, 			0, 				0, 			11, 			3, 			2},
							{ 11 , 	txtsubmenu11, 	0,				0, 			0, 			15, 		12},
							{ 12 , 	txtsubmenu12, 	0, 				0, 			0, 			11,			13},
							{ 13 , 	txtsubmenu13, 	0, 				0, 			0, 			12, 		14},
							{ 14 , 	txtsubmenu14, 	0, 				0, 			0, 			13,			15},
							{ 15 , 	back,			0, 				0, 			0, 			14, 		11},
						{ 2 , 	txtmenu2, 			0, 				0, 			21, 		1, 			3},
							{ 21 , 	txtsubmenu21, 	0, 				&p_item[0],	0, 			23, 		22},
							{ 22 , 	txtsubmenu22, 	0, 				&p_item[1],	0, 			21,			23},
							{ 23 , 	back,			0, 				0, 			0, 			22, 		21},
						{ 3 , 	txtmenu3, 			0, 				0, 			31, 		2, 			1},
							{ 31 , 	txtsubmenu31, 	&system_info,	0, 			0, 			32, 		32},
							{ 32 , 	back,			0, 				0, 			0, 			31, 		31},
						//array termination
						{ 0,0,0,0,0,0,0},
						};


MenuItem_t* gpsActualItem = &m_item[0];

unsigned char get_index(unsigned char ucID)
{
	unsigned char index=1;

	while(m_item[index].ucID)
	{
		if(m_item[index].ucID==ucID) break;
		index++;
	}
	return(index);
}


void init_menu(void)
{
	volatile unsigned char start_index=0, act_index=0, item_nr=0;
	volatile unsigned char index=0;
	//set io pins

// fixme wie was wo???

	//get array inidices
	while(m_item[index].ucID)
		{
		//get sub menu item array position
		if(m_item[index].ucSubMenu)	m_item[index].ucSubMenu = get_index(m_item[index].ucSubMenu);
		//get previous menu item array position
		if(m_item[index].ucPrev)m_item[index].ucPrev = get_index(m_item[index].ucPrev);
		//get next menu item array position
		if(m_item[index].ucNext) m_item[index].ucNext = get_index(m_item[index].ucNext);		
		//next menu item
		index++;
		}
	//get nr of items in the group and use ucItem_sel as "proceeded-marker"
	//start with first menu after the mainmenu m_item[1]
	index=1;
	while(m_item[index].ucID)
		{
		//start with first array item
		act_index = index;
		//nr of group items is 1
		item_nr = 1;
		//check if actual menu item has group members and item is not processed (.ucItem_sel=0)
		if(m_item[act_index].ucNext && !m_item[act_index].ucItem_sel)
			{
			//save start index
			start_index = get_index(m_item[act_index].ucID);
			do
				{
				//assign group position
				m_item[act_index].ucItem_nr=item_nr++;
				//get next item in the group
				act_index = m_item[act_index].ucNext;
				}
			while(start_index!=act_index);
			//save number of items in the group
			//start with first array item
			act_index = index;
			//calculate nr of items in the group
			item_nr--;
			//save start index again
			start_index = get_index(m_item[act_index].ucID);
			do
				{
				//assign nr of group members
				m_item[act_index].ucNr_grp_items=item_nr;
				//mark item as processed
				m_item[act_index].ucItem_sel=1;
				//get next item in the group
				act_index = m_item[act_index].ucNext;
				}
			while(start_index!=act_index);
			}
		//get next menu item
		index++;
		}
		//Clear the select flag
		index=0;
		while(m_item[index++].ucID) {m_item[index].ucItem_sel=0;}
		//Select first menu
		gpsActualItem = &m_item[m_item[0].ucSubMenu];
		(*gpsActualItem).ucItem_sel=1;
}

void show_menu(void)
{
	volatile unsigned char i=0, index=0, page=0, items_per_page=0;
	static unsigned char last_menu_id=255;
	static unsigned char last_page=255;
	static unsigned char parameter_en=0;
	char stringtemp[10];
	Parameter_t *pParam;

	
	if((*gpsActualItem).ucParam_en)//parameter edit-mode activ?
		{
		//reset normal menue page and menu id
		last_menu_id=255;
		last_page=255;
		//get parameter
		pParam = m_item[get_index((*gpsActualItem).ucID)].pParamID;
		//Show paramter name and limits
		if(!parameter_en)
			{
			//parameter name
			lcd_draw_filled_box(0, 155, 0, 0, 0, 320,40);
			lcd_write_flash_text(m_item[get_index((*gpsActualItem).ucID)].strName, 0, 0, 0, 2, 20, 0);
			lcd_draw_filled_box(0, 0, 0, 0, 41, 320,200);
			//Write old parameter value and limits
			itoa( (*pParam).sParam, stringtemp, 10 );// i=Integer;s=Zielstring
			lcd_write_ram_text("Old value:", 255, 255, 255, 2, 20, 0*32+45);
			lcd_write_ram_text(stringtemp, 255, 255, 255, 2, 200, 0*32+45);
			itoa( (*pParam).sLowerLimit, stringtemp, 10 );// i=Integer;s=Zielstring
			lcd_write_ram_text("Lower limit:", 255, 255, 255, 2, 20, 1*32+45);
			lcd_write_ram_text(stringtemp, 255, 255, 255, 2, 200, 1*32+45);
			itoa( (*pParam).sUpperLimit, stringtemp, 10 );// i=Integer;s=Zielstring
			lcd_write_ram_text("Upper limit:", 255, 255, 255, 2, 20, 2*32+45);
			lcd_write_ram_text(stringtemp, 255, 255, 255, 2, 200, 2*32+45);
			//Draw box arround actual value
			lcd_draw_box(0, 255, 0, 0, 4*32+50, 320,30);
			}
		//set marker parameter edit-mode active
		parameter_en=1;
		//clear old value
		lcd_draw_filled_box(0, 0, 0, 180, 4*32+51, 100,28);
		//write actual value
		itoa( (*pParam).sParam, stringtemp, 10 );// i=Integer;s=Zielstring
		lcd_write_ram_text("New value:", 255, 255, 255, 2, 20, 4*32+45);
		lcd_write_ram_text(stringtemp, 255, 255, 255, 2, 200, 4*32+45);
		}
	else
		{
		//reset paramter edit-mode
		parameter_en=0;
		//get page (1page = 5menuitems)
		page = ((*gpsActualItem).ucItem_nr-1)/5;
		//get items per page
		if((*gpsActualItem).ucNr_grp_items/((page+1)*5))
			{
			items_per_page = 5;
			}
		else
			{
			items_per_page = (*gpsActualItem).ucNr_grp_items-(page*5);
			}
		//if menu-plane is changed or new page is reached draw new headline
		if(last_menu_id!=(*gpsActualItem).ucTop)
			{
			//Write Menue header
			lcd_draw_filled_box(0, 155, 0, 0, 0, 320,40);
			lcd_write_flash_text(m_item[(*gpsActualItem).ucTop].strName, 0, 0, 0, 2, 20, 0);
			}
		//page changed?
		if(page!=last_page || last_menu_id!=(*gpsActualItem).ucTop)
			{
			lcd_draw_filled_box(0, 0, 0, 0, 41, 320,200);
			} 
			//save new menu id and page
			last_menu_id = (*gpsActualItem).ucTop;
			last_page=page;
		//List Menue items
		//get index of first group item (ucItem_nr/ucNr_grp_items)
		index = get_index((*gpsActualItem).ucID);
		while(m_item[index].ucItem_nr!=page*5+1){index=m_item[index].ucPrev;}
		//list menu items
		for(i=0;i<items_per_page;i++)
			{
			if(m_item[index].ucItem_sel)
				{
				lcd_draw_box(0, 255, 0, 0, i*32+50, 300,30);
				lcd_write_flash_text(m_item[index].strName, 255, 255, 255, 2, 20, i*32+45);
				}
			else
				{
				lcd_draw_box(0, 0, 0, 0, i*32+50, 300,30);
				lcd_write_flash_text(m_item[index].strName, 255, 255, 255, 2, 20, i*32+45);
				}
			index=m_item[index].ucNext;
			}
		//Draw position marker
		lcd_draw_filled_box(0, 0, 0, 308, 49, 12,180);
		lcd_draw_filled_box(0, 255, 0, 313, 49, 2, 182);
		lcd_draw_filled_box(0, 255, 0, 309, 51+(((*gpsActualItem).ucItem_nr-1)*170/((*gpsActualItem).ucNr_grp_items-1)), 10,8);
		}
}

void restore_menu(void)
{
	//Write Menue header
	lcd_draw_filled_box(0, 155, 0, 0, 0, 320,40);
	lcd_write_flash_text(m_item[(*gpsActualItem).ucTop].strName, 0, 0, 0, 2, 20, 0);
	lcd_draw_filled_box(0, 0, 0, 0, 41, 320,200);
	//Show menue-itmes
	show_menu();
}

void menu_next(unsigned char step)//down
{
	unsigned char top_id, i, lower_limit;
	Parameter_t *pParam;
	
	for(i=0;i<step;i++)
		{
		if((*gpsActualItem).ucParam_en)//parameter edit-mode activ?
			{
			//get parameter
			pParam = m_item[get_index((*gpsActualItem).ucID)].pParamID;
			//get limit
			lower_limit = (*pParam).sLowerLimit;
			//set new value
			if(((*pParam).sParam-step)>lower_limit)
				{
				(*pParam).sParam-=step;
				}
			else
				{
				(*pParam).sParam=lower_limit;
				}			

			}
		else
			{ 
			//save top id
			top_id = (*gpsActualItem).ucTop;
			//Deselect item
			(*gpsActualItem).ucItem_sel=0;
			// switch to next item
			gpsActualItem = &m_item[(*gpsActualItem).ucNext];
			//Select next item
			(*gpsActualItem).ucItem_sel=1;
			//save top id
			(*gpsActualItem).ucTop=top_id;
			}
		//show changed menue
		show_menu();
		}
}

void menu_prev(unsigned char step)//up
{
	unsigned char top_id, i, upper_limit;
	Parameter_t *pParam;
	
	for(i=0;i<step;i++)
		{
		if((*gpsActualItem).ucParam_en)//parameter edit-mode activ?
			{
			//get parameter
			pParam = m_item[get_index((*gpsActualItem).ucID)].pParamID;
			//get limit
			upper_limit = (*pParam).sUpperLimit;
			//set new value
			if(((*pParam).sParam+step)<upper_limit)
				{
				(*pParam).sParam+=step;
				}
			else
				{
				(*pParam).sParam=upper_limit;
				}			

			}
		else
			{  
			//save top id
			top_id = (*gpsActualItem).ucTop;
			//Deselect item
			(*gpsActualItem).ucItem_sel=0;
			// switch to next item
			gpsActualItem = &m_item[(*gpsActualItem).ucPrev];
			//Select next item
			(*gpsActualItem).ucItem_sel=1;
			//save top id
			(*gpsActualItem).ucTop=top_id;
			}
		//show changed menue
		show_menu();
		}
}

void menu_select(void)
{	
	unsigned char top_id;
	
	//if parameter available
	if((*gpsActualItem).pParamID)
		{
		if((*gpsActualItem).ucParam_en)//parameter edit-mode activ?
			{
			(*gpsActualItem).ucParam_en=0x0; //toggle enable
			}
		else
			{
			(*gpsActualItem).ucParam_en=0x1; //toggle enable
			}
		}
	//if submenu available? then select sub menu
	else if((*gpsActualItem).ucSubMenu)
		{
		//save top id
		top_id = get_index((*gpsActualItem).ucID);
		//Deselect item
		(*gpsActualItem).ucItem_sel=0;
		// switch to next item
		gpsActualItem = &m_item[(*gpsActualItem).ucSubMenu];
		//Select next item
		(*gpsActualItem).ucItem_sel=1;
		//save top id
		(*gpsActualItem).ucTop=top_id;
		}
	//if function available
	else if((*gpsActualItem).pAction)
		{
		(*gpsActualItem).pAction();
		}
	//go back to top level
	else if((*gpsActualItem).ucTop)
		{
		//Deselect item
		(*gpsActualItem).ucItem_sel=0;
		// switch to next item
		gpsActualItem = &m_item[(*gpsActualItem).ucTop];
		//Select next item
		(*gpsActualItem).ucItem_sel=1;
		}
	
	//show changed menue
	show_menu();
}

void system_info(void)
{
		char stringtemp[17];
		uint32_t time =0;
		uint32_t* pTime;
		pTime = &time;	
		while(1)
		{
			//Funktion screen
			lcd_draw_filled_box(0, 155, 0, 0, 0, 320,40);
			lcd_write_ram_text("System information", 0, 0, 0, 2, 20, 0);
			lcd_draw_filled_box(0, 0, 0, 0, 41, 320,200);
			lcd_write_ram_text("Stack Task1:", 255, 255, 255, 2, 20, 0*32+45);
			lcd_write_ram_text("Stack Task2:", 255, 255, 255, 2, 20, 1*32+45);
			lcd_write_ram_text("Stack Task3:", 255, 255, 255, 2, 20, 2*32+45);
			lcd_write_ram_text("Ticks:", 255, 255, 255, 2, 20, 3*32+45);
		
			//Show task stack usage
			itoa( OS_GetUnusedStack(1), stringtemp, 10 );// i=Integer;s=Zielstring
			
			lcd_write_ram_text(stringtemp, 255, 255, 255, 2, 220, 0*32+45);
			itoa( OS_GetUnusedStack(2), stringtemp, 10 );// i=Integer;s=Zielstring
			
			lcd_write_ram_text(stringtemp, 255, 255, 255, 2, 220, 1*32+45);
			itoa( OS_GetUnusedStack(3), stringtemp, 10 );// i=Integer;s=Zielstring
			
			lcd_write_ram_text(stringtemp, 255, 255, 255, 2, 220, 2*32+45);
			OS_GetTicks(pTime);
			itoa( time, stringtemp,10);// i=Integer;s=Zielstring  // nix & never float!!! fixme fixme fixme
			
			lcd_write_ram_text(stringtemp, 255, 255, 255, 2, 100, 3*32+45);
		}	

		//back
		restore_menu();
}
