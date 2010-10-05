/*
 * eeprom.h
 *
 *  Created on: 26.09.2010
 *      Author: Fabian
 */

#ifndef EEPROM_H_
#define EEPROM_H_

#define EEPROM_PAGESIZE 32

#define EEPROM_LEN   EEPROM_END-EEPROM_START


#define EEPROM(_pageAddr, _byteAddr) \
	((uint8_t *) MAPPED_EEPROM_START)[_pageAddr*EEPROM_PAGESIZE + _byteAddr]



/* Definitions of macros. */

/*! \brief Enable EEPROM block sleep-when-not-used mode.
 *
 *  This macro enables power reduction mode for EEPROM.
 *  It means that the EEPROM block is disabled when not used.
 *  Note that there will be a penalty of 6 CPU cycles if EEPROM
 *  is accessed.
 */
#define EEPROM_EnablePowerReduction() ( NVM.CTRLB |= NVM_EPRM_bm )

/*! \brief Disable EEPROM block sleep-when-not-used mode.
 *
 *  This macro disables power reduction mode for EEPROM.
 */
#define EEPROM_DisablePowerReduction() ( NVM.CTRLB &= ~NVM_EPRM_bm )

/*! \brief Enable EEPROM mapping into data space.
 *
 *  This macro enables mapping of EEPROM into data space.
 *  EEPROM starts at EEPROM_START in data memory. Read access
 *  can be done similar to ordinary SRAM access.
 *
 *  \note This disables IO-mapped access to EEPROM, although page erase and
 *        write operations still needs to be done through IO register.
 */
#define EEPROM_EnableMapping() ( NVM.CTRLB |= NVM_EEMAPEN_bm )

/*! \brief Disable EEPROM mapping into data space.
 *
 *  This macro disables mapping of EEPROM into data space.
 *  IO mapped access is now enabled.
 */
#define EEPROM_DisableMapping() ( NVM.CTRLB &= ~NVM_EEMAPEN_bm )

/*! \brief Non-Volatile Memory Execute Command
 *
 *  This macro set the CCP register before setting the CMDEX bit in the
 *  NVM.CTRLA register.
 *
 *  \note The CMDEX bit must be set within 4 clock cycles after setting the
 *        protection byte in the CCP register.
 */
#define NVM_EXEC()	asm("push r30"      "\n\t"	\
			    "push r31"      "\n\t"	\
    			    "push r16"      "\n\t"	\
    			    "push r18"      "\n\t"	\
			    "ldi r30, 0xCB" "\n\t"	\
			    "ldi r31, 0x01" "\n\t"	\
			    "ldi r16, 0xD8" "\n\t"	\
			    "ldi r18, 0x01" "\n\t"	\
			    "out 0x34, r16" "\n\t"	\
			    "st Z, r18"	    "\n\t"	\
    			    "pop r18"       "\n\t"	\
			    "pop r16"       "\n\t"	\
			    "pop r31"       "\n\t"	\
			    "pop r30"       "\n\t"	\
			    )

/* Prototyping of functions. */
void EEPROM_WriteByte( uint8_t pageAddr, uint8_t byteAddr, uint8_t value );
uint8_t EEPROM_ReadByte( uint8_t pageAddr, uint8_t byteAddr );
void EEPROM_WaitForNVM( void );
void EEPROM_FlushBuffer( void );
void EEPROM_LoadByte( uint8_t byteAddr, uint8_t value );
void EEPROM_LoadPage( const uint8_t * values );
void EEPROM_AtomicWritePage( uint8_t pageAddr );
void EEPROM_ErasePage( uint8_t pageAddress );
void EEPROM_SplitWritePage( uint8_t pageAddr );
void EEPROM_EraseAll( void );


#endif /* EEPROM_H_ */
