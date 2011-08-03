#include "../OS/FabOS.h"
#include "../lcd/lcd.h"
#include "../menu/menu.h"
#include "../touch/touchpad.h"
#include "../comm/serial.h"
#include "../menu/menu_variant.h"
#include <string.h>
#include <stdio.h>

extern void menu_init(void);

extern void handleCommError(uint8_t errNo);
extern uint8_t getLastCommError(void);
extern uint16_t GetvalueFromUI(void);
extern void HandOverValueToUI(uint16_t value, uint16_t upper, uint16_t lower, uint16_t stepsize);
extern void touchtest(void);

extern uint8_t CommErrArr[]; // fixme debug only!!!
extern uint8_t CommErrArrIdx;

extern particle_t myP; // touchpad.c
extern Battery_Info_t g_tBattery_Info; // serial_master.c
extern uint8_t g_GotNewComand; // command.c
extern Command_t g_tCommand; // command.c
extern uint8_t gCommandsKnown; // command.c

uint8_t g_bMenuActive = 0;//DISPLAYTEST; // 1 = the menue is active

#define DISPLAYTEST 0
#define TOUCHTEST 1


char buf[10];
char sbuf[50];

void TaskDisplay(void)
{

	uint16_t ypos = 0;
	uint32_t t1, t2;// just for finding error fixme
	uint8_t i;
	static uint8_t oCMDsKnown =0;

#define FONTSIZE 1
#define LINEDIFF FONTSIZE*16

	lcd_init( BPP24, ORIENTATION_0);

	menu_init();

	lcd_clear();//lcd clear needed here because a new screen is shown
	OS_SetAlarm( OSALMWaitDisp, 1000 );

	OS_GetTicks( &t1 ); // just for finding error fixme
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
		OS_WaitAlarm( OSALMWaitDisp );
		OS_SetAlarm( OSALMWaitDisp, 500 );

		if(g_GotNewComand) // fixme was hat das hier verloren?
		{
			// gespeicherte Commands vom Slave in Parameter eintragen.
			OS_MutexGet( OSMTXCommand );

			parCurrent.sValue = g_tCommand.sCurrentSetpoint;
			parVoltage.sValue = g_tCommand.usVoltageSetpoint_mV;
			parCellCount.sValue = g_tCommand.ucUserCellCount;

			parMaxcap.sValue = g_tCommand.unQ_max_mAs / 3600; // mAs in mAh
			parMaxtime.sValue = g_tCommand.usT_max_s;
			parBalActVolt.sValue = g_tCommand.usMinBalanceVolt_mV;

			parPWMfrequency.sValue = g_tCommand.basefrequency;
			parRefreshPeriod.sValue = g_tCommand.refreshrate;
			parBalActVolt.sValue = g_tCommand.usMinBalanceVolt_mV;

			OS_MutexRelease( OSMTXCommand );

			g_GotNewComand = 0; // reset it.
		}

		static uint8_t bMenuCleared = 0;

		if(getLastCommError() == 0 || DISPLAYTEST == 1)
		{
			if(g_bMenuActive)
			{
				if(bMenuCleared == 0)
				{
					lcd_clear();
					bMenuCleared = 1;
				}

				menu_show();

				// debug
				lcd_print( WHITE, BLACK, 1, 150, 200,"Particle: %i , %i   " ,(uint16_t)myP.position,(uint16_t)myP.velocity);
			}
			else
			{
				if (gCommandsKnown)
				{
					
					if (oCMDsKnown == 0)
					{
							HandOverValueToUI(	parCurrent.sValue,
							parCurrent.sUpperLimit,
							parCurrent.sLowerLimit,
							parCurrent.sStepSize
							);
					}
					
					
					// update charge current form particle.
					parCurrent.sValue = GetvalueFromUI(); // fixme
					g_tCommand.sCurrentSetpoint = parCurrent.sValue;
				}
				oCMDsKnown = gCommandsKnown;

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

				for (i = 0; i < 6; ++i)
				{
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
				lcd_print(WHITE, BLACK, 1, 200, ypos,"Setp %i mV   ",g_tBattery_Info.sVSetpoint);
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
					strcpy(buf,"Charging    ");
					break;
					case eBattFull:
					strcpy(buf,"Full        ");
					break;
					case eBattWaiting:
					strcpy(buf,"Waiting     ");
					break;
					case eBattUnknown:
					strcpy(buf,"Unknown     ");
					break;
					case eBattError:
					strcpy(buf,"Batt.Error  ");
					break;
					default:
					strcpy(buf,"Waaargh!!!  ");
					break;
				}
				lcd_print(GREEN,BLACK,2,0,180,buf);

				OS_GetTicks(&t2);

				t2=t2-t1;
				lcd_print(GREY, BLACK, FONTSIZE, 0, 220,"Time: %i s     " ,(uint16_t)t2/1000);

				lcd_print(WHITE, BLACK, FONTSIZE, 230, 220,"I set: %i",g_tCommand.sCurrentSetpoint);

			}
		}
		else
		{
			lcd_clear();
			lcd_print(RED,BLACK,2,0,180, "Comm Err %i  ",getLastCommError());
			lcd_print(RED,BLACK,1,0,210, "Total Errors %i  ",CommErrArrIdx);


			sbuf[0]=0;
			for (i = 0; i< CommErrArrIdx;i++)
			{
				sprintf(buf, " %i",CommErrArr[i]);
				strcat(sbuf,buf);
			}
			lcd_print(WHITE,BLACK,1,0,30,sbuf);

		}
#endif
	}
}

