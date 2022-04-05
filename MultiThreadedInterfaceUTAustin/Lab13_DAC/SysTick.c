// SysTick.c
// Runs on LM4F120 or TM4C123, 
// for edX lab 13 
// Implementation of the SysTick
// Felipe Baidon
// March 10, 2022

#include "..//tm4c123gh6pm.h"
#include "SysTick.h"


// **************SysTick_Init*********************
//This function initializes the SysTick interrupts
// and configures the SysTick to run with the Main clock
// Input: none
// Output: none
void SysTick_init(void)
{
	NVIC_ST_CTRL_R = 0;
	NVIC_SYS_PRI3_R = NVIC_SYS_PRI3_R&0x00FFFFFF; // priority 0  
	NVIC_ST_CTRL_R = 0x00000007;  // enable with core clock and interrupts
}


// **************SysTick_SetFreq*********************
// This functions sets the reload value into
// the SysTick reload register and clears
// Input: reloadValue The translated frequency into reload value
// Output: none
void SysTick_SetFreq(unsigned long reloadValue)
{
	NVIC_ST_RELOAD_R = reloadValue;
	NVIC_ST_CURRENT_R = 0;  
  NVIC_ST_CTRL_R |= 0x00000002;  // ensure interrupts are enabled 
}

