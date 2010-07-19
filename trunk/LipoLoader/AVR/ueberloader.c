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
/*
=
{
sCurrentSetpoint = 1000,
usMinBalanceVolt_mV = 3000,
usVoltageSetpoint_mV = 4150,
eChargerMode = eModeAuto
};
 */
ADC_Values_t g_tADCValues;
Battery_Info_t g_tBattery_Info;

uint8_t gucDisChTicks[6];
uint8_t bBalancerOverload; // True, if one cell has reached Voltage limit.

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
		*o = (temp+5) / 10; // "halbes dazu, wg Rundungsfehler"
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
		*o = (temp+5) / 10; // "halbes dazu, wg Rundungsfehler"
	}
}

// ********* Stuff
void TaskGovernor(void)
{
	g_tCalibration.sADCOffset = ADCinit();

	int16_t sU_in_act,sU_out_act;
	int16_t sU_in_act_flt,sU_out_act_flt;
	int16_t sI_out_act,sI_out_act_flt; // mV / mA

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

		sU_in_act = ADC_ScaleVolt_mV(g_asADCvalues[0]) ;
		sU_out_act = ADC_ScaleVolt_mV(g_asADCvalues[1]) ;;


		if((ADCA.CH2.MUXCTRL & (0xf<<3)) == ADC_CH_MUXPOS_PIN7_gc) // is high current config...
		{
			// high current
			sI_out_act = ADC_ScaleHighAmp_mA(g_asADCvalues[2]-sZeroHiMeas);
			sU_out_act -= sI_out_act*1/125; // High current resistor value (0.005R)(+3mR MosFet) * Current
		}
		else
		{
			// low current
			sI_out_act = ADC_ScaleLowAmp_mA(g_asADCvalues[2]- (uint32_t)g_tCalibration.sADCOffset);
			sU_out_act -= sI_out_act*2/19; // Low current resistor value (0.105R) * Current
		}

		sFilter(&sU_out_act_flt, &sU_out_act);
		OS_MutexGet(OSMTXBattInfo);
		g_tBattery_Info.sActVoltage_mV = sU_out_act_flt;
		OS_MutexRelease(OSMTXBattInfo);

		if(sI_out_act > 2500)
			ADC_ActivateHiCurrentMeas();
		else if (sI_out_act < 2000)
			ADC_ActivateLoCurrentMeas();

		int32_t nConverterPower_W = ((sI_out_act) * (sU_out_act /*- usU_in_act*/))/1000; // fixme converter pwr


		sFilter(&sI_out_act_flt, &sI_out_act);
		OS_MutexGet(OSMTXBattInfo);
		g_tBattery_Info.sActCurrent_mA = sI_out_act_flt;
		g_tBattery_Info.usConverterPower_W = (uint16_t)nConverterPower_W; // fixme remove
		OS_MutexRelease(OSMTXBattInfo);


 // just in case...


		if (sU_in_act < 7000)
			emstop(1);
		if (sU_in_act > 22000)
			emstop(2);
		if (sI_out_act > 10000)
			emstop(3);
		if (sU_out_act > 4250*6)
			emstop(4);

		OS_ENTERCRITICAL;
		int16_t myISetpoint = g_tCommand.sCurrentSetpoint;
		int16_t myUSetpoint = g_tCommand.usVoltageSetpoint_mV*g_tBattery_Info.ucNumberOfCells;
		OS_LEAVECRITICAL;

		// calculate Current setpoint
		static int16_t I_Set_mA_Ramped = 0;

		static uint8_t ccc=0;

		if(g_tBattery_Info.eState == eBattCharging)
		{
			ccc++;
			if (ccc == 20) ccc=0;
			if(sU_out_act_flt < myUSetpoint /*&& sConverterPower < MAXCONVERTERPOWER_W*/ && sU_in_act > 8000 && I_Set_mA_Ramped <= myISetpoint)
			{
				if (I_Set_mA_Ramped < myISetpoint && ccc==0 && bBalancerOverload == 0)
				{
					I_Set_mA_Ramped++;
				}
			}
			else // usU_in_act< MAX_SUPP_VOLT ? for discharge
			{
				if (I_Set_mA_Ramped > 0 && ccc==0)
				{
					I_Set_mA_Ramped--; // maybe more?
				}
			}

			if(myISetpoint <= 0) I_Set_mA_Ramped = 0; // switch off on zero.

			/*if(		sI_out_act > (myISetpoint+(myISetpoint/5)) ||
					usU_out_act > (myUSetpoint+(myUSetpoint/5))
				)
			{
				// overshoot prevention
				I_Set_mA_Ramped = 0;
				LED_OFF;
			}
			else*/
			LED_ON;
		}
		else
		{
			I_Set_mA_Ramped = 0;
			LED_OFF;
		}

		vGovernor( I_Set_mA_Ramped,	sI_out_act	);

//		//OS_WaitTicks(10);
//		ADCStartConvAll(); // start next conversion, which again triggers this task,
	}
}



