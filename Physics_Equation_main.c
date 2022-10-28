/*
*	Kyle Emerson and Mateo Vrooman
*	HW7: Physics Equation
*	Use Keypad.h and Keypad.c files to read inputs from the keypad
*	Use LCD.h and LCD.c files to print inputs onto LCD screen
*/

#include <stdio.h>
#include "Keypad.h"
#include "LCD.h"
#include <string.h>

int main(void){
	/* SysTick Delay Setups*/
	Systick_Init();
	/* Device Setups */
	KeyPad_Init();
	LCD_initialization();
	LCD_init_pins();
	
	uint16_t num, pressed;
	num = 0;
	uint16_t *numptr = &num;
	
	write_data(0xE4); //Write Epsilon character
	write_command(0x06); //Move cursor right
	
	while(1){
	pressed = Read_KeyPad(numptr);
		if (pressed != 0){
			Print_Keys(numptr);
		}
	}
	
	
}
