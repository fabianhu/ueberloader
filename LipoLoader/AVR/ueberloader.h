/* definitions for menu */

#include <avr/io.h>

typedef struct ADC_Values_tag
{
	int16_t		TempInt[2];
	int16_t		VCC_mVolt;
	int16_t 	Bandgap;
	int16_t		TempCPU;
}ADC_Values_t;

typedef struct BatteryCell_tag
{
	uint16_t usVoltage_mV;
	uint32_t unCharge_mAs;
	uint32_t unCharge_mWs;
	uint32_t unDisCharge_mAs;
	uint32_t unDisCharge_mWs;
/*
 * Ri = delta U / (delta) I
 *
 * */
}BatteryCell_t;

typedef struct Battery_tag
{
	uint8_t ucNumberOfCells;
	BatteryCell_t Cells[6];
	uint16_t usVoltage_mV;
	uint32_t unCharge_mAs;
	uint32_t unCharge_mWs;
	uint32_t unTimeCharging_s;
}Battery_t;


typedef struct Command_tag
{
	uint32_t Q_max_mAh; // max mAh
	uint16_t U_Setpoint_mV; // volt per cell
	uint16_t I_Max_Set_mA;  // max current
	uint16_t MinBalance_mV;
	uint8_t  Go;
}Command_t;

enum BatteryStatus
{
	eBattUnknown,
	eBattEmpty,
	eBattfull,
	eBattError
};
