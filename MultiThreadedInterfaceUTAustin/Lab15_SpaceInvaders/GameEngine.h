// GameEngine.h
// Runs on LM4F120 or TM4C123, 
// for space invaders implementation 
// This file contains the Game Engine for the Space invaders game
// Felipe Baidon
// April 27, 2022
#ifndef GAMEENGINE_H_
#define GAMEENGINE_H_

/*------------GameEngine_InitDisplay------------
	This function initializes and draws the videogame
	main screen 
	Input: none
	Output: none
	*/
void GameEngine_InitDisplay(void);


/*------------GameEngine_MoveEnemies------------
	This function updates the position of the 
	space invaders
	Input: none
	Output: none
	*/
void GameEngine_MoveEnemies(void);

/*------------GameEngine_RefreshScreen------------
	This function draws the screen at a certain rate, monitored by
	the RefreshScreen flag
	Input: none
	Output: none
	*/
void GameEngine_RefreshScreen(void);

/*------------GameEngine_GetEnemiesLife------------
	This function determines if there is any space invader still alive
	in the screen
	Input: none
	Output: 1 in case there are Space Invaders left, 0 otherwise
	*/
unsigned int GameEngine_GetEnemiesLife(void);

/*------------GameEngine_DisplayGameOver------------
	This function writes game over to the nokia display whenever
	all the space invaders have been killed
	Input: none
	Output: none
*/
void GameEngine_DisplayGameOver(void);


#endif
