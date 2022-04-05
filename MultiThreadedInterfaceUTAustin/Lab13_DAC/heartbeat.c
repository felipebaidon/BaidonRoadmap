// heartbeat.c
// Runs on LM4F120 or TM4C123, 
// for edX lab 13 
// Implementation of the heartbeat
// Felipe Baidon
// March 10, 2022

#include "..//tm4c123gh6pm.h"

// ***** Heartbeat_Init *****
//
//This function initializes PF3 as a HeartBeat
//
//Input: none
//Output: none
//
void Heartbeat_Init(void)
{
	volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x00000020;     // 1) activate clock for Port F
  delay = SYSCTL_RCGC2_R;           // allow time for clock to start
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0
  // only PF0 needs to be unlocked, other bits can't be locked
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog on PF
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) PCTL GPIO on PF4-0
  GPIO_PORTF_DIR_R = 0x0E;          // 5) PF3-1 out
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) disable alt funct on PF7-0
  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital I/O on PF4-0
}

// ***** Heartbeat_Toggle *****
// This function toggles PF3
//
//Input: none
//Output: none
//
void Heartbeat_Toggle(void)
{
	GPIO_PORTF_DATA_R ^= 0x08;
}

