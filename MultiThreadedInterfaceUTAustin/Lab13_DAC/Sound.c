// Sound.c
// Runs on LM4F120 or TM4C123, 
// edX lab 13 
// Use the SysTick timer to request interrupts at a particular period.
// Daniel Valvano, Jonathan Valvano
// December 29, 2014
// This routine calls the 4-bit DAC

#include "Sound.h"
#include "DAC.h"
#include "SysTick.h"
#include "..//tm4c123gh6pm.h"

#define MAIN_CLOCK_FREQ	80000000 //hz


// **************Sound_Init*********************
// Initialize Systick periodic interrupts
// Also calls DAC_Init() to initialize DAC
// Input: none
// Output: none
void Sound_Init(void){
	SysTick_init();
	DAC_Init();
  
}

// **************Sound_Tone*********************
// Change Systick periodic interrupts to start sound output
// Input: interrupt period
//           Units of period are 12.5ns
//           Maximum is 2^24-1
//           Minimum is determined by length of ISR
// Output: none
void Sound_Tone(double period){
// this routine sets the RELOAD and starts SysTick
	unsigned int reloadValue = 0;
	
	reloadValue = (int)( MAIN_CLOCK_FREQ / period) - 1;
	
	SysTick_SetFreq(reloadValue);
}


// **************Sound_Off*********************
// stop outputing to DAC
// Output: none
void Sound_Off(void){
 // this routine stops the sound output
}


// Interrupt service routine
// Executed every 12.5ns*(period)
void SysTick_Handler(void){
	
   
}
