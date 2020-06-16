/*
 * LCD.h
 *
 *  Created on: Jun 27, 2019
 *      Author: DiaaEldeen
 */

#ifndef LCD_H_
#define LCD_H_

#include "handy.h"


#define keypad_PORT_clock_en	SYSCTL->RCGCGPIO |= BIT11;	//GPIO M
#define keypad_PORT 			GPIOM
#define keypad_PORT_DATA 		GPIOM_DATA


#define LCD_PORT_clock_en	SYSCTL->RCGCGPIO |= BIT9;		//GPIO K
#define LCD_PORT 			GPIOK
#define LCD_PORT_DATA		GPIOK_DATA
#define RS (1<<0)	//pin 0
#define EN (1<<1)	//pin 1

//LCD commands
#define clearScreen 0x01
#define returnHome 0x02
#define decrementalCursorMode 0x04  //         write from right to left
#define incrementalCursorMode 0x06  //default  write from left to right
#define shiftingDisplayRightMode 0x05
#define shiftingDisplayLeftMode 0x07
#define displayOff_cursorOff 0x08
#define displayOn_cursorOff 0x0C
#define displayOn_cursorBlinking 0x0e // 0x0e for just underscore blink , 0x0f for whole digit blink
#define shiftCursorLeft 0x10
#define shiftCursorRight 0x14
#define shiftDisplayLeft 0x18 // this is to write from left to right
#define shiftDisplayRight 0x1c //this is to write from right to left
#define firstLine 0x80   //first position in the first line
#define secondLine 0xc0  //first position in the second line
#define bitMode_4 0x28
#define bitMode_8 0x38
#define CGRAM_beginning_Address 0x40


#define LCD_clear LCD_Command(returnHome);LCD_Command(clearScreen);

void LCD_Latch (void);
void LCD_Command(char COM);
void LCD_Print_Char(char DATA);
void LCD_Initialize(void);
void LCD_Print_String(char* str);
void LCD_X_Y(uint8_t x , uint8_t y);   //start from x=1 and y=1
void LCD_Print_Char_X_Y(char character , uint8_t x, uint8_t y);
void LCD_Print_String_X_Y(char* str , uint8_t x, uint8_t y);
void LCD_Print_int_X_Y(int32_t number , uint8_t x ,uint8_t y);
void LCD_Print_uint_X_Y(uint32_t number , uint8_t x ,uint8_t y);
void LCD_Print_float_X_Y(double number , uint8_t precision ,uint8_t x ,uint8_t y);
void LCD_Custom_Char (uint8_t location ,char *characterArray);
void LCD_Print_packedBCD(unsigned char x);

char keypad_scan();

#endif /* LCD_H_ */
