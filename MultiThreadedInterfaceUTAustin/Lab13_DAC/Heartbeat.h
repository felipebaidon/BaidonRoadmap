// Heartbeat.h
// Runs on LM4F120 or TM4C123, 
// for edX lab 13 
// Implementation of the SysTick
// Felipe Baidon
// March 12, 2022

// ***** Heartbeat_Init *****
//
//This function initializes PF3 as a Heartbeat
//
//Input: none
//Output: none
//
void Heartbeat_Init(void);

// ***** Heartbeat_Toggle *****
// This function toggles PF3
//
//Input: none
//Output: none
//
void Heartbeat_Toggle(void);
