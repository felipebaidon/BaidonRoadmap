// SpaceInvaders.c
// Runs on LM4F120/TM4C123
// Jonathan Valvano and Daniel Valvano
// This is a starter project for the edX Lab 15
// In order for other students to play your game
// 1) You must leave the hardware configuration as defined
// 2) You must not add/remove any files from the project
// 3) You must add your code only this this C file
// I.e., if you wish to use code from sprite.c or sound.c, move that code in this file
// 4) It must compile with the 32k limit of the free Keil

// April 10, 2014
// http://www.spaceinvaders.de/
// sounds at http://www.classicgaming.cc/classics/spaceinvaders/sounds.php
// http://www.classicgaming.cc/classics/spaceinvaders/playguide.php
/* This example accompanies the books
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2013

   "Embedded Systems: Introduction to Arm Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2013

 Copyright 2014 by Jonathan W. Valvano, valvano@mail.utexas.edu
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
// ******* Required Hardware I/O connections*******************
// Slide pot pin 1 connected to ground
// Slide pot pin 2 connected to PE2/AIN1
// Slide pot pin 3 connected to +3.3V 
// fire button connected to PE0
// special weapon fire button connected to PE1
// 8*R resistor DAC bit 0 on PB0 (least significant bit)
// 4*R resistor DAC bit 1 on PB1
// 2*R resistor DAC bit 2 on PB2
// 1*R resistor DAC bit 3 on PB3 (most significant bit)
// LED on PB4
// LED on PB5

// Blue Nokia 5110
// ---------------
// Signal        (Nokia 5110) LaunchPad pin
// Reset         (RST, pin 1) connected to PA7
// SSI0Fss       (CE,  pin 2) connected to PA3
// Data/Command  (DC,  pin 3) connected to PA6
// SSI0Tx        (Din, pin 4) connected to PA5
// SSI0Clk       (Clk, pin 5) connected to PA2
// 3.3V          (Vcc, pin 6) power
// back light    (BL,  pin 7) not connected, consists of 4 white LEDs which draw ~80mA total
// Ground        (Gnd, pin 8) ground

// Red SparkFun Nokia 5110 (LCD-10168)
// -----------------------------------
// Signal        (Nokia 5110) LaunchPad pin
// 3.3V          (VCC, pin 1) power
// Ground        (GND, pin 2) ground
// SSI0Fss       (SCE, pin 3) connected to PA3
// Reset         (RST, pin 4) connected to PA7
// Data/Command  (D/C, pin 5) connected to PA6
// SSI0Tx        (DN,  pin 6) connected to PA5
// SSI0Clk       (SCLK, pin 7) connected to PA2
// back light    (LED, pin 8) not connected, consists of 4 white LEDs which draw ~80mA total

#include "..//tm4c123gh6pm.h"
#include "Nokia5110.h"
#include "Random.h"
#include "TExaS.h"
#include "SysTick.h"
#include "gpio.h"
#include "dac.h"
#include "timer.h"
#include "adc.h"
#include "conversion.h"
#include "sound.h"
#include "GameEngine.h"

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

void Delay100ms(unsigned long count); // time delay in 0.1 seconds
void Delay10ms(void);


unsigned long Timer0Count;
unsigned long Timer2Count;

unsigned long ADCReady;
unsigned long RefreshScreen;

unsigned long ADCdata;
unsigned long Distance;
unsigned char String[10];

#define C_NOTE	523.251 //Hz

const unsigned char SineWave[SIZE_OF_SINE_TABLE] = {4,5,6,7,7,7,6,5,4,3,2,1,1,1,2,3};

int main()
{	
	unsigned int currentFireButtonState, previousFireButtonState;
	unsigned int currentSpecialButtonState, previousSpecialButtonState;
	
	TExaS_Init(SSI0_Real_Nokia5110_Scope);  // set system clock to 80 MHz
	
	GPIO_ButtonInit();
	GPIO_LEDInit();
	ADC0_Init();
	Sound_Init();
	Timer2_Init();
	Timer0_Init();
	GameEngine_InitDisplay();
	EnableInterrupts();
	
	previousFireButtonState = GPIO_FireButtonIn();
	previousSpecialButtonState = GPIO_SpecialButtonIn();
	
	
	while(GameEngine_GetEnemiesLife())
	{
		
		currentFireButtonState = GPIO_FireButtonIn();
		currentSpecialButtonState = GPIO_SpecialButtonIn();
		
		if( currentFireButtonState && !previousFireButtonState)
		{
			GPIO_TurnOnFireIndicator();
			Sound_Shoot();
		}
		else if(!currentFireButtonState && previousFireButtonState)
		{
			GPIO_TurnOffFireIndicator();
		}
		
		if( currentSpecialButtonState && !previousSpecialButtonState)
		{
			GPIO_TurnOnSpecialIndicator();
			Sound_Shoot();
		}
		else if(!currentSpecialButtonState && previousSpecialButtonState)
		{
			GPIO_TurnOffSpecialIndicator();
		}
		
		if( ADCReady == 1)
		{
				Distance = Convert(ADCdata);
				ConvertDistance(Distance);
//				Nokia5110_Clear();
//				Nokia5110_SetCursor(0,0);
//				Nokia5110_OutString((char*)String);
				ADCReady = 0;
		}
		
		if(RefreshScreen == 1)
		{
			GameEngine_RefreshScreen();
			RefreshScreen = 0;
		}
		
		previousFireButtonState = currentFireButtonState;
		previousSpecialButtonState = currentSpecialButtonState;
		
		Delay10ms();
	}
	
	GameEngine_DisplayGameOver();
}
 
 void Timer2A_Handler(void){ 
  TIMER2_ICR_R = 0x00000001;   // acknowledge timer2A timeout
  Timer2Count++;
	ADCdata= ADC0_In();
  ADCReady = 1; // trigger	
}
 
void Timer0A_Handler(void)
{
	TIMER0_ICR_R = 0x00000001;
  Timer0Count++;
	GameEngine_MoveEnemies();
  RefreshScreen = 1; // trigger	
}
 
void Delay100ms(unsigned long count){unsigned long volatile time;
  while(count>0){
    time = 727240;  // 0.1sec at 80 MHz
    while(time){
	  	time--;
    }
    count--;
  }
}

//---------------------Delay10ms---------------------
// wait 10ms for switches to stop bouncing
// Input: none
// Output: none
void Delay10ms(void){unsigned long volatile time;
  time = 14545;  // 10msec
  while(time){
		time--;
  }
}

