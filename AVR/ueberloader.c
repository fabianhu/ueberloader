/*

The Üeberloader

with N-channel high side driver

*/
#include "OS/FabOS.h"
#include "ueberloader.h"
#include "adc.h"
#include "pwm.h"

// *********  Task definitions
OS_DeclareTask(TaskGovernor,200);
OS_DeclareTask(TaskBalance,200);
OS_DeclareTask(Task3,200);

//OS_DeclareQueue(DemoQ,10,4);

ADC_Values_t MyADCValues;

// *********  Prototypes
extern void emstop(uint8_t e);




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

	vPWM_Init();

	OS_WaitTicks(1); // wait during timer sync

	PORTD.PIN1CTRL = 0b01000000; // invert Waveform at PORTD Pin1
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

			int16_t usDiffAbs = (diff>0)?diff:-diff;

			if(usDiffAbs < s_Command.I_Max_Set/20 && s_Command.I_Max_Set > 3000)
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

			vPWM_Set(usPower,usStartstep);


		}
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



