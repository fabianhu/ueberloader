/*
	This is the Üeberloader Display Main

	(c) 2010 Fabian Huslik

*/

#include "../OS/FabOS.h"
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <string.h>

#include "../comm/usart.h"
#include "../comm/serial.h"
#include "../lcd/lcd.h"
#include "../menu/menu.h"
#include "../touch/touchpad.h"


extern Battery_Info_t g_tBattery_Info;
extern Command_t g_tCommand;
extern uint8_t glCommError; // fixme do better!!
extern uint16_t glTestMutexBlocked;

extern UCIFrame_t g_tUCIRXFrame;
extern uint8_t    g_ucRXLength;
extern uint8_t test;
extern uint16_t touchpads[TOUCHCOUNT];
extern particle_t myP;

		
// Defines for this file
#define DISPLAYTEST 0
#define TOUCHTEST 1

// *********  Task definitions
OS_DeclareTask(TaskTouch,200);
OS_DeclareTask(TaskDisplay,700);
OS_DeclareTask(TaskCommand,300);

//OS_DeclareQueue(DemoQ,10,4);

// *********  Prototypes
void CPU_init(void);
void emstop(uint8_t e);
void touchtest(void);
extern void DoParticle(void);
extern void process_touch(void);//change value
int16_t touchGetSchwerpunkt(void);


// Global variables
uint8_t g_bMenuActive = DISPLAYTEST; // 1 = the menue is active
static volatile uint16_t a,b,c,d;


// *********  THE main()
int main(void)
{
	CPU_init();

	OS_CreateTask(TaskTouch, OSTSKTouch);
    OS_CreateTask(TaskDisplay, OSTSKDisplay);
    OS_CreateTask(TaskCommand, OSTSKCommand);

	OS_CreateAlarm(OSALMWaitDisp,OSTSKDisplay);
	OS_CreateAlarm(OSALMCommandWait,OSTSKCommand);
	OS_CreateAlarm(OSALMCommandRepeat,OSTSKCommand);
	OS_CreateAlarm(OSALMCommandTimeout,OSTSKCommand);
	OS_CreateAlarm(OSALTouchRepeat,OSTSKTouch);
	OS_CreateAlarm(OSALTouchPause,OSTSKTouch);

	OS_StartExecution() ;
	while(1)
	{
		// THIS IS the idle task which will be preempted by all other tasks.
		// NO OS_Wait.. functions are allowed here!!!
		
		asm("nop"); //at least one instruction is required!!!

		if(PMIC.STATUS & 4 || !(PMIC.CTRL &4))
		{
			asm("break"); // just for finding error fixme
		}

#if OS_USEMEMCHECKS == 1
		a = OS_GetUnusedStack(OSTSKCommand);
		b = OS_GetUnusedStack(OSTSKTouch);
		c = OS_GetUnusedStack(OSTSKDisplay);
		d = OS_GetUnusedStack(OS_NUMTASKS);
#endif

	}
}


void TaskDisplay(void)
{

	uint16_t ypos=0;
	uint32_t t1,t2;
	//uint8_t i;

#define FONTSIZE 1
#define LINEDIFF FONTSIZE*16

	lcd_init(BPP24, ORIENTATION_0);

	menu_init();

	lcd_clear();//lcd clear needed here because a new screen is shown
	OS_WaitTicks(OSALMWaitDisp,333);
	lcd_clear();//lcd clear needed here because a new screen is shown

	//touchSetValue(0L,-100L,100L);
OS_GetTicks(&t1); // just for finding error fixme
	while(1)
	{


		ypos = 0;

		

#if TOUCHTEST == 1
		// the touch is tested here!

		touchtest(); 

		
				OS_GetTicks(&t2);

				t2=t2-t1;
				lcd_print(GREY, BLACK, FONTSIZE, 0, 220,"Time: %i ms     " ,(uint16_t)t2);
#else



		if(!glCommError || DISPLAYTEST == 1)
		{

			if (g_bMenuActive)
			{
				menu_show();
			}
			else
			{
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
				lcd_print(GREY, BLACK, FONTSIZE, 0, 220,"Time: %i ms     " ,(uint16_t)t2);
			}
		}
		else
		{
				//commError(glCommError);
		}

#endif


		OS_WaitTicks(OSALMWaitDisp,10);
		if(PMIC.STATUS & 4 || !(PMIC.CTRL &4))
		{
			asm("break"); // just for finding error fixme
		}
	}

}

