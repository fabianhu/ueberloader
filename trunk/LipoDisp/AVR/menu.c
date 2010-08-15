 /*
 * This is the Menu
 *
 * */


#include "menu.h"

//menue strings
char txtmainmenu[] PROGMEM="Main Menu";
char txtback[] PROGMEM="(back)";

char txtmenu1[] PROGMEM="Injection";
	char txtsubmenu11[] PROGMEM="Single shoot";
	char txtsubmenu12[] PROGMEM="Sequence";
char txtmenu2[] PROGMEM="Settings";
	char txtsubmenu21[] PROGMEM="Trigger Mode";
	char txtsubmenu22[] PROGMEM="Sequence";
	char txtsubmenu23[] PROGMEM="Current Profiles";


Parameter_t p_item[] = {//sParam	sLowerLimit		sUpperLimit;
						{0,			0,				100},
						{50,		50,				60},
						};
						
						 
MenuItemName_t n_item[]	=	{//Menu item name
						{"Profile1"},
						{"Profile2"},
						{"Profile3"},
						{"Profile4"},
						{"Profile5"},
						};	

MenuItem_t m_item[] = {//ucID	strName						pAction			pParam		ucSubMenu	ucPrev		ucNext 	//array index (only f. debug)
						{ 255 ,	txtmainmenu, 				0, 				0, 			1, 			255,		255},	//0
						{ 1 , 	txtmenu1, 					0, 				0, 			11, 		0, 			2},		//1
							{ 11 , 	txtsubmenu11, 			0,				0, 			0, 			0, 			12},	//2
							{ 12 , 	txtsubmenu12, 			0, 				0, 			0, 			11,			13},	//3
							{ 13 , 	txtback,				0, 				0, 			0, 			12, 		0},		//4
						{ 2 , 	txtmenu2, 					0, 				0, 			21, 		1, 			0},		//5
							{ 21 , 	txtsubmenu21, 			0, 				0,			0, 			0, 			22},	//6
							{ 22 , 	txtsubmenu22, 			0, 				0,			0, 			21,			23},	//7
							{ 23 , 	txtsubmenu23, 			0, 				0,			231,		22,			24},	//8
								{ 231 ,	"Profile01", 		0, 				0,			0, 			236,		232},	//9
								{ 232 ,	"Profile02", 		0, 				0,			0, 			231,		233},	//10
								{ 233 ,	"Profile03", 		0, 				0,			0, 			232,		234},	//11
								{ 234 ,	"Profile04", 		0, 				0,			0, 			233,		235},	//12
								{ 235 ,	"Profile05", 		0, 				0,			0, 			234,		236},	//13
								{ 236 ,	txtback, 			0, 				0,			0, 			235,		231},	//14
							{ 24 , 	txtback,				0, 				0, 			0, 			23, 		0},		//15
						//array termination fixme could we omit this? (using "for"?)
						{ 0,0,0,0,0,0,0},
						};


MenuItem_t* gpsActualItem = &m_item[0];

unsigned char get_index(unsigned char ucID)
{
	uint8_t index=0;
	
	if (ucID)
	{
		while(m_item[index].ucID) // fixme bitte for und sizeof() ??
		{
			if(m_item[index].ucID==ucID) //ucID found
			{
				return index; // return result.
			}
			index++;
		}
		// not found
	}
	return 0; // return error value

}

void menu_errorhandler(uint8_t error, uint8_t array_index)
{
	switch (error)
		{
		case INFINITE_LOOP: 
			{
			//lcd_write_var("INFINITE LOOP: ID%0",(uint16_t)array_index,1,0);
			lcd_print(WHITE, BLACK, 1, 0, 0, "INFINITE_LOOP at ID:%i",array_index);
			}  break;
      	
		case WRONG_SUB_UCID: 
			{
			//lcd_write_var("WRONG SUB UCID: ID%0",(uint16_t)array_index,1,0);
			lcd_print(WHITE, BLACK, 1, 0, 0, "WRONG_SUB_UCID at ID:%i",array_index);
			}  break;
	    
		case WRONG_PREV_UCID: 
			{
			//lcd_write_var("WRONG PREV UCID: ID%0",(uint16_t)array_index,1,0);
			lcd_print(WHITE, BLACK, 1, 0, 0, "WRONG_PREV_UCID at ID:%i",array_index);
			}  break;
		
		case WRONG_NEXT_UCID: 
			{
			//lcd_write_var("WRONG NEXT UCID: ID%0",(uint16_t)array_index,1,0);
			lcd_print(WHITE, BLACK, 1, 0, 0, "WRONG_NEXT_UCID at ID:%i",array_index);
			}  break;
		
		default:
			{};
      }

	while(1); //stop program execution here
}

