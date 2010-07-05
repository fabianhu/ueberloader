/*
	This is the Üeberloader Display Main

	(c) 2010 Fabian Huslik

*/

#include "OS/FabOS.h"
#include <avr/pgmspace.h>
#include <string.h>

#include "usart.h"
#include "serial.h"
#include "lcd.h"
#include "menu.h"
#include "touchpad.h"

extern Battery_Info_t g_tBattery_Info;
extern uint8_t glCommError; // fixme do better!!

// *********  Task definitions
OS_DeclareTask(TaskDisplay,700);
OS_DeclareTask(TaskCommand,300);
//OS_DeclareTask(TaskCommRX,200);
OS_DeclareTask(TaskMonitor,200);

//OS_DeclareQueue(DemoQ,10,4);

// *********  Prototypes
void CPU_init(void);

void emstop(uint8_t e);


// Global variables


// *********  THE main()
int main(void)
{
	CPU_init();

	lcd_init();

	menu_init();

    OS_CreateTask(TaskDisplay, 0);
    OS_CreateTask(TaskCommand, 1);
	OS_CreateTask(TaskMonitor, 2);
    //OS_CreateTask(TaskCommRX, 2);

	OS_CreateAlarm(OSALMWaitDisp,OSTSKDisplay);
	OS_CreateAlarm(OSALMCommandWait,OSTSKCommand);
	OS_CreateAlarm(OSALMCommandRepeat,OSTSKCommand);
	OS_CreateAlarm(OSALMCommandTimeout,OSTSKCommand);
	OS_CreateAlarm(OSALMonitorRepeat,OSTSKMonitor);

	OS_StartExecution() ;
	while(1)
	{
		// THIS IS the idle task which will be preempted by all other tasks.
		// NO OS_Wait.. functions are allowed here!!!
		
		asm("nop"); //at least one instruction is required!!!
	}

}

void TaskDisplay(void)
{
	uint16_t ypos=0;
	uint32_t t1,t2;
	uint8_t i;

#define FONTSIZE 1
#define LINEDIFF FONTSIZE*16

	touch_init();

	lcd_clear();//lcd clear needed here because a new screen is shown
	lcd_show_init_screen();
	OS_WaitTicks(OSALMWaitDisp,1000);
	lcd_clear();//lcd clear needed here because a new screen is shown

	while(1)
	{
		ypos = 0;

		OS_GetTicks(&t1);

		//fixme for lcd_print function test
		if(!glCommError)
		{
			//the lcd_print function overwrites old text-> no lcd_clear needed!
			//lcd_clear();
			lcd_print(YELLOW, BLACK, 2, 0, ypos,"vBatt  /t%i mV    ",g_tBattery_Info.usActVoltage_mV);
			ypos += LINEDIFF*2;
			lcd_print(YELLOW, BLACK, 2, 0, ypos,"Current/t%i mA   ",g_tBattery_Info.sActCurrent_mA);
			ypos += LINEDIFF*2;
			lcd_print(WHITE, BLACK, FONTSIZE, 230, 200,"Batt. %i Cells ",g_tBattery_Info.ucNumberOfCells);

			/*ypos = 100;
			lcd_print(WHITE, BLACK, FONTSIZE, 0, ypos,"Pin0/t%i/tt      " ,touchGetPad(0));
			ypos += LINEDIFF;
			lcd_print(WHITE, BLACK, FONTSIZE, 0, ypos,"Pin1/t%i/tt      " ,touchGetPad(1));
			ypos += LINEDIFF;
			lcd_print(WHITE, BLACK, FONTSIZE, 0, ypos,"Pin2/t%i/tt      " ,touchGetPad(2));
			ypos += LINEDIFF;
			lcd_print(WHITE, BLACK, FONTSIZE, 0, ypos,"Pin3/t%i/tt      " ,touchGetPad(3));
			ypos += LINEDIFF;
			lcd_print(WHITE, BLACK, FONTSIZE, 0, ypos,"Pin4/t%i/tt      " ,touchGetPad(4));
*/
			ypos = 64;
			
			for (i = 0; i < 6; ++i) {
			lcd_print(WHITE, BLACK, FONTSIZE, 0,ypos,"Cell%i/t%i/tmV/t%i mAh     " ,i,g_tBattery_Info.Cells[i].usVoltage_mV, g_tBattery_Info.Cells[i].unDisChTicks);
			ypos += LINEDIFF;
			}

			ypos = 64;
			lcd_print(WHITE, BLACK, 1, 200, ypos,"PWM %i   ",g_tBattery_Info.usPWM);
			ypos += LINEDIFF;
			lcd_print(WHITE, BLACK, 1, 200, ypos,"ASYNC %i   ",g_tBattery_Info.usPWMStep);
			ypos += LINEDIFF;
			lcd_print(WHITE, BLACK, 1, 200, ypos,"ConvPwr %i   ",g_tBattery_Info.usConverterPower_W);
			ypos += LINEDIFF;
			lcd_print(WHITE, BLACK, 1, 200, ypos,"Setp %i mA   ",g_tBattery_Info.sISetpoint);
			ypos += LINEDIFF;
			lcd_print(WHITE, BLACK, 1, 200, ypos,"diff %i mA   ",g_tBattery_Info.sDiff);
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
				case eBattUnknown:
					strcpy(buf,"Waiting   ");
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
		else
		{
			commError(glCommError);

		}


		OS_WaitTicks(OSALMWaitDisp,100);
	}

}

extern UCIFrame_t g_tUCIRXFrame;
extern uint8_t    g_ucRXLength;
extern uint16_t glCommerrcnt; // fixme remove!

extern uint8_t vWaitForResult( void)
{
	uint8_t ret;
	uint8_t commerror = 0;
	ret = OS_WaitEventTimeout(OSEVTDataRecvd,OSALMCommandTimeout, 200);
    if(ret == OSEVTDataRecvd)
		{
			HandleSerial(&g_tUCIRXFrame);
		}
		else
		{
			commerror = 22;
			glCommerrcnt++; // fixme remove!
		}
    g_ucRXLength = 0;
    g_tUCIRXFrame.len = UCIHEADERLEN;
    return commerror;
}

void TaskCommand(void)
{
	OS_SetAlarm(OSALMCommandRepeat,1000);
	while(1)
	{
		OS_WaitAlarm(OSALMCommandRepeat);
		OS_SetAlarm(OSALMCommandRepeat,1000);

		UCIFrame_t myU;

		myU.ID = 55;
		myU.UCI = UCI_GET_OPVs;
		myU.len = UCIHEADERLEN;
		UCISendBlockCrc(&myU);
	    glCommError = vWaitForResult();

	    myU.ID = 55;
	    myU.UCI = UCI_GET_CMDs;
	    myU.len = UCIHEADERLEN;
	    UCISendBlockCrc(&myU);
	    glCommError = vWaitForResult();

	}
}


void TaskMonitor()
{
		
	while(1)
	{
		OS_WaitTicks(OSALMonitorRepeat,102);
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
	TCC1.INTCTRLB = TC_CCAINTLVL_HI_gc; // enable compare match A
	TCC1.CCA = 16000;// compare at 32000 gives 1ms clock

	//Enable Interrupts in INT CTRL
	PMIC.CTRL = PMIC_HILVLEN_bm|PMIC_MEDLVLEN_bm|PMIC_LOLVLEN_bm;

	USARTinit();


	// *** NO global interrupts enabled at this point!!!



}

ISR(OSC_XOSCF_vect)
{
	// oscillator failure
	emstop(99);// emergency stop here!
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
		default:
			break;	
	}
	
	dummy = ErrNo; // dummy code
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