#define ADCWAITTIME 1
#define BALANCEREPEATTIME 100L

void TaskBalance(void)
{
	int16_t sTemp;
	int16_t sBalanceCells[6]; // quasi static
	uint8_t ucBalanceBits;
	uint8_t i;

	// direction for balancer pins
	PORTC.DIRSET = 0b11111100;

	OS_WaitTicks(OSALMBalRepeat,100); // wait for ADC init

	ADC_StartConvCh3Pin(0);

	while(1)
	{
		OS_WaitAlarm(OSALMBalRepeat);
		OS_SetAlarm(OSALMBalRepeat, BALANCEREPEATTIME);

		for(i=1;i<6;i++)
		{
			ADC_StartConvCh3Pin(i);
			OS_WaitTicks(OSALMBalWait,ADCWAITTIME);
			// push voltage of channel into array
			sTemp = ADC_ScaleCell_mV(ADCA.CH3.RES);

			sFilter(&sBalanceCells[i], &sTemp);
		}
		ADC_StartConvCh3Pin(10);
		OS_WaitTicks(OSALMBalWait,ADCWAITTIME);
		sTemp = ADC_ScaleCell_mV(ADCA.CH3.RES);
		sFilter(&sBalanceCells[0], &sTemp);

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
		int16_t mean = 0;
		uint8_t bBalance=0;

		OS_MutexGet(OSMTXCommand);
		uint16_t usBalanceRelease_mV = g_tCommand.usMinBalanceVolt_mV;
		uint16_t usMaxCell_mV = g_tCommand.usVoltageSetpoint_mV;
		OS_MutexRelease(OSMTXCommand);

		// Calculate release
		for(i=0;i<g_tBattery_Info.ucNumberOfCells;i++)
		{
			mean += sBalanceCells[i];
			if(sBalanceCells[i] > usBalanceRelease_mV) // if eine Zelle hoch genug, dann gehts los.
				bBalance = 1;
		}

		// calculate overvolt protection per cell
		uint8_t j=0;
		for(i=0;i<g_tBattery_Info.ucNumberOfCells;i++)
		{
			if(sBalanceCells[i]>usMaxCell_mV)
			{
				j++;
			}
		}
		if(j)
		{
			bBalancerOverload = 1;
		}
		else
		{
			bBalancerOverload = 0;
		}

		// balancing allowed
		if(bBalance == 1 && g_tBattery_Info.eState == eBattCharging)
		{
			mean /= g_tBattery_Info.ucNumberOfCells;
			mean +=3; // add some difference to prevent swinging

			// Balancer logic
			for(i=0;i<6;i++)
			{
				if(sBalanceCells[i] > mean /*|| i >= g_tBattery_Info.ucNumberOfCells*/)
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
				/*if( i >= g_tBattery_Info.ucNumberOfCells)
				{
					// switch on Balancer for this cell
					PORTC.OUTSET = (1<<(2+i));
					ucBalanceBits |= (1<<i);
				}
				else
				{*/
					// switch off Balancer for this cell
					PORTC.OUTCLR = (1<<(2+i));
					ucBalanceBits &= ~(1<<i);
				//}
			}

			//PORTC.OUTCLR = (0b111111<<2);
			ucBalanceBits = 0;
		}

		OS_MutexGet(OSMTXBattInfo);
		// Copy voltage stuff
		for(i=0;i<6;i++)
		{
			g_tBattery_Info.Cells[i].sVoltage_mV = sBalanceCells[i];
			if(ucBalanceBits & (1<<i))
			{
				gucDisChTicks[i]++;
			}
		}
		OS_MutexRelease(OSMTXBattInfo);

		StateMachineBattery();



	}// while(1)
}



