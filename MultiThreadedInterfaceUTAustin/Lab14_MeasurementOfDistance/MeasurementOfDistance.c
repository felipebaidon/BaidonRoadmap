// MeasurementOfDistance.c
// Runs on LM4F120/TM4C123
// Use SysTick interrupts to periodically initiate a software-
// triggered ADC conversion, convert the sample to a fixed-
// point decimal distance, and store the result in a mailbox.
// The foreground thread takes the result from the mailbox,
// converts the result to a string, and prints it to the
// Nokia5110 LCD.  The display is optional.
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

// Slide pot pin 3 connected to +3.3V
// Slide pot pin 2 connected to PE2(Ain1) and PD3
// Slide pot pin 1 connected to ground


#include "ADC.h"
#include "..//tm4c123gh6pm.h"
#include "Nokia5110.h"
#include "TExaS.h"
#include "conversion.h"
#include "SysTick.h"

#define A_CONSTANT	500
#define B_CONSTANT	0

void EnableInterrupts(void);

unsigned char String[10];
unsigned long Distance;   
unsigned long ADCdata;    
unsigned long Flag;       

int main(void){ 
  volatile unsigned long delay;
	unsigned long initialADCData;
	unsigned long initialDistance;
	unsigned int welcomeMsg = 1;
	
  TExaS_Init(ADC0_AIN1_PIN_PE2, SSI0_Real_Nokia5110_Scope);
	ADC0_Init();
	Nokia5110_Init();
	SysTick_Init();
  EnableInterrupts();
	
	Nokia5110_Clear();
	Nokia5110_SetCursor(0,0);
	
	initialADCData = ADC0_In();
	initialDistance = Convert(initialADCData);
	
	Nokia5110_OutString((unsigned char*)"Measurement of Distance Program, Move Cursor to begin");
	
  while(1)
		{ 
			if( Flag == 1)
			{
				Distance = Convert(ADCdata);
				ConvertDistance(Distance);
				if( (initialDistance != Distance) || !welcomeMsg)
				{
					welcomeMsg = 0;
					Nokia5110_Clear();
					Nokia5110_SetCursor(0,0);
					Nokia5110_OutString(String);
				}
				Flag = 0;
			}
  }
}

void SysTick_Handler(void){ 
	ADCdata = ADC0_In();
	Flag = 1;
}
