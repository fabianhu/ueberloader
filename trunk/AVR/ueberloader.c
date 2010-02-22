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
void SetEnableBuck(void);
void SetEnableBoost(void);

// with N-channel high side driver

/*
#define PWMA_PWM_NOR 	PWMA_OFF;TCCR1A |=  0b10000000;
#define PWMA_PWM_REV	PWMA_OFF;TCCR1A |=  0b11000000;
#define PWMA_OFF 		TCCR1A &= ~0b11000000; PORTD &= ~(1<<PD5); //OCR1A = 0;
#define PWMA_DISC_ON 	PWMA_OFF; PWMA_PWM_REV; OCR1A = MINSWITCHOFFPWM; //PORTD |= (1<<PD5); //OCR1A = 0;

#define PWMB_PWM_NOR 	PWMB_OFF;TCCR1A |=  0b00100000;
#define PWMB_PWM_REV 	PWMB_OFF;TCCR1A |=  0b00110000;
#define PWMB_OFF 		TCCR1A &= ~0b00110000; PORTD &= ~(1<<PD4); //OCR1B = 0;
// illegal! #define PWMB_ON 		TCCR1A &= ~0b00110000; PORTD |= (1<<PD4); //OCR1B = 0;
#define PWMB_DISC_ON	PWMB_OFF; PWMB_PWM_NOR; OCR1B = 0xff-MINSWITCHOFFPWM;
*/
//#define ENABLE_A_ON 	TCC0.CCBBUF = 1280;
#define ENABLE_A_OFF	TCC0.CCBBUF = 0;

//#define ENABLE_B_ON 	TCD0.CCBBUF = 1280;
#define ENABLE_B_OFF	TCD0.CCBBUF = TCD0.PERBUF +4; // inverted
/*
#define ENABLE_A_OFF 	PORTD &= ~(1<<PD3);
#define ENABLE_B_ON 	PORTD |= (1<<PD2);
#define ENABLE_B_OFF 	PORTD &= ~(1<<PD2);

#define LED1_ON 		PORTA |= (1<<PA7);
#define LED1_OFF 		PORTA &= ~(1<<PA7);
*/

void emstop(uint8_t e)
{
	cli(); // stop OS
			
//	PWMA_OFF;
//	PWMB_OFF;
	ENABLE_A_OFF;
	ENABLE_B_OFF;
	PORTB.DIRSET = 0b1000;
	PORTB.OUTSET = 0b1000;

	/*while(1)
	{
		asm("nop");
	}*/

	CCP = CCP_IOREG_gc; // unlock
	RST.CTRL = 1; // SW reset

}

extern int16_t g_sADCvalues[3];

struct 
{
	uint32_t Q_max;
	uint16_t U_Max;
	uint16_t I_Max_Set;
	uint8_t  CellCount;
	uint8_t  Go;
}
s_Command = {0,0,0,0,0};

uint16_t I_Max_ABS = 25000;

volatile uint16_t g_I_filt;
volatile uint8_t up=0,dn=0; // fixme remove test vars

/*ISR(TCC0_CCD_vect) // versuch, ein 1/10 anschaltverhältnis zu machen.
{
	static uint8_t c;
	c++;
	if (c>=10)
	{
		TCC0.CTRLB |= 0b00100000;
		c=0;
	}
	else
	{
		TCC0.CTRLB &= ~0b00100000;

	}
}*/
#define PERIOD_DIV 10ul
#define PERIOD_H 2560ul // 1280 = 100kHz
//define PERIOD_L PERIOD_H + (PERIOD_H +4)* PERIOD_DIV // common divider...
#define PERIOD_L PERIOD_H + (PERIOD_H )* PERIOD_DIV // odd divider...
#define PERIOD_MAX 0xffff
#define MINSWITCHOFFPWM 300

#define STARTMAX 1000ul;
uint16_t usStartstep =STARTMAX;


