// soundtest.c
// Runs on LM4F120 or TM4C123, 
// for space invaders implementation 
// This file contains functions and related information 
// for the Sound module
// Felipe Baidon
// April 19, 2022


#include "..//tm4c123gh6pm.h"

unsigned int SoundOn;

/* This function initializes the Soundtest
		module */
void Soundtest_init(void)
{
	SoundOn = 0;
}

/* This function turns on the sound generation by
		setting the SoundOn flag high
*/
void Soundtest_TurnOn(void)
{
	SoundOn = 1;
}

/* This function turns off the sound generation by writting
		0 to the output
*/
void Soundtest_TurnOff(void)
{
	GPIO_PORTB_DATA_R = 0;
	SoundOn = 0;
}

/* This is a getter function for the SoundOn flag*/
unsigned int Soundtest_GetFlag(void)
{
	return SoundOn;
}