void touchtest(void)
{
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

				//uint16_t sp = touchGetSchwerpunkt();
				//lcd_draw_pixel(GREEN,g,sp/2);

				lcd_draw_pixel(RED,g,(myP.position/0xff)+160);
				lcd_draw_pixel(GREEN,g,myP.velocity+128);
				lcd_draw_pixel(YELLOW,g,myP.force+160);
				if (g == 320)
				{
					lcd_clear();
					g =0;
				}





				/*ypos =32;
				lcd_print(WHITE, BLACK, FONTSIZE, 0, ypos,"P1/t%i      " ,touchpads[0]);
				ypos += LINEDIFF;
				lcd_print(WHITE, BLACK, FONTSIZE, 0, ypos,"P2/t%i      " ,touchpads[1]);
				ypos += LINEDIFF;
				lcd_print(WHITE, BLACK, FONTSIZE, 0, ypos,"P3/t%i      " ,touchpads[2]);*/
}

extern UCIFrame_t g_tUCIRXFrame;
extern uint8_t    g_ucRXLength;

uint8_t vWaitForResult(void)
{
	uint8_t ret;
	uint8_t commerror = 0;

	ret = OS_WaitEventTimeout(OSEVTDataRecvd,OSALMCommandTimeout, 150); // at 9600 baud it takes 105 ms to transfer the stuff.
    if(ret == OSEVTDataRecvd)
	{
		HandleSerial(&g_tUCIRXFrame);
	}
	else
	{
		commerror = 100;
	}
    g_ucRXLength = 0;
    g_tUCIRXFrame.len = UCIHEADERLEN;
    return commerror;
}

uint8_t g_NewComand =0; // indicates new command to be sent

void TaskCommand(void)
{
	OS_SetAlarm(OSALMCommandRepeat,1000);
	while(1)
	{
		OS_WaitAlarm(OSALMCommandRepeat);
		if(PMIC.STATUS & 4 || !(PMIC.CTRL &4))
		{
			asm("break");// just for finding error fixme
		}
		OS_SetAlarm(OSALMCommandRepeat,200);

		UCIFrame_t myU;

		myU.ID = 55;
		myU.UCI = UCI_GET_OPVs;
		myU.len = UCIHEADERLEN;
		UCISendBlockCrc(&myU);
	    glCommError = vWaitForResult();

	    if(g_NewComand)
	    {
	    	myU.ID = 55;
			myU.UCI = UCI_SET_CMDs;
			myU.len = UCIHEADERLEN+sizeof(Command_t);
	    	OS_MutexGet(OSMTXCommand);
				memcpy(myU.values,&g_tCommand,sizeof(Command_t));
	    	OS_MutexRelease(OSMTXCommand);
	    	UCISendBlockCrc(&myU);
	    	//OS_WaitTicks(OSALMCommandWait,20);
			//glCommError = vWaitForResult();
	    	g_NewComand = 0;
	    }

	    /*myU.ID = 55;
	    myU.UCI = UCI_GET_CMDs;
	    myU.len = UCIHEADERLEN;
	    UCISendBlockCrc(&myU);
	    glCommError = vWaitForResult();*/
	}
}

typedef struct eePars_tag
{
	uint16_t usMagic; // version number
	int16_t sCurrentSetpoint;
	uint16_t usMinBalanceVolt_mV;

}eepars_t;

eepars_t* ParMaster = (eepars_t*)0;
eepars_t* ParMirror = (eepars_t*)64;

void TaskTouch()
{
	OS_SetAlarm(OSALTouchRepeat,10);

#define TOUCHSENSELEVEL 15

	static uint8_t firstrun = 1;

	touch_init();
	while(1)
	{	
	
	if(firstrun && (eeprom_read_word(&ParMaster->usMagic)== 12312))
		{
			firstrun = 0;
			OS_PREVENTSCHEDULING;
			g_tCommand.sCurrentSetpoint = 800;
			g_tCommand.usMinBalanceVolt_mV = 2400;//eeprom_read_word(&(ParMaster->usMinBalanceVolt_mV));
			g_tCommand.usVoltageSetpoint_mV = 4150; //4150;
			g_tCommand.eChargerMode = eModeAuto;
			g_tCommand.ucUserCellCount = 0;
			OS_ALLOWSCHEDULING;
			}

	if(firstrun)
		{
			eeprom_write_word(&(ParMaster->usMagic),12312);
			eeprom_write_word(&(ParMaster->usMinBalanceVolt_mV),3000);
		}

		OS_WaitAlarm(OSALTouchRepeat);
		if(PMIC.STATUS & 4 || !(PMIC.CTRL &4))
		{
			asm("break"); // just for finding error fixme
		}
		OS_SetAlarm(OSALTouchRepeat,10); // every 10ms

		g_NewComand = 1;


		DoParticle();

//		if (g_bMenuActive)
//		{
//			touch();//change value
//		}
//		else
//		{
//			// check, if the menu is to be activated.
//			// fixme do it
//			touch(); // fixme do not change values here, but only look if menu is to be activated.
//		}

	}
}