void menu_init(void)
{
	volatile uint8_t start_index=0, act_index=0, item_nr=0;
	volatile uint8_t ucID = 0, index=0, temp_index=0, error=0, errorlocation=0;

	gpsActualItem = &m_item[0];

	//get and check array inidices
	while(gpsActualItem->ucID != 0)//(m_item[index].ucID)
		{
		//save menu arra index
			errorlocation = gpsActualItem->ucID;															//save menu array index	
		//get sub menu item array position
			ucID = m_item[index].ucSubMenu;																//get sub menu ucID
			temp_index = get_index(gpsActualItem->ucSubMenu);											//get array index of sub menu
			if((temp_index == 0 && gpsActualItem->ucSubMenu) && ucID != 255)								//check return value of get_index
				{
				error = WRONG_SUB_UCID;
				break;
				}
			else
				{		
				gpsActualItem->ucSubMenu = temp_index;													//save array index of sub menu ucID
				}
		//get previous menu item array position
			ucID = gpsActualItem->ucPrev;																//get sub menu ucID
			temp_index = get_index(gpsActualItem->ucPrev);												//get array index of sub menu
			if((temp_index == 0 && gpsActualItem->ucPrev) && ucID != 255)													//check return value of get_index
				{
				error = WRONG_PREV_UCID;
				break;
				}
			else
				{		
				gpsActualItem->ucPrev = temp_index;														//save array index of prev menu ucID
				}
		//get next menu item array position
			ucID = m_item[index].ucNext;																//get sub menu ucID
			temp_index = get_index(gpsActualItem->ucNext);												//get array index of sub menu
			if((temp_index == 0 && gpsActualItem->ucNext) && ucID != 255)									//check return value of get_index
				{
				error = WRONG_NEXT_UCID;
				break;
				}
			else
				{		
				gpsActualItem->ucNext = temp_index;														//save array index of next menu ucID
				}		
		//next menu item
			gpsActualItem++;
		}
	//reset pointer
	gpsActualItem = &m_item[0];
	if(!error)
		{
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
				while((start_index!=act_index) && (m_item[act_index].ucNext !=0) && (item_nr<255));
				//check for infinite loops
				if(item_nr==255)
					{
					error = INFINITE_LOOP;
					break;
					}
				//if last item has no ucNext, store group position in this item
				if(!m_item[act_index].ucNext)
					{
					m_item[act_index].ucItem_nr = item_nr++;
					}
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
				while((start_index!=act_index) && (m_item[act_index].ucNext !=0) && (item_nr<255));
				//check for infinite loops
				if(item_nr==255)
					{
					error = INFINITE_LOOP;
					break;
					}
				//if last item has no ucNext, store nr of group items in this item
				if(!m_item[act_index].ucNext)
					{
					m_item[act_index].ucNr_grp_items = item_nr;
					}
				}
			//get next menu item
			index++;
			}
			if(!error)
				{
				//Clear the select flag
				index=0;
				while(m_item[index++].ucID) {m_item[index].ucItem_sel=0;}
				//Select first menu
				gpsActualItem = &m_item[m_item[0].ucSubMenu];
				gpsActualItem->ucItem_sel=1;
				}
		}
	
	if(error)	
		{
		//call errorhandler
		menu_errorhandler(error,errorlocation);
		}

}

