/*
 * SHARED serial.h
 *
 *  Created on: 25.05.2010
 *      Author: Fabian
 */

#ifndef SERIAL_H_
#define SERIAL_H_

#include <avr/io.h>

#define UCI_SET_CMDs 			22 // Set actual command struct (Command_t) (no answer expected todo
#define UCI_GET_CMDs			33 // Get Actual command setting (Command_t)
#define UCI_GET_OPVs 			44 // Get actual Operating values (Battery_Info_t)
#define UCI_GET_INTs 			55 // todo

typedef enum //BatteryStatus
{
	eBattUnknown, // = idle
	eBattCharging,
	eBattFull,
	eBattError,
	//eBattEmpty,
}BatteryStatus_t;

typedef struct BatteryCell_tag
{
	uint16_t usVoltage_mV;
	uint32_t unDisCharge_mAs;
	uint32_t unDisCharge_mWs;
	uint32_t unDisChTicks; // this will vanish, if the other is working!
}BatteryCell_t;

typedef struct Battery_Info_tag
{
	BatteryStatus_t eState;
	uint16_t usActVoltage_mV;
	int16_t  sActCurrent_mA;
	uint8_t  ucNumberOfCells;
	uint32_t unCharge_mAs;
	uint32_t unCharge_mWs;
	uint32_t unTimeCharging_s;

	BatteryCell_t Cells[6];

}Battery_Info_t;

typedef enum
{
	eModeAuto,
	eModeManual
}ChargerMode_t;

typedef struct Command_tag
{
	uint32_t unQ_max_mAs; // max Capacity
	uint16_t usVoltageSetpoint_mV; // volt per cell
	uint16_t usCurrentSetpoint;  // max current
	uint16_t usMinBalanceVolt_mV; // voltage to start balancing
	uint8_t  ucUserCellCount; // 0 for auto
	ChargerMode_t  eChargerMode;
}Command_t;

typedef struct UCIFrame_tag
{
	uint8_t ID;
	uint8_t UCI;
	uint8_t crc;
	uint8_t len;
	uint8_t values[sizeof(Battery_Info_t)]; // biggest element to be sent/recvd
}UCIFrame_t;

#define UCIHEADERLEN 4

void UCISendBlockCrc( UCIFrame_t* pU);
uint8_t UCIGetCRC( UCIFrame_t* pU);



#endif /* SERIAL_H_ */




