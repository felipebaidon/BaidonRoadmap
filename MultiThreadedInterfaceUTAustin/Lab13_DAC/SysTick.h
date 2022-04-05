// SysTick.c
// Runs on LM4F120 or TM4C123, 
// for edX lab 13 
// Implementation of the SysTick
// Felipe Baidon
// March 11, 2022


/*Function Prototypes */

// **************SysTick_Init*********************
//This function initializes the SysTick interrupts
// and configures the SysTick to run with the Main clock
// Input: none
// Output: none
void SysTick_init(void);

// **************SysTick_SetFreq*********************
// This functions sets the reload value into
// the SysTick reload register and clears
// Input: reloadValue The translated frequency into reload value
// Output: none
void SysTick_SetFreq(unsigned long reloadValue);
