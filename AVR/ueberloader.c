/*

The Üeberloader

*/
#include "OS/FabOS.h"
#include "adc.h"

// *********  Task definitions
OS_DeclareTask(TaskGovernor,200);
OS_DeclareTask(TaskBalance,200);
OS_DeclareTask(Task3,200);

//OS_DeclareQueue(DemoQ,10,4);

// *********  Prototypes
void CPU_init(void);

// with N-channel high side driver
#define MINSWITCHOFFPWM 15 // = 1,5µs

#define PWMA_PWM_NOR 	PWMA_OFF;TCCR1A |=  0b10000000;
#define PWMA_PWM_REV	PWMA_OFF;TCCR1A |=  0b11000000;
#define PWMA_OFF 		TCCR1A &= ~0b11000000; PORTD &= ~(1<<PD5); //OCR1A = 0;
#define PWMA_DISC_ON 	PWMA_OFF; PWMA_PWM_REV; OCR1A = MINSWITCHOFFPWM; //PORTD |= (1<<PD5); //OCR1A = 0;

#define PWMB_PWM_NOR 	PWMB_OFF;TCCR1A |=  0b00100000;
#define PWMB_PWM_REV 	PWMB_OFF;TCCR1A |=  0b00110000;
#define PWMB_OFF 		TCCR1A &= ~0b00110000; PORTD &= ~(1<<PD4); //OCR1B = 0;
// illegal! #define PWMB_ON 		TCCR1A &= ~0b00110000; PORTD |= (1<<PD4); //OCR1B = 0;
#define PWMB_DISC_ON	PWMB_OFF; PWMB_PWM_NOR; OCR1B = 0xff-MINSWITCHOFFPWM;

#define ENABLE_A_ON 	PORTD |= (1<<PD3);
#define ENABLE_A_OFF 	PORTD &= ~(1<<PD3);
#define ENABLE_B_ON 	PORTD |= (1<<PD2);
#define ENABLE_B_OFF 	PORTD &= ~(1<<PD2);

#define LED1_ON 		PORTA |= (1<<PA7);
#define LED1_OFF 		PORTA &= ~(1<<PA7);

