#ifndef SOUNDTEST_H
#define SOUNDTEST_H
// soundtest.h
// Runs on LM4F120 or TM4C123, 
// for space invaders implementation 
// This file contains functions and related information 
// for the Sound module
// Felipe Baidon
// April 19, 2022

/* This function initializes the Soundtest
		module */
void Soundtest_init(void);

/* This function turns on the sound generation by
		setting the SoundOn flag high
*/
void Soundtest_TurnOn(void);

/* This function turns off the sound generation by writting
		0 to the output
*/
void Soundtest_TurnOff(void);

/* This is a getter function for the SoundOn flag*/
unsigned int Soundtest_GetFlag(void);

#endif
