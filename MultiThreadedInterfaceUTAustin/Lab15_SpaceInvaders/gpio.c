// gpio.c
// Runs on LM4F120 or TM4C123, 
// for space invaders implementation 
// This file contains functions and related information 
//to setup the gpio used in the videogame development
// Felipe Baidon
// April 14, 2022

#include "..//tm4c123gh6pm.h"

/* This function initializes the GPIO pins that are used
	as Button inputs*/
void GPIO_ButtonsInit(void)
{
	unsigned long delay;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE;
	delay = SYSCTL_RCGC2_R;
	GPIO_PORTE_DIR_R &= ~0X3;
	GPIO_PORTE_AFSEL_R &= ~0X3;
	GPIO_PORTE_AMSEL_R &= ~0X3;
	GPIO_PORTE_PCTL_R &= ~0X3;
	GPIO_PORTE_PDR_R |= 0X3;
	GPIO_PORTE_DEN_R |= 0X3;
	
}

/* This function initializes the GPIO pins that are
		used as ouputs to drive the LEDs*/
void GPIO_IndicatorInit(void)
{
	unsigned long delay;
	
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;
	delay = SYSCTL_RCGC2_R;
	
	GPIO_PORTB_DIR_R |= 0X30;
	GPIO_PORTB_AFSEL_R &= ~0X30;
	GPIO_PORTB_AMSEL_R &= ~0X30;
	GPIO_PORTB_PCTL_R &= ~0X30;
	GPIO_PORTB_DEN_R |= 0X30;
}

/* This function reads the state of the PE0 and returns it */
unsigned int GPIO_FireButtonIn(void)
{
	unsigned int button;
	
	button=(GPIO_PORTE_DATA_R & 0X1);
	
	return button;
}

/*This function reads the state of the PE1 and returns it 
*/
unsigned int GPIO_SpecialButtonIn(void)
{
	unsigned int button;
	
	button = (GPIO_PORTE_DATA_R & 0X2);
	
	return button;
	
}

/* This function sets the pin used to drive the fire indicator */
void GPIO_TurnOnFireIndicator(void)
{
	GPIO_PORTB_DATA_R |= 0X10;
}

/*This function resets the pin used to drive the fire indicator*/
void GPIO_TurnOffFireIndicator(void)
{
	GPIO_PORTB_DATA_R &= ~0X10;
}

/*This function sets the pin used to drive the special indicator*/
void GPIO_TurnOnSpecialIndicator(void)
{
	GPIO_PORTB_DATA_R |= 0X20;
}

/*This functions resets the pin used to drive the special indicator*/
void GPIO_TurnOffSpecialIndicator(void)
{
	GPIO_PORTB_DATA_R &= ~0X20;
}
