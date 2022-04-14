// Heartbeat.c
// Runs on LM4F120 or TM4C123, 
// for edX lab 13 
// This module initializes the Heartbeat interface to monitor the interrupts
// Felipe Baidon
// March 19, 2022

#include "..//tm4c123gh6pm.h"

void HeartBeat_init(void)
{
	unsigned long delay;
	SYSCTL_RCGCGPIO_R|= SYSCTL_RCGC2_GPIOF;
	delay = SYSCTL_RCGCGPIO_R;
	GPIO_PORTF_DIR_R |= 0x02;
	GPIO_PORTF_AFSEL_R &= ~0X02;
	GPIO_PORTF_DR2R_R |= 0X02;
	GPIO_PORTF_DEN_R |= 0X02;	
}
