// Lab13.c
// Runs on LM4F120 or TM4C123
// Use SysTick interrupts to implement a 4-key digital piano
// edX Lab 13 
// Daniel Valvano, Jonathan Valvano
// December 29, 2014
// Port B bits 3-0 have the 4-bit DAC
// Port E bits 3-0 have 4 piano keys

#include "..//tm4c123gh6pm.h"
//#include "Sound.h"
#include "Piano.h"
#include "TExaS.h"
#include "heartbeat.h"
#include "DAC.h"

#define C_KEY_PRESSED	1
#define D_KEY_PRESSED	2
#define E_KEY_PRESSED	4
#define G_KEY_PRESSED	8

#define C_NOTE	523.251 //Hz
#define D_NOTE	587.330
#define E_NOTE	659.255
#define G_NOTE	783.991

// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void delay(unsigned long msec);
//int main(void){ // Real Lab13 
//	// for the real board grader to work 
//	// you must connect PD3 to your DAC output
//	unsigned int keyPressed = 0;
//	
//  TExaS_Init(SW_PIN_PE3210, DAC_PIN_PB3210,ScopeOn); // activate grader and set system clock to 80 MHz
//// PortE used for piano keys, PortB used for DAC        
//  Sound_Init(); // initialize SysTick timer and DAC
//  Piano_Init();
//	Heartbeat_Init();
//  EnableInterrupts();  // enable after all initialization are done
//  while(1){                
//// input from keys to select tone
//		keyPressed = Piano_In();
//		
//			switch(keyPressed)
//			{			
//				case C_KEY_PRESSED:
//					Sound_Tone(C_NOTE);
//					break;
//				case D_KEY_PRESSED:
//					Sound_Tone(D_NOTE);
//					break;
//				case E_KEY_PRESSED:
//					Sound_Tone(E_NOTE);
//					break;
//				case G_KEY_PRESSED:
//					Sound_Tone(G_NOTE);
//					break;
//			}
//  }
//            
//}

int main(void){ // this main is to debug the DAC
      // you must connect PD3 to your DAC output
  TExaS_Init(SW_PIN_PE3210, DAC_PIN_PB3210,ScopeOn);
  DAC_Init(); // initialize SysTick timer and DAC
  EnableInterrupts();  // enable after all initialization are done
   while(1){unsigned long i; // static debugging
     for(i=0;i<16;i++){
       DAC_Out(i);
       delay(10); // connect PD3 to DAC output
     }
   }  
 }

// Inputs: Number of msec to delay
// Outputs: None
void delay(unsigned long msec){ 
  unsigned long count;
  while(msec > 0 ) {  // repeat while there are still delay
    count = 16000;    // about 1ms
    while (count > 0) { 
      count--;
    } // This while loop takes approximately 3 cycles
    msec--;
  }
}


