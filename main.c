/*
 * main.c
 */

#include "handy.h"
#include "LCD.h"
#include "UART.h"
#include "EEPROM.h"
#include "string.h"


void Lock_initialize();
void correct_message();
void incorrect_message();
void choose_new_password();
char* enter_password();


uint16_t eepromPasswordAddress = 1;


int main(void)
{
	delay_ms(100);

	Lock_initialize();

	//reading the typed password from the user
	while(1){

		char* pEnteredPassword = enter_password();

		//password matching check and displaying the result
		char* pCorrectPassword = EEPROM_read_string(eepromPasswordAddress);	//reading the password from the EEPROM
		if (strcmp(pEnteredPassword, pCorrectPassword ) == 0)    //match
			correct_message();
		else												   	 //not matched
			incorrect_message();

		free(pCorrectPassword);
		free(pEnteredPassword);
	}
}



void GPIOJ_handler(){

	choose_new_password();
	GPIO_button->ICR |= P0 | P1;
}

void Lock_initialize(){

	LCD_Initialize();
	char Character1[8] = { 0x04, 0x0a, 0x11, 0xff, 0xff, 0xff, 0xff, 0xff };
	LCD_Custom_Char (0,Character1);  // lock char
	char Character2[8] = { 0x04, 0x0a, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff };
	LCD_Custom_Char (1,Character2);  // unlock char

	buttons_EK_setup();
	__enable_irq();

	EEPROM_initialize();
	//if no password found in the EEPROM, initialize it with a password : 0000
	if( EEPROM_read_word(eepromPasswordAddress -1) !='1' ){
		char password []= "0000";					  //this ends with a null terminator, correct way to write char array
		EEPROM_write_string(eepromPasswordAddress, password);			//storing the password in EEPROM
	}

}

char* enter_password(){

	LCD_clear
	LCD_Print_String_X_Y("Password:", 1, 1);

	char * pEnteredPassword = malloc(7);
	char keyPressed =0;
	
	int8_t i =0;
	for (i =0; i<31; i++)
	{
		//increase allocated space by 7 whenever current space is full
		if( (i+1)%7==0 )  pEnteredPassword = realloc(pEnteredPassword, i+1+7);
		keyPressed=keypad_scan();

		if (keyPressed=='D'){
			pEnteredPassword[i] = '\0';
			break;
		}

		LCD_Print_Char_X_Y(keyPressed, i+10, 1);
		LCD_Command(shiftDisplayLeft);
		pEnteredPassword[i] = keyPressed;
	}

	return pEnteredPassword;
}

//takes the new password from the user and stores it in EEPROM memory
void choose_new_password(){
	while(1){
		LCD_clear
		LCD_Print_String_X_Y("Enter the old", 1, 1);	LCD_Print_String_X_Y("Password",1,2);
		delay_ms(2000);
		char* pEnteredPassword = enter_password();
		//password matching check and displaying the result
		char* pCorrectPassword = EEPROM_read_string(eepromPasswordAddress);		//reading the password from the EEPROM
		if ( strcmp(pEnteredPassword, pCorrectPassword ) == 0 ){	//match

			LCD_clear
			LCD_Print_String_X_Y("Enter the new",1,1);	LCD_Print_String_X_Y("Password",1,2);
			delay_ms(2000);
			break;
		}
		else		incorrect_message();					//passwords not matched

		free(pCorrectPassword);
		free(pEnteredPassword);
	}

	/*********************************************************************************************/
	LCD_clear
	LCD_Print_String_X_Y("Password:", 1, 1);
	char keyPressed =0;
	uint8_t numberOfChars = 0;

	uint8_t i=0;
	for(i=0;i<31;i++)
	{
		keyPressed=keypad_scan();
		if( keypad_scan()=='D' ) {
			numberOfChars = i;
			break;
		}
		EEPROM_write_word(eepromPasswordAddress+i, keyPressed);

		LCD_Print_Char_X_Y(keyPressed, i+10, 1);
		LCD_Command(shiftDisplayLeft);
	}

	EEPROM_write_word( eepromPasswordAddress+numberOfChars, '\0');
	EEPROM_write_word( eepromPasswordAddress-1, '1' );   //a flag to check if there is a written password or not
	LCD_Print_String_X_Y("Password set",numberOfChars+1,2);
	delay_ms(2000);
	LCD_clear
	LCD_Print_String_X_Y("Password:", 1, 1);
	};

void correct_message(){

	LCD_clear
	LCD_Print_String_X_Y("Correct",5,1);
	LCD_Print_Char_X_Y(1,16,2);			//unlock char
	delay_ms(2000);
	LCD_clear
	LCD_Print_String_X_Y("Password:",1,1);
}

void incorrect_message(){

	LCD_clear
	LCD_Print_String_X_Y("InCorrect", 4, 1);LCD_Print_Char_X_Y(0,16,2);
	LCD_Print_String_X_Y("Try again", 1, 2);
	delay_ms(3000);
}

void SystemInit(void)
{
	SCB->CPACR |= 0x00F00000;
}
