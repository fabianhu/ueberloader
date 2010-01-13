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
/* with P-channel high side
#define PWMA_ON 	TCCR1A |=  0b10000000; 
#define PWMA_OFF 	TCCR1A &= ~0b11000000; PORTD &= ~(1<<PD5); //OCR1A = 0;
#define PWMA_ST_ON 	TCCR1A &= ~0b11000000; PORTD |= (1<<PD5); //OCR1A = 0;

#define PWMB_ON 	TCCR1A |=  0b00110000;
#define PWMB_OFF 	TCCR1A &= ~0b00110000; PORTD |= (1<<PD4); //OCR1B = 0;
*/

// with N-channel high side driver
#define PWMA_ON 	TCCR1A |=  0b10000000; 
#define PWMA_OFF 	TCCR1A &= ~0b11000000; PORTD &= ~(1<<PD5); //OCR1A = 0;
#define PWMA_ST_ON 	PWMA_OFF; TCCR1A |=  0b11000000; OCR1A = 9; //PORTD |= (1<<PD5); //OCR1A = 0;

#define PWMB_ON 	TCCR1A |=  0b00100000;
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

struct 
{
	uint32_t Q_max;
	uint16_t U_Max;
	uint16_t I_Max_Set;
	uint8_t  CellCount;
	uint8_t  Go;
}
s_Command = {0,0,0,0};

uint16_t I_Max_ABS = 15000;

volatile uint16_t g_I_filt;


void Task1(void)
{
	uint16_t U_in_act,U_out_act,I_out_act;
	uint8_t boost = 0, boostch = 1;;

	// set working parameters
	if (PINC & (1<<PC1))
		s_Command.U_Max = 4150*3;
		else
		s_Command.U_Max = 4150*6;

	if (!(PINC & (1<<PC7)))
		{
			// no limits ever!
			s_Command.U_Max = 26000; // set to 26V
			I_Max_ABS = 29500;
		}

	ADCStartConvAll(); // start first conversion

	while(1)
	{
		OS_WaitEvent(1); // wait for ADC // this task alternates with ADC

		U_in_act = g_usADCvalues[0]*27; // [mV]  5V = 27.727V
		U_out_act = g_usADCvalues[1]*27;
		I_out_act = g_usADCvalues[2]*29; // [mA/10]  5V = 3.04A  (3.61V = 2.2A)
		cli();
		g_I_filt = I_out_act;
		sei();

		if (U_in_act <8000 || U_in_act > 25000 || I_out_act > 30000) 
		emstop(1); // just in case...


		if(
			(
				I_out_act > (s_Command.I_Max_Set+(s_Command.I_Max_Set/10)) || 
				U_out_act > (s_Command.U_Max+(s_Command.U_Max/10)) || !(PIND & (1<<PD7))
			)
			&& (PINC & (1<<PC7)) // disable by jumper
		  )
		{
			// overshoot prevention			
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

				if(boostch)
				{
					// FET1: switch on
					PWMA_ST_ON 

					// FET4: increased PWM increases voltage and I_out_act
					PWMB_ON
				}

				if(U_out_act < s_Command.U_Max &&  I_out_act < s_Command.I_Max_Set)
				{
					if(OCR1B < 128)
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
						boostch =1;
					}
				}

			}
			else
			{
				// step down
				// FET1 action is direct.
				// FET4 action is reverse!!

				if(boostch)
				{
					// FET4: switch off
					PWMB_OFF 

					// increased PWM INcreases voltage and I_out_act.
					PWMA_ON
				}

				if(U_out_act < s_Command.U_Max &&  I_out_act < s_Command.I_Max_Set)
				{
					if(OCR1A < 0xff)
					{
						OCR1A++;
					}
					else
					{
						boost = 1;
						boostch =1;
					}
				}
				else
				{
					if(OCR1A > 0)
						OCR1A--;
				}
			}
		}

		ADCStartConvAll(); // start next conversion, which again triggers this task,
	}
}

void Task2(void)
{
	uint16_t i;
	
	OS_SetAlarm(1,10);
	while(1)
	{
		OS_WaitAlarm();
		OS_SetAlarm(1,10);
		// TODO add your code here

		if(!(PIND & (1<<PD7))) // set to 0
		{
			s_Command.I_Max_Set =0;
			OS_SetAlarm(1,1000);
		}		
		else
		{
			cli();
			i = s_Command.I_Max_Set;
			if(!(PINC & (1<<PD0)) && (i + 1000) <= I_Max_ABS)
			{
				i += 1000;
				s_Command.I_Max_Set = i ; // increase by 100 mA
				OS_SetAlarm(1,333); // look again in 333ms
			}
			sei();
		}
	}
}

void Task3(void)
{
	OS_SetAlarm(2,1000);
	while(1)
	{
		OS_WaitAlarm();
		OS_SetAlarm(2,1000);
		// count the charged Q

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
	OCR0   = 250; // interrupt every 1 ms at 16 MHz

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


	// init digital inputs
	PORTD |= (1<<PD7);
	PORTC |= (1<<PC0)|(1<<PC1)|(1<<PC7); // pullup on

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
		default:
			break;	
	}
	
	dummy = ErrNo; // dummy code
	#if OS_DO_TESTSUITE == 1
	asm("break"); // for automated tests of OS. may be removed in production code.
	#endif
}
#endif

