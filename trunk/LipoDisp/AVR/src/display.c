#include "../OS/FabOS.h"
#include "../lcd/lcd.h"
#include "../menu/menu.h"
#include "../touch/touchpad.h"
#include "../comm/serial.h"
#include "../menu/menu_variant.h"
#include <string.h>


extern void menu_init(void);
void linewriter(void);


extern uint8_t getLastCommError(void);


extern Battery_Info_t g_tBattery_Info; // serial_master.c
extern uint8_t g_GotNewComand; // command.c
extern Command_t g_tCommand; // command.c // attention!!!!!! not task safe!!!


#define DISPLAYTEST 0
#define TOUCHTEST 0


char buf[30];
char sbuf[50];

void DrawBox( uint16_t ypos ) 
{
	#define BOXLEFT 160
	
	
	lcd_draw_line(255,0,0,	BOXLEFT,	ypos+4,				320,	ypos+4);
	lcd_draw_line(255,0,0,	320,	ypos+4,				320,	ypos+7*16-4);
	lcd_draw_line(255,0,0,	BOXLEFT,	ypos+7*16-4,	320,	ypos+7*16-4);
	lcd_draw_line(255,0,0,	BOXLEFT,	ypos+4,				BOXLEFT,	ypos+7*16-4);
	
	lcd_draw_line(255,0,0,	BOXLEFT,	ypos+4,				320,	ypos+7*16-4);
	lcd_draw_line(255,0,0,	BOXLEFT,	ypos+7*16-4,		320,	ypos+4);
	
}

