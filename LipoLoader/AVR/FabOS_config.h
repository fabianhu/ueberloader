/*
	FabOS for ATMEL AVR user configuration file
	
	(c) 2008-2010 Fabian Huslik
*/

// *********  USER Configurable Block BEGIN

#define OS_NUMTASKS  6 // Number of (OS_Create)Tasks ; never >8 (idle task is not counted here!)
#define OS_NUMMUTEX  3 // Number of Mutexes
#define OS_NUMALARMS 8 // Number of Alarms

#if defined (__AVR_ATmega32__)
	#define OS_ScheduleISR 			TIMER1_COMPA_vect // Interrupt Vector used for OS-tick generation (check out CustomOS_ISRCode if you want to add isr code)
	#define OS_ALLOWSCHEDULING 		TIMSK |= (1<<OCIE1A);	// turn Timer Interrupt ON
	#define OS_PREVENTSCHEDULING 	TIMSK &= ~(1<<OCIE1A); // turn Timer Interrupt OFF
#elif defined (__AVR_ATmega644P__)
	#define OS_ScheduleISR 			TIMER1_COMPA_vect
	#define OS_ALLOWSCHEDULING 		TIMSK1 |= (1<<OCIE1A);	// turn Timer Interrupt ON
	#define OS_PREVENTSCHEDULING 	TIMSK1 &= ~(1<<OCIE1A); // turn Timer Interrupt OFF
#elif defined (__AVR_ATxmega32A4__)
	#define OS_ScheduleISR 			TCC1_CCA_vect
	#define OS_ALLOWSCHEDULING 		sei();//TCC1.INTCTRLB |= 3 ;//;	// turn Timer Interrupt ON
	#define OS_PREVENTSCHEDULING 	cli();//TCC1.INTCTRLB &= ~3 ; // turn Timer Interrupt OFF
#else
	#error "MCU Timer ISR not defined. Set correct ISR vector in FabOS_config.h"
#endif


#define OS_USECLOCK 1 		// Use "OS_GetTicks()" which returns a 32bit timer tick
#define OS_USECOMBINED 1 	// Use "OS_WaitEventTimeout()" which is easier to use, than combining alarms and events to get the functionality.
#define OS_USEEXTCHECKS 1	// check wrong usage of OS API -> does not work, but no damage to OS stability.
#define OS_USEMEMCHECKS 1 	// Enable "OS_get_unused_Stack()" and "OS_GetQueueSpace()"
#define OS_UNUSEDMASK 0xAA  // unused Stack RAM will be filled with this byte, if OS_USEMEMCHECKS == 1.
#define OS_TRACE_ON  0 		// enable trace to OS_Tracebuffer[]
#define OS_TRACESIZE 1000	// size of OS_Tracebuffer[] (depending on memory left ;-)

#define OS_TypeAlarmTick_t uint16_t // change this type to uint32_t, if you need longer wait time than 65535 OS ticks.

// Task definitions

#define OSTSKGovernor 0
#define OSTSKBalance 1
#define OSTSKCommRX 2
#define OSTSKMonitor 3
#define OSTSKLED 4
#define OSTSKState 5

// Event Names

#define OSEVTDataRecvd 1
#define OSEVTState 1

// Mutex Names

#define OSMTXBattInfo 0
#define OSMTXCommand  1
#define OSMTXIntern   2 // todo unused

// Alarm Names

#define OSALMCommTimeout  0
#define OSALMWaitGov      1
#define OSALMBalRepeat    2
#define OSALMBalWait      3
#define OSALMonitorRepeat 4
#define OSALMLEDRepeat    5
#define OSALMLEDWait      6
#define OSALMStateWait    7

// *********  USER Configurable Block END 

#define OS_DO_TESTSUITE 0	// compile and execute the automated software tests. set to 0 for production use of OS.
