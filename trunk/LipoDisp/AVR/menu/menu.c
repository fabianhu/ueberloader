 /*
 * This is the Menu
 *
 * */

#include "../OS/FabOS.h"
#include "menu.h"
#include "menu_variant.h"

extern uint16_t GetvalueFromUI(void);
extern MenuItem_t m_items[MENUESIZE];
extern void HandOverValueToUI(uint16_t value, uint16_t upper, uint16_t lower, uint16_t stepsize);
//extern void touchSetValue(int16_t v, int16_t lower, int16_t upper);

Menue_t MyMenue = MENUEINIT;

void GetSubMenuCount(uint8_t *Size, uint8_t *StartIndex)
{
	uint8_t i, ResultStartIndex=0, ResultSize=0;
	for(i=m_items[MyMenue.gucSelectedItem].ucParent+1;i<MENUESIZE;i++)
	{
		if(m_items[i].ucParent == m_items[MyMenue.gucSelectedItem].ucParent)
			{
			if(ResultStartIndex == 0) ResultStartIndex = i;	
			ResultSize++;								
			}
	}
	//Assign values
	*Size = ResultSize;
	*StartIndex = ResultStartIndex;
}

void AdjustStartPos(uint8_t *Size, uint8_t *StartIndex)
{	
	if(*Size>ITEMS_PER_PAGE)
	{
		//bottom screen
		if((MyMenue.gucSelectedItem-*StartIndex)>=(*Size-(ITEMS_PER_PAGE/2)))
		{
		*StartIndex=MyMenue.gucSelectedItem-(ITEMS_PER_PAGE-(*Size-(MyMenue.gucSelectedItem-*StartIndex)));
		}
		else
		{
			//middle screen
			if((MyMenue.gucSelectedItem-*StartIndex)>=(ITEMS_PER_PAGE/2))
			{
				*StartIndex=MyMenue.gucSelectedItem-(ITEMS_PER_PAGE/2);
			}
		}
	}

}


void menu_init(void)
{
	uint8_t i;
	uint8_t SubMenuGroupSize, StartIndex;

	//get and check array inidices
	for(i=0;i<MENUESIZE;i++)
	{
		if(m_items[i].ucParent < MENUESIZE )
		{
		}
		else
		{
		//putt
		}
	}
	//set actual limits for UI
	GetSubMenuCount(&SubMenuGroupSize, &StartIndex);

	HandOverValueToUI(SubMenuGroupSize, SubMenuGroupSize, 1,1);

}

uint16_t debug33;

void menu_show(void)
{	
	uint8_t SubMenuGroupSize=0, StartIndex=0, i=0, LCDPos=0;
	uint8_t items_per_page;
	char StrTmp[MAX_ITEM_NAME_CHARLENGTH];
	static uint8_t LastMenuIndex=0;
	static uint8_t SelParLCDPos = 0;
	static uint8_t OldParentID = 255;
	static uint16_t OldParameterValue=255;
	static uint16_t NewParameterValue=255;
	
	//get new value from UI
	OS_MutexGet(OSMTXMENU);//OS_PREVENTSCHEDULING; // for MyMenue.MenuMode AND MyMenue.gucSelectedItem
	if(MyMenue.MenuMode)	//if parameter edit mode is active
	{
		GetSubMenuCount(&SubMenuGroupSize, &StartIndex);
		//MyMenue.gucSelectedItem = (StartIndex+SubMenuGroupSize)-(uint8_t)GetvalueFromUI();
		debug33 = GetvalueFromUI();
		MyMenue.gucSelectedItem = (StartIndex+SubMenuGroupSize)-(uint8_t)debug33; // menue runs backwards.
		
	}
	else	//check for new menu index
	{
		NewParameterValue=GetvalueFromUI();		
		//if(GetvalueFromUI()!=m_items[MyMenue.gucSelectedItem].pParamID->sValue || MyMenue.ShowParameter)//new menu item is selected
		if(NewParameterValue!=OldParameterValue || MyMenue.ShowParameter)//new menu item is selected
		{
			MyMenue.ShowParameter = 0;
				//save value
			OldParameterValue=NewParameterValue;		
			//draw selected item
			if((m_items[MyMenue.gucSelectedItem].ucSettings & 0x03) == SRAM)
			{
				menu_draw_selected_parameter(m_items[MyMenue.gucSelectedItem].strName, OldParameterValue, m_items[MyMenue.gucSelectedItem].pParamID->sType, SelParLCDPos);
			}
			else
			{	
				strcpy_P(StrTmp,m_items[MyMenue.gucSelectedItem].strName);
				menu_draw_selected_parameter(StrTmp, OldParameterValue, m_items[MyMenue.gucSelectedItem].pParamID->sType, SelParLCDPos);
			}
		
			//m_items[MyMenue.gucSelectedItem].pParamID->sValue;
		}

	}
	OS_MutexRelease(OSMTXMENU);//OS_ALLOWSCHEDULING;

	if(LastMenuIndex!=MyMenue.gucSelectedItem || MyMenue.ShowMenu)//new menu item is selected
	{
		//save new index
		LastMenuIndex=MyMenue.gucSelectedItem;
		//get array start index of the submenu items
		GetSubMenuCount(&SubMenuGroupSize,&StartIndex);
		//send values to UI
		//calculate items_per_page
		if(SubMenuGroupSize>ITEMS_PER_PAGE) 
		{
			items_per_page = ITEMS_PER_PAGE;
		}
		else
		{
			items_per_page = SubMenuGroupSize;
		}
		//write menu header
		if(OldParentID!=m_items[MyMenue.gucSelectedItem].ucParent || MyMenue.ShowMenu)
		{
			OldParentID=m_items[MyMenue.gucSelectedItem].ucParent;
			if((m_items[m_items[MyMenue.gucSelectedItem].ucParent].ucSettings&0x03) == SRAM)
			{
				menu_draw_header(m_items[m_items[MyMenue.gucSelectedItem].ucParent].strName);
			}
			else
			{	
				strcpy_P(StrTmp,m_items[m_items[MyMenue.gucSelectedItem].ucParent].strName);
				menu_draw_header(StrTmp);
			}
			//del old menu items
			menu_del_menuitems();
		}
		//reset show menu
		MyMenue.ShowMenu = 0;
		//write position information
		menu_draw_groupposition(MyMenue.gucSelectedItem-StartIndex+1, SubMenuGroupSize);
		//Adjust Start index
		AdjustStartPos(&SubMenuGroupSize, &StartIndex);
		//write menu items
		LCDPos = 0;
		for(i=StartIndex;i<StartIndex+items_per_page;i++)
		{
			if(i==MyMenue.gucSelectedItem) //draw item
			{
				//save pos
				SelParLCDPos = LCDPos;
				//draw selected item
				if((m_items[i].ucSettings&0x03) == SRAM)
				{
					menu_draw_selected_item(m_items[i].strName, (uint16_t)m_items[i].pParamID, m_items[i].pParamID->sValue, m_items[i].pParamID->sType, LCDPos);
				}
				else
				{	
					strcpy_P(StrTmp,m_items[i].strName);
					menu_draw_selected_item(StrTmp, (uint16_t)m_items[i].pParamID, m_items[i].pParamID->sValue, m_items[i].pParamID->sType, LCDPos);
				}
			}
			else
			{
				if((m_items[i].ucSettings&0x03) == SRAM)
				{
					menu_draw_unselected_items(m_items[i].strName, (uint16_t)m_items[i].pParamID, m_items[i].pParamID->sValue, m_items[i].pParamID->sType, LCDPos);
				}
				else
				{	
					strcpy_P(StrTmp,m_items[i].strName);
					menu_draw_unselected_items(StrTmp, (uint16_t)m_items[i].pParamID, m_items[i].pParamID->sValue, m_items[i].pParamID->sType, LCDPos);
				}
			}
		LCDPos++;
		}
	}
}

