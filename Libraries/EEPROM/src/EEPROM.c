/*
 * EEPROM.c
 *
 *  Created on: Sep 14, 2019
 *      Author: DiaaEldeen
 */


#include "EEPROM.h"


// A wrapper function for accessing EEPROM addresses
#define address_write(address) {\
	EEPROM->EEBLOCK = (address) / 16;\
	EEPROM->EEOFFSET = (address) % 16;\
}



/* Read a word of 4 bytes size from the EEPROM at that (address)
 * It also could be used to initialize a value in an address as following
 * if ((EEPROM_read_word(addr) == 0xffffffff))
 * 	EEPROM_write(addr, 1);
 */
uint32_t EEPROM_read_word(uint16_t address) {

	POLL_WORKING_BIT();
	address_write(address);
	return EEPROM->EERDWR;
}


// Write a (data) word of 4 bytes size in the EEPROM at that (address)
void EEPROM_write_word(uint16_t address, uint32_t data) {

	POLL_WORKING_BIT();
	address_write(address);
	EEPROM->EERDWR = data;
}


// Read (n) words from the EEPROM starting from that (address)
uint32_t* EEPROM_read_array(uint16_t address, uint16_t n ) {

	uint32_t* data = (uint32_t*) malloc(n);

	address_write(address);

	uint16_t i=0;
	for(i=0; i<n; i++)
		data[i] = EEPROM_read_word(address++);

	return data;
}


/* Read (n) words casted to characters from the EEPROM starting from
 * that (address) until a null terminator is reached
 */
char* EEPROM_read_string(uint16_t address ) {

	char* data = NULL;
	uint16_t index =0;

	while( EEPROM_read_word(address) ) {

		data = realloc(data, index+1);
		data[index] = (char) EEPROM_read_word(address++);
		index++;
	}

	data[index] = '\0';
	return data;
}



void EEPROM_write_string(uint16_t address , char* data) {

	while(*data)
		EEPROM_write_word(address++, (uint32_t)*data++);

	EEPROM_write_word(address, '\0');  // adding null terminator
}



void EEPROM_initialize() {

	SYSCTL->RCGCEEPROM |= BIT0;
	while( !(SYSCTL->PREEPROM & BIT0));	// W8 until peripheral is ready

	delay_us(1);
	POLL_WORKING_BIT();

	ASSERT_TRUE(EEPROM->EESUPP == 0);	// Programming or erasing has not failed

	//EEPROM software reset
	SYSCTL->SREEPROM |= BIT0;
	delay_us(1);
	SYSCTL->SREEPROM &=~BIT0;

	delay_us(2);
	POLL_WORKING_BIT();

	ASSERT_TRUE(EEPROM->EESUPP == 0);	// Programming or erasing has not failed
}
