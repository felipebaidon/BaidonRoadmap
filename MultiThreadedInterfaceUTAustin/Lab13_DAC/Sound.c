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
#define SIZE_OF_SINE_TABLE	16

unsigned int Index;

const unsigned char SineWave[SIZE_OF_SINE_TABLE] = {4,5,6,7,7,7,6,5,4,3,2,1,1,1,2,3};

// **************Sound_Init*********************
// Initialize Systick periodic interrupts
// Also calls DAC_Init() to initialize DAC
// Input: none
// Output: none
void Sound_Init(void){
	Index = 0;
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
	unsigned int SysTickFreq = 0;
	unsigned int reloadValue = 0;

	SysTickFreq = period * SIZE_OF_SINE_TABLE; //relation is resulting frequency = systick freq / size of sine table
	reloadValue = (int)( MAIN_CLOCK_FREQ / SysTickFreq) - 1;
	
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
	
	Index = (Index+1)&0x0F;  
  DAC_Out(SineWave[Index]); 
   
}
