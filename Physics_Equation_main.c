/*
*	Kyle Emerson and Mateo Vrooman
*	HW7: Physics Equation
*	Use Keypad.h and Keypad.c files to read inputs from the keypad
*	Use LCD.h and LCD.c files to print inputs onto LCD screen
*/

#include <stdio.h>
#include "Keypad.c"
#include "LCD.c"
#include <string.h>

int main(void){
	/* SysTick Delay Setups*/
	Systick_Init();
	SysTick_init();
	/* Device Setups */
	KeyPad_Init();
	LCD_initialization();
	LCD_init_pins();
	
	uint16_t num, pressed;
	num = 0;
	uint16_t *numptr = &num;
	int key;
	
	key = Read_KeyPad(numptr);
	while (key != 0){
		
	
	
	
}
