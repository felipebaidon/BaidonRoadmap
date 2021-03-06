// ADC.c
// Runs on LM4F120/TM4C123
// Provide functions that initialize ADC0 SS3 to be triggered by
// software and trigger a conversion, wait for it to finish,
// and return the result. 
// Daniel Valvano
// January 15, 2016

/* This example accompanies the book
   "Embedded Systems: Introduction to ARM Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2015

 Copyright 2016 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

#include "ADC.h"
#include "..//tm4c123gh6pm.h"

#define ADC_SENSE_INPUT		0x04
#define MAX_ADC0_SPD_125	0x300

// This initialization function sets up the ADC 
// Max sample rate: <=125,000 samples/second
// SS3 triggering event: software trigger
// SS3 1st sample source:  channel 1
// SS3 interrupts: enabled but not promoted to controller
void ADC0_Init(void){ 
	
	unsigned long delay;

	SYSCTL_RCGC2_R |= SYSCTL_GPIOHBCTL_PORTE;
	delay = SYSCTL_RCGC2_R;
	GPIO_PORTE_DIR_R &= ~ADC_SENSE_INPUT;
	GPIO_PORTE_AFSEL_R |= ADC_SENSE_INPUT;
	GPIO_PORTE_DEN_R &= ~ADC_SENSE_INPUT;
	GPIO_PORTE_AMSEL_R |= ADC_SENSE_INPUT;
	
	SYSCTL_RCGC0_R |= SYSCTL_RCGC0_ADC0;
	delay = SYSCTL_RCGC0_R;
	SYSCTL_RCGC0_R &= ~MAX_ADC0_SPD_125;
	ADC0_SSPRI_R = ADC_SSPRI_SS3_M;
	
	//configuration between UT and datahseet differs from here
	ADC0_ACTSS_R &= ~ADC_ACTSS_ASEN3;
	ADC0_EMUX_R |= ADC_EMUX_EM3_PROCESSOR;
	ADC0_SSMUX3_R &= ~ 0Xf;
	ADC0_SSMUX3_R += 1;
	ADC0_SSCTL3_R |= 0X06;
	ADC0_ACTSS_R |= ADC_ACTSS_ASEN3;	

}


//------------ADC0_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
unsigned long ADC0_In(void){  
	unsigned int sample;
	
	//initiate sampling ADCPSSI
	ADC0_PSSI_R |= 0x08;
	while((ADC0_RIS_R&0x08)== 0){};
	sample = ADC0_SSFIFO3_R & 0xfff;
  ADC0_ISC_R |= 0X08;
		
  return sample;
}