void menu_select(void)
{
	uint8_t SubMenuGroupSize=0, StartIndex=0;
	OS_MutexGet(OSMTXMENU);//OS_PREVENTSCHEDULING; // for MyMenue.MenuMode AND MyMenue.gucSelectedItem / Parameter
	
	if(m_items[MyMenue.gucSelectedItem].ucJumpTrg)//jump to target menu item
	{
		//assign jump target
		MyMenue.gucSelectedItem = m_items[MyMenue.gucSelectedItem].ucJumpTrg;
		//get array start index of the submenu items
		GetSubMenuCount(&SubMenuGroupSize, &StartIndex);
		//send values to UI
		HandOverValueToUI(SubMenuGroupSize-(MyMenue.gucSelectedItem-StartIndex), SubMenuGroupSize, 1, 1);
	}	
	else if(m_items[MyMenue.gucSelectedItem].pParamID)//toggle parameter edit mode
	{
		if(!MyMenue.MenuMode)
		{	
			//save parameter
			m_items[MyMenue.gucSelectedItem].pParamID->sValue=GetvalueFromUI();
			//get array start index of the submenu items
			GetSubMenuCount(&SubMenuGroupSize,&StartIndex);
			//send values to UI
			HandOverValueToUI(SubMenuGroupSize-(MyMenue.gucSelectedItem-StartIndex), SubMenuGroupSize, 1,1);
			//parameter deactivate
			MyMenue.MenuMode = 1;
			MyMenue.ShowMenu = 1; //init menu view

		}
		else
		{	
			//send parameter values and limits to UI
			HandOverValueToUI(	m_items[MyMenue.gucSelectedItem].pParamID->sValue,
								m_items[MyMenue.gucSelectedItem].pParamID->sUpperLimit,
								m_items[MyMenue.gucSelectedItem].pParamID->sLowerLimit,
								m_items[MyMenue.gucSelectedItem].pParamID->sStepSize
								);

			//parameter active
			MyMenue.MenuMode = 0;

			MyMenue.ShowParameter = 1; //init parameter view
		}
	}
	else if(m_items[MyMenue.gucSelectedItem].pAction)//execute function
	{	
		//execute function
		m_items[MyMenue.gucSelectedItem].pAction();
		//get array start index of the submenu items
		GetSubMenuCount(&SubMenuGroupSize, &StartIndex);
		//send values to UI
		HandOverValueToUI(SubMenuGroupSize-(MyMenue.gucSelectedItem-StartIndex), SubMenuGroupSize, 1,1);
		MyMenue.ShowMenu = 1; //init menu view
	}
	else
	{
//		asm("break");
	}
	OS_MutexRelease(OSMTXMENU);//OS_ALLOWSCHEDULING;
}


