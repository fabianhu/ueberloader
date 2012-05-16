#include "OS/FabOS.h"
#include "adc.h"
#include "filter.h"
#include "serial.h"
#include "cal.h"

extern Battery_Info_t g_tBattery_Info; // todo avoid

extern Command_t g_tCommand ;

typedef struct ADC_Values_tag
{
	int16_t		TempInt;
	int16_t		VCC_mVolt;
	int16_t 	Bandgap;
	int16_t		TempCPU;
}ADC_Values_t;

uint8_t bBalancerOverload; // True, if one cell has reached Voltage limit.
//uint8_t aucDisChTicks[6]; // increased for each discharge cycle
uint32_t aunCharge_mAs[6]; // accumulated charge
ADC_Values_t g_tADCValues;

#define ADCWAITTIME 1
#define BALANCEREPEATTIME 330L

uint8_t g_ucCalCommand = 0;


void TaskBalance(void)
{
	int32_t nTemp;
	int16_t sTemp;
	int16_t sBalanceCellsRaw[6]; // quasi static
	int16_t sBalanceCells[6]; // quasi static
	uint8_t ucBalanceBits = 0;
	uint8_t i;

	// direction for balancer pins
	PORTC.DIRSET = 0b11111100;

	OS_WaitTicks(OSALMBalRepeat,100); // wait for ADC init

	ADC_StartConvCh3Pin( 0 );

	while(1)
	{
		OS_WaitAlarm( OSALMBalRepeat );
		OS_SetAlarm( OSALMBalRepeat, BALANCEREPEATTIME);

		for(i = 1; i < 6 ; i++)
		{
			ADC_StartConvCh3Pin( i );
			OS_WaitTicks(OSALMBalWait,ADCWAITTIME);
			// push voltage of channel into array
			sTemp = ADC_ScaleCell_mV( ADCA.CH3.RES );

			sFilter( &sBalanceCellsRaw[i], &sTemp );
		}
		ADC_StartConvCh3Pin( 10 );
		OS_WaitTicks(OSALMBalWait,ADCWAITTIME);
		sTemp = ADC_ScaleCell_mV( ADCA.CH3.RES );
		sFilter( &sBalanceCellsRaw[0], &sTemp );


		switch (g_ucCalCommand) {
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
		CalibrateCells(&sBalanceCellsRaw[0], &sBalanceCells[0]);


		ADC_StartConvCh3Pin( 11 ); // temperature external2
		OS_WaitTicks(OSALMBalWait,ADCWAITTIME);
		nTemp = ADCA.CH3.RES;
		OS_DISABLEALLINTERRUPTS;
		g_tADCValues.TempInt = nTemp; // fixme scaling!
		OS_ENABLEALLINTERRUPTS;

		ADC_StartConvInt( 0 ); // CPU temperature
		OS_WaitTicks(OSALMBalWait,ADCWAITTIME);
		nTemp = ADCA.CH3.RES * 10 / 33; // what would be measured, if it was done at 1V ref (/ 3.3).
		nTemp = ( nTemp * ( 273ul + 85ul ) / g_tCalibration.usCPUTemp85C );
		OS_DISABLEALLINTERRUPTS;
		g_tADCValues.TempCPU = nTemp; // fixme scaling!
		OS_ENABLEALLINTERRUPTS;

		ADC_StartConvInt( 1 ); // Bandgap reference
		OS_WaitTicks(OSALMBalWait,ADCWAITTIME);
		OS_DISABLEALLINTERRUPTS;
		g_tADCValues.Bandgap = ADCA.CH3.RES; // bit value for 1.10V ! at ref = Usupp/1.6
		OS_ENABLEALLINTERRUPTS;
		nTemp = ( 2048ul * 1105ul ) / g_tADCValues.Bandgap; // 1088ul old value // by knowing, that the voltage is 1.088V, we calculate the ADCRef voltage.
		OS_DISABLEALLINTERRUPTS;
		g_tCalibration.sADCRef_mV = nTemp;
		OS_ENABLEALLINTERRUPTS;

		ADC_StartConvInt( 2 ); // VCC_mVolt measurement
		OS_WaitTicks(OSALMBalWait,ADCWAITTIME);
		nTemp = ADCA.CH3.RES * 10ul;
		nTemp = nTemp * g_tCalibration.sADCRef_mV / 2048ul;
		OS_DISABLEALLINTERRUPTS;
		g_tADCValues.VCC_mVolt = nTemp;
		OS_ENABLEALLINTERRUPTS;

		// mean Cell Voltage
		int16_t mean = 0;
		uint8_t bBalance = 0;

		OS_MutexGet( OSMTXCommand );
		uint16_t usBalanceRelease_mV = g_tCommand.usMinBalanceVolt_mV;
		uint16_t usMaxCell_mV = g_tCommand.usVoltageSetpoint_mV;
		OS_MutexRelease( OSMTXCommand );

		// Calculate release
		for(i = 0; i < g_tBattery_Info.ucNumberOfCells ; i++)
		{
			mean += sBalanceCells[i];
			if(sBalanceCells[i] > usBalanceRelease_mV) // if eine Zelle hoch genug, dann gehts los.
				bBalance = 1;
		}
		mean = mean / g_tBattery_Info.ucNumberOfCells;

		// balancing allowed
		static uint8_t onlyEveryThreeCycles = 0;

		onlyEveryThreeCycles++;

		if(bBalance == 1 && g_tBattery_Info.eState == eBattCharging && onlyEveryThreeCycles >= 3)
		{
			// Balancer logic
			for(i = 0; i < 6 ; i++) // process all cells
			{
				if(sBalanceCells[i] > mean )
				{
					// switch on Balancer for this cell
					PORTC.OUTSET = ( 1 << ( 2 + i ) );
					ucBalanceBits |= ( 1 << i );


					int32_t curr_mA;
					curr_mA = sBalanceCells[i] / 22; // 22 Ohms
					aunCharge_mAs[i] = (int32_t)curr_mA * BALANCEREPEATTIME / 1000L; // add the mAs produced in this cycle
				}
				else
				{
					// switch off Balancer for this cell
					PORTC.OUTCLR = ( 1 << ( 2 + i ) );
					ucBalanceBits &= ~( 1 << i );
				}
			}
			onlyEveryThreeCycles = 0;
		}
		else
		{
			// balancer off
			PORTC.OUTCLR = (0b111111<<2);
			ucBalanceBits = 0;
		}

		// calculate overvolt protection per cell
		uint8_t j = 0;
		for(i = 0; i < g_tBattery_Info.ucNumberOfCells ; i++)
		{
			if(sBalanceCells[i] > usMaxCell_mV)
			{
				j++;
			}
		}
		if(j>0)
		{
			bBalancerOverload = 1;
		}
		else
		{
			bBalancerOverload = 0;
		}


		// copy stuff for other tasks
		OS_MutexGet( OSMTXBattInfo );

		for(i = 0; i < 6 ; i++)
		{
			// Copy voltage stuff
			g_tBattery_Info.Cells[i].sVoltage_mV = sBalanceCells[i];
			g_tBattery_Info.Cells[i].unDisCharge_mAs += aunCharge_mAs[i];
			aunCharge_mAs[i]=0;
		}
		OS_MutexRelease( OSMTXBattInfo );

		//StateMachineBattery();

		OS_SetEvent( OSTSKState, OSEVTState); // do in separate task to stay in pace.

	}// while(1)
}

uint8_t Balancer_GetOverload(void)
{
	return bBalancerOverload;
}
