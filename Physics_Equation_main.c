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

#define MAX_DIGITS 2

#define c (uint8_t) 0x63 //c char
#define equal_sign (uint8_t) 0x3D // =
#define fwd_parenth (uint8_t) 0x28 // (
#define aft_parenth (uint8_t) 0x29 // )
#define epsilon (uint8_t) 0xE3 // epsilon
#define mu (uint8_t) 0xE4 //mu
#define mult (uint8_t) 0x4A // *
#define exponent (uint8_t) 0x5E // ^
#define neg_sign (uint8_t) 0x2D // -

int main(void){
	/* SysTick Delay Setups*/
	Systick_Init();
	/* Device Setups */
	KeyPad_Init();
	LCD_initialization();
	LCD_init_pins();
	
	uint16_t num, pressed = 0;
	num = 0;
	uint16_t *numptr = &num;
	int i = 0;
	int bit_address[MAX_DIGITS];
	int temp1, temp2;
	write_command(0xC0); //second line
	write_data(0xE4); //Write Epsilon character
	write_command(0x06); //Move cursor right
	
	while(pressed == 0){
	pressed = Read_KeyPad(numptr);	
	}

}
