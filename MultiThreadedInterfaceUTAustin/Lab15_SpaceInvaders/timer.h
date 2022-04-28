#ifndef TIMER_H
#define TIMER_H

// timer.h
// Runs on LM4F120 or TM4C123, 
// for space invaders implementation 
// This file contains functions and related information 
//to setup the timer functionality
// Felipe Baidon
// April 18, 2022


/* This function is used to setup the timer2 module
		which is used to drive the sense of movement on the cursor through
		the ADC*/
void Timer2_Init(void);

/* This function is used to setup the timer0 module which is used
		to refresh the position of the space invaders in the screen */
void Timer0_Init(void);

#endif


