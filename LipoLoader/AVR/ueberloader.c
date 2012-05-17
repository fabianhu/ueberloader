/*
 This is the Üeberloader
 with N-channel high side driver

 (c) 2010 Fabian Huslik
 */

// fixme refresh kürzer
// todo kalibrierung der Gesamtspannungsmessung / Spannungsabfall über Shunt berücksichtigen -> war schon drin, checken.
// todo Frequenz auf 60kHz festnageln
// todo Leistungslimitierung
// todo Temperaturmessung / Limit
// check Spannungs plausibilisierung unempfindlicher : OK
// check Balancer Limit aktiviert:
// check Balancer Algo verbessert:
// check refresh periode einstellbar
// check Strommessung / Bereichsumschaltung HI ist FALSCH!

#include "OS/FabOS.h"
#include "ueberloader.h"
#include "adc.h"
#include "cal.h"
#include "pwm.h"
#include "usart.h"
#include "serial.h"
#include "eeprom.h"
#include "crc.h"
#include "filter.h"
#include "balancer.h"


//OS_DeclareQueue(DemoQ,10,4);
// Prototypes


// ******** Globals

Command_t g_tCommand;
Battery_Info_t g_tBattery_Info;




//uint16_t I_Max_ABS = 25000;
//volatile uint16_t g_I_filt;
extern int16_t g_asADCDMAvalues[3]; // fast ADC values via DMA
extern uint8_t g_ParReady;
ChargerMode_t g_eChargerMode = eModeAuto;

// *********  Prototypes
uint8_t GetCellcount(void);
extern void emstop(uint8_t e);

#define GOVTEST 0

#define VOLTGOV_KI 100

#define BALANCEMINCHARGECURRENT 50


// ********* Stuff
void TaskGovernor(void)
{
	int16_t sU_in_act, sU_out_act;
	static int16_t /*sU_in_act_flt,*/sU_out_act_flt;
	static int16_t sI_out_act, sI_out_act_flt; // mV / mA
	static int16_t sZeroHiMeas;

	if(sizeof(Command_t) % 2 == 1)
	{
		emstop( 255 ); // Command_t is not 16bit aligned. Not checkable at compile time.
	}

	if(eeprom_ReadBlockWCRC((uint8_t*)&g_tCommand, EEPROM_COMMAND_START, sizeof(Command_t)))
	{
		// set defaults: todo fill with useful values
		g_tCommand.basefrequency = 60;
		g_tCommand.refreshrate = 10;
		g_eChargerMode = eModeAuto;
		g_tCommand.sCurrentSetpoint = 1000;
		g_tCommand.ucUserCellCount = 6;
		g_tCommand.unQ_max_mAs = 100000;
		g_tCommand.usMinBalanceVolt_mV = 2000;
		g_tCommand.usT_max_s = 10000;
		g_tCommand.usVoltageSetpoint_mV = 3850;
	}

	g_ParReady = 1;

	vPWM_Init();
	ADCinit(); // call only in governor task

	OS_WaitTicks(OSALMWaitGov,1); // wait during timer sync

	PORTD.PIN1CTRL = 0b01000000; // invert Waveform at PORTD Pin1
	PORTC.DIRSET = 0b00000011; // set Port C as output
	PORTD.DIRSET = 0b00001011; // set Port D as output with LED

	ADC_ActivateHiCurrentMeas();

	EVSYS.STROBE = ( 1 << 7 ); //fire event 7, which triggers the ADC
	OS_WaitTicks(OSALMWaitGov,100); // wait during first ADC conversion

	sZeroHiMeas = g_asADCDMAvalues[2]; // todo filter over several samples

	//ADC_ActivateLoCurrentMeas();
    CalibInit();

	OS_WaitTicks(OSALMWaitGov,200); // wait for first slow ADC loop...



	while(1)
	{

		EVSYS.STROBE = ( 1 << 7 ); //fire event 7, which triggers the ADC

		OS_WaitTicks(OSALMWaitGov,1); // wait during ADC conversion

		sU_in_act = ADC_ScaleVolt_mV( g_asADCDMAvalues[0]);
		sU_out_act = ADC_ScaleVolt_mV( g_asADCDMAvalues[1]); // corrected below!
		sFilterVar( &sU_out_act_flt, &sU_out_act,16 );

		if(( ADCA.CH2.MUXCTRL & ( 0xf << 3 ) ) == ADC_CH_MUXPOS_PIN7_gc) // is high current config...
		{
			// high current
			sI_out_act = ADC_ScaleHighAmp_mA( g_asADCDMAvalues[2], sZeroHiMeas );
			// voltage measurement correction
			sU_out_act -= sI_out_act * 1 / 125; // High current resistor value (0.005R)(+3mR MosFet) * Current
		}
		else
		{
			// low current
			sI_out_act = ADC_ScaleLowAmp_mA( g_asADCDMAvalues[2]);
			// voltage measurement correction
			sU_out_act -= sI_out_act * 2 / 19; // Low current resistor value (0.105R) * Current
		}

		if(sI_out_act > 2500)
			ADC_ActivateHiCurrentMeas();
		else
			if(sI_out_act < 2000)
				ADC_ActivateLoCurrentMeas();

		sFilter( &sI_out_act_flt, &sI_out_act );


		// int32_t nConverterPower_W = ( ( sI_out_act ) * ( sU_out_act /*- usU_in_act*/) ) / 1000 / 1000; // fixme converter pwr

		OS_MutexGet( OSMTXBattInfo );
		g_tBattery_Info.sActVoltage_mV = sU_out_act_flt;
		g_tBattery_Info.sActCurrent_mA = sI_out_act_flt;
		// g_tBattery_Info.usConverterPower_W = (uint16_t)abs(nConverterPower_W); // fixme wieder rein
		OS_MutexRelease( OSMTXBattInfo );

		// just in case...

		// supply undervoltage
		if(sU_in_act < 7000)
			emstop( 1 );
		// supply overvoltage
		if(sU_in_act > 22000)
			emstop( 2 );

		if(g_tBattery_Info.eState == eBattCharging)
		{
			if(sI_out_act > 10000 && abs(sI_out_act_flt) > 100)
				emstop( 3 );
		}

		static uint8_t errcntOverVolt = 0;
		if(sU_out_act > 4200 * 6 && abs(sI_out_act_flt) > 100) // catch the governor
			errcntOverVolt++;
		else
			errcntOverVolt = 0;

		if(errcntOverVolt > 10)
			emstop( 4 );

		OS_DISABLEALLINTERRUPTS;
		int16_t myISetpoint = g_tCommand.sCurrentSetpoint;
		int16_t myUSetpoint = g_tCommand.usVoltageSetpoint_mV
				* g_tBattery_Info.ucNumberOfCells;
		g_tBattery_Info.sVSetpoint = myUSetpoint;
		OS_ENABLEALLINTERRUPTS;

		// calculate Current setpoint
		int16_t I_Set_mA_Ramped = 0;
		static int32_t U_Integrator = 0;

		if(g_tBattery_Info.eState == eBattCharging)
		{
			if(Balancer_GetOverload())
			{
				myISetpoint = min(BALANCEMINCHARGECURRENT,myISetpoint);
			}


			I_Set_mA_Ramped = PID(myUSetpoint-sU_out_act_flt, &U_Integrator,0,VOLTGOV_KI,0,0,myISetpoint); // todo adjust integrator


			if(myISetpoint <= 0)
			{
				I_Set_mA_Ramped = 0; // switch off on zero.
				U_Integrator=0; // reset integrator!!
			}

		}
		else
		{
			I_Set_mA_Ramped = 0;
			U_Integrator=0; // reset integrator!!
		}

		vGovernor( I_Set_mA_Ramped, sI_out_act );

	}
}

