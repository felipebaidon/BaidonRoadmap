// SysTick.c
// Runs on LM4F120 or TM4C123, 
// for space invaders implementation 
// This file contains functions and related information 
//to setup the systick functionality
// Felipe Baidon
// April 14, 2022


#include "..//tm4c123gh6pm.h"
#include "SysTick.h"

#define MAIN_CLOCK_FREQ	80000000

/* Global variables*/
	FuncPtr CallbackFunction;
	
/* This function writes to the system control registers to setup
	the SysTick. it receives the handler to a function to be executed in 
	the interrupt
*/
void SysTick_Init(FuncPtr callback, unsigned long period)
{
	NVIC_ST_CTRL_R = 0;
	NVIC_ST_RELOAD_R = period -1;
	NVIC_ST_CURRENT_R = 0;
	NVIC_SYS_PRI3_R = NVIC_SYS_PRI3_R&0x00FFFFFF; // priority 0  
	NVIC_ST_CTRL_R = 0x00000007;  // enable with core clock and interrupts
	CallbackFunction = callback;
}

 void SysTick_Handler(void)
 {
	 CallbackFunction();
 }
