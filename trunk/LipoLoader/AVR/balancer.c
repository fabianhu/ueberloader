#include "OS/FabOS.h"
#include "ueberloader.h"
#include "adc.h"
#include "filter.h"
#include "serial.h"
#include "cal.h"
#include "pwm.h" // for "min"

extern Battery_Info_t g_tBattery_Info; // todo avoid

extern Command_t g_tCommand;

typedef struct ADC_Values_tag
{
	int16_t TempInt;
	int16_t VCC_mVolt;
	int16_t Bandgap;
	int16_t TempCPU;
} ADC_Values_t;

#define BALANCEFINISHCOUNT 10

uint8_t ucBalancerFinished = BALANCEFINISHCOUNT; // 0 indicated balancer is finished
//uint8_t aucDisChTicks[6]; // increased for each discharge cycle
uint32_t aunCharge_mAs[6]; // accumulated charge
ADC_Values_t g_tADCValues;

uint16_t BattVoltFromBalancer;
int16_t CurrentSetpointfromBal;


#define ADCWAITTIME 2
#define BALANCEREPEATTIME 100
#define DECIDEEVERYNCYCLES 2  // 1= every second time
#define BALANCEROHMS 22 // resistor value!

uint8_t g_ucCalCommand = 0;

void sFilterBalancer(int16_t* o, int16_t* n) // with jump possibility, if filtered value is off by more than 10%
{
	int32_t temp;
	int16_t out;

	temp = (int32_t)*o * 7L + (int32_t)*n;
	out = ( temp + 5L ) / 8L; // "halbes dazu, wg Rundungsfehler"

	// check, if difference larger than 10% of old.
	if(abs(*n-*o) > *o / 10)
	{
		*o = *n;
	}
	else
	{
		*o = out;
	}
}

