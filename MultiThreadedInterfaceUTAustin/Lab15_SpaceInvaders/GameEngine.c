// GameEngine.c
// Runs on LM4F120 or TM4C123, 
// for space invaders implementation 
// This file contains the Game Engine for the Space invaders game
// Felipe Baidon
// April 27, 2022

#include "..//tm4c123gh6pm.h"
#include "sprites.h"
#include "Nokia5110.h"

/*------------GameEngine_InitHardware------------
	This function initializes the peripherals used to setup
	the buttons, indicators, and joystick
	Input: none
	Output: none*/
void GameEngine_InitHardware(void)
{

}

/*------------GameEngine_InitDisplay------------
	This function initializes and draws the videogame
	main screen 
	Input: none
	Output: none*/
void GameEngine_InitDisplay(void)
{
	Nokia5110_Init();
	Sprites_Init();
	Sprites_Draw();
}

/*------------GameEngine_MoveEnemies------------
	This function updates the position of the 
	space invaders
	Input: none
	Output: none
	*/
void GameEngine_MoveEnemies(void)
{
	Sprites_Move();
}

/*------------GameEngine_RefreshScreen------------
	This function draws the screen at a certain rate, monitored by
	the RefreshScreen flag
	Input: none
	Output: none*/
void GameEngine_RefreshScreen(void)
{
	
}
	
/*------------GameEngine_GetEnemiesLife------------
	This function determines if there is any space invader still alive
	in the screen
	Input: none
	Output: 1 in case there are Space Invaders left, 0 otherwise*/
unsigned int GameEngine_GetEnemiesLife(void)
{
	unsigned int anyLife = 0;
	unsigned int i;
	
	for(i= 0; i< NUMBER_OF_ENEMIES; i++)
	{
		anyLife |=  Enemy[i].life;
	}
	
	return anyLife;
}

/*------------GameEngine_DisplayGameOver------------
	This function writes game over to the nokia display whenever
	all the space invaders have been killed
	Input: none
	Output: none
*/
void GameEngine_DisplayGameOver(void)
{
	Nokia5110_Clear();
	Nokia5110_SetCursor(0,0);
	Nokia5110_OutString((char*)"Game Over");
}