// *********  Code to be executed inside Timer ISR used for the OS, defined in FabOS_config.h
void OS_CustomISRCode(void)
{
	TCC1.CNT=0;
	//TCC1.INTFLAGS = 0xff;

}


// *********  Controller initialisation
void CPU_init(void)
{
	// init OS timer and interrupt


	CCP = CCP_IOREG_gc; // unlock
	OSC.XOSCFAIL = OSC_XOSCFDEN_bm; // enable NMI for oscillator failure.

	// xtal = 16 MHz,
	// PLL (128 MHz) -> peripheral x4
	// Presc. B (64MHz) -> peripheral x2
	// Presc. C (32MHz) -> CPU
	OSC.XOSCCTRL = 0;
	OSC.CTRL |= OSC_RC2MEN_bm; // enable XTAL
	OSC.PLLCTRL = 0 | 8; // configure pll x 8;
	while (!(OSC.STATUS & OSC_RC2MRDY_bm))
	{
		asm("nop"); // wait for the bit to become set
	}
	OSC.CTRL |= OSC_PLLEN_bm; // enable PLL

	CCP = CCP_IOREG_gc; // unlock
	CLK.PSCTRL = CLK_PSADIV_1_gc|CLK_PSBCDIV_1_1_gc; 
	while (!(OSC.STATUS & OSC_PLLRDY_bm))
	{
		asm("nop"); // wait for the bit to become set
	}
	 
	CCP = CCP_IOREG_gc; // unlock
	CLK.CTRL = CLK_SCLKSEL_PLL_gc; // select PLL to run with
 
	// setup Timer for OS
	TCC1.CTRLA = TC_CLKSEL_DIV1_gc; // select clk for clock source
	TCC1.CTRLB = TC0_CCAEN_bm;
	TCC1.CTRLC = 0;
	TCC1.CTRLD = 0;
	TCC1.CTRLE = 0;
	TCC1.INTCTRLA = 0;
	TCC1.INTCTRLB = TC_CCAINTLVL_HI_gc; // enable compare match A // highest prio, NO other ISR using OS-API must interrupt this one!
	TCC1.CCA = 16000;//  gives 1ms clock @ 16 MHz

	//Enable Interrupts in INT CTRL
	PMIC.CTRL = PMIC_HILVLEN_bm|PMIC_MEDLVLEN_bm|PMIC_LOLVLEN_bm;

	USARTinit();

	// *** NO global interrupts enabled at this point!!!
}

ISR(OSC_XOSCF_vect)
{
	//oscillator failure
	asm("break");// just for finding error fixme
	emstop(99);// emergency stop here!
	CCP = CCP_IOREG_gc; // unlock
	RST.CTRL = 1; // SW reset

}


#if OS_USEEXTCHECKS == 1
void OS_ErrorHook(uint8_t ErrNo)
{
	static uint8_t dummy =0;
	
	switch(ErrNo)
	{
		case 2:
			// OS_WaitEvent: waiting in idle is not allowed
			break;	
		case 4:
			// OS_WaitAlarm: waiting in idle is not allowed
			break;
		case 5:
			// OS_MutexGet: invalid Mutex number
			break;
		case 6:
			// OS_MutexRelease: invalid Mutex number
			break;
		case 7:
			// OS_Alarm misconfiguration
			break;
		case 8:
			// OS_WaitAlarm: Alarm was not active
			break;
		case 9:
			// OS_WaitAlarm: Alarm is not assigned to the task (critical!)
			break;
		default:
			break;	
	}
	
	dummy = ErrNo; // dummy code

	#if OS_DO_TESTSUITE == 1
	asm("break"); // for automated tests of OS. may be removed in production code.
	#endif
}
#endif

void emstop(uint8_t e)
{
	cli(); // stop OS

	// Disable timer and Pins Port C
	TCC0.CTRLA = 0;
	TCC0.CTRLB = 0;
	PORTC.OUTCLR = 0b11111111;

	// Disable timer and Pins Port D
	TCD0.CTRLA = 0;
	TCD0.CTRLB = 0;
	PORTD.OUTCLR = 0b00000011;

	// Switch on LED BL
	PORTB.DIRSET = 0b0100;
	PORTB.OUTSET = 0b0100;

	while(1)
	{
		asm("nop");
	}

	CCP = CCP_IOREG_gc; // unlock
	RST.CTRL = 1; // SW reset

}