void TaskBalance(void)
{
	int32_t nTemp;
	int16_t sTemp;
	int16_t sBalanceCellsRaw[6]; // quasi static
	int16_t sBalanceCells[6]; // quasi static
	uint8_t ucBalanceBits = 0;
	uint8_t i, j;
	// balancing allowed
	static uint8_t EveryNCycles = 0; // 0 = Balancer is guaranteed to be off.

	// direction for balancer pins
	PORTC.DIRSET = 0b11111100;

	OS_WaitTicks( OSALMBalRepeat, 100 );
	// wait for ADC init

	ADC_StartConvCh3Pin( 0 );

	while(1)
	{
		OS_WaitAlarm( OSALMBalRepeat );
		OS_SetAlarm( OSALMBalRepeat, BALANCEREPEATTIME );

		if(EveryNCycles == 0) // measure only, if the balancer was off!!
		{
			
			for(j = 0; j < 8 ; j++) // do conversion 8 times and filter
			{
				for(i = 1; i < 6 ; i++)
				{
					ADC_StartConvCh3Pin( i );
					OS_WaitTicks( OSALMBalWait, ADCWAITTIME );
					// push voltage of channel into array
					sTemp = ADC_ScaleCell_mV( ADCA.CH3.RES );

					sFilterBalancer( &sBalanceCellsRaw[i], &sTemp );
				}

				ADC_StartConvCh3Pin( 10 );
				OS_WaitTicks( OSALMBalWait, ADCWAITTIME );
				sTemp = ADC_ScaleCell_mV( ADCA.CH3.RES );
				sFilterBalancer( &sBalanceCellsRaw[0], &sTemp );
			}
		}

		ADC_StartConvCh3Pin( 11 ); // temperature external2
		OS_WaitTicks( OSALMBalWait, ADCWAITTIME );
		nTemp = ADCA.CH3.RES;
		OS_DISABLEALLINTERRUPTS;
		g_tADCValues.TempInt = nTemp; // fixme scaling!
		OS_ENABLEALLINTERRUPTS;

		ADC_StartConvInt( 0 ); // CPU temperature
		OS_WaitTicks( OSALMBalWait, ADCWAITTIME );
		nTemp = ADCA.CH3.RES * 10 / 33; // what would be measured, if it was done at 1V ref (/ 3.3).
		nTemp = ( nTemp * ( 273ul + 85ul ) / g_tCalibration.usCPUTemp85C );
		OS_DISABLEALLINTERRUPTS;
		g_tADCValues.TempCPU = nTemp; // fixme scaling!
		OS_ENABLEALLINTERRUPTS;

		ADC_StartConvInt( 1 ); // Bandgap reference
		OS_WaitTicks( OSALMBalWait, ADCWAITTIME );
		OS_DISABLEALLINTERRUPTS;
		g_tADCValues.Bandgap = ADCA.CH3.RES; // bit value for 1.10V ! at ref = Usupp/1.6
		OS_ENABLEALLINTERRUPTS;
		nTemp = ( 2048ul * 1105ul ) / g_tADCValues.Bandgap; // 1088ul old value // by knowing, that the voltage is 1.088V, we calculate the ADCRef voltage.
		sTemp = nTemp;
		OS_DISABLEALLINTERRUPTS;
		sFilter( &g_tCalibration.sADCRef_mV, &sTemp );
		OS_ENABLEALLINTERRUPTS;

		ADC_StartConvInt( 2 ); // VCC_mVolt measurement
		OS_WaitTicks( OSALMBalWait, ADCWAITTIME );
		nTemp = ADCA.CH3.RES * 10ul;
		nTemp = nTemp * g_tCalibration.sADCRef_mV / 2048ul;

		OS_DISABLEALLINTERRUPTS;
		g_tADCValues.VCC_mVolt = nTemp;
		OS_ENABLEALLINTERRUPTS;

		// end of adc conversons

		switch(g_ucCalCommand)
		{
			case 1:
				CalCellsLow( &sBalanceCellsRaw[0] );
				g_ucCalCommand = 0;
				break;
			case 2:
				CalCellsHigh( &sBalanceCellsRaw[0] );
				g_ucCalCommand = 0;
				break;
			default:
				break;
		}

		// ok, now we have all cells, now apply the calibration
		Cal_Apply( &sBalanceCellsRaw[0], &sBalanceCells[0] );
		
		
		// Calculate base values
		uint16_t minCellVolt = 4300;
		uint16_t maxCellVolt = 0;
		uint16_t SumCellVolts = 0;
		
		for(i = 0; i < g_tBattery_Info.ucNumberOfCells ; i++)
		{
			minCellVolt = min(sBalanceCells[i],minCellVolt);
			maxCellVolt = max(sBalanceCells[i],maxCellVolt);
			SumCellVolts += sBalanceCells[i];
		}			

		BattVoltFromBalancer = SumCellVolts;
		
		
		OS_MutexGet( OSMTXCommand );
		uint16_t usBalanceRelease_mV = g_tCommand.usMinBalanceVolt_mV;
		int16_t myISetpoint = g_tCommand.sCurrentSetpoint;
		uint16_t myUSetpoint = g_tCommand.usVoltageSetpoint_mV;
		OS_MutexRelease( OSMTXCommand );
		

		static int32_t U_Integrator = 0;
		int16_t I_Set_mA_Ramped;
		#define VOLTGOV_KP 1 // /32
		#define VOLTGOV_KI 10000   // alt und gut bewährt, etwas langsam: 8000 war gut  // bei 16000 geht der Strom vor dem Abschalten stark rauf und runter  --> zu viel
		#define VOLTGOV_KD 0 // inactive in PID()!
		#define BALANCEMINCHARGECURRENT 2300/BALANCEROHMS/2 // 100 for 22R
		
		
		if(EveryNCycles == 0) // only control if balancer is off.
		{
			if(g_tBattery_Info.eState == eBattCharging)
			{
				int32_t diff = (int32_t)myUSetpoint - (int32_t)maxCellVolt + 2;			 // Stromregler lädt die 2mV höher als eigentlich eingestellt --> verbessertes Abschaltverhalten	
				if(maxCellVolt > myUSetpoint + 10)										// 10 mV more than desired --> Regler erschrickt und reduziert schnell
				{
					if(maxCellVolt > myUSetpoint + 20)									// 20 mV more than desired --> Regler erschrickt und reduziert schnell
					{
					I_Set_mA_Ramped = I_Set_mA_Ramped - (I_Set_mA_Ramped/10);			// reduce by 10%
					}
					else
					{
						I_Set_mA_Ramped = I_Set_mA_Ramped - (I_Set_mA_Ramped/100);		// reduce by 1%	
					}
					PID(0 , &U_Integrator, 0, 0, 0, 0, I_Set_mA_Ramped);
				}
				else
				{
					I_Set_mA_Ramped = PID(diff , &U_Integrator, VOLTGOV_KP, VOLTGOV_KI, VOLTGOV_KD, 0, myISetpoint ); // todo adjust integrator			
				}
				if(I_Set_mA_Ramped <= 0)
				{
					I_Set_mA_Ramped = 0; // switch off on zero.
				}
			}
			else
			{
				I_Set_mA_Ramped = 0;
				U_Integrator = 0; // reset integrator!!
			}
			OS_DISABLEALLINTERRUPTS;
			CurrentSetpointfromBal = I_Set_mA_Ramped;
			OS_ENABLEALLINTERRUPTS;

			if(maxCellVolt > usBalanceRelease_mV && g_tBattery_Info.eState == eBattCharging )		// wenn die Zelle mit der höchsten Spannung, mehr Spannung hat als die balActVolt
			{

				// Balancer logic
				for(i = 0; i < 6; i++)// process all cells
				{
					if(sBalanceCells[i] > minCellVolt + 2 ) // 2mV for hysteresis   // schwere Geburt		//Wenn die Zelle mehr Spannung hat als die mit der niedrigsten Spannung --> balancieren
					{
						// switch on Balancer for this cell
						PORTC.OUTSET = ( 1 << ( 2 + i ) );
						ucBalanceBits |= ( 1 << i );

						int32_t curr_mA;
						curr_mA = sBalanceCells[i] / BALANCEROHMS;
						aunCharge_mAs[i] = (int32_t)curr_mA * BALANCEREPEATTIME / 1000L;// add the mAs produced in this cycle
						ucBalancerFinished = BALANCEFINISHCOUNT;

					}
					else
					{
						// switch off Balancer for this cell
						PORTC.OUTCLR = ( 1 << ( 2 + i ) );
						ucBalanceBits &= ~( 1 << i );
					
					}
				}
				if(ucBalanceBits == 0)
					if(ucBalancerFinished > 0) ucBalancerFinished--;
			}		
		}
		EveryNCycles++;
		
		if(EveryNCycles > DECIDEEVERYNCYCLES)
		{
			// balancer off
			PORTC.OUTCLR = (0b111111<<2);
			ucBalanceBits = 0;
			
			EveryNCycles = 0; // cycle 0 after wait had the balancer off.
		}

		
		
		// copy stuff for other tasks
		OS_MutexGet( OSMTXBattInfo );

		for(i = 0; i < 6 ; i++)
		{
			// Copy voltage stuff
			g_tBattery_Info.Cells[i].sVoltage_mV = sBalanceCells[i];
			g_tBattery_Info.Cells[i].unDisCharge_mAs += aunCharge_mAs[i];
			aunCharge_mAs[i] = 0;
		}
		OS_MutexRelease( OSMTXBattInfo );

		//StateMachineBattery();

		OS_SetEvent( OSTSKState, OSEVTState ); // do in separate task to stay in pace.

	} // while(1)
}


uint8_t Balancer_GetFinished(void)
{		
	return (ucBalancerFinished == 0)?1:0;						// wenn Balancer fertig ist --> es kann abgeschalten werden
}

uint16_t BalancerGetBattVolt(void)
{
	uint16_t ret;
	OS_DISABLEALLINTERRUPTS;
	ret = BattVoltFromBalancer;
	OS_ENABLEALLINTERRUPTS;
	return ret;
}

int16_t BalancerGetCurrentSetp(void)
{
	int16_t ret = 0;
	OS_DISABLEALLINTERRUPTS;
	if(g_tBattery_Info.eState == eBattCharging)
	{	
		ret = CurrentSetpointfromBal;
	}		
	OS_ENABLEALLINTERRUPTS;
	return ret;
}