void menu_show(void)
{
	//lcd_write_string( char *text_string, uint8_t row, uint8_t column ) ;
	//lcd_write_var( char *text_string, uint16_t var, uint8_t row, uint8_t column );
	
	volatile unsigned char i=0, index=0, page=0, items_per_page=0;
	static unsigned char last_menu_id=255;
	static unsigned char last_page=255;
	static unsigned char parameter_en=0;
	char strtmp[21];
	Parameter_t *pParam;

	
	//if(gpsActualItem->ucParam_en)//parameter edit-mode activ?
	
	//get page (1page = ITEMSCOUNT_PAGE menuitems)
	page = (gpsActualItem->ucItem_nr-1)/ITEMSCOUNT_PAGE;	//page 0 to...
	//get items per page
	if(gpsActualItem->ucNr_grp_items/((page+1)*ITEMSCOUNT_PAGE))
		{
		items_per_page = ITEMSCOUNT_PAGE;
		}
	else{
		items_per_page = gpsActualItem->ucNr_grp_items-(page*ITEMSCOUNT_PAGE);
		}
	//Page changed?
	if(page!=last_page || last_menu_id!=gpsActualItem->ucTop)
		{
		//Delete screen
		lcd_clear();
		//Reset menu id & page
		last_menu_id = 255;
		last_page=255;
		} 	
	//if menu-plane has changed draw new headline
	if(last_menu_id!=gpsActualItem->ucTop)
		{
		//Write Menue header
		strcpy_P(strtmp, m_item[gpsActualItem->ucTop].strName);
		//lcd_write_string(strtmp,1,0) ;
		lcd_print(WHITE, BLACK, 2, 0, 0, strtmp);
		}
	//Save new menu id and page
	last_menu_id = gpsActualItem->ucTop;
	last_page=page;
	//List Menue items
	//get index of first group item (ucItem_nr/ucNr_grp_items)
	index = get_index(gpsActualItem->ucID);
	while(m_item[index].ucItem_nr!=page*ITEMSCOUNT_PAGE+1){index=m_item[index].ucPrev;}
	//list menu items
	for(i=0;i<items_per_page;i++)
		{
		if(m_item[index].ucItem_sel)
			{
			//draw selected item
			strcpy_P(strtmp, m_item[index].strName);
			//lcd_write_string(strtmp,i+2,2) ;
			//lcd_write_string("*",i+2,19) ;
			lcd_print(WHITE, BLACK, 1, 40, i*30+40, strtmp);
			}
		else
			{
			//draw unselected item
			strcpy_P(strtmp, m_item[index].strName);
			//lcd_write_string(strtmp,i+2,2) ;
			//lcd_write_string(" ",i+2,19) ;
			//lcd_print(WHITE, BLACK, 1, 40, i*30+40, strtmp);
			}
		//get next item
		index=m_item[index].ucNext;
		}
	//Draw position marker
		
}

void menu_next(unsigned char step)//down
{
	unsigned char top_id, i, lower_limit;
	Parameter_t *pParam;
	
	for(i=0;i<step;i++)
		{
		if(gpsActualItem->ucParam_en)//parameter edit-mode activ?
			{
			//get parameter
			pParam = m_item[get_index(gpsActualItem->ucID)].pParamID;
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
			if(gpsActualItem->ucNext) //next menu item value existend?
				{
				//save top id
				top_id = gpsActualItem->ucTop;
				//Deselect item
				gpsActualItem->ucItem_sel=0;
				// switch to next item
				gpsActualItem = &m_item[gpsActualItem->ucNext];
				//Select next item
				gpsActualItem->ucItem_sel=1;
				//save top id
				gpsActualItem->ucTop=top_id;
				}
			}
		//show changed menue
		menu_show();
		}
}

void menu_prev(unsigned char step)//up
{
	unsigned char top_id, i, upper_limit;
	Parameter_t *pParam;
	
	for(i=0;i<step;i++)
		{
		if(gpsActualItem->ucParam_en)//parameter edit-mode activ?
			{
			//get parameter
			pParam = m_item[get_index(gpsActualItem->ucID)].pParamID;
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
			if(gpsActualItem->ucPrev) //next menu item value existend?
				{
				//save top id
				top_id = gpsActualItem->ucTop;
				//Deselect item
				gpsActualItem->ucItem_sel=0;
				// switch to next item
				gpsActualItem = &m_item[gpsActualItem->ucPrev];
				//Select next item
				gpsActualItem->ucItem_sel=1;
				//save top id
				gpsActualItem->ucTop=top_id;
				}
			}
		//show changed menue
		menu_show();
		}
}

void menu_select(void)
{	
	unsigned char top_id;
	
	//Activate parameter edit mode
	if(gpsActualItem->pParamID)
		{
		if(gpsActualItem->ucParam_en)//parameter edit-mode activ?
			{
			gpsActualItem->ucParam_en=0x0; //toggle enable
			}
		else
			{
			gpsActualItem->ucParam_en=0x1; //toggle enable
			}
		}
	//Select submenu
	else if(gpsActualItem->ucSubMenu)
		{
		//Save top id
		top_id = get_index(gpsActualItem->ucID);
		//Deselect item
		gpsActualItem->ucItem_sel=0;
		//Switch to next item
		gpsActualItem = &m_item[gpsActualItem->ucSubMenu];
		//Select next item
		gpsActualItem->ucItem_sel=1;
		//Save top id 
		gpsActualItem->ucTop=top_id;
		}
	//Execute function
	else if(gpsActualItem->pAction)
		{
		gpsActualItem->pAction();
		}
	//Select top level menu item
	else if(gpsActualItem->ucTop)
		{
		//Deselect item
		gpsActualItem->ucItem_sel=0;
		// switch to next item
		gpsActualItem = &m_item[gpsActualItem->ucTop];
		//Select next item
		gpsActualItem->ucItem_sel=1;
		}
	
	//show changed menue
	menu_show();
}


