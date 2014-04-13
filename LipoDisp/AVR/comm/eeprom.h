#include "serial.h"

// Display

#define EEPROM_PAR_START			(EEPROM_START)
#define EEPROM_PAR_LEN				(16)  // hard coded form menu_variant.c

#if ((EEPROM_PAR_START + EEPROM_PAR_LEN) >= EEPROM_END)
	#error fsck it
#endif

void eeprom_WriteBlockWCRC( uint8_t* blk, uint8_t* dest, uint16_t size);
uint8_t eeprom_ReadBlockWCRC( uint8_t* dest, uint8_t* src, uint16_t size);