// do the stuff, which is to be done every second.
void TaskMonitor(void)
{

	OS_WaitTicks(OSALMonitorRepeat,100); // wait for ADC init

	OS_SetAlarm( OSALMonitorRepeat, 1000 );
	while(1)
	{
		OS_WaitAlarm( OSALMonitorRepeat );
		OS_SetAlarm( OSALMonitorRepeat, 1000 );
		// count the charged total Q
		if(g_tBattery_Info.eState == eBattCharging)
		{
			OS_MutexGet( OSMTXBattInfo );
			g_tBattery_Info.unCharge_mAs += g_tBattery_Info.sActCurrent_mA;
			g_tBattery_Info.unTimeCharging_s++;
			OS_MutexRelease( OSMTXBattInfo );
		}
	}
}

#define CELLDIFF_mV 300 // millivolt tolerance per cell to total voltage
#define MINCELLVOLTAGE_mV 2000 // minimum cell voltage
// Calculate cell count ; Return cell count. 0 = error
uint8_t GetCellcount(void)
{
	uint8_t i, ucCellCount = 0;
	uint16_t usUges_mV;
	int16_t diff;
	int16_t myCellVoltage[6];
	int16_t myBattVoltage;

	usUges_mV = 0;

	OS_MutexGet( OSMTXBattInfo );
	myBattVoltage = g_tBattery_Info.sActVoltage_mV;
	for(i = 0; i < 6 ; i++)
	{
		myCellVoltage[i] = g_tBattery_Info.Cells[i].sVoltage_mV;
	}
	OS_MutexRelease( OSMTXBattInfo );

	for(i = 0; i < 6 ; i++) // 0..5 iterate over possible cell count
	{
		if(myCellVoltage[i] > MINCELLVOLTAGE_mV)
		{
			usUges_mV += myCellVoltage[i]; // sum up cell voltage
			ucCellCount++;
		}
	}

	diff = usUges_mV - myBattVoltage;
	if(abs(diff) < CELLDIFF_mV * ucCellCount)
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
	OS_MutexGet( OSMTXBattInfo );
	g_tBattery_Info.unCharge_mAs = 0;
	g_tBattery_Info.unCharge_mWs = 0;
	g_tBattery_Info.unTimeCharging_s = 0;
	for(i = 0; i < 6 ; ++i)
	{
		g_tBattery_Info.Cells[i].unDisCharge_mAs = 0;
		g_tBattery_Info.Cells[i].unDisCharge_mWs = 0;
	}

	OS_MutexRelease( OSMTXBattInfo );
}