void TaskDisplay(void)
{

	uint16_t ypos = 0;
	uint8_t i;


#define FONTSIZE 1
#define LINEDIFF FONTSIZE*16

	lcd_init( BPP24, ORIENTATION_0);

	menu_init();

	lcd_clear();//lcd clear needed here because a new screen is shown
	OS_SetAlarm( OSALMWaitDisp, 1000 );

	while(1)
	{
		ypos = 0;
        #if TOUCHTEST == 1
		OS_WaitAlarm(OSALMWaitDisp);
		OS_SetAlarm(OSALMWaitDisp,100);
		// the touch is tested here!

		touchtest();

		OS_GetTicks(&t2);

		t2=t2-t1;
		lcd_print(GREY, BLACK, FONTSIZE, 0, 220,"Time: %d ms     " ,(uint16_t)t2/100);
#else
		OS_WaitAlarm( OSALMWaitDisp );
		OS_SetAlarm( OSALMWaitDisp, 300 ); // FIXME only for linewriter !!! ansonsten 500 !
		if(g_GotNewComand) // was hat das hier verloren? -> umziehen! --> N�, weil die daten im ISR empfangen werden, und ein Mutex im ISR nicht geht.
		{
			// gespeicherte Commands vom Slave in Parameter eintragen.
			OS_MutexGet( OSMTXCommand );

			myPar.parCurrent.sValue = g_tCommand.sCurrentSetpoint;
			myPar.parChVoltSET.sValue = g_tCommand.usVoltageSetpoint_mV;
			// not existing myPar.parCellCount.sValue = g_tCommand.ucUserCellCount;

			myPar.parMaxcap.sValue = g_tCommand.unQ_max_mAh; // in mAh
			myPar.parMaxtime.sValue = g_tCommand.usT_max_s;
			myPar.parBalActVoltSET.sValue = g_tCommand.usMinBalanceVolt_mV;

			myPar.parRefreshPeriod.sValue = g_tCommand.refreshrate;
			
			myPar.parMinSupplyVolt.sValue = g_tCommand.SuppMin_mV;

			OS_MutexRelease( OSMTXCommand );

			g_GotNewComand = 0; // reset it.
		}
		
		static eBatteryStatus_t LastState;

		if (g_tBattery_Info.eState != LastState)
		{
			lcd_clear();
			menue_forcedraw();
		}
		LastState = g_tBattery_Info.eState;


		if(g_tBattery_Info.eState == eBattWaiting )
		{
			menu_show();

			// debug
			//lcd_print( WHITE, BLACK, 1, 150, 200,"Particle: %d , %d   " ,(uint16_t)myP.position,(uint16_t)myP.velocity);
		}
		else if (g_tBattery_Info.eState == eBattSupplyUntervolt ) 
		{
			lcd_print(RED, BLACK, 2, 20, 40,"Supply"); // sch.... f�r mehrere Lader fixme									
			lcd_print(RED, BLACK, 2, 20, 80,"undervoltage");
			lcd_print(WHITE, BLACK, 2, 20, 120,"Supply %02d.%01dV   ",g_tBattery_Info.sSupplyVolt_mV/1000,(g_tBattery_Info.sSupplyVolt_mV%1000)/100); //																								
		}
		
		#if GRAPH_AT_LOCK == 1
		// touchtest();

		else if (s_ucKeyLock == 1)
		{
			linewriter();
		}
		#endif
		else
		{
			// show the overview page

			//the lcd_print function overwrites old text-> no lcd_clear needed!
			//lcd_clear();
			
			lcd_print(RED, BLACK, 3, 290, ypos,"%d",1); // actual charger instance No.

			lcd_print(YELLOW, BLACK, 2, 0, ypos,"%02d.%02d V  ",g_tBattery_Info.sActVoltage_mV/1000,(g_tBattery_Info.sActVoltage_mV%1000)/10);
			lcd_print(YELLOW, BLACK, 2, 160, ypos,"%d.%02d A  ",g_tBattery_Info.sActCurrent_mA/1000,(g_tBattery_Info.sActCurrent_mA%1000)/10);
			ypos += 2*LINEDIFF;
			
			uint16_t cht = g_tBattery_Info.unTimeCharging_s;
			uint16_t mah = g_tBattery_Info.unCharge_mAs/3600;
			if(mah < 999)
				lcd_print(WHITE, BLACK, 2, 0, ypos,"%03d mAh ",mah);
			else
				lcd_print(WHITE, BLACK, 2, 0, ypos,"%01d.%02d Ah  ",mah/1000,(mah%1000)/10);      // zus�tzliches Leerzeichen eingef�gt, damit das h vom mAh �berschrieben wird
			lcd_print(WHITE, BLACK, 2, 160, ypos,"%d:%02d:%02d  ",cht/3600,(cht%3600)/60,cht%60);
			ypos += 2*LINEDIFF;

			//DrawBox(ypos);  �berfl�ssig
			

			#define CELLCOL1 15
			#define CELLCOL2 60
			
			lcd_print(WHITE, BLACK, FONTSIZE, CELLCOL1,ypos,"mV"); // single cells headline
			lcd_print(WHITE, BLACK, FONTSIZE, CELLCOL2,ypos,"mAh"); // single cells headline

			ypos += LINEDIFF;
			for (i = 0; i < 6; ++i) 
			{
				lcd_print(WHITE, BLACK, FONTSIZE, 0,ypos,"%d", i+1);
				if(i<g_tBattery_Info.ucNumberOfCells)
				{
					lcd_print(WHITE, BLACK, FONTSIZE, CELLCOL1,ypos,"%04d  ", g_tBattery_Info.Cells[i].sVoltage_mV);
					uint16_t mahRest =  g_tBattery_Info.Cells[i].unDisCharge_mAs%3600;
					lcd_print(WHITE, BLACK, FONTSIZE, CELLCOL2,ypos,"%03d.%02d  ", (uint16_t)(g_tBattery_Info.Cells[i].unDisCharge_mAs/3600), mahRest/36);
				}				
				else
				{
					lcd_print(DGREY, BLACK, FONTSIZE, CELLCOL1,ypos,"%04d  ", g_tBattery_Info.Cells[i].sVoltage_mV);
					//lcd_print(GREY, BLACK, FONTSIZE, CELLCOL2,ypos,"%05d  ", g_tBattery_Info.Cells[i].unDisCharge_mAs/36);
				}					
				ypos += LINEDIFF;
			}

			ypos -= 6*LINEDIFF;			// 6 Zeilen hoch schieben
			lcd_print(WHITE, BLACK, 1, 160, ypos,"Temp");
			lcd_print(WHITE, BLACK, 1, 220, ypos,"%02d C   ",g_tBattery_Info.ucDegC);
			ypos += LINEDIFF;
			lcd_print(WHITE, BLACK, 1, 160, ypos,"Supply"); //
			lcd_print(WHITE, BLACK, 1, 220, ypos,"%02d.%01dV   ",g_tBattery_Info.sSupplyVolt_mV/1000,(g_tBattery_Info.sSupplyVolt_mV%1000)/100); //
			ypos += LINEDIFF;
			lcd_print(WHITE, BLACK, 1, 160, ypos,"Iset");
			lcd_print(WHITE, BLACK, 1, 220, ypos,"%04d mA   ",g_tCommand.sCurrentSetpoint);
			ypos += LINEDIFF;
			lcd_print(WHITE, BLACK, 1, 160, ypos,"Uset");
			lcd_print(WHITE, BLACK, 1, 220, ypos,"%04d mV   ",g_tBattery_Info.sVSetpoint);
			ypos += LINEDIFF;
			lcd_print(WHITE, BLACK, 1, 160, ypos,"PWM");
			lcd_print(WHITE, BLACK, 1, 220, ypos," %04d   ",g_tBattery_Info.usPWM);
			ypos += 2*LINEDIFF;
						
						
			/*lcd_print(WHITE, BLACK, 1, 200, ypos,"ASYNC %d   ",g_tBattery_Info.usPWMStep);
			ypos += LINEDIFF;
			lcd_print(WHITE, BLACK, 1, 200, ypos,"ErrCnt %d   ",g_tBattery_Info.ErrCnt); //g_tBattery_Info.usConverterPower_W);
			ypos += LINEDIFF;
			lcd_print(WHITE, BLACK, 1, 160, ypos,"SlaveErr %d    ",(g_tBattery_Info.LastErr));
			ypos += LINEDIFF;*/

			
			static uint8_t counter;
			switch (g_tBattery_Info.eState)
			{
				case eBattCharging:
					if (counter < 3) strcpy(buf,"Battery is charging     ");							// fime: beautyfy me						
					if ((counter >= 3) && (counter < 6)) strcpy(buf,"Battery is charging.");
					if ((counter >= 6) && (counter < 9)) strcpy(buf,"Battery is charging..");
					if ((counter >= 9) && (counter < 12)) strcpy(buf,"Battery is charging...");
					if ((counter >= 12) && (counter < 15)) strcpy(buf,"Battery is charging....");
					if (counter >= 15) strcpy(buf,"Battery is charging.....");
					if (counter >= 17) counter = 0;
					break;
				case eBattFull:
					if (counter <= 1)  strcpy(buf,"Battery full - job done"); 
					else { strcpy(buf,"Battery full          "); counter = 0;	}
					break;
				case eBattWaiting:
					strcpy(buf,"Waiting     ");
					break;
				case eBattUnknown:
					strcpy(buf,"   ");
					break;
				case eBattError:
					strcpy(buf,"Bat.Err  ");
					break;
				case eBattMaxCap:
					strcpy(buf,"Max. capacity reached");
					break;
				case eBattMaxTime:
					strcpy(buf,"Max. time reached");
					break;
				default:
					strcpy(buf,"Waaargh!!!  ");
					break;
			}
			lcd_print(GREEN,BLACK,2,0,ypos,buf);
			counter++;
		
			
			
			
			

			/*OS_GetTicks(&t2);

			t2=t2-t1;
			lcd_print(GREY, BLACK, FONTSIZE, 0, 220,"Time: %d s     " ,(uint16_t)t2/1000);

			lcd_print(WHITE, BLACK, FONTSIZE, 230, 220,"I set: %d",g_tCommand.sCurrentSetpoint);

			lcd_print(WHITE, BLACK, FONTSIZE, 230, 200,"Batt. %d Cells ",g_tBattery_Info.ucNumberOfCells);*/


		}
	}

#endif
	
}

