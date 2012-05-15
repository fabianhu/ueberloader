/*
	This is the Üeberloader Display Main

	(c) 2010 Fabian Huslik

	BUILD WITH avr-toolchain-installer-3.0.0.240-win32.win32.x86.exe 

*/

// fixme Automatische Tastensperre beim Laden
// Strom Setpoint ist global der gleiche

#include "../OS/FabOS.h"
#include <avr/eeprom.h>

#include "../comm/usart.h"

// *********  Task definitions
OS_DeclareTask(TaskTouch,300);
OS_DeclareTask(TaskDisplay,700);
OS_DeclareTask(TaskCommand,400);

//OS_DeclareQueue(DemoQ,10,4);

// *********  Prototypes
void CPU_init(void);
void emstop(uint8_t e);
//void touchtest(void);
//extern void handleCommError(uint8_t errNo);
//extern uint8_t getLastCommError(void);

// Global variables
static volatile uint16_t a,b,c,d; // stack space remaining

uint8_t g_debug,g_debug2,g_debug3;
int16_t g_debug4;

// *********  THE main()
int main(void)
{
	CPU_init();

	OS_CreateTask(TaskTouch, OSTSKTouch);
    OS_CreateTask(TaskDisplay, OSTSKDisplay);
    OS_CreateTask(TaskCommand, OSTSKCommand);

	OS_CreateAlarm(OSALMWaitDisp,OSTSKDisplay);
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

#if OS_USEMEMCHECKS == 1
		a = OS_GetUnusedStack(OSTSKCommand);
		b = OS_GetUnusedStack(OSTSKTouch);
		c = OS_GetUnusedStack(OSTSKDisplay);
		d = OS_GetUnusedStack(OS_NUMTASKS);
#endif

	}
}


// *********  Code to be executed inside Timer ISR used for the OS, defined in FabOS_config.h
void OS_CustomISRCode(void)
{
	TCC1_CNT=0;
	//TCC1.INTFLAGS = 0xff;

}


// *********  Controller initialisation
void CPU_init(void)
{
	// init OS timer and interrupt


	CCP = CCP_IOREG_gc; // unlock
	OSC_XOSCFAIL = OSC_XOSCFDEN_bm; // enable NMI for oscillator failure.

	// Desired Clock : 16MHz,
	// PLL (16 MHz) -> peripheral x1
	// Presc. B (16MHz) -> peripheral x1
	// Presc. C (16MHz) -> CPU
	//OSC_XOSCCTRL = 0;
	OSC_CTRL = OSC_RC2MEN_bm; // enable XTAL
	OSC_PLLCTRL = OSC_PLLSRC_RC2M_gc | 8; // configure pll x 8; (min 10MHz!)
	while (!(OSC_STATUS & OSC_RC2MRDY_bm))
	{
		asm("nop"); // wait for the bit to become set
	}
	OSC_DFLLCTRL = OSC_RC2MCREF_bm; // enable auto calib.
	DFLLRC2M_CTRL = DFLL_ENABLE_bm;

	OSC_CTRL |= OSC_PLLEN_bm; // enable PLL

	CCP = CCP_IOREG_gc; // unlock
	CLK_PSCTRL = CLK_PSADIV_1_gc|CLK_PSBCDIV_1_1_gc;
	while (!(OSC_STATUS & OSC_PLLRDY_bm))
	{
		asm("nop"); // wait for the bit to become set
	}
	 
	CCP = CCP_IOREG_gc; // unlock
	CLK_CTRL = CLK_SCLKSEL_PLL_gc; // select PLL to run with
 

	// setup Timer for OS
	TCC1_CTRLA = TC_CLKSEL_DIV1_gc; // select clk for clock source
	TCC1_CTRLB = TC0_CCAEN_bm;
	TCC1_CTRLC = 0;
	TCC1_CTRLD = 0;
	TCC1_CTRLE = 0;
	TCC1_INTCTRLA = 0;
	TCC1_INTCTRLB = TC_CCAINTLVL_HI_gc; // enable compare match A // highest prio, NO other ISR using OS-API must interrupt this one!
	TCC1_CCA = 16000;//  gives 1ms clock @ 16 MHz

	//Enable Interrupts in INT CTRL
	PMIC_CTRL = PMIC_HILVLEN_bm; //|PMIC_MEDLVLEN_bm|PMIC_LOLVLEN_bm;

	USARTinit();


	// adjust Power Reduction Register
	PR.PRGEN = 0b00011100;
	PR.PRPA = 0b00000111; //
	PR.PRPB = 0b00000111; //
	PR.PRPC = 0b01111001; // tcc1 used
	PR.PRPD = 0b01111011; //
	PR.PRPE = 0b01101000; // usarte0 used

	// *** NO global interrupts enabled at this point!!!
}

ISR(OSC_XOSCF_vect)
{
	//oscillator failure
	emstop(99);// emergency stop here!
	CCP = CCP_IOREG_gc; // unlock
	RST_CTRL = 1; // SW reset

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
//	asm("break"); // for automated tests of OS. may be removed in production code.
	#if OS_DO_TESTSUITE == 1
	asm("break"); // for automated tests of OS. may be removed in production code.
	#endif
}
#endif

void emstop(uint8_t e)
{
	cli(); // stop OS

	// Disable timer and Pins Port C
	TCC0_CTRLA = 0;
	TCC0_CTRLB = 0;
	PORTC_OUTCLR = 0b11111111;

	// Disable timer and Pins Port D
	TCD0_CTRLA = 0;
	TCD0_CTRLB = 0;
	PORTD_OUTCLR = 0b00000011;

	// Switch on LED BL
	PORTB_DIRSET = 0b0100;
	PORTB_OUTSET = 0b0100;

	while(1)
	{
		asm("nop");
	}

	CCP = CCP_IOREG_gc; // unlock
	RST_CTRL = 1; // SW reset

}


