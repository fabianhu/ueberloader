/*
	This is the Üeberloader
	with N-channel high side driver

	(c) 2010 Fabian Huslik
*/
#include "OS/FabOS.h"
#include "ueberloader.h"
#include "adc.h"
#include "pwm.h"
#include "usart.h"

//OS_DeclareQueue(DemoQ,10,4);

// ******** Globals

Command_t myCommand = {0,0,0,0,0};
ADC_Values_t myADCValues;
Battery_t myBattery;
//uint16_t usStartstep =STARTMAX;
//uint16_t I_Max_ABS = 25000;
//volatile uint16_t g_I_filt;
extern int16_t g_sADCvalues[3]; // fast ADC values
extern Calibration_t myCalibration;

// *********  Prototypes
uint8_t GetCellcount(void);
void StateMachineBattery(void);
extern void emstop(uint8_t e);


#define LED_ON  PORTD.OUTSET = (1<<3);
#define LED_OFF PORTD.OUTCLR = (1<<3);

void filter(uint16_t* o, uint16_t* n)
{
	uint32_t temp;
	temp = *o * 9ul + *n;
	*o = temp / 10;
}


// ********* Stuff
void TaskGovernor(void)
{
	myCalibration.sADCOffset = ADCinit();

	uint16_t usU_in_act,usU_out_act;
	int16_t sI_out_act; // mV / mA

	vPWM_Init();

	OS_WaitTicks(1); // wait during timer sync

	PORTD.PIN1CTRL = 0b01000000; // invert Waveform at PORTD Pin1
	PORTC.DIRSET = 0b00000011; // set Port C as output
	PORTD.DIRSET = 0b00001011; // set Port D as output with LED

	ADC_ActivateHiCurrentMeas();

	EVSYS.STROBE = (1<<7);  //fire event 7, which triggers the ADC
	OS_WaitTicks(1); // wait during first ADC conversion

	int16_t sZeroHiMeas = g_sADCvalues[2];

	//ADC_ActivateLoCurrentMeas();

	OS_WaitTicks(200); // wait for first ADC loop...

	while(1)
	{

		EVSYS.STROBE = (1<<7);  //fire event 7, which triggers the ADC

		OS_WaitTicks(1); // wait during ADC conversion

		usU_in_act = ADC_ScaleVolt_mV(g_sADCvalues[0]) ;
		usU_out_act = ADC_ScaleVolt_mV(g_sADCvalues[1]) ;;

		OS_ENTERCRITICAL;
		filter(&myBattery.usVoltage_mV, &usU_out_act);
		//myBattery.usVoltage_mV = usU_out_act;
		OS_LEAVECRITICAL;

		if((ADCA.CH2.MUXCTRL & (0xf<<3)) == ADC_CH_MUXPOS_PIN7_gc) // is high current config...
		{
			// high current
			// fixme
			sI_out_act = (g_sADCvalues[2]-sZeroHiMeas) * 10; // [mA] 1V = 10A  (fixme)
		}
		else
		{
			// low current
			sI_out_act = (g_sADCvalues[2]- (uint32_t)myCalibration.sADCOffset)*3/2; // [mA]  2V = 3A  (fixme)
		}

		if(sI_out_act > 2500)
			ADC_ActivateHiCurrentMeas();
		else if (sI_out_act < 2000)
			ADC_ActivateLoCurrentMeas();

//		cli();
//		g_I_filt = I_out_act;
//		sei();

 // just in case...


		if (usU_in_act < 8000)
			emstop(1);
		if (usU_in_act > 22000)
			emstop(2);
		if (sI_out_act > 10000)
			emstop(3);
		if (usU_out_act > 25000)
			emstop(4);


		OS_ENTERCRITICAL;
		uint16_t myISetpoint = myCommand.I_Max_Set_mA;
		uint16_t myUSetpoint = myCommand.U_Setpoint_mV*myBattery.ucNumberOfCells;
		OS_LEAVECRITICAL;

		if(myBattery.eState == eBattCharging)
		{
			vGovernor(
					myISetpoint,
					myUSetpoint,
					sI_out_act,
					usU_out_act,
					usU_in_act
					);
			LED_ON;
		}
		else
		{
			vGovernor(0,0,0,0,0); // set all off.
			LED_OFF;
		}

//		//OS_WaitTicks(10);
//		ADCStartConvAll(); // start next conversion, which again triggers this task,
	}
}



