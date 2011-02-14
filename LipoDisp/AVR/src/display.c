
#include "../OS/FabOS.h"
#include "../lcd/lcd.h"
#include "../menu/menu.h"
#include "../touch/touchpad.h"
#include "../comm/serial.h"
#include "../menu/menu_variant.h"
#include <string.h>

extern void menu_init(void);

extern uint8_t glCommError; // serial_master.c
extern particle_t myP; // touchpad.c
extern Battery_Info_t g_tBattery_Info; // serial_master.c
extern uint8_t 		g_GotNewComand ; // command.c
extern Command_t 		g_tCommand; // command.c

uint8_t g_bMenuActive = 0;//DISPLAYTEST; // 1 = the menue is active

#define DISPLAYTEST 1


void TaskDisplay(void)
{

	uint16_t ypos=0;
	uint32_t t1,t2;
	uint8_t i;

#define FONTSIZE 1
#define LINEDIFF FONTSIZE*16

	lcd_init(BPP24, ORIENTATION_0);

	menu_init();

	lcd_clear();//lcd clear needed here because a new screen is shown
	OS_SetAlarm(OSALMWaitDisp,1000);

	OS_GetTicks(&t1); // just for finding error fixme
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
		lcd_print(GREY, BLACK, FONTSIZE, 0, 220,"Time: %i ms     " ,(uint16_t)t2/100);
#else
		OS_WaitAlarm(OSALMWaitDisp);
		OS_SetAlarm(OSALMWaitDisp,250);


		if(g_GotNewComand)
		{
			// gespeicherte Commands vom Slave in Parameter eintragen.
			OS_MutexGet(OSMTXCommand);
			 parCurrent.sValue = g_tCommand.sCurrentSetpoint;
			 parBalActVolt.sValue = g_tCommand.usMinBalanceVolt_mV;
			 parMaxcap.sValue = g_tCommand.unQ_max_mAs/3600;  // mAs in mAh
			 parMaxtime.sValue = g_tCommand.usT_max_s;
			 parVoltSetpoint.sValue = g_tCommand.usVoltageSetpoint_mV;
			 parCellCount.sValue = g_tCommand.ucUserCellCount;


			 //g_tCommand.eChargerMode; // umstellen per Action // fixme
			 parPWMfrequency.sValue = 7;
			 parRefreshPeriod.sValue = 9; // fixme


			 OS_MutexRelease(OSMTXCommand);

		}


		static uint8_t bMenuCleared=0;

		if(!glCommError || DISPLAYTEST == 1)
		{

			if (g_bMenuActive)
			{
				if (bMenuCleared == 0)
				{
					lcd_clear();
					bMenuCleared = 1;
				}

				menu_show();

				// debug
				lcd_print(WHITE, BLACK, 1, 150, 200,"Particle: %i , %i   " ,(uint16_t)myP.position,(uint16_t)myP.velocity);
			}
			else
			{
				if (bMenuCleared == 1)
				{
					lcd_clear();
					bMenuCleared = 0;
				}
				// show the overview page

				//the lcd_print function overwrites old text-> no lcd_clear needed!
				//lcd_clear();

				// touchtest();

				lcd_print(YELLOW, BLACK, 2, 0, ypos,"vBatt  /t%i mV    ",g_tBattery_Info.sActVoltage_mV);
				ypos += LINEDIFF*2;
				lcd_print(YELLOW, BLACK, 2, 0, ypos,"Current/t%i mA   ",g_tBattery_Info.sActCurrent_mA);
				ypos += LINEDIFF*2;
				lcd_print(WHITE, BLACK, FONTSIZE, 230, 200,"Batt. %i Cells ",g_tBattery_Info.ucNumberOfCells);

				ypos = 64;

				for (i = 0; i < 6; ++i) {
				lcd_print(WHITE, BLACK, FONTSIZE, 0,ypos,"Cell%i/t%i/tmV/t%i uAh     " ,i,g_tBattery_Info.Cells[i].sVoltage_mV, g_tBattery_Info.Cells[i].unDisCharge_mAs/36);
				ypos += LINEDIFF;
				}

				ypos = 64;
				lcd_print(WHITE, BLACK, 1, 200, ypos,"PWM %i   ",g_tBattery_Info.usPWM);
				ypos += LINEDIFF;
				lcd_print(WHITE, BLACK, 1, 200, ypos,"ASYNC %i   ",g_tBattery_Info.usPWMStep);
				ypos += LINEDIFF;
				lcd_print(WHITE, BLACK, 1, 200, ypos,"Setp %i mA   ",g_tBattery_Info.sISetpoint);
				ypos += LINEDIFF;
				lcd_print(WHITE, BLACK, 1, 200, ypos,"diff %i mA   ",g_tBattery_Info.sDiff);
				ypos += LINEDIFF;
				lcd_print(WHITE, BLACK, 1, 200, ypos,"Time %i s   ",(uint16_t)g_tBattery_Info.unTimeCharging_s);
				ypos += LINEDIFF;
				lcd_print(WHITE, BLACK, 1, 200, ypos,"ErrCnt %i   ",g_tBattery_Info.ErrCnt); //g_tBattery_Info.usConverterPower_W);
				ypos += LINEDIFF;
				lcd_print(WHITE, BLACK, 1, 160, ypos,"Charge %i mAh   ",(g_tBattery_Info.unCharge_mAs/3600));
				ypos += LINEDIFF;
				lcd_print(WHITE, BLACK, 1, 160, ypos,"SlaveErr %i    ",(g_tBattery_Info.LastErr));
				ypos += LINEDIFF;

				char buf[10];
				switch (g_tBattery_Info.eState)
				{
					case eBattCharging:
						strcpy(buf,"Charging  ");
						break;
					case eBattFull:
						strcpy(buf,"Full      ");
						break;
					case eBattWaiting:
						strcpy(buf,"Waiting   ");
						break;
					case eBattUnknown:
						strcpy(buf,"Unknown   ");
						break;
					case eBattError:
						strcpy(buf,"Batt.Error");
						break;
					default:
						strcpy(buf,"Waaargh!!!");
						break;
				}
				lcd_print(GREEN,BLACK,2,0,180,buf);


				OS_GetTicks(&t2);

				t2=t2-t1;
				lcd_print(GREY, BLACK, FONTSIZE, 0, 220,"Time: %i ms     " ,(uint16_t)t2/100);
			}
		}
		else
		{
				//commError(glCommError);
		}

#endif



	}

}

