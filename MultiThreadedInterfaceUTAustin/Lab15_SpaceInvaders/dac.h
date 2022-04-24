#ifndef DAC_H
#define DAC_H

// dac.h
// Runs on LM4F120 or TM4C123, 
// for space invaders implementation 
// This file contains functions and related information 
//to setup the DAC funtionality
// Felipe Baidon
// April 14, 2022

/* This function writes to the DAC-related registers 
		to configure the DAC */
void DAC_Init(void);

/*This function retrieves the data obtained written to the
	DAC-related output register */
void DAC_Out(unsigned int data);


/* This function turns off the sound generation by writting
		0 to the output
*/
void Sound_TurnOff(void);

#endif
