#include "Keypad.h"
#include "stm32f446xx.h"
#include <stdio.h>
#include <stdint.h>


void Systick_Init(){
	SysTick -> CTRL = 0;	//disable during step
	SysTick -> LOAD = 0x00FFFFFF;	//loads max reload value
	SysTick -> VAL = 0;	//clears current value
	SysTick -> CTRL = 0x00000005;	//enables systick at 16mhz
}

void SysTick_Delay(uint32_t delayms){
	SysTick -> LOAD = ((delayms * 16000) - 1); //1ms countdown to zero
	SysTick -> VAL = 0;
	while ((SysTick -> CTRL & 0x00010000) == 0);
}
/*
Row pins: PC0, PC2, PC4, and PC6
Col pins: PC8, PC10, PC12
Numbers on keypad are connected as follows:
		1 (PC8, PC0)	2	(PC10, PC0)	3(PC12, PC0)
		
		4	(PC8, PC2)	5	(PC10, PC2)	6(PC12, PC2)

		7 (PC8, PC4)	8	(PC10, PC4)	9(PC12, PC4)

		* (PC8, PC6)	0	(PC10, PC6)	#(PC12, PC6)
*/

/***| Hex2Bit() |*******************************//*
*Brief: Is used for 2y:2y+1 type definitions in registers
*Brief: where yth bit is given as the input
*Params:
*           (uint32) hex_num: yth bit is given 
*Returns:
*           eg. 01000000 => returns counter bit6, y=6
********************************************************/
uint8_t Hex2Bit (uint32_t hex_num){
	uint8_t bit_count = 0;
	while(hex_num >> 1){
		bit_count++;
		hex_num = hex_num>>1;
	}
	return bit_count;
}

void KeyPad_Init(){
	RCC -> AHB1ENR |= 0x14; //Enables GPIOC Clock
	GPIOC -> MODER &=~ (uint32_t) ((3 << (2 * Hex2Bit(R0))) | (3 << (2 * Hex2Bit(R1))) | (3 << (2*Hex2Bit(R1))) | (3 << (2*Hex2Bit(R2))) | (3 << (2*Hex2Bit(R3))) | (3 << (2*Hex2Bit(C0))) | (3 << (2*Hex2Bit(C1))) | (3 << (2*Hex2Bit(C2))));
	GPIOC -> PUPDR &=~ (uint32_t) ((3 << (2 * Hex2Bit(R0))) | (3 << (2 * Hex2Bit(R1))) | (3 << (2*Hex2Bit(R1))) | (3 << (2*Hex2Bit(R2))) | (3 << (2*Hex2Bit(R3))));
	GPIOC -> PUPDR = (uint32_t) ((1 << (2 * Hex2Bit(R0))) | (1 << (2 * Hex2Bit(R1))) | (1 << (2*Hex2Bit(R1))) | (1 << (2*Hex2Bit(R2))) | (1 << (2*Hex2Bit(R3)))); //Enable pull up resistors in columns
}
/***| Read_Keypad() |***********************************//*
*Brief: Checks for the button press by grounding each col as
*Brief: assigning each col as GND one by one while the rest
*Brief: are floating input with infinite impedance. As a result,
*Brief: if any press is hit, that row will be grounded over the
*Brief: col that is assigned as GND.
*|--|--| -> Row0=> GND if 1 is pressed
*|--|--| -> Row1=> GND if 4 is pressed
*|--|--| -> Row2=> GND if 7 is pressed
*|--|--| -> Row3=> GND if * is pressed
*|=>GND
*Brief: Caution: If two cols are hit at the same time and they
*Brief: are assigned as both OUT, there will be a short circuit
*Brief: that can fry MCU. Hence, multiple repeating IN settings
*Brief: are placed for safety.
*Params:
*           (uint16) *numptr: pressed number pointer
*Returns:
*           None
********************************************************/
uint8_t Read_KeyPad(uint16_t *numptr){
	uint8_t col, row;
	for(col=0; col < 3; col++){
		GPIOC -> MODER &=~ (uint32_t) ((3 << (2 * Hex2Bit(R0))) | (3 << (2 * Hex2Bit(R1))) | (3 << (2*Hex2Bit(R1))) | (3 << (2*Hex2Bit(R2))) | (3 << (2*Hex2Bit(R3))) | (3 << (2*Hex2Bit(C0))) | (3 << (2*Hex2Bit(C1))) | (3 << (2*Hex2Bit(C2))));
		GPIOC -> MODER |= (uint32_t)(1 <<2*(col+4));
		GPIOC -> ODR &=~ (uint32_t)(1 << 2*(col+4));
		SysTick_Delay(10);
		row = GPIOC -> IDR & (R0|R1|R2|R3);
		while (!(GPIOC->IDR & R0) | !(GPIOC->IDR & R1) | !(GPIOC->IDR & R2) | !(GPIOC->IDR & R3));
		
		if(row != 0x0F) break;
	}
	GPIOC -> MODER &=~ (uint32_t) ((3 << (2 * Hex2Bit(R0))) | (3 << (2 * Hex2Bit(R1))) | (3 << (2*Hex2Bit(R1))) | (3 << (2*Hex2Bit(R2))) | (3 << (2*Hex2Bit(R3))) | (3 << (2*Hex2Bit(C0))) | (3 << (2*Hex2Bit(C1))) | (3 << (2*Hex2Bit(C2))));
	
	if(col == 3) return 0;
	if (row == 0x0E) *numptr = col + 1;			//key in row 0
	if (row == 0x0D) *numptr = col + 4;	//key in row 1
	if (row == 0x0B) *numptr = col + 7;	//key in row 2
	if (row == 0x07) *numptr = col + 10;	//key in row 3
	return 1;
}

void Print_Keys(uint16_t *numptr){
	if (*numptr == 12) printf("#\n");
	if (*numptr == 11) printf("0\n");
	if (*numptr == 10) printf("*\n");
	if (*numptr <  10) printf("%d\n", *numptr);
}
