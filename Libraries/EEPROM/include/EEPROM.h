/*
 * EEPROM.h
 *
 *  Created on: Sep 14, 2019
 *      Author: DiaaEldeen
 */

#ifndef EEPROM_H_
#define EEPROM_H_


#include "handy.h"

#define POLL_WORKING_BIT()	while( EEPROM->EEDONE & BIT0 );	//poll until the working bit is 0


uint32_t EEPROM_read_word(uint16_t address);
uint32_t* EEPROM_read_array(uint16_t address , uint16_t n );
char* EEPROM_read_string(uint16_t address );
void EEPROM_write_word(uint16_t address , uint32_t data);
void EEPROM_write_string(uint16_t address , char* data);   //adds a null terminator
void EEPROM_initialize();

#endif /* EEPROM_H_ */
