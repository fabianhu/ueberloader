/* definitions for menu */

#include <avr/io.h>
#include <avr/pgmspace.h>

typedef struct Parameter_tag
{
	int16_t		sParam;
	int16_t		sLowerLimit;
	int16_t		sUpperLimit;
}Parameter_t;

typedef struct MenuItem_tag
{
	uint8_t 		ucID;
	const char* 	strName;
	void(*			pAction)(void);
	Parameter_t*	pParamID;
	uint8_t			ucSubMenu; // also "UP"
	uint8_t			ucPrev;
	uint8_t			ucNext;
}
MenuItem_t;




