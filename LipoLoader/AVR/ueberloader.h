/* definitions for menu */

#include <avr/io.h>

typedef struct ADC_Values_tag
{
	int16_t		Cell_mVolt[6];
	int16_t		TempInt[2];
	int16_t		VCC_mVolt;
	int16_t 	Bandgap;
	int16_t		TempCPU;
}ADC_Values_t;

typedef struct AccuCell_tag
{
	uint8_t  bUsed:1;
	uint8_t  :7;
	uint16_t usVoltage_mV;
	uint32_t unCharge_mAs;
	uint32_t unCharge_mWs;
	uint32_t unDisCharge_mAs;
	uint32_t unDisCharge_mWs;
/*
 * Ri = delta U / (delta) I
 *
 * */
}AccuCell_t;

typedef struct Accu_tag
{
	uint8_t ucNumberOfCells;
	AccuCell_t Cells[6];
	uint16_t usVoltage_mV;
	uint32_t unCharge_mAs;
	uint32_t unCharge_mWs;


}Accu_t;
