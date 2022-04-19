// dac.c 
// Runs on LM4F120 or TM4C123, 
// for space invaders implementation 
// This file contains functions and related information 
//to setup the DAC funtionality
// Felipe Baidon
// April 14, 2022

#include "..//tm4c123gh6pm.h"
#include "dac.h"

/* This function writes to the DAC-related registers 
		to configure the DAC */
void DAC_Init(void)
{
	unsigned long delay;
	
	SYSCTL_RCGCGPIO_R |= 0X02;
	delay = SYSCTL_RCGCGPIO_R;
	
	GPIO_PORTB_DIR_R |= 0X0f;
	GPIO_PORTB_AFSEL_R &= ~0X0f;
	GPIO_PORTB_AMSEL_R &= ~0X0f;
	GPIO_PORTB_PCTL_R &= ~0X0f;
	GPIO_PORTB_DEN_R |= 0X0f;
}

/*This function retrieves the data obtained written to the
	DAC-related output register */
void DAC_out(unsigned int data)
{
	GPIO_PORTB_DATA_R = data;
}

