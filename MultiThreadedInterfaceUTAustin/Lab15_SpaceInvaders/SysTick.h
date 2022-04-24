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

/* Global Typedefs*/
typedef void(*FuncPtr)(void);

/* This function writes to the system control registers to setup
	the SysTick. it receives the handler to a function to be executed in 
	the interrupt
*/
void SysTick_Init(FuncPtr callback, unsigned long period);

#endif