void TaskMonitor(void)
{
	uint8_t i;
	int32_t curr_mA, charge_mAs;

	OS_WaitTicks(OSALMonitorRepeat,100); // wait for ADC init

	OS_SetAlarm(OSALMonitorRepeat,1000);
	while(1)
	{
		OS_WaitAlarm(OSALMonitorRepeat);
		OS_SetAlarm(OSALMonitorRepeat,1000);
		// count the charged Q
		if(g_tBattery_Info.eState == eBattCharging)
		{
			OS_MutexGet(OSMTXBattInfo);
			g_tBattery_Info.unCharge_mAs += g_tBattery_Info.sActCurrent_mA;
			g_tBattery_Info.unTimeCharging_s++;
			OS_MutexRelease(OSMTXBattInfo);
		}
		OS_MutexGet(OSMTXBattInfo);
		// calc cell Balancing mAh
		for(i=0;i<6;i++)
		{
			curr_mA = g_tBattery_Info.Cells[i].sVoltage_mV/10;
			charge_mAs = (int32_t)curr_mA * (int32_t)gucDisChTicks[i] * BALANCEREPEATTIME /1000L; // max one tick per run
			g_tBattery_Info.Cells[i].unDisCharge_mAs += charge_mAs;
			gucDisChTicks[i]=0;
		}
		OS_MutexRelease(OSMTXBattInfo);



	}
}

void StateMachineBattery(void) // ONLY run in TaskBalance!
{
	uint8_t NumberOfCells = 0;

	OS_MutexGet(OSMTXBattInfo);
	uint16_t myBattVoltage = g_tBattery_Info.sActVoltage_mV;
	int16_t myBattCurrent = g_tBattery_Info.sActCurrent_mA;
	OS_MutexRelease(OSMTXBattInfo);

	OS_MutexGet(OSMTXCommand);
	uint16_t usCommandCurrent = g_tCommand.sCurrentSetpoint;
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
						ResetLastBatteryInfo();
					}
					break;
				case eModeManual:
					// Manual mode
					if(	GetCellcount(g_tBattery_Info.Cells,&myBattVoltage)== g_tCommand.ucUserCellCount &&
							g_tCommand.ucUserCellCount >0)
					{
						g_tBattery_Info.ucNumberOfCells = g_tCommand.ucUserCellCount;
						g_tBattery_Info.eState = eBattCharging;
						ResetLastBatteryInfo();
					}
					else
					{
						g_tBattery_Info.eState = eBattError; // set Error for Display
					}
					break;
				default:
					break;
			}

			// fixme erase all measured values?
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
					g_tBattery_Info.eState = eBattUnknown;
					// fixme wait here some time
				}
			break;

//		case eBattEmpty:
//			// vollständig angesteckt, Sollwert nicht erreicht
//			if(1/*all cells >= setpoint*/)
//				eBattState = eBattFull;
//			break;
		case eBattFull:
			// vollständig angesteckt, Sollwert erreicht
			if(GetCellcount(g_tBattery_Info.Cells,&myBattVoltage)!=g_tBattery_Info.ucNumberOfCells)
			{
				g_tBattery_Info.ucNumberOfCells = 0;
				g_tBattery_Info.eState = eBattUnknown;
			}
			break;
		case eBattError:
			// fixme // set Event "Battery disconnected" for Display ??

			//g_tBattery_Info.eState = eBattUnknown;
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
	uint8_t i,ucCellCount=0;
	uint16_t usUges_mV;
	int16_t diff;

	usUges_mV = 0;

	for (i= 0;i<6;i++) // 0..5 iterate over possible cell count
	{
		if(cells[i].sVoltage_mV > MINCELLVOLTAGE_mV)
		{
			usUges_mV += cells[i].sVoltage_mV; // sum up cell voltage
			ucCellCount ++;
		}
	}

	diff = usUges_mV - *pusBattVoltage;
	if(abs(diff)< CELLDIFF_mV*ucCellCount)
	{
		// correct!
		return ucCellCount;
	}
	else
	{
		// voltage error -> no exact match
		return 0;
	}

}


void ResetLastBatteryInfo(void)
{
	uint8_t i;
	OS_MutexGet(OSMTXBattInfo);
	g_tBattery_Info.unCharge_mAs = 0;
	g_tBattery_Info.unCharge_mWs = 0;
	g_tBattery_Info.unTimeCharging_s =0;
	for (i = 0; i < 6; ++i)
	{
		g_tBattery_Info.Cells[i].unDisCharge_mAs = 0;
		g_tBattery_Info.Cells[i].unDisCharge_mWs = 0;
	}

	OS_MutexRelease(OSMTXBattInfo);
}

