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
#include "serial.h"

//OS_DeclareQueue(DemoQ,10,4);

// ******** Globals

Command_t g_tCommand = {0,0,0,0,0};
ADC_Values_t g_tADCValues;
Battery_Info_t g_tBattery_Info;

//uint16_t usStartstep =STARTMAX;
//uint16_t I_Max_ABS = 25000;
//volatile uint16_t g_I_filt;
extern int16_t g_asADCvalues[3]; // fast ADC values
extern Calibration_t g_tCalibration;

// *********  Prototypes
uint8_t GetCellcount(BatteryCell_t cells[], uint16_t* pusBattVoltage);
void StateMachineBattery(void);
extern void emstop(uint8_t e);


#define LED_ON  PORTD.OUTSET = (1<<3);
#define LED_OFF PORTD.OUTCLR = (1<<3);

void usFilter(uint16_t* o, uint16_t* n)
{
	uint32_t temp;
	if(*o == 0)
		*o = *n;
	else
	{
		temp = *o * 9ul + *n;
		*o = temp / 10;
	}
}

void sFilter(int16_t* o, int16_t* n)
{
	int32_t temp;
	if(*o == 0)
		*o = *n;
	else
	{
		temp = *o * 9ul + *n;
		*o = temp / 10;
	}
}

// ********* Stuff
void TaskGovernor(void)
{
	g_tCalibration.sADCOffset = ADCinit();

	uint16_t usU_in_act,usU_out_act;
	int16_t sI_out_act; // mV / mA

	vPWM_Init();

	OS_WaitTicks(OSALMWaitGov,1); // wait during timer sync

	PORTD.PIN1CTRL = 0b01000000; // invert Waveform at PORTD Pin1
	PORTC.DIRSET = 0b00000011; // set Port C as output
	PORTD.DIRSET = 0b00001011; // set Port D as output with LED

	ADC_ActivateHiCurrentMeas();

	EVSYS.STROBE = (1<<7);  //fire event 7, which triggers the ADC
	OS_WaitTicks(OSALMWaitGov,1); // wait during first ADC conversion

	int16_t sZeroHiMeas = g_asADCvalues[2];

	//ADC_ActivateLoCurrentMeas();

	OS_WaitTicks(OSALMWaitGov,200); // wait for first ADC loop...

	while(1)
	{

		EVSYS.STROBE = (1<<7);  //fire event 7, which triggers the ADC

		OS_WaitTicks(OSALMWaitGov,1); // wait during ADC conversion

		usU_in_act = ADC_ScaleVolt_mV(g_asADCvalues[0]) ;
		usU_out_act = ADC_ScaleVolt_mV(g_asADCvalues[1]) ;;

		OS_MutexGet(OSMTXBattInfo);
		usFilter(&g_tBattery_Info.usActVoltage_mV, &usU_out_act);
		OS_MutexRelease(OSMTXBattInfo);

		if((ADCA.CH2.MUXCTRL & (0xf<<3)) == ADC_CH_MUXPOS_PIN7_gc) // is high current config...
		{
			// high current
			sI_out_act = (g_asADCvalues[2]-sZeroHiMeas) * 10; // [mA] 1V = 10A  (fixme)
		}
		else
		{
			// low current
			sI_out_act = (g_asADCvalues[2]- (uint32_t)g_tCalibration.sADCOffset)*3/2; // [mA]  2V = 3A  (fixme)
		}

		if(sI_out_act > 2500)
			ADC_ActivateHiCurrentMeas();
		else if (sI_out_act < 2000)
			ADC_ActivateLoCurrentMeas();

		OS_MutexGet(OSMTXBattInfo);
		sFilter(&g_tBattery_Info.sActCurrent_mA, &sI_out_act);
		OS_MutexRelease(OSMTXBattInfo);


 // just in case...


		if (usU_in_act < 6000)
			emstop(1);
		if (usU_in_act > 22000)
			emstop(2);
		if (sI_out_act > 10000)
			emstop(3);
		if (usU_out_act > 25000)
			emstop(4);


		OS_ENTERCRITICAL;
		uint16_t myISetpoint = g_tCommand.usCurrentSetpoint;
		uint16_t myUSetpoint = g_tCommand.usVoltageSetpoint_mV*g_tBattery_Info.ucNumberOfCells;
		OS_LEAVECRITICAL;

		if(g_tBattery_Info.eState == eBattCharging)
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
	uint16_t usBalanceCells[6]; // quasi static
	uint8_t ucBalanceBits;
	uint8_t i;

	// direction for balancer pins
	PORTC.DIRSET = 0b11111100;

	OS_WaitTicks(OSALMBalRepeat,100); // wait for ADC init

	ADC_StartConvCh3Pin(0);

	while(1)
	{
		OS_WaitTicks(OSALMBalWait,ADCWAITTIME);


		for(i=1;i<6;i++)
		{
			ADC_StartConvCh3Pin(i);
			OS_WaitTicks(OSALMBalWait,ADCWAITTIME);
			// push voltage of channel into array
			usTemp = ADC_ScaleCell_mV(ADCA.CH3.RES);
			usFilter(&usBalanceCells[i], &usTemp);
		}
		ADC_StartConvCh3Pin(10);
		OS_WaitTicks(OSALMBalWait,ADCWAITTIME);
		usTemp = ADC_ScaleCell_mV(ADCA.CH3.RES);
		usFilter(&usBalanceCells[0], &usTemp);

		ADC_StartConvCh3Pin(11); // temperature external2
		OS_WaitTicks(OSALMBalWait,ADCWAITTIME);
		sTemp = ADCA.CH3.RES;
		OS_ENTERCRITICAL;
		g_tADCValues.TempInt = sTemp ; // fixme scaling!
		OS_LEAVECRITICAL;

		ADC_StartConvInt(0); // CPU temperature
		OS_WaitTicks(OSALMBalWait,ADCWAITTIME);
		sTemp = ADCA.CH3.RES*10 /33; // what would be measured, if it was done at 1V ref (/ 3.3).
		sTemp = ( sTemp * (273ul+85ul) / g_tCalibration.usCPUTemp85C) ;
		OS_ENTERCRITICAL;
		g_tADCValues.TempCPU = sTemp ; // fixme scaling!
		OS_LEAVECRITICAL;

		ADC_StartConvInt(1); // Bandgap reference
		OS_WaitTicks(OSALMBalWait,ADCWAITTIME);
		OS_ENTERCRITICAL;
		g_tADCValues.Bandgap = ADCA.CH3.RES; // bit value for 1.10V ! at ref = Usupp/1.6
		OS_LEAVECRITICAL;
		sTemp = (2048ul*1088ul)/g_tADCValues.Bandgap; // by knowing, that the voltage is 1.088V, we calculate the ADCRef voltage. // fixme !!!! Temperature test!!
		OS_ENTERCRITICAL;
		g_tCalibration.sADCRef_mV = sTemp;
		OS_LEAVECRITICAL;


		ADC_StartConvInt(2); // VCC_mVolt measurement
		OS_WaitTicks(OSALMBalWait,ADCWAITTIME);
		sTemp = ADCA.CH3.RES * 10ul;
		sTemp = sTemp * g_tCalibration.sADCRef_mV / 2048ul ;
		OS_ENTERCRITICAL;
		g_tADCValues.VCC_mVolt = sTemp;
		OS_LEAVECRITICAL;

		// mean Cell Voltage
		uint16_t mean = 0, bBalance=0;

		OS_MutexGet(OSMTXCommand);
		uint16_t usBalanceRelease_mV = g_tCommand.usMinBalanceVolt_mV;
		OS_MutexRelease(OSMTXCommand);

		// Calculate release
		for(i=0;i<g_tBattery_Info.ucNumberOfCells;i++)
		{
			mean += usBalanceCells[i];
			if(usBalanceCells[i] > usBalanceRelease_mV)
				bBalance = 1;
		}

		// balancing allowed
		if(bBalance == 1 && g_tBattery_Info.eState == eBattCharging)
		{
			mean /= g_tBattery_Info.ucNumberOfCells;
			mean +=3; // add some difference to prevent swinging

			// Balancer logic
			for(i=0;i<6;i++)
			{
				if(usBalanceCells[i] > mean || i > g_tBattery_Info.ucNumberOfCells)
				{
					// switch on Balancer for this cell
					PORTC.OUTSET = (1<<(2+i));
					ucBalanceBits |= (1<<i);
				}
				else
				{
					// switch off Balancer for this cell
					PORTC.OUTCLR = (1<<(2+i));
					ucBalanceBits &= ~(1<<i);
				}

			}
		}
		else
		{
			// balancer off
			for(i=0;i<6;i++)
			{
				if( i >= g_tBattery_Info.ucNumberOfCells)
				{
					// switch on Balancer for this cell
					PORTC.OUTSET = (1<<(2+i));
					ucBalanceBits |= (1<<i);
				}
				else
				{
					// switch off Balancer for this cell
					PORTC.OUTCLR = (1<<(2+i));
					ucBalanceBits &= ~(1<<i);
				}
			}

			//PORTC.OUTCLR = (0b111111<<2);
			ucBalanceBits = 0;
		}

		OS_MutexGet(OSMTXBattInfo);
		// Copy voltage stuff
		for(i=0;i<6;i++)
		{
			g_tBattery_Info.Cells[i].usVoltage_mV = usBalanceCells[i];
			if(ucBalanceBits & (1<<i))
			{
				g_tBattery_Info.Cells[i].unDisChTicks++;
			}
		}
		OS_MutexRelease(OSMTXBattInfo);

		StateMachineBattery();



	}// while(1)
}



void TaskMonitor(void)
{
	OS_WaitTicks(OSALMonitorRepeat,100); // wait for ADC init

	OS_SetAlarm(OSALMonitorRepeat,1000);
	while(1)
	{
		OS_WaitAlarm(OSALMonitorRepeat);
		OS_SetAlarm(OSALMonitorRepeat,1000);
		// count the charged Q

	}
}

void StateMachineBattery(void) // ONLY run in TaskBalance!
{
	uint8_t NumberOfCells = 0;

	OS_MutexGet(OSMTXBattInfo);
	uint16_t myBattVoltage = g_tBattery_Info.usActVoltage_mV;
	int16_t myBattCurrent = g_tBattery_Info.sActCurrent_mA;
	OS_MutexRelease(OSMTXBattInfo);

	OS_MutexGet(OSMTXCommand);
	uint16_t usCommandCurrent = g_tCommand.usCurrentSetpoint;
	OS_MutexRelease(OSMTXCommand);

	switch (g_tBattery_Info.eState)
	{
		case eBattUnknown:
			// nicht vollständig angesteckt
			switch(g_tCommand.eChargerMode)
			{
				case eModeAuto:
					// charge if ok.
					NumberOfCells = GetCellcount(g_tBattery_Info.Cells,&myBattVoltage);
					if(NumberOfCells > 0)
					{
						g_tBattery_Info.ucNumberOfCells = NumberOfCells;
						g_tBattery_Info.eState = eBattCharging;
					}
					break;
				case eModeManual:
					// Manual mode
					if(	GetCellcount(g_tBattery_Info.Cells,&myBattVoltage)== g_tCommand.ucUserCellCount &&
							g_tCommand.ucUserCellCount >0)
					{
						g_tBattery_Info.ucNumberOfCells = g_tCommand.ucUserCellCount;
						g_tBattery_Info.eState = eBattCharging;
					}
					else
					{
						g_tBattery_Info.eState = eBattError; // set Error for Display
					}
					break;
				default:
					break;
			}
			break;
		case eBattCharging:
				// Charging!
				if(myBattVoltage >= g_tCommand.usVoltageSetpoint_mV*g_tBattery_Info.ucNumberOfCells &&
						myBattCurrent < usCommandCurrent / 10) // fixme 10 ?
				{
					g_tBattery_Info.eState = eBattFull;
				}
				if(GetCellcount(g_tBattery_Info.Cells,&myBattVoltage)!=g_tBattery_Info.ucNumberOfCells)
				{
					g_tBattery_Info.ucNumberOfCells = 0;
					g_tBattery_Info.eState = eBattError;
				}
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
uint8_t GetCellcount(BatteryCell_t cells[], uint16_t* pusBattVoltage)
{
	uint8_t i,unCellCount=0;
	uint16_t usUges_mV;
	int16_t diff;

	usUges_mV = 0;

	for (i= 0;i<6;i++) // 0..5 iterate over possible cell count
	{
		if(cells[i].usVoltage_mV > MINCELLVOLTAGE_mV)
		{
			usUges_mV += cells[i].usVoltage_mV; // sum up cell voltage
			unCellCount ++;
		}
	}

	diff = usUges_mV - *pusBattVoltage;
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


