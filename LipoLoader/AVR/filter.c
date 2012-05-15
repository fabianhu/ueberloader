#include <avr/io.h>
#include "filter.h"

void sFilter(int16_t* o, int16_t* n) // with jump possibility, if filtered value is off by more than 10%
{
	int32_t temp;
	int16_t out;

	temp = (int32_t)*o * 9L + (int32_t)*n;
	out = ( temp + 5L ) / 10L; // "halbes dazu, wg Rundungsfehler"

	// check, if difference larger than 10% of old.
	if(abs(*n-*o) > *o / 10)
	{
		*o = *n;
	}
	else
	{
		*o = out;
	}
}

void sFilterVar(int16_t* o, int16_t* n, uint8_t base) // with jump possibility, if filtered value is off by more than 10%
{
	int32_t temp;
	int16_t out;

	temp = (int32_t)*o * (int32_t)(base-1) + (int32_t)*n;
	out = ( temp + base/2 ) / base; // "halbes dazu, wg Rundungsfehler"

	// check, if difference larger than 10% of old.
	if(abs(*n-*o) > *o / 10)
	{
		*o = *n;
	}
	else
	{
		*o = out;
	}
}

void RampUpDn(int16_t* ramped, int16_t target)
{
	if(*ramped < target)
		(*ramped)++;
	else if(*ramped > target)
		(*ramped)--;
}
