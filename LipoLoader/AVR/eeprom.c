#include "OS/FabOS.h"
#include <avr/eeprom.h>
#include "eeprom.h"
#include <avr/eeprom.h>
#include "crc.h"

// write block to eeprom with CRC16 sum AFTER it.
void eeprom_WriteBlockWCRC( uint8_t* blk, uint8_t* dest, uint16_t size)
{
	uint16_t crc=0;
	OS_PREVENTSCHEDULING;
	eeprom_update_block(blk, dest, size);
	// add crc check!!!
	crc = calcCRC16S(blk,size);
	eeprom_update_block((uint8_t*)&crc, dest+size , 2);
	OS_ALLOWSCHEDULING;
}

// read block from eeprom with CRC16 sum AFTER it.
// DISCARD result, if return value is <> 0
uint8_t eeprom_ReadBlockWCRC( uint8_t* dest, uint8_t* src, uint16_t size)
{
	uint16_t crcEE = 0, crcRD = 0;

	OS_PREVENTSCHEDULING;
	// read parameters from eeprom
	eeprom_read_block( dest, src, size );
	//check CRC
	eeprom_read_block( (uint8_t*)&crcEE, src + size, 2 );

	// calculate over read data
	crcRD = calcCRC16S( dest, size);

	OS_ALLOWSCHEDULING;

	if(crcRD != crcEE)
	{return 1;}
	else
	{return 0;}
}