#define CHARGEDELAY 10 // equals 1s

void TaskState(void)
{
	uint8_t i, j, t, to;

	while(1)
	{
		OS_WaitEvent( OSEVTState );

		uint8_t NumberOfCells = 0;

		OS_MutexGet( OSMTXBattInfo );
		uint16_t myBattVoltage = g_tBattery_Info.sActVoltage_mV;
		int16_t myBattCurrent = g_tBattery_Info.sActCurrent_mA;
		OS_MutexRelease( OSMTXBattInfo );

		OS_MutexGet( OSMTXCommand );
		uint16_t usCommandCurrent = g_tCommand.sCurrentSetpoint;
		OS_MutexRelease( OSMTXCommand );

		switch(g_tBattery_Info.eState)
		{
			case eBattWaiting:
				// nicht vollständig angesteckt
				switch(g_eChargerMode)
				{
					case eModeAuto:
						// charge if ok.
						j = 0;
						to = GetCellcount();
						for(i = 0; i < CHARGEDELAY ; i++)
						{
							OS_WaitTicks(OSALMStateWait,100); // sums up to 1s
							t = GetCellcount();
							if(t == to)
								j++;
							to = t;
						}

						NumberOfCells = GetCellcount();
						if(NumberOfCells > 0 && j == CHARGEDELAY) // it was equal for 1s...
						{
							OS_WaitTicks(OSALMStateWait,100);
							g_tBattery_Info.ucNumberOfCells = NumberOfCells;
							g_tBattery_Info.eState = eBattCharging;
							ResetLastBatteryInfo();
						}
						break;
					case eModeManual:
						// Manual mode
						if(GetCellcount() == g_tCommand.ucUserCellCount
								&& g_tCommand.ucUserCellCount > 0) // todo manual mode aktivieren
						{
							OS_WaitTicks(OSALMStateWait,100);
							g_tBattery_Info.ucNumberOfCells
									= g_tCommand.ucUserCellCount;
							g_tBattery_Info.eState = eBattCharging;
							ResetLastBatteryInfo();
						}
						else
						{
							g_tBattery_Info.eState = eBattError; // set Error for Display
						}
						break;
					case eModeStop:
						// Manual Stop mode
						// how to get out of here? -> the mode is set manually to another.
						break;
					default:
						break;
				}
				break;
			case eBattCharging:
				// Charging!
				switch(g_eChargerMode)
				{
					case eModeManual: // todo NOT YET IMPEMENTED
					case eModeAuto:
						if(myBattVoltage >= g_tCommand.usVoltageSetpoint_mV
								* g_tBattery_Info.ucNumberOfCells
								&& myBattCurrent < usCommandCurrent / 10 && Balancer_GetFinished())
						{
							g_tBattery_Info.eState = eBattFull;
						}
						if(GetCellcount() != g_tBattery_Info.ucNumberOfCells)
						{
							g_tBattery_Info.ucNumberOfCells = 0;
							g_tBattery_Info.eState = eBattUnknown;
						}
						break;
					case eModeStop:
						// Manual Stop mode
						g_tBattery_Info.eState = eBattWaiting;
						// how to get out of here? -> the mode is set manually to another.
						break;
					default:
						break;
				}
				break;

				//		case eBattEmpty:
				//			// vollständig angesteckt, Sollwert nicht erreicht
				//			if(1/*all cells >= setpoint*/)
				//				eBattState = eBattFull;
				//			break;
			case eBattFull:
				// vollständig angesteckt, Sollwert erreicht
				if(GetCellcount() != g_tBattery_Info.ucNumberOfCells)
				{
					g_tBattery_Info.ucNumberOfCells = 0;
					g_tBattery_Info.eState = eBattUnknown;
				}
				break;

			case eBattUnknown:
				OS_WaitTicks(OSALMStateWait,7000);
				g_tBattery_Info.eState = eBattWaiting;
				break;

			case eBattError:
				OS_WaitTicks(OSALMStateWait,10000);
				g_eChargerMode = eActModeStop;
				g_tBattery_Info.eState = eBattWaiting;
				break;
			default:
				emstop( 22 );
				break;

		}
	}
}