void TaskGovernor(void)
{
	uint16_t usPower = 0; // 0=0 ; Vollgas = PERIOD_H *2
	uint16_t usADCoffset = ADCinit();

	uint32_t unTemp;
	uint16_t usU_in_act,usU_out_act,usI_out_act; // mV / mA
//
//	uint16_t bStarted =0;

	PORTD.PIN5CTRL = PORT_OPC_PULLUP_gc;
	PORTD.PIN6CTRL = PORT_OPC_PULLUP_gc;
	PORTD.PIN7CTRL = PORT_OPC_PULLUP_gc;

	// set working parameters
	//if (PORTD.IN & PIN7_bm)
		//s_Command.U_Max = 4150*3;
		//else
		s_Command.U_Max = 4150*6;

	/*if (!(PINC & (1<<PC7)))
		{
			// no limits ever!
			s_Command.U_Max = 26000; // set to 26V
			I_Max_ABS = 29500;
		}*/


	TCC0.CTRLA = TC_CLKSEL_DIV1_gc;
	TCC0.CTRLB = 0b00110000 | TC_WGMODE_SS_gc; // single slope
	TCC0.CTRLC = 0; // not used: manually activate output compare
	TCC0.CTRLD = TC_EVACT_RESTART_gc | TC_EVSEL_CH0_gc; // event0 does restart of the timer.
	TCC0.CTRLE = 0; // not used: last bit: Byte-mode
	TCC0.PERBUF = PERIOD_H; // 100khz
	TCC0.CCABUF = 0;
	TCC0.CCBBUF = 0;

	HIRESC.CTRL = HIRES_HREN_TC0_gc;

	TCC0.INTCTRLA = 0; // no error/overflow interrupt
	//TCC0.INTCTRLB = TC_CCDINTLVL_HI_gc;

	TCD0.CTRLA = TC_CLKSEL_DIV1_gc;
	TCD0.CTRLB = 0b00110000 | TC_WGMODE_SS_gc; // single slope
	TCD0.CTRLC = 0; // not used: manually activate output compare
	TCD0.CTRLD = TC_EVACT_RESTART_gc | TC_EVSEL_CH0_gc; // event0 does restart of the timer.
	TCD0.CTRLE = 0; // not used: last bit: Byte-mode
	TCD0.PERBUF = PERIOD_H;// 0+9*4; // 10khz (9 Überläufe, 4 wg. hires)
	TCD0.CCABUF = PERIOD_H-MINSWITCHOFFPWM;
	TCD0.CCBBUF = 0;

	HIRESD.CTRL = HIRES_HREN_TC0_gc;

	// sync timers via event 0
	EVSYS.STROBE = 0b00000001;

	OS_WaitTicks(1); // wait during timer sync

	PORTD.PIN1CTRL = 0b01000000;
	PORTC.DIRSET = 0b00000011; // set Port C as output
	PORTD.DIRSET = 0b00000011; // set Port D as output


	OS_WaitTicks(1); // wait during first ADC conversion

	while(1)
	{

		EVSYS.STROBE = (1<<7);  //fire event 7, which triggers the ADC

		//OS_WaitEvent(1); // wait for ADC // this task alternates with ADC
		OS_WaitTicks(1); // wait during ADC conversion

		unTemp = ((uint32_t)g_sADCvalues[0] /*- (uint32_t)usADCoffset*/)*6683ul/1000ul; // [mV]  3,3V/1,6=2,06V - Offset = 1,96V -> 160.75 bits/V
		usU_in_act = unTemp ;

		// 27k / 2k2 is 1.95V at 26V (27,375V @ 4095 bits) = 6,6833 mv/bit

		unTemp = ((uint32_t)g_sADCvalues[1] /*- (uint32_t)usADCoffset*/)*6683ul/1000ul; // [mV]  3,3V/1,6=2,06V - Offset = 1,96V -> 160.75 bits/V
		usU_out_act = unTemp;

		usI_out_act = g_sADCvalues[2]*7; // [mA/10]  2V = 3A  (fixme)
//		cli();
//		g_I_filt = I_out_act;
//		sei();

 // just in case...
		if (usU_in_act < 8000)
			emstop(1);
		if (usU_in_act > 20000)
			emstop(2);
		if (usI_out_act > 26000)
			emstop(3);
		if (usU_out_act > 30000)
			emstop(4);


		if(s_Command.I_Max_Set <= 0)
		{
			usPower = 0;
			ENABLE_A_OFF;ENABLE_B_OFF;
			usStartstep = STARTMAX;
		}
		else
		{
			//ENABLE_A_ON;
			//ENABLE_B_ON;
			
//			if(
//					usI_out_act > (s_Command.I_Max_Set+(s_Command.I_Max_Set/10)) ||
//					usU_out_act > (s_Command.U_Max+(s_Command.U_Max/10))
//				)
//			{
//				// overshoot prevention
//				usPower = 0;
//			}
//			else
//			{

			int16_t diff = usI_out_act - s_Command.I_Max_Set;

			if(usU_out_act < s_Command.U_Max && usI_out_act < s_Command.I_Max_Set)
			{
				if (usPower < PERIOD_H*17/10)
					usPower++;
			}
			else
			{
				if(usPower>0)
					usPower--;
			}


/*// debug test code
			static uint8_t dir =0;
			if (dir==0)
			{
				if (usPower < PERIOD_H + 2*MINSWITCHOFFPWM)//PERIOD_H*17/10)
					usPower++;
				else
					dir =1;
			}
			else
			{
				if (usPower >= PERIOD_H - 2*MINSWITCHOFFPWM)//PERIOD_H*17/10)
					usPower--;
				else
					dir =0;

			}
*/
			static uint8_t cn=0;

			int16_t diffa = (diff>0)?diff:-diff;


			if(diffa < s_Command.I_Max_Set/20 && s_Command.I_Max_Set > 3000)
			{
				if (++cn == 3)
				{
					cn=0;
					if (usStartstep >0)
						usStartstep--; // muss null werden.
				}
				//usStartstep =0;
			}
			else
			{
				//usStartstep = STARTMAX;
			}

			if ( usPower <= PERIOD_H - MINSWITCHOFFPWM )
			{
				//buck
				// links IN
				TCC0.CCABUF = usPower;
				TCC0.PERBUF = PERIOD_H;

				// rechts IN
				TCD0.CCABUF = PERIOD_L - MINSWITCHOFFPWM;
				TCD0.PERBUF = PERIOD_L;

				SetEnableBuck();
			}
			else if(usPower <= PERIOD_H)
			{
				uint32_t u;
				u = MINSWITCHOFFPWM - (PERIOD_H - (uint32_t)usPower);
				u = u*100*PERIOD_DIV  / MINSWITCHOFFPWM; // result = 1..10 (*100)
				u = PERIOD_H * u /100;
				TCC0.PERBUF = PERIOD_H + u;
				TCC0.CCABUF = TCC0.PERBUF - MINSWITCHOFFPWM;

				TCD0.PERBUF = PERIOD_L;
				TCD0.CCABUF = PERIOD_L - MINSWITCHOFFPWM;

				SetEnableBuck();
				//EVSYS.STROBE = (1<<0); // sync timers
			}
			else if(usPower <= PERIOD_H + MINSWITCHOFFPWM)
			{
				uint32_t u;
				u = MINSWITCHOFFPWM - (usPower - PERIOD_H);
				u = u*100*PERIOD_DIV / MINSWITCHOFFPWM; // result = 1..10 (*100)
				u = PERIOD_H * u / 100;
				TCD0.PERBUF = PERIOD_H + u;
				TCD0.CCABUF = TCD0.PERBUF - MINSWITCHOFFPWM;

				TCC0.PERBUF = PERIOD_L;
				TCC0.CCABUF = TCC0.PERBUF - MINSWITCHOFFPWM;

				SetEnableBoost();
				//EVSYS.STROBE = (1<<0); // sync timers
			}
			else
			{
				//boost
				// links IN
				TCC0.CCABUF = TCC0.PERBUF - MINSWITCHOFFPWM; // + speed runtersetzen fixme

				// rechts IN
				TCD0.CCABUF = PERIOD_H*2 - usPower;//PERIOD_H*2 - power - 2* MINSWITCHOFFPWM ;

				// links ENABLE immer an:
				TCC0.CCBBUF = PERIOD_MAX;



				TCC0.PERBUF = PERIOD_L;
				TCD0.PERBUF = PERIOD_H;

				SetEnableBoost();
			}

			// sync timers
			/*if (TCC0.PERBUF > TCD0.PERBUF)
			{
				// TCC is the resetter
				TCC0.CCDBUF = TCC0.PER;
				EVSYS.CH0MUX = 0b11000111; // select compare match TCC - D to create reset of both timers.

			}
			else
			{	// TCD is the resetter
				TCD0.CCDBUF = TCD0.PER;
				EVSYS.CH0MUX = 0b11010111; // select compare match TCD - D to create reset of both timers.
			}*/

			/*
			 * Start im step down: enable links an, enable rechts aus.
			 *
			 * Wenn power > Per.... dann enable rechts pulsen mit PERIOD_H*2 - power - 2* MINSWITCHOFFPWM
			 *
			 *
			 *
			 *power größer als PERIOD_H - MINSWITCHOFFPWM: erniedrigen der Frequenz bis ende bei PERIOD_H.
			 *dann rechte seite periodendauer schneller machen bis PERIOD_H + MINSWITCHOFFPWM
			 *
			 *
			 * */

			//} xxed if above


		}
//		//OS_WaitTicks(10);
//		ADCStartConvAll(); // start next conversion, which again triggers this task,
	}
}



