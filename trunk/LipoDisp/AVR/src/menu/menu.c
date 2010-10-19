 /*
 * This is the Menu
 *
 * */


#include "menu_variant.h"
#include "OS/FabOS.h"

extern uint16_t GetvalueFromUI(void);
extern MenuItem_t m_items[MENUESIZE];
extern void HandOverValueToUI(uint16_t value, uint16_t upper, uint16_t lower, uint8_t type);
//extern void touchSetValue(int16_t v, int16_t lower, int16_t upper);

uint8_t gucSelectedItem = 1;
uint8_t ShowParameter = 0;
uint8_t ShowMenu =0;

uint8_t MenuMode = 1;

void GetSubMenuCount(uint8_t *Size, uint8_t *StartIndex)
{
	uint8_t i, ResultStartIndex=0, ResultSize=0;
	for(i=m_items[gucSelectedItem].ucParent+1;i<MENUESIZE;i++)
	{
		if(m_items[i].ucParent == m_items[gucSelectedItem].ucParent)
			{
			if(!ResultStartIndex) ResultStartIndex = i;	
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
		if((gucSelectedItem-*StartIndex)>=(*Size-(ITEMS_PER_PAGE/2)))
		{
		*StartIndex=gucSelectedItem-(ITEMS_PER_PAGE-(*Size-(gucSelectedItem-*StartIndex)));
		}
		else
		{
			//middle screen
			if((gucSelectedItem-*StartIndex)>=(ITEMS_PER_PAGE/2)) 
			{
				*StartIndex=gucSelectedItem-(ITEMS_PER_PAGE/2);
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
	HandOverValueToUI(SubMenuGroupSize, SubMenuGroupSize, 1, 0);

}

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
	OS_PREVENTSCHEDULING // for MenuMode AND gucSelectedItem
	if(MenuMode)	//if parameter edit mode is active
	{
		GetSubMenuCount(&SubMenuGroupSize, &StartIndex);
		//gucSelectedItem = (StartIndex+SubMenuGroupSize)-(uint8_t)GetvalueFromUI();
		gucSelectedItem = (StartIndex+SubMenuGroupSize)-(uint8_t)GetvalueFromUI();
	}
	else	//check for new menu index
	{
		NewParameterValue=GetvalueFromUI();		
		//if(GetvalueFromUI()!=m_items[gucSelectedItem].pParamID->sParam || ShowParameter)//new menu item is selected
		if(NewParameterValue!=OldParameterValue || ShowParameter)//new menu item is selected
		{
			ShowParameter = 0;
				//save value
			OldParameterValue=NewParameterValue;		
			//draw selected item
			if((m_items[gucSelectedItem].BMSK_NAME_LOCATION) == SRAM)
			{
				menu_draw_selected_parameter(m_items[gucSelectedItem].strName, OldParameterValue, m_items[gucSelectedItem].pParamID->sParType, SelParLCDPos);
			}
			else
			{	
				strcpy_P(StrTmp,m_items[gucSelectedItem].strName);
				menu_draw_selected_parameter(StrTmp, OldParameterValue, m_items[gucSelectedItem].pParamID->sParType, SelParLCDPos);
			}
		
			//m_items[gucSelectedItem].pParamID->sParam;	
		}

	}
	OS_ALLOWSCHEDULING

	if(LastMenuIndex!=gucSelectedItem || ShowMenu)//new menu item is selected
	{
		//save new index
		LastMenuIndex=gucSelectedItem;
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
		if(OldParentID!=m_items[gucSelectedItem].ucParent || ShowMenu)
		{
			OldParentID=m_items[gucSelectedItem].ucParent;
			if((m_items[m_items[gucSelectedItem].ucParent].BMSK_NAME_LOCATION) == SRAM)
			{
				menu_draw_header(m_items[m_items[gucSelectedItem].ucParent].strName);
			}
			else
			{	
				strcpy_P(StrTmp,m_items[m_items[gucSelectedItem].ucParent].strName);
				menu_draw_header(StrTmp);
			}
			//del old menu items
			menu_del_menuitems();
		}
		//reset show menu
		ShowMenu = 0;
		//write position information
		menu_draw_groupposition(gucSelectedItem-StartIndex+1, SubMenuGroupSize);
		//Adjust Start index
		AdjustStartPos(&SubMenuGroupSize, &StartIndex);
		//write menu items
		LCDPos = 0;
		for(i=StartIndex;i<StartIndex+items_per_page;i++)
		{
			if(i==gucSelectedItem) //draw item
			{
				//save pos
				SelParLCDPos = LCDPos;
				//draw selected item
				if((m_items[i].BMSK_NAME_LOCATION) == SRAM)
				{
					menu_draw_selected_item(m_items[i].strName, (uint16_t)m_items[i].pParamID, m_items[i].pParamID->sParam, m_items[i].pParamID->sParType, LCDPos);
				}
				else
				{	
					strcpy_P(StrTmp,m_items[i].strName);
					menu_draw_selected_item(StrTmp, (uint16_t)m_items[i].pParamID, m_items[i].pParamID->sParam, m_items[i].pParamID->sParType, LCDPos);
				}
			}
			else
			{
				if((m_items[i].BMSK_NAME_LOCATION) == SRAM)
				{
					menu_draw_unselected_items(m_items[i].strName, (uint16_t)m_items[i].pParamID, m_items[i].pParamID->sParam, m_items[i].pParamID->sParType, LCDPos);
				}
				else
				{	
					strcpy_P(StrTmp,m_items[i].strName);
					menu_draw_unselected_items(StrTmp, (uint16_t)m_items[i].pParamID, m_items[i].pParamID->sParam, m_items[i].pParamID->sParType, LCDPos);
				}
			}
		LCDPos++;
		}
	}
}

void menu_select(void)
{
	uint8_t SubMenuGroupSize=0, StartIndex=0;
	OS_PREVENTSCHEDULING // for MenuMode AND gucSelectedItem / Parameter
	
	if(m_items[gucSelectedItem].ucJumpTrg)//jump to target menu item
	{
		//assign jump target
		gucSelectedItem = m_items[gucSelectedItem].ucJumpTrg;
		//get array start index of the submenu items
		GetSubMenuCount(&SubMenuGroupSize, &StartIndex);
		//send values to UI
		HandOverValueToUI(SubMenuGroupSize-(gucSelectedItem-StartIndex), SubMenuGroupSize, 1, 0);
	}	
	else if(m_items[gucSelectedItem].pParamID)//toggle parameter edit mode
	{
		if(!MenuMode)
		{	
			//save parameter
			m_items[gucSelectedItem].pParamID->sParam=GetvalueFromUI();
			//get array start index of the submenu items
			GetSubMenuCount(&SubMenuGroupSize,&StartIndex);
			//send values to UI
			HandOverValueToUI(SubMenuGroupSize-(gucSelectedItem-StartIndex), SubMenuGroupSize, 1,0);
			//parameter deactivate
			MenuMode = 1;
			ShowMenu = 1; //init menu view

		}
		else
		{	
			//send parameter values and limits to UI
			HandOverValueToUI(m_items[gucSelectedItem].pParamID->sParam, m_items[gucSelectedItem].pParamID->sUpperLimit, m_items[gucSelectedItem].pParamID->sLowerLimit, m_items[gucSelectedItem].pParamID->sParType);

			//parameter active
			MenuMode = 0;

			ShowParameter = 1; //init parameter view
		}
	}
	else if(m_items[gucSelectedItem].pAction)//execute function
	{	
		//execute function
		m_items[gucSelectedItem].pAction();
		//get array start index of the submenu items
		GetSubMenuCount(&SubMenuGroupSize, &StartIndex);
		//send values to UI
		HandOverValueToUI(SubMenuGroupSize-(gucSelectedItem-StartIndex), SubMenuGroupSize, 1,0);
		ShowMenu = 1; //init menu view
	}
	else
	{
//		asm("break");
	}
	OS_ALLOWSCHEDULING
}