#define ADCWAITTIME 1

void TaskBalance(void)
{
	int16_t sTemp;
	uint16_t usTemp;
	uint8_t i;

	// direction for balancer pins
	PORTC.DIRSET = 0b11111100;

	OS_WaitTicks(100); // wait for ADC init

	ADC_StartConvCh3Pin(0);

	//OS_SetAlarm(1,10);
	while(1)
	{
		OS_WaitTicks(ADCWAITTIME);


		for(i=1;i<6;i++)
		{
			ADC_StartConvCh3Pin(i);
			OS_WaitTicks(ADCWAITTIME);
			// push voltage of channel into array
			usTemp = ADC_ScaleCell_mV(ADCA.CH3.RES);
			OS_ENTERCRITICAL;
			filter(&myBattery.Cells[i].usVoltage_mV, &usTemp);
			OS_LEAVECRITICAL;
		}
		ADC_StartConvCh3Pin(10);
		OS_WaitTicks(ADCWAITTIME);
		usTemp = ADC_ScaleCell_mV(ADCA.CH3.RES);
		OS_ENTERCRITICAL;
		filter(&myBattery.Cells[0].usVoltage_mV, &usTemp);
		OS_LEAVECRITICAL;

		ADC_StartConvCh3Pin(11); // temperature external2
		OS_WaitTicks(ADCWAITTIME);
		sTemp = ADCA.CH3.RES;
		OS_ENTERCRITICAL;
		myADCValues.TempInt[1] = sTemp ; // fixme scaling!
		OS_LEAVECRITICAL;

		ADC_StartConvInt(0); // CPU temperature
		OS_WaitTicks(ADCWAITTIME);
		sTemp = ADCA.CH3.RES*10 /33; // what would be measured, if it was done at 1V ref (/ 3.3).
		sTemp = ( sTemp * (273ul+85ul) / myCalibration.usCPUTemp85C) ;
		OS_ENTERCRITICAL;
		myADCValues.TempCPU = sTemp ; // fixme scaling!
		OS_LEAVECRITICAL;

		ADC_StartConvInt(1); // Bandgap reference
		OS_WaitTicks(ADCWAITTIME);
		OS_ENTERCRITICAL;
		myADCValues.Bandgap = ADCA.CH3.RES; // bit value for 1.10V ! at ref = Usupp/1.6
		OS_LEAVECRITICAL;
		sTemp = (2048ul*1088ul)/myADCValues.Bandgap; // by knowing, that the voltage is 1.088V, we calculate the ADCRef voltage. // fixme !!!! Temperature test!!
		OS_ENTERCRITICAL;
		myCalibration.sADCRef_mV = sTemp;
		OS_LEAVECRITICAL;


		ADC_StartConvInt(2); // VCC_mVolt measurement
		OS_WaitTicks(ADCWAITTIME);
		sTemp = ADCA.CH3.RES * 10ul;
		sTemp = sTemp * myCalibration.sADCRef_mV / 2048ul ;
		OS_ENTERCRITICAL;
		myADCValues.VCC_mVolt = sTemp;
		OS_LEAVECRITICAL;

		// mean Cell Voltage
		uint16_t mean = 0, bBalance=0;
		for(i=0;i<myBattery.ucNumberOfCells;i++)
		{
			mean += myBattery.Cells[i].usVoltage_mV;
			if(myBattery.Cells[i].usVoltage_mV > myCommand.MinBalance_mV)
				bBalance = 1;
		}
	

		// balancing allowed
		if(bBalance == 1 && myBattery.eState == eBattCharging)
		{
			mean /= myBattery.ucNumberOfCells;
			mean +=3;

			// Balancer logic
			for(i=0;i<myBattery.ucNumberOfCells;i++)
			{
				if(myBattery.Cells[i].usVoltage_mV > mean)
				{
					// switch on Balancer for this cell
					PORTC.OUTSET = (1<<(2+i));
				}
				else
				{
					// switch off Balancer for this cell
					PORTC.OUTCLR = (1<<(2+i));
				}

			}
		}
		else
		{
			// balancer off
			PORTC.OUTCLR = (0b111111<<2);
		}

		StateMachineBattery();

		OS_WaitTicks(10);

	}// while(1)
}

