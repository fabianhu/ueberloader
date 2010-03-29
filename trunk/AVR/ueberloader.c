/*
	This is the �eberloader
	with N-channel high side driver

	(c) 2010 Fabian Huslik
*/
#include "OS/FabOS.h"
#include "ueberloader.h"
#include "adc.h"
#include "pwm.h"

// *********  Task definitions
OS_DeclareTask(TaskGovernor,200);
OS_DeclareTask(TaskBalance,200);
OS_DeclareTask(Task3,200);
OS_DeclareTask(Task4,200);

//OS_DeclareQueue(DemoQ,10,4);

ADC_Values_t MyADCValues;

// *********  Prototypes
extern void emstop(uint8_t e);




extern int16_t g_sADCvalues[3];
extern Calibration_t myCalibration;

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
	myCalibration.usADCOffset = ADCinit();

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

		unTemp = ((uint32_t)g_sADCvalues[0] - (uint32_t)myCalibration.usADCOffset)*6683ul/1000ul; // [mV]  3,3V/1,6=2,06V - Offset = 1,96V -> 160.75 bits/V
		usU_in_act = unTemp ;

		// 27k / 2k2 is 1.95V at 26V (27,375V @ 4095 bits) = 6,6833 mv/bit

		unTemp = ((uint32_t)g_sADCvalues[1] - (uint32_t)myCalibration.usADCOffset)*6683ul/1000ul; // [mV]  3,3V/1,6=2,06V - Offset = 1,96V -> 160.75 bits/V
		usU_out_act = unTemp;


		if((ADCA.CH2.MUXCTRL & (0xf<<3)) == ADC_CH_MUXPOS_PIN7_gc) // is high currnet config...
		{
			// high current
			// fixme

		}
		else
		{
			// low current
			usI_out_act = (g_sADCvalues[2]- (uint32_t)myCalibration.usADCOffset)*7/10; // [mA]  2V = 3A  (fixme)
		}

//		cli();
//		g_I_filt = I_out_act;
//		sei();

 // just in case...
		if (usU_in_act < 8000)
			emstop(1);
		if (usU_in_act > 22000)
			emstop(2);
		if (usI_out_act > 26000)
			emstop(3);
		if (usU_out_act > 25000)
			emstop(4);


		vGovernor(1000,16000,usI_out_act,usU_out_act);



//		//OS_WaitTicks(10);
//		ADCStartConvAll(); // start next conversion, which again triggers this task,
	}
}




void TaskBalance(void)
{
	static uint8_t ucPhase; // ADC conversion phase, equal to ADC channel, within 0..5

	uint16_t usResult;

	ADCStartConvCh(0);

	OS_SetAlarm(1,10);
	while(1)
	{
		OS_WaitAlarm();
		OS_SetAlarm(1,10);

		usResult = ADCA.CH3.RES;

		switch (ucPhase)
		{
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
				// push voltage of channel into array
				OS_ENTERCRITICAL;
				MyADCValues.Cell_mVolt[ucPhase] = usResult; // fixme scaling!
				OS_LEAVECRITICAL;
				ucPhase++;
				ADCStartConvCh(ucPhase);
				break;
			case 5:
				// push voltage of channel into array

				OS_ENTERCRITICAL;
				MyADCValues.Cell_mVolt[5] = usResult; // fixme scaling!
				OS_LEAVECRITICAL;
				ucPhase++;
				ADCStartConvCh(10); // temp 1
				break;
			case 6:
				// temperature external1
				OS_ENTERCRITICAL;
				MyADCValues.TempInt[0] = usResult ; // fixme scaling!
				OS_LEAVECRITICAL;
				ucPhase++;
				ADCStartConvCh(11); // temp 2
				break;
			case 7:
				// temperature external2
				OS_ENTERCRITICAL;
				MyADCValues.TempInt[1] = usResult ; // fixme scaling!
				OS_LEAVECRITICAL;
				ucPhase++;
				ADCStartConvInt(0);
				break;
			case 8:
				// CPU temperature
				OS_ENTERCRITICAL;
				MyADCValues.TempCPU = usResult ; // fixme scaling!
				OS_LEAVECRITICAL;
				ucPhase++;
				ADCStartConvInt(2);
				break;
			case 9:
				// VCC_mVolt measurement
				OS_ENTERCRITICAL;
				MyADCValues.VCC_mVolt = usResult ; // fixme scaling!
				OS_LEAVECRITICAL;
				ucPhase = 0;
				ADCStartConvCh(0);
				break;
			default:
				emstop(98);
				break;
		}

		// trigger next conversion inside previous case


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

				// fixme setpoint kann nicht h�her, wenn power-mode (rechtes enable) nicht ein ist.
			}
		}

}



void Task4(void)
{


	OS_SetAlarm(3,10);
	while(1)
	{
		OS_WaitAlarm();
		OS_SetAlarm(3,2);

	



	}
}


