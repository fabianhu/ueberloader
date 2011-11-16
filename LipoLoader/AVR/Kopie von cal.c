// Calibration

#include "OS/FabOS.h"

#define GAINMULTIPLIER 32000LL
#define CALEXPECT_H	4200

int32_t anCalCellsGain[6] = {GAINMULTIPLIER,GAINMULTIPLIER,GAINMULTIPLIER,GAINMULTIPLIER,GAINMULTIPLIER,GAINMULTIPLIER};
int32_t anCalCellsOffsetL[6] = {0,0,0,0,0,0};
int32_t anCalCellsOffsetH[6] = {0,0,0,0,0,0};

int32_t ret;

int16_t CalibrateGainOffs(int16_t raw, int32_t gain, int32_t offset)
{

ret = (int32_t)raw + offset;

ret = ret * gain;

ret = ret / GAINMULTIPLIER;

return (int16_t)ret;
}

// 2x Offset
int16_t Calibrate(int16_t raw, uint8_t i)
{
	int32_t ret;
	int16_t max = CALEXPECT_H - anCalCellsOffsetH[i];
	int16_t min = - anCalCellsOffsetL[i];

	int16_t OffsetH = raw * anCalCellsOffsetH[i] / max;
	int16_t OffsetL = (1-(raw/ min)) * anCalCellsOffsetL[i] ; // fixme umstellen


	ret = raw + OffsetH + OffsetL;

	return (int16_t)ret;
}


int16_t FindOffset(int16_t raw) // only applicable at 0-point !!!
{
	return -raw;
}

int32_t ret;
	int32_t a,b;	

int16_t FindGain(int16_t raw, int16_t expected, int32_t offset)
{
	

	a = (int32_t)expected * GAINMULTIPLIER;

	b = (int32_t)raw + offset;

	ret = a / b;

	return (int16_t)ret;
}

int16_t FindOffsetH(int16_t raw, int16_t expected)
{

	return (int16_t)expected-raw;
}

void CalCellsLow(int16_t* values)
{
	for (int i = 0; i < 6; ++i) {
		anCalCellsOffsetL[i] = FindOffset(values[i]);
	}
}

void CalCellsHigh(int16_t* values)
{
	for (int i = 0; i < 6; ++i) {
		anCalCellsGain[i] = FindGain(values[i],4200,anCalCellsOffsetL[i]);
	}
}

void CalibrateCells(int16_t* values)
{
	for (int i = 0; i < 6; ++i)
	{
		//values[i]= Calibrate(values[i],anCalCellsGain[i],anCalCellsOffsetL[i]);
		values[i]= Calibrate(values[i],i);
	}
}


