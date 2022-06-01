#ifndef TIMER_H
#define TIMER_H

// timer.h
// Runs on LM4F120 or TM4C123, 
// for space invaders implementation 
// This file contains functions and related information 
//to setup the timer functionality
// Felipe Baidon
// April 18, 2022

/*Global Typedefs*/
typedef void (*Callback)(void);

/* Global Variables*/
extern unsigned long Timer_EnemiesPositionUpdated;

/* This function is used to setup the timer2 and timer0 modules
*/
void Timer_InitTimers(Callback senseInput, Callback refreshPosition);

#endif


