/*
 * master.h
 *
 *  Created on: 25.05.2010
 *      Author: Besitzer
 */

#ifndef MASTER_H_
#define MASTER_H_

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
	uint32_t unCharge_mAs;
	uint32_t unCharge_mWs;
	uint32_t unDisCharge_mAs;
	uint32_t unDisCharge_mWs;
}BatteryCell_t;

typedef struct Battery_tag
{
	uint16_t usVoltage_mV;
	int16_t  sCurrent_mA;
	uint8_t ucNumberOfCells; // ONLY changed by Balancer task!
	BatteryCell_t Cells[6]; // ONLY changed by Balancer task!
	uint32_t unCharge_mAs;
	uint32_t unCharge_mWs;
	uint32_t unTimeCharging_s;
	BatteryStatus_t eState;

}Battery_t;


















#endif /* MASTER_H_ */
