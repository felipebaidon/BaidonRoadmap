// SysTick.c
// Runs on LM4F120 or TM4C123, 
// for edX lab 13 
// Implementation of the SysTick
// Felipe Baidon
// March 10, 2022

#include "SysTick.h"


// Initialize SysTick interrupts to trigger at 40 Hz, 25 ms
void SysTick_Init(void){
	NVIC_ST_CTRL_R = 0;           
	NVIC_ST_RELOAD_R = 1999999;     
	NVIC_ST_CURRENT_R = 0;        
  NVIC_SYS_PRI3_R = NVIC_SYS_PRI3_R&0x00FFFFFF;              
  NVIC_ST_CTRL_R = 0x00000007;  
}
