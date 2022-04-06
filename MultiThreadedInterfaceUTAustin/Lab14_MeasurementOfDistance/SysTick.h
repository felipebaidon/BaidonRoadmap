// SysTick.h
// Runs on LM4F120 or TM4C123, 
// for edX lab 13 
// Implementation of the SysTick
// Felipe Baidon
// March 19, 2022

#include "..//tm4c123gh6pm.h"


// Initialize SysTick interrupts to trigger at 40 Hz, 25 ms
void SysTick_Init(void);