uint8_t scaletoscreen(int16_t val,int16_t max)
{
	int32_t ret;

	ret = (int32_t)val*240L/(int32_t)max;


	return limit(240-ret,0,240);
}


void linewriter(void)
{
	//the screen is 320 px wide and 240 px high
	static uint16_t g;
	g++;
	if (g == 320)
	{
		lcd_clear();
		//lcd_draw_line(YELLOW,0,30,320,30);
		g =0;
	}


	lcd_draw_pixel( YELLOW,g,  scaletoscreen(g_tBattery_Info.sActVoltage_mV-18000,25200-18000)  );
	lcd_draw_pixel( GREY  ,g,  scaletoscreen(g_tBattery_Info.sISetpoint,2000)  );
	lcd_draw_pixel( WHITE ,g,  scaletoscreen(g_tBattery_Info.sActCurrent_mA,2000)  );
	lcd_draw_pixel( RED   ,g,  scaletoscreen(g_tBattery_Info.usPWM,4500)  );
//	lcd_draw_pixel( GREEN ,g,  scaletoscreen(g_tBattery_Info.sDiff+5000,10000)  );


	lcd_print(YELLOW, BLACK, 1, 0, 16,"U: %d mV  " ,(uint16_t)g_tBattery_Info.sActVoltage_mV);
	lcd_print(GREY  , BLACK, 1, 0, 80,"S: %d mA  " ,(uint16_t)g_tBattery_Info.sISetpoint);
	lcd_print(WHITE , BLACK, 1, 0, 32,"I: %d mA  " ,(uint16_t)g_tBattery_Info.sActCurrent_mA);
	lcd_print(RED   , BLACK, 1, 0, 48,"P: %d %   " ,(uint16_t)g_tBattery_Info.usPWM);
//	lcd_print(GREEN , BLACK, 1, 0, 64,"D: %d Z   " ,(uint16_t)g_tBattery_Info.sDiff);



}

