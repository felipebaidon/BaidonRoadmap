#ifndef SYSTICK_H
#define SYSTICK_H
// SysTick.h
// Runs on LM4F120 or TM4C123, 
// for space invaders implementation 
// This file contains functions and related information 
//to setup the systick functionality
// Felipe Baidon
// April 14, 2022

/* Global defines */
#define SIZE_OF_SINE_TABLE	16

/* This function writes to the system control registers to setup
	the SysTick.
*/
void SysTick_Init(void);

/* This function is used on run time to setup a different frequency depending on the sound 
		to be played, it converts the period to the reload value to be assed on to the Sound_SetPeriod 
		function*/
void SysTick_SetTone(double period);

#endif

