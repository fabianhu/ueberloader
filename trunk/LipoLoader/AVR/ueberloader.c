/*
	This is the Üeberloader
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
	myCalibration.sADCOffset = ADCinit();

	uint32_t unTemp;
	uint16_t usU_in_act,usU_out_act;
	int16_t sI_out_act; // mV / mA
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

	vActivateHiCurrentMeas();

	EVSYS.STROBE = (1<<7);  //fire event 7, which triggers the ADC
	OS_WaitTicks(1); // wait during first ADC conversion

	int16_t sZeroHiMeas = g_sADCvalues[2];

	//vActivateLoCurrentMeas();

	while(1)
	{

		EVSYS.STROBE = (1<<7);  //fire event 7, which triggers the ADC

		//OS_WaitEvent(1); // wait for ADC // this task alternates with ADC
		OS_WaitTicks(1); // wait during ADC conversion

		unTemp = ((uint32_t)g_sADCvalues[0] - (uint32_t)myCalibration.sADCOffset)*6683ul/1000ul; // [mV]  3,3V/1,6=2,06V - Offset = 1,96V -> 160.75 bits/V
		usU_in_act = unTemp ;

		// 27k / 2k2 is 1.95V at 26V (27,375V @ 4095 bits) = 6,6833 mv/bit

		unTemp = ((uint32_t)g_sADCvalues[1] - (uint32_t)myCalibration.sADCOffset)*6683ul/1000ul; // [mV]  3,3V/1,6=2,06V - Offset = 1,96V -> 160.75 bits/V
		usU_out_act = unTemp;

		if((ADCA.CH2.MUXCTRL & (0xf<<3)) == ADC_CH_MUXPOS_PIN7_gc) // is high current config...
		{
			// high current
			// fixme
			sI_out_act = (g_sADCvalues[2]-sZeroHiMeas) * 5; // [mA]  2V = 3A  (fixme)
		}
		else
		{
			// low current
			sI_out_act = (g_sADCvalues[2]- (uint32_t)myCalibration.sADCOffset)*7/10; // [mA]  2V = 3A  (fixme)
		}

		if(sI_out_act > 2500)
			vActivateHiCurrentMeas();
		else if (sI_out_act < 2000)
			vActivateLoCurrentMeas();

//		cli();
//		g_I_filt = I_out_act;
//		sei();

 // just in case...

 /* fixme reactivate
		if (usU_in_act < 8000)
			emstop(1);
		if (usU_in_act > 22000)
			emstop(2);
		if (sI_out_act > 10000)
			emstop(3);
		if (usU_out_act > 25000)
			emstop(4);
*/

		uint16_t Temp;
		OS_ENTERCRITICAL;
		Temp = MyADCValues.TempInt[1];
		OS_LEAVECRITICAL;

		if (Temp < 200)
			Temp = 0;

		Temp = Temp*4;
		


		vGovernor(Temp,18000,sI_out_act,usU_out_act);



//		//OS_WaitTicks(10);
//		ADCStartConvAll(); // start next conversion, which again triggers this task,
	}
}




void TaskBalance(void)
{
	static uint8_t ucPhase; // ADC conversion phase, equal to ADC channel, within 0..5

	int16_t sResult;
	int32_t nTemp;

	ADCStartConvCh3Pin(0);

	OS_SetAlarm(1,10);
	while(1)
	{
		OS_WaitAlarm();
		OS_SetAlarm(1,10);

		sResult = ADCA.CH3.RES ;//- myCalibration.sADCOffset;

		switch (ucPhase) // fixme remove the cases
		{
			case 0:
				ucPhase++;
				ADCStartConvCh3Pin(ucPhase);
				break;
			case 1:
			case 2:
			case 3:
			case 4:
				// push voltage of channel into array
				nTemp = (int32_t)sResult * (int32_t)myCalibration.sADCRef_mV / 957ul;
				
				OS_ENTERCRITICAL;
				MyADCValues.Cell_mVolt[ucPhase] = nTemp; // fixme scaling!
				OS_LEAVECRITICAL;
				ucPhase++;
				ADCStartConvCh3Pin(ucPhase);
				break;
			case 5:
				// push voltage of channel into array
				nTemp = (int32_t)sResult * (int32_t)myCalibration.sADCRef_mV / 957ul;

				OS_ENTERCRITICAL;
				MyADCValues.Cell_mVolt[5] = nTemp; // fixme scaling!
				OS_LEAVECRITICAL;
				ucPhase++;
				ADCStartConvCh3Pin(10); // Cell 0
				break;
			case 6:
				// push voltage of channel into array
				nTemp = (int32_t)sResult * (int32_t)myCalibration.sADCRef_mV / 957ul;
				
				OS_ENTERCRITICAL;
				MyADCValues.Cell_mVolt[0] = nTemp; // fixme scaling!
				OS_LEAVECRITICAL;
				ucPhase++;
				ADCStartConvCh3Pin(11);
			case 7:
				// temperature external2
				OS_ENTERCRITICAL;
				MyADCValues.TempInt[1] = sResult ; // fixme scaling!
				OS_LEAVECRITICAL;
				ucPhase++;
				ADCStartConvInt(0);
				break;
			case 8:
				// CPU temperature
				sResult = sResult *10 /33; // what would be measured, if it was done at 1V ref (/ 3.3).
				nTemp = ( sResult * (273ul+85ul) / myCalibration.usCPUTemp85C) ; // fixme falsch!
				OS_ENTERCRITICAL;
				MyADCValues.TempCPU = nTemp ; // fixme scaling!
				OS_LEAVECRITICAL;
				ucPhase++;
				ADCStartConvInt(1);
				break;
			case 9:
				// CPU BANDGAP
				OS_ENTERCRITICAL;
				MyADCValues.Bandgap = sResult; // bit value for 1.10V ! at ref = Usupp/1.6
				OS_LEAVECRITICAL;

				nTemp = (2048ul*1088ul)/MyADCValues.Bandgap; // by knowing, that the voltage is 1.088V, we calculate the ADCRef voltage. // fixme !!!! Temperature test!!
				OS_ENTERCRITICAL;

				myCalibration.sADCRef_mV = nTemp;
				OS_LEAVECRITICAL;

				ucPhase++;
				ADCStartConvInt(2);
				break;
			case 10:
				// VCC_mVolt measurement
				nTemp = sResult * 10ul;
				nTemp = nTemp * myCalibration.sADCRef_mV / 2048ul ;
				OS_ENTERCRITICAL;
				MyADCValues.VCC_mVolt = nTemp;
				OS_LEAVECRITICAL;
				ucPhase = 0;
				ADCStartConvCh3Pin(0);
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

				// fixme setpoint kann nicht höher, wenn power-mode (rechtes enable) nicht ein ist.
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


