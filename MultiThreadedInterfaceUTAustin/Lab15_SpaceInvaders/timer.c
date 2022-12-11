// timer.c
// Runs on LM4F120 or TM4C123, 
// for space invaders implementation 
// This file contains functions and related information 
//to setup the timer functionality
// Felipe Baidon
// April 18, 2022

#include "..//tm4c123gh6pm.h"
#include "timer.h"
/* File Scope Macro definitions */

/* Global extern variables */
unsigned long Timer_EnemiesPositionUpdated;
unsigned int Timer_InvaderShoots;

/*File scope function prototypes*/
static void Timer2_Init(void);
static void Timer0_Init(void);

Callback Timer_ReadInput;
Callback Timer_MoveInvaders;

void Timer_InitTimers(Callback senseInput, Callback refreshPosition)
{
	Timer2_Init();
	Timer_ReadInput = senseInput;
	Timer0_Init();
	Timer_MoveInvaders = refreshPosition;
	Timer_InvaderShoots = 0;
	
}

/* This function is used to setup the timer2 module
		which is used to drive the sense of movement on the cursor through
		the ADC*/
static void Timer2_Init(void){ 
  unsigned long volatile delay;
  SYSCTL_RCGC1_R |= 0x40000;   // 0) activate timer2
  delay = SYSCTL_RCGC1_R;
  TIMER2_CTL_R = 0x00000000;    // 1) disable timer2A during setup
  TIMER2_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER2_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER2_TAILR_R = 1000000-1;    // 4) reload value
  TIMER2_TAPR_R = 0;            // 5) bus clock resolution
  TIMER2_ICR_R = 0x00000001;    // 6) clear timer2A timeout flag
  TIMER2_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF)|0x80000000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 39, interrupt number 23
  NVIC_EN0_R = 1<<23;           // 9) enable IRQ 23 in NVIC
  TIMER2_CTL_R = 0x00000001;    // 10) enable timer2A
}

/* This function is used to setup the timer0 module which is used
		to refresh the position of the space invaders in the screen */
static void Timer0_Init(void)
{
	unsigned long delay;
	SYSCTL_RCGC1_R |= SYSCTL_RCGC1_TIMER0;
	delay = SYSCTL_RCGC1_R;
	TIMER0_CTL_R = 0x00000000;
	TIMER0_CFG_R = 0;
	TIMER0_TAMR_R = 0x02;
	TIMER0_TAILR_R = 80000000/30 - 1;
	TIMER0_TAPR_R = 0;
	TIMER0_ICR_R = 0x00000001;
	TIMER0_IMR_R = 0x00000001;
	NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF)|0x90000000; // 9) priority 5
	NVIC_EN0_R = 1<<19;           // 9) enable IRQ 35 in NVIC
  TIMER0_CTL_R = 0x00000001;    // 10) enable timer0A
	
}

 void Timer2A_Handler(void){ 
  TIMER2_ICR_R = 0x00000001;   // acknowledge timer2A timeout
	Timer_ReadInput(); 
}
 
void Timer0A_Handler(void)
{
	TIMER0_ICR_R = 0x00000001;
	Timer_MoveInvaders();;
  Timer_EnemiesPositionUpdated = 1;
	Timer_InvaderShoots++;
}


