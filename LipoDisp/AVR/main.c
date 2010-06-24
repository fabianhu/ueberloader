/*
	This is the Üeberloader Display Main

	(c) 2010 Fabian Huslik

*/

#include "OS/FabOS.h"
#include <avr/pgmspace.h>

#include "usart.h"
#include "serial.h"
#include "lcd.h"
#include "menu.h"
#include "touchpad.h"


// *********  Task definitions
OS_DeclareTask(TaskDisplay,500);
OS_DeclareTask(TaskCommand,200);
OS_DeclareTask(TaskCommRX,200);
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
    OS_CreateTask(TaskCommRX, 2);
	OS_CreateTask(TaskMonitor, 3);

	OS_CreateAlarm(OSALMWaitDisp,OSTSKDisplay);
	OS_CreateAlarm(OSALMCommandWait,OSTSKCommand);
	OS_CreateAlarm(OSALMCommandRepeat,OSTSKCommand);
	OS_CreateAlarm(OSALMCommTimeout,OSTSKCommRX);
	OS_CreateAlarm(OSALMonitorRepeat,OSTSKMonitor);

	OS_StartExecution() ;
	while(1)
	{
		// THIS IS the idle task which will be preempted by all other tasks.
		// NO OS_Wait.. functions are allowed here!!!
		
		asm("nop"); //at least one instruction is required!!!
	}

}

extern char font[96][36] PROGMEM;

uint16_t lcd_get_text_len(char *text, uint8_t size)
{
	uint8_t *ptr_letter;
	uint16_t total_width=0;
	while(*text!=0x00)
	{
		//get letter width
		ptr_letter = font[*text-32];
		total_width += pgm_read_byte(ptr_letter)+size;
		//next letter
		text++;
	}
	return total_width*size;
}

void lcd_write_value_after_text_w_cl(char* text,uint16_t value, char* unit, uint8_t r,uint8_t g,uint8_t b, uint8_t size, uint16_t xpos, uint16_t ypos)
{
	uint16_t ret,ret2;
	char buf[8];
	ret = lcd_get_text_len(text,size);
	itoa(value,buf,10);
	ret2 = lcd_get_text_len(buf,size);
	ret2 += lcd_get_text_len(unit,size);
	lcd_draw_filled_box(0,0,0,ret,ypos,ret2+size*10,size*16); // size*10 is spare, if next number is shorter.
	ret = lcd_write_ram_text(buf,r,g,b,size,ret,ypos);
	lcd_write_ram_text(unit,r,g,b,size,ret,ypos);

}

void TaskDisplay(void)
{
	uint16_t ypos=0;
	uint32_t t1,t2;

#define FONTSIZE 2
#define LINEDIFF FONTSIZE*16

	touch_init();

	lcd_show_init_screen();
	OS_WaitTicks(OSALMWaitDisp,500);
	lcd_clear();

	lcd_write_ram_text("Pin0 ",255,255,255,FONTSIZE,0,ypos);
	ypos += LINEDIFF;
	lcd_write_ram_text("Pin1 ",255,255,255,FONTSIZE,0,ypos);
	ypos += LINEDIFF;
	lcd_write_ram_text("Pin2 ",255,255,255,FONTSIZE,0,ypos);
	ypos += LINEDIFF;
	lcd_write_ram_text("Pin3 ",255,255,255,FONTSIZE,0,ypos);
	ypos += LINEDIFF;
	lcd_write_ram_text("Pin4 ",255,255,255,FONTSIZE,0,ypos);
	ypos += LINEDIFF;
	ypos += LINEDIFF;
	lcd_write_ram_text("Time ",255,255,255,FONTSIZE,0,ypos);


	while(1)
	{
		ypos = 0;

		OS_GetTicks(&t1);

		lcd_write_value_after_text_w_cl("Pin0 ",touchGetPad(0)," touchs",255,255,255,FONTSIZE,0,ypos);
		ypos += LINEDIFF;
		lcd_write_value_after_text_w_cl("Pin1 ",touchGetPad(1)," touchs",255,255,255,FONTSIZE,0,ypos);
		ypos += LINEDIFF;
		lcd_write_value_after_text_w_cl("Pin2 ",touchGetPad(2)," touchs",255,255,255,FONTSIZE,0,ypos);
		ypos += LINEDIFF;
		lcd_write_value_after_text_w_cl("Pin3 ",touchGetPad(3)," touchs",255,255,255,FONTSIZE,0,ypos);
		ypos += LINEDIFF;
		lcd_write_value_after_text_w_cl("Pin4 ",touchGetPad(4)," touchs",255,255,255,FONTSIZE,0,ypos);
		ypos += LINEDIFF;
		ypos += LINEDIFF;

		OS_GetTicks(&t2);

		t2=t2-t1;
		lcd_write_value_after_text_w_cl("Time ",(uint16_t)t2," ms",255,255,255,FONTSIZE,0,ypos);



		OS_WaitTicks(OSALMWaitDisp,33);
		//lcd_clear();

		/*
		lcd_show_init_screen();
		OS_WaitTicks(OSALMWaitDisp,2001);
		menu_show();
		OS_WaitTicks(OSALMWaitDisp,2001);
		menu_select();
		OS_WaitTicks(OSALMWaitDisp,501);
		menu_next(1);
		OS_WaitTicks(OSALMWaitDisp,501);
		menu_next(1);
		OS_WaitTicks(OSALMWaitDisp,501);
		menu_next(1);
		OS_WaitTicks(OSALMWaitDisp,501);
		menu_next(1);
		OS_WaitTicks(OSALMWaitDisp,501);
		menu_select();
		OS_WaitTicks(OSALMWaitDisp,501);
		menu_next(1);
		OS_WaitTicks(OSALMWaitDisp,501);
		menu_next(1);
		OS_WaitTicks(OSALMWaitDisp,501);
		menu_select();
		OS_WaitTicks(OSALMWaitDisp,501);
		menu_select();
		OS_WaitTicks(OSALMWaitDisp,65535);
*/
	}

}

void TaskCommand(void)
{
	while(1)
	{
		OS_WaitTicks(OSALMCommandRepeat,1000);

		UCIFrame_t myU;

		myU.ID = 55;
		myU.UCI = UCI_GET_OPVs;
		myU.len = UCIHEADERLEN;
		UCISendBlockCrc(&myU);

		OS_WaitTicks(OSALMCommandWait,100);

		myU.ID = 55;
		myU.UCI = UCI_GET_CMDs;
		myU.len = UCIHEADERLEN;
		UCISendBlockCrc(&myU);

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
