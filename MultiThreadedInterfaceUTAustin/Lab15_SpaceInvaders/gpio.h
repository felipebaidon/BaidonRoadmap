#ifndef GPIO_H
#define GPIO_H
// gpio.h
// Runs on LM4F120 or TM4C123, 
// for space invaders implementation 
// This file contains functions and related information 
//to setup the gpio used in the videogame development
// Felipe Baidon
// April 14, 2022

/* This function initializes the GPIO pins that are used
	as Button inputs*/
void GPIO_ButtonInit(void);

/* This function initializes the GPIO pins that are
		used as ouputs to drive the LEDs*/
void GPIO_LEDInit(void);

/* This function reads the state of the PE0 and returns it */
unsigned int GPIO_FireButtonIn(void);

/*This function reads the state of the PE1 and returns it 
*/
unsigned int GPIO_SpecialButtonIn(void);

/* This function sets the pin used to drive the fire indicator */
void GPIO_ToggleFireIndicator(void);

/*This function resets the pin used to drive the fire indicator*/
void GPIO_TurnOffFireIndicator(void);

/*This function sets the pin used to drive the special indicator*/
void GPIO_TurnOnSpecialIndicator(void);

/*This functions resets the pin used to drive the special indicator*/
void GPIO_TurnOffSpecialIndicator(void);

#endif
