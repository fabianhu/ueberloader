// Calibration

#include "OS/FabOS.h"

#define GAINMULTIPLIER 10000ul

int16_t anCalCellsGain[6] = {GAINMULTIPLIER,GAINMULTIPLIER,GAINMULTIPLIER,GAINMULTIPLIER,GAINMULTIPLIER,GAINMULTIPLIER};
int16_t anCalCellsOffset[6] = {0,0,0,0,0,0};

int16_t Calibrate(int16_t raw, int32_t gain, int16_t offset)
{

int32_t ret;

ret = (int32_t)raw + (int32_t) offset;

ret = ret * gain;

ret = ret / GAINMULTIPLIER;

return (int16_t)ret;
}

int16_t FindOffset(int16_t raw) // only applicable at 0-point !!!
{
	return -raw;
}


int16_t FindGain(int16_t raw, int16_t expected, int16_t offset)
{
	int32_t ret;
	int32_t a,b;	

	a = (int32_t)expected * GAINMULTIPLIER;

	b = (int32_t)raw + (int32_t)offset;

	ret = a / b;

	return (int16_t)ret;
}

void CalCellsLow(int16_t* values)
{
	for (int i = 0; i < 6; ++i) {
		anCalCellsOffset[i] = FindOffset(values[i]);
	}
}

void CalCellsHigh(int16_t* values)
{
	for (int i = 0; i < 6; ++i) {
		anCalCellsGain[i] = FindGain(values[i],4200,anCalCellsOffset[i]);
	}
}

void CalibrateCells(int16_t* values)
{
	for (int i = 0; i < 6; ++i)
	{
		values[i]= Calibrate(values[i],anCalCellsGain[i],anCalCellsOffset[i]);
	}
}


