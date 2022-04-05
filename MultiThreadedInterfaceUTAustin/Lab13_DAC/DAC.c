// DAC.c
// Runs on LM4F120 or TM4C123, 
// edX lab 13 
// Implementation of the 4-bit digital to analog converter
// Daniel Valvano, Jonathan Valvano
// December 29, 2014
// Port B bits 3-0 have the 4-bit DAC

#include "DAC.h"
#include "..//tm4c123gh6pm.h"

// **************DAC_Init*********************
// Initialize 4-bit DAC 
// Input: none
// Output: none
void DAC_Init(void){
	
	unsigned int delay;

	SYSCTL_RCGCGPIO_R |= 0X02;
	delay = SYSCTL_RCGCGPIO_R;
	GPIO_PORTB_DIR_R |= 0X3f;
	GPIO_PORTB_AFSEL_R &= ~0X3f;
	GPIO_PORTB_AMSEL_R &= ~0X3f;
	GPIO_PORTB_DR8R_R |= 0X3f;
	GPIO_PORTB_DEN_R |= 0X3f;
}


// **************DAC_Out*********************
// output to DAC
// Input: 4-bit data, 0 to 15 
// Output: none
void DAC_Out(unsigned long data){
	
	GPIO_PORTB_DATA_R = data;
	
}