void TaskComm(void)
{
	OS_WaitTicks(1000); // wait for ADC init

	OS_SetAlarm(2,10);
		while(1)
		{
			OS_ENTERCRITICAL;
			myCommand.I_Max_Set_mA = 500;
			myCommand.MinBalance_mV = 3000;
			myCommand.U_Setpoint_mV = 4150;
			myCommand.Mode = eModeAuto;
			OS_LEAVECRITICAL;

			USART_TX_Test();

			OS_WaitTicks(1000);
		}

}



void TaskMonitor(void)
{
	OS_WaitTicks(100); // wait for ADC init

	OS_SetAlarm(3,1000);
	while(1)
	{
		OS_WaitAlarm();
		OS_SetAlarm(3,1000);
		// count the charged Q

	}
}

void StateMachineBattery(void) // ONLY run in TaskBalance!
{
	uint8_t NumberOfCells = 0;
	switch (myBattery.eState)
	{
		case eBattUnknown:
			// nicht vollständig angesteckt
			switch(myCommand.Mode)
			{
				case eModeAuto:
					// charge if ok.
					NumberOfCells = GetCellcount();
					if(NumberOfCells > 0)
					{
						myBattery.ucNumberOfCells = NumberOfCells;
						myBattery.eState = eBattCharging;
					}
					break;
				case eModeManual:
					// Manual mode
					if(	GetCellcount()== myCommand.UserCellCount &&
							myCommand.UserCellCount >0)
					{
						myBattery.ucNumberOfCells = myCommand.UserCellCount;
						myBattery.eState = eBattCharging;
					}
					else
					{
						myBattery.eState = eBattError; // set Error for Display
					}
					break;
				default:
					break;
			}
			break;
		case eBattCharging:
				// Charging!
				/*if(myBattery.usVoltage_mV >= myCommand.U_Setpoint_mV*myBattery.ucNumberOfCells)
					myBattery.eState = eBattFull;*/
				/*if(GetCellcount()!=myBattery.ucNumberOfCells)
				{
					emstop(44); // Wackelkontakt am Balancer
				}*/

			break;

//		case eBattEmpty:
//			// vollständig angesteckt, Sollwert nicht erreicht
//			if(1/*all cells >= setpoint*/)
//				eBattState = eBattFull;
//			break;
		case eBattFull:
			// vollständig angesteckt, Sollwert erreicht
			break;
		case eBattError:
			// fixme switch off everything and wait for user commands.
			emstop(21);
			break;
		default:
			emstop(22);
			break;

	}
}


#define CELLDIFF_mV 100 // millivolt tolerance per cell to total voltage
#define MINCELLVOLTAGE_mV 2000 // minimum cell voltage

// Calculate cell count ; Return cell count. 0 = error
uint8_t GetCellcount(void) // ONLY run in TaskBalance!
{
	uint8_t i,unCellCount=0;
	uint16_t usUges_mV;
	int16_t diff;

	usUges_mV = 0;

	for (i= 0;i<6;i++) // 0..5 iterate over possible cell count
	{
		if(myBattery.Cells[i].usVoltage_mV > MINCELLVOLTAGE_mV)
		{
			usUges_mV += myBattery.Cells[i].usVoltage_mV; // sum up cell voltage
			unCellCount ++;
		}
	}

	diff = usUges_mV - myBattery.usVoltage_mV;
	if(abs(diff)< CELLDIFF_mV*unCellCount)
	{
		// correct!
		return unCellCount;
	}
	else
	{
		// voltage error -> no exact match
		return 0;
	}

}


