/* definitions for menu */

#include <avr/io.h>

typedef enum //BatteryStatus
{
	eBattUnknown, // = idle
	eBattCharging,
	eBattFull,
	eBattError,
	//eBattEmpty,
}BatteryStatus_t;

typedef enum
{
	eModeAuto,
	eModeManual
}ChargerMode_t;

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

typedef struct Battery_Balancer_tag
{
	uint8_t ucNumberOfCells; // ONLY changed by Balancer task!
	BatteryCell_t Cells[6]; // ONLY changed by Balancer task!
	uint32_t unCharge_mAs;
	uint32_t unCharge_mWs;
	uint32_t unTimeCharging_s;
	BatteryStatus_t eState;
}Battery_Balancer_t;

typedef struct Battery_Governor_tag
{
	uint16_t usVoltage_mV;
	int16_t  sCurrent_mA;

}Battery_Governor_t;

typedef struct Command_tag
{
	//uint32_t Q_max_mAh; // max mAh
	uint16_t usVoltageSetpoint_mV; // volt per cell
	uint16_t usCurrentSetpoint;  // max current
	uint16_t usMinBalanceVolt_mV;
	uint8_t  ucUserCellCount;
	ChargerMode_t  eChargerMode;
}Command_t;

#define abs(X) (((X)<0)?-(X):(X))
