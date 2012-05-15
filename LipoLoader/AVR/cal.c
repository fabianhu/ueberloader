// Calibration

#include "OS/FabOS.h"
#include "eeprom.h"
#include "cal.h"

#define GAINMULTIPLIER 10000ul


Calibration_t g_tCalibration;

typedef struct t_CalibData_tag
{
	int16_t anCellGain[6]; // = 	{ GAINMULTIPLIER, GAINMULTIPLIER, GAINMULTIPLIER, GAINMULTIPLIER,GAINMULTIPLIER, GAINMULTIPLIER };
	int16_t anCellOffset[6];// =  { 0, 0, 0, 0, 0, 0 };
	int16_t VoltGain;
	int16_t VoltOffset;
}t_CalibData_t;

t_CalibData_t t_CalibData;

int32_t za, zb, zc;

void CalibInit(void)
{
	// read back the values from eeprom
	if(eeprom_ReadBlockWCRC((uint8_t*)&t_CalibData,(void*)(EEPROM_CALIB_START),sizeof(t_CalibData_t)))
	{
		// crc fail
		for(int i = 0 ; i < 6 ; ++i)
		{
			t_CalibData.anCellOffset[i] = 0;
			t_CalibData.anCellGain[i] = GAINMULTIPLIER;
			t_CalibData.VoltOffset = 0;
			t_CalibData.VoltGain = GAINMULTIPLIER;
		}
	}
	else
	{
		// crc good
	}

}


int16_t Calibrate(int16_t raw, int32_t gain, int16_t offset)
{

	za = (int32_t)raw + (int32_t)offset;

	zb = za * gain;

	zc = zb / GAINMULTIPLIER;

	return (int16_t)zc;
}

int16_t FindOffset(int16_t raw) // only applicable at 0-point !!!
{
	return -raw;
}

int32_t ret;
int32_t a, b;
int16_t FindGain(int16_t raw, int16_t expected, int16_t offset)
{

	a = (int32_t)expected * GAINMULTIPLIER;

	b = (int32_t)raw + (int32_t)offset;

	ret = a / b;

	return (int16_t)ret;
}

void CalCellsLow(int16_t* values)
{
	for(int i = 0 ; i < 6 ; ++i)
	{
		t_CalibData.anCellOffset[i] = FindOffset( values[i] );
	}

}

void CalCellsHigh(int16_t* values)
{
	for(int i = 0 ; i < 6 ; ++i)
	{
		t_CalibData.anCellGain[i] = FindGain( values[i], 4200, t_CalibData.anCellOffset[i] );
	}

	eeprom_WriteBlockWCRC((uint8_t*)&t_CalibData,(void*)(EEPROM_CALIB_START),sizeof(t_CalibData_t));
}

void CalibrateCells(int16_t* raw, int16_t* out)
{
	int16_t temp;

	for(int i = 0 ; i < 6 ; ++i)
	{
		temp = Calibrate( raw[i], t_CalibData.anCellGain[i], t_CalibData.anCellOffset[i] );
		if(temp < 0)
		{
			out[i] = 0;
		}
		else
		{
			out[i] = temp;
		}
	}
}

