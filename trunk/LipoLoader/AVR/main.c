/*
	This is the Üeberloader Main

	(c) 2010 Fabian Huslik

*/

#include "OS/FabOS.h"
#include "ueberloader.h"
#include "adc.h"
#include "usart.h"

// *********  Task definitions
OS_DeclareTask(TaskGovernor,300);
OS_DeclareTask(TaskBalance,300);
OS_DeclareTask(TaskCommRX,300);
OS_DeclareTask(TaskMonitor,200);
OS_DeclareTask(TaskLED,150);
OS_DeclareTask(TaskState,150);

//OS_DeclareQueue(DemoQ,10,4);

#include "serial.h"
extern Battery_Info_t g_tBattery_Info;
extern uint16_t glTestMutexBlocked;

extern uint8_t gCommErr;
extern uint16_t gCommErrCnt;

// *********  Prototypes
void CPU_init(void);

void emstop(uint8_t e);


// Global variables

Calibration_t g_tCalibration;

uint16_t UnusedStackSpace[OS_NUMTASKS+1];

// *********  THE main()
int main(void)
{
	CPU_init();

    uint8_t i;

	OS_CreateTask(TaskGovernor, OSTSKGovernor);
    OS_CreateTask(TaskBalance, OSTSKBalance);
    OS_CreateTask(TaskCommRX, OSTSKCommRX);
	OS_CreateTask(TaskMonitor, OSTSKMonitor);
	OS_CreateTask(TaskLED, OSTSKLED);
	OS_CreateTask(TaskState, OSTSKState);

	OS_CreateAlarm(OSALMBalRepeat, OSTSKBalance);
	OS_CreateAlarm(OSALMBalWait, OSTSKBalance);
	OS_CreateAlarm(OSALMCommTimeout, OSTSKCommRX);
	OS_CreateAlarm(OSALMWaitGov, OSTSKGovernor);
	OS_CreateAlarm(OSALMonitorRepeat, OSTSKMonitor);
	OS_CreateAlarm(OSALMLEDRepeat, OSTSKLED);
	OS_CreateAlarm(OSALMLEDWait, OSTSKLED);
	OS_CreateAlarm(OSALMStateWait, OSTSKState);

	OS_StartExecution() ;
	while(1)
	{
		// THIS IS the idle task which will be preempted by all other tasks.
		// NO OS_Wait.. functions are allowed here!!!
		
		// TODO add your code here
		asm("nop"); //at least one instruction is required!!!

		g_tBattery_Info.ErrCnt = gCommErrCnt;
		g_tBattery_Info.LastErr = gCommErr;


		for (i = 0; i <= OS_NUMTASKS; ++i)
		{
			UnusedStackSpace[i] = OS_GetUnusedStack(i);
		}

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
	OSC.XOSCCTRL = OSC_FRQRANGE_12TO16_gc | OSC_XOSCSEL_XTAL_16KCLK_gc;
	OSC.CTRL |= OSC_XOSCEN_bm; // enable XTAL
	OSC.PLLCTRL = OSC_PLLSRC_XOSC_gc | 8; // configure pll x 8;
	while (!(OSC.STATUS & OSC_XOSCRDY_bm))
	{
		asm("nop"); // wait for the bit to become set
	}
	OSC.CTRL |= OSC_PLLEN_bm; // enable PLL

	CCP = CCP_IOREG_gc; // unlock
	CLK.PSCTRL = CLK_PSADIV_1_gc|CLK_PSBCDIV_2_2_gc; 
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
	TCC1.CCA = 32000;// compare at 32000 gives 1ms clock

	//Enable Interrupts in INT CTRL
	PMIC.CTRL = PMIC_HILVLEN_bm|PMIC_MEDLVLEN_bm|PMIC_LOLVLEN_bm;

	USARTinit();

	// invert RX pin
	// not nec. PORTE.PIN2CTRL = 1<<6; //INVEN;

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

void emstop(uint8_t e) // fixme emstop reason speichern!
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

	// Light up LED
	PORTD.DIRSET = 0b1000;
	PORTD.OUTSET = 0b1000;

	while(1)
	{
		asm("nop");
	}

	CCP = CCP_IOREG_gc; // unlock
	RST.CTRL = 1; // SW reset

}

#define LEDMAXTIME 20
#define LEDRATIO 8

void TaskLED(void)
{
	static uint8_t i=0;



	OS_SetAlarm(OSALMLEDRepeat,LEDMAXTIME);
	while(1)
	{
		OS_WaitAlarm(OSALMLEDRepeat);
		OS_SetAlarm(OSALMLEDRepeat,LEDMAXTIME);


		switch (g_tBattery_Info.eState)
		{
			case eBattWaiting:
				// nicht vollständig angesteckt
				LED_ON;
				OS_WaitTicks(OSALMLEDWait,50);
				LED_OFF;
				OS_WaitTicks(OSALMLEDWait,2000-LEDMAXTIME-50);
				break;
			case eBattUnknown:
				// nicht vollständig angesteckt
				LED_ON;
				OS_WaitTicks(OSALMLEDWait,200);
				LED_OFF;
				OS_WaitTicks(OSALMLEDWait,200);
				break;
			case eBattCharging:
				// Charging!

				LED_ON;
				i++;

				OS_WaitTicks(OSALMLEDWait,i/LEDRATIO/2);
				if (i>LEDMAXTIME*LEDRATIO) i=0;

				LED_OFF;

				break;

	//		case eBattEmpty:
	//			break;

			case eBattFull:
				LED_ON;
				break;
			case eBattError:
				LED_ON;
				OS_WaitTicks(OSALMLEDWait,250);
				LED_OFF;
				OS_WaitTicks(OSALMLEDWait,250);
				break;
			default:
				LED_OFF;
				break;

		}


	}

}
