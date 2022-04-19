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

/*File scope function definitions */
static void SysTick_SetPeriod(unsigned long period);

/* This function writes to the system control registers to setup
	the SysTick.
*/
void SysTick_Init(void)
{
	NVIC_ST_CTRL_R = 0;
	NVIC_SYS_PRI3_R = NVIC_SYS_PRI3_R&0x00FFFFFF; // priority 0  
	NVIC_ST_CTRL_R = 0x00000007;  // enable with core clock and interrupts
}


/* This function writes a different value to the SysTick reload
	registes and starts a new count.
*/
static void SysTick_SetPeriod(unsigned long period)
{
	NVIC_ST_CTRL_R = 0;
	NVIC_ST_RELOAD_R = period -1;
	NVIC_ST_CURRENT_R = 0;
	NVIC_SYS_PRI3_R = NVIC_SYS_PRI3_R&0x00FFFFFF; // priority 0  
	NVIC_ST_CTRL_R = 0X07;
}

/* This function is used on run time to setup a different frequency depending on the sound 
		to be played, it converts the period to the reload value to be assed on to the Sound_SetPeriod 
		function*/
void SysTick_SetTone(double period){
	unsigned int SysTickFreq = 0;
	unsigned int reloadValue = 0;

	SysTickFreq = period * SIZE_OF_SINE_TABLE; //relation is resulting frequency = systick freq / size of sine table
	reloadValue = (int)( MAIN_CLOCK_FREQ / SysTickFreq) - 1;
	
	SysTick_SetPeriod(reloadValue);
}
