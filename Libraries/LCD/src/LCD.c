/*
 * LCD.c
 *
 *  Created on: Jun 27, 2019
 *      Author: DiaaEldeen
 */

#include "LCD.h"


void LCD_Latch (void)    //latching function for holding the input data
{
	LCD_PORT_DATA(EN) = EN;    //1- set for enable pin  (high)
	delay_us(1);		  //2- delay 1 us  (for latching)
	LCD_PORT_DATA(EN) = 0;	  //3- reset for enable PIN
	delay_us(100);
}

void LCD_Command(char COM)
{
	LCD_PORT_DATA(RS) = 0;				 //reset register select pin RS  ( 1 for data , 0 for command)
	LCD_PORT_DATA(0xf0) =  (COM&0xf0);	 // high nibble is send first in 4 bit-mode by masking
	LCD_Latch();                         //latching for data sent
	LCD_PORT_DATA(0xf0) =  ((COM&0x0f)<<4);  //sending low nibble by masking
	LCD_Latch();						//latching for data sent

	if (COM < 4)
		delay_ms(2); 	// commands 1 and 2 need up to 1.64ms
}

void LCD_Print_Char(char DATA)
{
	LCD_PORT_DATA(RS) = RS;					//set register select pin RS ( 1 for data , 0 for command)
	LCD_PORT_DATA(0xf0) =  (DATA&0xf0);  	  //sending high nibble by masking
	LCD_Latch();								  //latching for data sent
	LCD_PORT_DATA(0xf0) =  ((DATA&0x0f)<<4); 	//sending low nibble by masking
	LCD_Latch();								  //latching for data sent
}

void LCD_Initialize(void)
{
	LCD_PORT_clock_en
	LCD_PORT->DIR |=0xf3;  // 4 data pins + 2 control pins output
	LCD_PORT->DEN |=0xf3;

	//initialization sequence
	delay_ms(20);
	LCD_PORT_DATA(0xf0) =  0x30;
	LCD_Latch();
	delay_ms(10);
	LCD_PORT_DATA(0xf0) =  0x30;
	LCD_Latch();
	delay_ms(1);
	LCD_PORT_DATA(0xf0) =  0x30;
	LCD_Latch();
	delay_ms(1);
	LCD_PORT_DATA(0xf0) =  0x20;	//Select bus width (0x30 - for 8-bit and 0x20 for 4-bit)
	LCD_Latch();
	delay_ms(1);

	//reset the screen and set it to 4 bit mode 2 lines and 5*7 font
	LCD_Command(bitMode_4);   // 4 bit-mode
	LCD_Command(clearScreen);
	LCD_Command(returnHome);
	LCD_Command(displayOn_cursorOff);   // ON
}

void LCD_Print_String(char* str)
{
	while (*str>0)	LCD_Print_Char(*str++);
}
void LCD_X_Y(uint8_t x , uint8_t y)   //start from x=1 and y=1
{
	if (y==1) LCD_Command(firstLine+x-1);
	else if (y==2) LCD_Command(secondLine+x-1);
}

void LCD_Print_Char_X_Y(char character , uint8_t x, uint8_t y)
{
	LCD_X_Y(x , y);
	LCD_Print_Char(character);
}

void LCD_Print_String_X_Y(char* str , uint8_t x, uint8_t y)
{
	LCD_X_Y(x , y);
	LCD_Print_String(str);
}


void LCD_Print_int_X_Y(int32_t number , uint8_t x ,uint8_t y)
{
	char intNum_String [12];
	itoa(number,intNum_String,10);
	LCD_Print_String_X_Y(intNum_String,x,y);
}

void LCD_Print_uint_X_Y(uint32_t number , uint8_t x ,uint8_t y)
{
	char intNum_String [11];
	utoa(number,intNum_String,10);
	LCD_Print_String_X_Y(intNum_String,x,y);
}


//max precision is 6, max digits including (.) is 11
void LCD_Print_float_X_Y(double number , uint8_t precision ,uint8_t x ,uint8_t y)
{
	char doubleNum_String [12];
	ftoa(number, precision ,doubleNum_String);
	LCD_Print_String_X_Y(doubleNum_String,x,y);
}


void LCD_Custom_Char (uint8_t location ,char *characterArray)   //locations from 0 to 7 for characters storage
{
	if(location<8)
	{
		LCD_Command (CGRAM_beginning_Address+(8*location));	// Command 0x40 and onwards forces the device to point CGRAM address
		uint8_t i;
		for(i=0;i<8;i++)	// Write 8 byte for generation of 1 character
			LCD_Print_Char(characterArray[i]);
	}
}

void LCD_Print_packedBCD(unsigned char x)
{
	LCD_Print_Char('0'+(x>>4) ) ;
	LCD_Print_Char('0'+(x & 0x0f) )  ;
}



char keypad_scan()
{
	keypad_PORT_clock_en
	keypad_PORT->DEN = 0xff;
	char keypad_keys[4][4] = {{'1','2','3','A'},{'4','5','6','B'},{'7','8','9','C'},{'*','0','#','D'}}; //if you want to change the returned char , change this array

	keypad_PORT->DIR = 0x0f;	//columns input, rows output or columns output, rows input
	char keypad_First_State ;
	keypad_PORT->PUR = 0xf0;		//internal pull up
	keypad_PORT_DATA(0x0f) = 0;		//ground output pins

	while(1)
	{
		if ( keypad_PORT_DATA(0xff) != 0xf0 && keypad_PORT_DATA(0xff) != 0x0f ) //if any button is pressed
		{
			delay_ms(150);		//for button debouncing
			keypad_First_State = keypad_PORT_DATA(0xff);   //capture first state (column state or raw state)
			keypad_PORT->DIR ^= 0xff;		//swap columns and raws directions from in to out and from out to in
			keypad_PORT->PUR ^=0xff;		//activate internal pull up for the swapped input raws  or columns
			keypad_PORT_DATA(0x0f^0xff) = 0;	//ground output pins

			uint8_t i;
			for (i=0;i<4;i++)          //loop all the keypad character array until it matches the pressed key
			{
				uint8_t j;
				for (j=4;j<8;j++)
				{
					if ( (keypad_First_State|keypad_PORT_DATA(0xff)) ==  (0xff ^ ((1<<j)|(1<<i)) )  )	// if the pressed key matched the keypad array char
					{
						while (! ( (keypad_PORT_DATA(0x0f)==0x0f) |  (keypad_PORT_DATA(0xf0)==0xf0) ) ); //polling , w8 until key is no longer pressed
						return keypad_keys[i][j-4];     //return the pressed key
					}
				}
			}
			//_delay_ms(150);
		}
	}
}