void SetEnableBuck(void)
{
	uint32_t u;
	// links ENABLE immer an:
	TCC0.CCBBUF = PERIOD_MAX;

	if(usStartstep)
	{
		u= ((uint32_t)TCD0.PERBUF+4ul) * (uint32_t)usStartstep / STARTMAX;

		// rechts ENABLE falls start immer aus:
		TCD0.CCBBUF = u;//(invertiert!)
	}
	else
	{
		TCD0.CCBBUF = 0;
	}
}

void SetEnableBoost(void)
{
	uint32_t u;

	// links ENABLE immer an:
	TCC0.CCBBUF = PERIOD_MAX;

	if(usStartstep)
	{
		u= (uint32_t)TCD0.CCABUF * (uint32_t)usStartstep / STARTMAX;// fixme stimmt nicht

		// rechts ENABLE läuft mit:
		TCD0.CCBBUF = u;//PERIOD_H*2 - power;//(invertiert!)


		// min of: TCD0.PERBUF + 4 - u and TCD0.CCABUF
	}
	else
	{
		//rechts ENABLE
		TCD0.CCBBUF = 0; //(invertiert!)
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
				phase++;
				break;
			case 6:
				// VCC measurement
				phase++;
				break;
			case 7:
				// zero offset
				phase++;
				break;
			case 8:
				// CPU temperature
				phase++;
				break;
			case 9:
				// temperature external1
				phase++;
				break;
			case 10:
				// temperature external2
				phase =0;
				break;
			default:
				phase = 0;
		}

		// trigger next conversion inside last case


	}
}

void Task3(void)
{
	/*OS_SetAlarm(2,1000);
	while(1)
	{
		OS_WaitAlarm();
		OS_SetAlarm(2,1000);
		// count the charged Q

	}*/
	uint16_t i;

	OS_SetAlarm(2,10);
		while(1)
		{
			OS_WaitAlarm();
			OS_SetAlarm(2,10);
			// TODO add your code here

			if(!(PORTD.IN & PIN5_bm)) // set to 0
			{
				s_Command.I_Max_Set =0;
				OS_SetAlarm(2,1000);
			}
			else
			{
				cli();
				i = s_Command.I_Max_Set;
				if(!(PORTD.IN & PIN6_bm) && (i + 1000) <= I_Max_ABS)
				{
					i += 1000;
					s_Command.I_Max_Set = i ; // increase by 100 mA
					OS_SetAlarm(2,333); // look again in 333ms
				}
				sei();

				// fixme setpoint kann nicht höher, wenn power-mode (rechtes enable) nicht ein ist.
			}
		}

}



