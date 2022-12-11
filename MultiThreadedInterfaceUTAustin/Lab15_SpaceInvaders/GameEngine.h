// GameEngine.h
// Runs on LM4F120 or TM4C123, 
// for space invaders implementation 
// This file contains the Game Engine for the Space invaders game
// Felipe Baidon
// April 27, 2022
#ifndef GAMEENGINE_H_
#define GAMEENGINE_H_

/*------------GameEngine_InitHardware------------
	This function initializes the peripherals used to setup
	the buttons, indicators, sound and joystick
	Input: none
	Output: none*/
void GameEngine_Init(void);


/*------------GameEngine_GetEnemiesLife------------
	This function determines if there is any space invader still alive
	in the screen
	Input: none
	Output: 1 in case there are Space Invaders left, 0 otherwise
	*/
unsigned int GameEngine_GetEnemiesLife(void);

/*------------GameEngine_MainEngine------------
	This function handles the Main game Engine
	Input: none
	Output: none*/
void GameEngine_MainEngine(void);

#endif
