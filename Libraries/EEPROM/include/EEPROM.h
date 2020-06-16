/*
 * EEPROM.h
 *
 *  Created on: Sep 14, 2019
 *      Author: DiaaEldeen
 */

#ifndef EEPROM_H_
#define EEPROM_H_


#include "handy.h"

#define poll_working_bit	while( EEPROM->EEDONE & BIT0 );	//poll until the working bit is 0

void EEPROM_initialize();
uint32_t EEPROM_read_word(uint16_t address);
uint32_t* EEPROM_read_array(uint16_t address , uint16_t n );
char* EEPROM_read_string(uint16_t address );
void EEPROM_write_word(uint16_t address , uint32_t data);
void EEPROM_write_string(uint16_t address , char* data);   //adds a null terminator


#endif /* EEPROM_H_ */
