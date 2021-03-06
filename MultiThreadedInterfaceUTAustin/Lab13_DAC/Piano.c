// Piano.c
// Runs on LM4F120 or TM4C123, 
// edX lab 13 
// There are four keys in the piano
// Daniel Valvano
// December 29, 2014

// Port E bits 3-0 have 4 piano keys

#include "Piano.h"
#include "..//tm4c123gh6pm.h"


// **************Piano_Init*********************
// Initialize piano key inputs
// Input: none
// Output: none
void Piano_Init(void){ 
	
	unsigned int delay;
		
	SYSCTL_RCGCGPIO_R |= 0X10;
	delay = SYSCTL_RCGCGPIO_R;
	GPIO_PORTE_DIR_R &= ~0X08;
	GPIO_PORTE_AFSEL_R &= ~0X08;
	GPIO_PORTE_AMSEL_R &= ~0X08;
	GPIO_PORTE_PDR_R |= 0X08;
	GPIO_PORTE_DEN_R |= 0X08;
	
  
}
// **************Piano_In*********************
// Input from piano key inputs
// Input: none 
// Output: 0 to 15 depending on keys
// 0x01 is key 0 pressed, 0x02 is key 1 pressed,
// 0x04 is key 2 pressed, 0x08 is key 3 pressed
unsigned long Piano_In(void){
	
	unsigned int input = 0;
	
	input = GPIO_PORTE_DATA_R & 0X0f;
  
  return input; // remove this, replace with input
}
