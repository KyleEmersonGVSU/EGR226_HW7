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
#define div_sign (uint8_t) 0x2F // /

int main(void){
	/* SysTick Delay Setups*/
	Systick_Init();
	/* Device Setups */
	KeyPad_Init();
	LCD_initialization();
	LCD_init_pins();
	printf("cum");
	uint16_t num, pressed = 0;
	num = 0;
	uint16_t *numptr = &num;
	int i = 0;
	int bit_address[MAX_DIGITS];
	int temp1 = 0, temp2 = 0, inputNum[3];
	
	write_command(0x0F);
	
	write_command(0xC0); //second line
	write_command(0x06); //Move cursor right

	
	while (1){
		for (i = 0;i < 2; i++){
			while(pressed == 0){
				pressed = Read_KeyPad(numptr);	
			}
			temp1 = *numptr;
			printf("%d", *numptr);
			temp1 = temp1 * 10;

			while(pressed == 0){
				pressed = Read_KeyPad(numptr);	
			}
			temp2 = *numptr;
			inputNum[i] = temp1 + temp2;
		}
		if (inputNum[0] == 6){
			write_data(c);
		} else if (inputNum[0] == 3){
			write_data(equal_sign);
		} else if (inputNum[0] == 2){
			if (inputNum[1] == 8){
				write_data(fwd_parenth);
			} else if (inputNum[1] == 9){
				write_data(aft_parenth);
			} else if (inputNum[1] == 13){
				write_data(neg_sign);
			} else if (inputNum[1] == 15){
				write_data(div_sign);
			}
		} else if (inputNum[0] == 14){
			if (inputNum[1] == 3){
				write_data(epsilon);
			} else if (inputNum[1] == 4){
				write_data(mu);
			}
		} else if (inputNum[0] == 4){
			write_data(mult);
		} else if (inputNum[1] == 5){
			write_data(exponent);
		}
		write_command(0x06); //move cursor right
	}
	

}
