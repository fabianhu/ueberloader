#include "serial.h"

#define EEPROM_COMMAND_START 		EEPROM_START
#define EEPROM_COMMAND_CRC_START 	(EEPROM_START + sizeof(Command_t))
#define EEPROM_COMMAND_LEN			(EEPROM_START + sizeof(Command_t)+2)

#define EEPROM_CALIB_START			EEPROM_START + EEPROM_COMMAND_LEN + 1

void eeprom_WriteBlockWCRC( uint8_t* blk, uint8_t* dest, uint16_t size);
uint8_t eeprom_ReadBlockWCRC( uint8_t* dest, uint8_t* src, uint16_t size);