void emstop(uint8_t e)
{
	cli(); // stop OS
			
//	PWMA_OFF;
//	PWMB_OFF;
//	ENABLE_A_OFF;
//	ENABLE_B_OFF;
	while(1)
	{
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
s_Command = {0,0,0,0,0};

uint16_t I_Max_ABS = 15000;

volatile uint16_t g_I_filt;
volatile uint8_t up=0,dn=0; // fixme remove test vars

void TaskGovernor(void)
{

	ADCinit();



//	uint16_t U_in_act,U_out_act,I_out_act;
//	uint8_t bOpModeBuck = 1; //, bBoostModeChange = 1;;
//
//	uint16_t bStarted =0;
//
//	uint8_t i;
//
//	// set working parameters
//	if (PINC & (1<<PC1))
//		s_Command.U_Max = 4150*3;
//		else
//		s_Command.U_Max = 4150*6;
//
//	if (!(PINC & (1<<PC7)))
//		{
//			// no limits ever!
//			s_Command.U_Max = 26000; // set to 26V
//			I_Max_ABS = 29500;
//		}
//
//	ADCStartConvAll(); // start first conversion
//
//
//
//	PWMA_PWM_NOR;
//	PWMB_PWM_REV;
//	OCR1A = 0xff;
//	OCR1B = 0xff;
//
//	uint16_t Power = 0,Limit=0;
//
//
	PORTC.DIRSET = 0b00000011; // set Port C as output
	PORTD.DIRSET = 0b00000011; // set Port D as output

	TCC0.CTRLA = TC_CLKSEL_DIV1_gc;
	TCC0.CTRLB = 0b00110000 | TC_WGMODE_SS_gc; // single slope
	TCC0.CTRLC = 0; // not used: manually activate output compare
	TCC0.CTRLD = TC_EVACT_RESTART_gc | TC_EVSEL_CH0_gc; // event0 does restart of the timer.
	TCC0.CTRLE = 0; // not used: last bit: Byte-mode
	TCC0.PER = 32000; // 1 ms
	TCC0.CCA = 16000; // gives 50% PWM
	TCC0.CCB = 15000;


	TCD0.CTRLA = TC_CLKSEL_DIV8_gc;
	TCD0.CTRLB = 0b00110000 | TC_WGMODE_SS_gc; // single slope
	TCD0.CTRLC = 0; // not used: manually activate output compare
	TCD0.CTRLD = TC_EVACT_RESTART_gc | TC_EVSEL_CH0_gc; // event0 does restart of the timer.
	TCD0.CTRLE = 0; // not used: last bit: Byte-mode
	TCD0.PERBUF = 32000; // 1 ms
	TCD0.CCABUF = 16000; // gives 50% PWM
	TCD0.CCBBUF = 15000;


	while(1)
	{
		//OS_WaitEvent(1); // wait for ADC // this task alternates with ADC
		OS_WaitTicks(1);



//
//		U_in_act = g_usADCvalues[0]*27; // [mV]  5V = 27.727V
//		U_out_act = g_usADCvalues[1]*27;
//		I_out_act = g_usADCvalues[2]*29; // [mA/10]  5V = 3.04A  (3.61V = 2.2A)
//		cli();
//		g_I_filt = I_out_act;
//		sei();
//
//		if (U_in_act <8000 || U_in_act > 25000 || I_out_act > 30000)
//		{
//			LED1_OFF;
//			if( U_in_act > 25000)
//				LED1_ON;
//
//			emstop(1); // just in case...
//		}
//
//		/*if(s_Command.I_Max_Set <= 0)
//		{
//			ENABLE_A_OFF; ENABLE_B_OFF;
//			LED1_OFF;
//			OCR1A =0;
//			OCR1B =0xff;
//			bStarted =0;
//		}
//		else*/
//		{
//			ENABLE_A_ON;
//			ENABLE_B_ON;
//
//
//			/*if(
//				(
//					I_out_act > (s_Command.I_Max_Set+(s_Command.I_Max_Set/10)) ||
//					U_out_act > (s_Command.U_Max+(s_Command.U_Max/10)) || !(PIND & (1<<PD7))
//				)
//				&& (PINC & (1<<PC7)) // disable by jumper
//			  )
//			{
//				// overshoot prevention
//				PWMA_OFF;
//				PWMB_OFF;
//				OCR1A =0;
//				OCR1Bi =MINSWITCHOFFPWM;
//				ENABLE_A_OFF; ENABLE_B_OFF;
//			//	bOpMode_Boost = 0;
//			}
//			else
//			{*/
//
//			// links unten und rechts oben eine Diode vorsehen (LTC3780 Referenzdesign sieht das vor)
//
//			/*if (Power > 256) // boost
//			{
//				// right side: switch on low longer for increasing power. (max 50% on)
//				// left side: keep high on with interruptions (at the end of right sides high)
//
//			}
//			else
//			{
//				// left side: switch on high longer for increasing power. (max 100% on)
//				// right side: keep high with interruptions (at the end of left sides high)
//
//			}
//			*/
//
//			/* startup:
//			 * Anhand der gemessenen Spannungen entscheiden: Start mit boost or buck.
//			 * Buck:
//			 * Versuch des Start im "cross-over" mode d.h. links und rechts gleichzeitig (high)kurz an.
//			 * Sollte der Strom richtig fließen, erhöhen des PWM synchron, bis der Strom erreicht ist. ???
//			 *
//			 * start im "pseudo boost mode":
//			 * beide aus, links kurz an - Spule laden - rechts kurz an, Spule entladen und Spannung hochschieben.
//			 *
//
//			 * Wir brauchen eine Strom-Messung in beide Richtungen? - nee doch nicht, wir kennen die Spannungen.
//			*/
//
//				if(up)//U_out_act < s_Command.U_Max &&  I_out_act < s_Command.I_Max_Set)
//				{
//					// increase power
//					if(OCR1A < (0xff-MINSWITCHOFFPWM))
//					{
//						// step down
//						OCR1A++;
//						bOpModeBuck = 1;
//					}
//					else
//					{
//						// step up
//						if(OCR1B > 128) // ex 128
//							OCR1B--;
//
//						bOpModeBuck = 0;
//					}
//					up=0;
//				}
//				else if (dn)
//				{
//					// decrease power
//					if(OCR1B < MINSWITCHOFFPWM )
//					{
//						// step down
//						if(OCR1A > 0)
//						 OCR1A--;
//						bOpModeBuck = 1;
//					}
//					else
//					{
//						// step up
//						OCR1B++;
//						bOpModeBuck = 0;
//					}
//					dn=0;
//				}
//
//				/*if(bOpModeBuck)
//				{
//					if(bStarted == 0)
//					{
//						//startup
//						OCR1B = OCR1A;
//						if (i<100)
//							i++;
//						else
//						{bStarted=1;i=0;}
//					}
//					else
//					{
//						//return to normal
//						if (i<10)
//							i++;
//						else
//						{
//							i=0;
//							if(OCR1B > MINSWITCHOFFPWM)
//							{
//								// step up
//								OCR1B--;
//							}
//						}
//					}
//				}*/
//
//
//				//OCR1B = 0xff - OCR1Bi;
//
//
//			//}
//		}
//		//OS_WaitTicks(10);
//		ADCStartConvAll(); // start next conversion, which again triggers this task,
	}
}

void TaskBalance(void)
{
	static uint8_t phase;

	OS_SetAlarm(1,10);
	while(1)
	{
		OS_WaitAlarm();
		OS_SetAlarm(1,10);

		switch (phase)
		{
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
				// push voltage of channel into array
				break;
			case 6:
				// VCC measurement
				break;
			case 7:
				// zero offset
				break;
			case 8:
				// CPU temperature
				break;
			case 9:
				// temperature external1
				break;
			case 10:
				// temperature external2
				break;
			default:
				phase = 0;
		}

		// increment phase
		if(phase < 10)
			phase++;
		else
			phase =0;

		// trigger next conversion
		switch (phase)
		{
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
				// push voltage of channel into array
				break;
			case 6:
				// VCC measurement
				break;
			case 7:
				// zero offset
				break;
			case 8:
				// CPU temperature
				break;
			case 9:
				// temperature external1
				break;
			case 10:
				// temperature external2
				break;
			default:
				phase = 0;
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