/*
void touchtest(void)
{

	myP.max = 32000;
	myP.min = 0;

	//			for (i = 0; i < 3; i++)
	//			{
	//				lcd_print(WHITE, BLACK, 1, 0, 16*i,"val/t%i/tIdx/t%i      " ,maxVal[i],maxIdx[i]);
	//			}

//				static int16_t oypos;
//				int16_t nypos1;
//
//
//				//touchGetValue(&nypos1);
//
//
//				lcd_print(WHITE, BLACK, FONTSIZE, 0, 16,"VAL/t%i/tt      " ,nypos1);





//				lcd_draw_box( 255,255,255,315,nypos1,5,5);
//				if(nypos1 >0)
//				{
//
//					lcd_draw_box(255,255,255,315,nypos1/4,5,5);
//					oypos = nypos1/4;
//				}

	static uint16_t g;


				g++;

				lcd_draw_pixel(RED,g,(-myP.position/0xff)+160);
				lcd_draw_pixel(GREEN,g,myP.velocity/0xff+128);
				lcd_draw_pixel(YELLOW,g,myP.force/0xff+160);
				if (g == 320)
				{
					lcd_clear();
					g =0;
				}


				lcd_print(WHITE, BLACK, 1, 0, 32,"State: %i  " ,(uint16_t)g_debug);
				lcd_print(WHITE, BLACK, 1, 0, 64,"Gesture: %i  " ,(uint16_t)g_debug2);
				lcd_print(WHITE, BLACK, 1, 0, 96,"Particle: %i , %i   " ,(uint16_t)myP.position,(uint16_t)myP.velocity);


				lcd_print(WHITE, BLACK, 1, 280, 20,"%i  " ,g_aucTouchpads[0]);
				lcd_print(WHITE, BLACK, 1, 280, 60,"%i  " ,g_aucTouchpads[1]);
				lcd_print(WHITE, BLACK, 1, 280, 120,"%i  " ,g_aucTouchpads[2]);
				lcd_print(WHITE, BLACK, 1, 280, 180,"%i  " ,g_aucTouchpads[3]);
				lcd_print(WHITE, BLACK, 1, 280, 220,"%i  " ,g_aucTouchpads[4]);

				lcd_print(WHITE, BLACK, 1, 200, 120,"%i  " ,(int16_t) Schwerpunkt);


//				ypos =32;
//				lcd_print(WHITE, BLACK, FONTSIZE, 0, ypos,"P1/t%i      " ,g_aucTouchpads[0]);
//				ypos += LINEDIFF;
//				lcd_print(WHITE, BLACK, FONTSIZE, 0, ypos,"P2/t%i      " ,g_aucTouchpads[1]);
//				ypos += LINEDIFF;
//				lcd_print(WHITE, BLACK, FONTSIZE, 0, ypos,"P3/t%i      " ,g_aucTouchpads[2]);
}
*/

