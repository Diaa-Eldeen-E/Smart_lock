/*
 * EEPROM.c
 *
 *  Created on: Sep 14, 2019
 *      Author: DiaaEldeen
 */


#include "EEPROM.h"


#define address_write(address) {\
	EEPROM->EEBLOCK = (address) / 16;\
	EEPROM->EEOFFSET = (address) % 16;\
}


void EEPROM_initialize(){
	SYSCTL->RCGCEEPROM |= BIT0;
	delay_us(2);
	poll_working_bit
	if ( EEPROM->EESUPP !=0 ) exit(1);
	//EEPROM software reset
	SYSCTL->SREEPROM |= BIT0;
	delay_us(1);
	SYSCTL->SREEPROM &=~BIT0;
	//
	delay_us(2);
	poll_working_bit
	if ( EEPROM->EESUPP !=0 ) exit(1);

}

//use this to initialize a value in an address... if ((EEPROM_read(0x01)==0xffffffff)) EEPROM_write(0x01,1);
uint32_t EEPROM_read_word(uint16_t address)
{
	poll_working_bit
	address_write(address);

	return EEPROM->EERDWR;
}

void EEPROM_write_word(uint16_t address, uint32_t data)
{
	poll_working_bit
	address_write(address);
	EEPROM->EERDWR = data;

}

uint32_t* EEPROM_read_array(uint16_t address, uint16_t n )
{
	uint32_t* data = (uint32_t*) malloc(n);

	address_write(address);

	uint16_t i=0;
	for(i=0; i<n; i++)
		data[i] = EEPROM_read_word(address++);

	return data;
}

char* EEPROM_read_string(uint16_t address )
{
	char* data = NULL;
	uint16_t index =0;

	while( EEPROM_read_word(address) ){
		data = realloc(data, index+1);
		data[index] = (char) EEPROM_read_word(address++);
		index++;
	}
	data[index] = '\0';
	return data;
}

void EEPROM_write_string(uint16_t address , char* data)
{
	while(*data)
		EEPROM_write_word(address++, (uint32_t)*data++);

	EEPROM_write_word(address, '\0');  // adding null terminator
}

