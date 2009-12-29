/*

The Üeberloader

*/
#include "OS/FabOS.h"
#include "adc.h"

// *********  Task definitions
OS_DeclareTask(Task1,200);
OS_DeclareTask(Task2,200);
OS_DeclareTask(Task3,200);

OS_DeclareQueue(DemoQ,10,4);

// *********  Prototypes
void CPU_init(void);

#define PWMA_ON 	TCCR1A |=  0b10000000; 
#define PWMA_OFF 	TCCR1A &= ~0b11000000; PORTD &= ~(1<<PD5); //OCR1A = 0;
#define PWMA_ST_ON 	TCCR1A &= ~0b11000000; PORTD |= (1<<PD5); //OCR1A = 0;

#define PWMB_ON 	TCCR1A |=  0b00110000;
#define PWMB_OFF 	TCCR1A &= ~0b00110000; PORTD |= (1<<PD4); //OCR1B = 0;

void emstop(uint8_t e)
{
	cli(); // stop OS
			
	PWMA_OFF;
	PWMB_OFF;
	while(1)
	{
		asm("nop");
	}
}


// *********  THE main()
int main(void)
{
	CPU_init();

    OS_CreateTask(Task1, 0);
    OS_CreateTask(Task2, 1);
    OS_CreateTask(Task3, 2);

	OS_StartExecution() ;
	while(1)
	{
		// THIS IS the idle task which will be preemted by all other tasks.
		// NO OS_Wait.. functions are allowed here!!!
		
		// TODO add your code here
		asm("nop");
	}

}

extern uint16_t g_usADCvalues[8];
		uint16_t Vin,Vout,Curr;
uint8_t boost = 0;

void Task1(void)
{
	ADCStartConvAll(); // start first conversion


	while(1)
	{
		OS_WaitEvent(1); // wait for ADC

		Vin = g_usADCvalues[0]*27; // [mV]  5V = 27.727V
		Vout = g_usADCvalues[1]*27;
		Curr = g_usADCvalues[2]*29; // [mA/10]  5V = 3.04A  (3.61V = 2.2A)


#define IMAX 20000
#define UMAX 4200*3
#define ISET 10000
#define USET 4100*3

#define MAXPWMA 255
#define MAXPWMB 128

		if (Vin <8000 || Vin > 25000 || Curr > 30000) 
		emstop(1); // just in case...


		if(Curr > IMAX || Vout > UMAX)
		{
			PWMA_OFF;
			PWMB_OFF;
			OCR1A =0;
			OCR1B =0;
			boost = 0;
		}
		else
		{
			// BUCK OR BOOST



			if(boost) 
			{
				// step up
				// FET1 action is direct.
				// FET4 action is reverse!!

				// FET1: switch on
				PWMA_ST_ON 

				// FET4: increased PWM increases voltage and current
				PWMB_ON

				if(Vout < USET &&  Curr < ISET)
				{
					if(OCR1B < MAXPWMB)
						OCR1B++;
				}
				else
				{
					if(OCR1B > 0)
					{
						OCR1B--;
					}
					else
					{
						boost = 0;
					}
				}

			}
			else
			{
				// step down
				// FET1 action is direct.
				// FET4 action is reverse!!

				// FET4: switch off
				PWMB_OFF 

				// increased PWM INcreases voltage and current.
				PWMA_ON

				if(Vout < USET &&  Curr < ISET)
				{
					if(OCR1A < MAXPWMA)
					{
						OCR1A++;
					}
					else
					{
						boost = 1;
					}
				}
				else
				{
					if(OCR1A > 0)
						OCR1A--;
				}


			}
		}


		ADCStartConvAll();

	}

}

void Task2(void)
{
	OS_SetAlarm(1,10);
	while(1)
	{
		OS_WaitAlarm();
		OS_SetAlarm(1,10);
		// TODO add your code here

	}
}

void Task3(void)
{
	OS_SetAlarm(2,10);
	while(1)
	{
		OS_WaitAlarm();
		OS_SetAlarm(2,10);
		// TODO add your code here

	}
}



// *********  Code to be executed inside Timer ISR used for the OS, defined in FabOS_config.h
void OS_CustomISRCode(void)
{
	TCNT0 =0;  // reset the timer on ISR to have correct timing
}



// *********  Controller initialisation
void CPU_init(void)
{
	// init OS timer and interrupt
	TCCR0  = 0b00000011; // ck / 64 = 125khz clock
	OCR0   = 125; // interrupt every 1 ms at 8 MHz

	TIMSK |= 1<<OCIE0; // Output Compare Interrupt ON


	// init PWM output

	DDRD |= (1<<PD4)|(1<<PD5);

	TCCR1A = 0b00000001; //0b10100001; // 8 bit fast PWM
	TCCR1B = 0b00001001; 
	OCR1A  = 0; 
	OCR1B  = 0; 
	
	PWMA_OFF
	PWMB_OFF
	
	ADCinit();

	//Timer0 Initializations for ATMEGA16
	//TCCR0 |= 5;  // Enable TMR0, set clock source to CLKIO/1024. Interrupts @ 32.768ms intervals @ 8 MHz. This means tasks can execute at least 130,000 instructions before being preempted.
	//TIMSK |= 1 ; // Interrupt on TMR0 Overflow.

	// *** NO global interrupts enabled at this point!!!
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
		case 3:
			// OS_SetAlarm: Multiple alarm per task
			break;	
		case 4:
			// OS_WaitAlarm: waiting in idle is not allowed
			break;	
		case 6:
			// OS_SetEvent: idle can not handle events
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

