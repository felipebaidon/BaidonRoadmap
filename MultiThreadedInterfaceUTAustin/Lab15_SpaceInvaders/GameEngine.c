// GameEngine.c
// Runs on LM4F120 or TM4C123, 
// for space invaders implementation 
// This file contains the Game Engine for the Space invaders game
// Felipe Baidon
// April 27, 2022

#include "..//tm4c123gh6pm.h"
#include "sprites.h"
#include "Nokia5110.h"
#include "gpio.h"
#include "dac.h"
#include "adc.h"
#include "sound.h"
#include "timer.h"
#include "conversion.h"


#define ENEMY_HEIGHT	 10
#define ENEMY_WIDTH		 16
#define MISSILE_HEIGHT	9 


/*File scope typedefs */
typedef struct 
{
	unsigned int currentState;
	unsigned int previousState;
	unsigned int wasPressed;
}Button_t;

/* File Scope variables*/
Button_t GameEngine_FireButton;
Button_t GameEngine_SpecialButton;
unsigned long GameEngine_ADCData;
unsigned long GameEngine_ShipPosition;
unsigned int GameEngine_CreateExplosion;

/* Function prototypes*/
static void GameEngine_InitDisplay(void);
static void GameEngine_SenseFireButton(void);
static void GameEngine_SenseSpecialButton(void);
static void GameEngine_SenseInput(void);
static void GameEngine_performFire(void);
static void GameEngine_performSpecialAttack(void);
static void GameEngine_Attack(void);
static void GameEngine_RefreshScreen(void);
static void GameEngine_SenseCursorPosition(void);
static void GameEngine_MovePlayerShip(void);
static void GameEngine_CheckGameOver(void);
static void GameEngine_DestroyInvader(void);

/*------------GameEngine_Init------------
	This function initializes the peripherals used to setup
	the buttons, indicators, sound and joystick
	Input: none
	Output: none*/
void GameEngine_Init(void)
{
	GameEngine_ADCData = 0;
	GameEngine_ShipPosition = 0;
	GameEngine_CreateExplosion = 0;
	
	GPIO_InitButtons(&GameEngine_SenseFireButton);
	ADC0_Init();
	Timer_InitTimers(&GameEngine_SenseInput, &Sprites_Move);
	Sound_Init();
	
	GameEngine_SenseCursorPosition();
	GameEngine_MovePlayerShip();
	
	GameEngine_InitDisplay();
	
	GameEngine_FireButton.previousState = GPIO_FireButtonIn();
	GameEngine_SpecialButton.previousState = GPIO_SpecialButtonIn();
}

/*------------GameEngine_InitDisplay------------
	This function initializes and draws the videogame
	main screen 
	Input:  The initial position of the cursor
	Output: none*/
static void GameEngine_InitDisplay(void)
{
	Nokia5110_Init();
	Sprites_Init(GameEngine_ShipPosition);
	Sprites_Draw();
}

/*------------GameEngine_SenseInput------------
	This function senses a button press or 
	joystick move, it runs on every timer interruption
	Input: none
	Output: none*/
static void GameEngine_SenseInput(void)
{
		GameEngine_SenseSpecialButton();
		GameEngine_SenseCursorPosition();
}

/*------------GameEngine_SenseFireButton------------
	This function checks if the Fire Button has been pressed or
	released, it runs in the timer thread 
	main screen 
	Input: none
	Output: none*/
static void GameEngine_SenseFireButton(void)
{
	GameEngine_FireButton.currentState = GPIO_FireButtonIn();
	
	if( GameEngine_FireButton.currentState && !GameEngine_FireButton.previousState)
	{
		GameEngine_FireButton.wasPressed = 1;
	}
	else if(!GameEngine_FireButton.currentState && GameEngine_FireButton.previousState)
	{
		GameEngine_FireButton.wasPressed = 0;
	}
}

/*------------GameEngine_SenseSpecialButton------------
	This function checks if the Special Button has been pressed or
	released, it runs in the timer thread 
	main screen 
	Input: none
	Output: none*/
static void GameEngine_SenseSpecialButton(void)
{
	GameEngine_SpecialButton.currentState = GPIO_SpecialButtonIn();

	if( GameEngine_SpecialButton.currentState && !GameEngine_SpecialButton.previousState)
	{
		GameEngine_SpecialButton.wasPressed = 1;
	}
	else if(!GameEngine_SpecialButton.currentState && GameEngine_SpecialButton.previousState)
	{
		GameEngine_SpecialButton.wasPressed = 0;
	}
}

/*------------GameEngine_SenseCursorPosition------------
	This function reads the position of the cursor and thus
	the player ship
	Input: none
	Output: none*/
static void GameEngine_SenseCursorPosition(void)
{
	GameEngine_ADCData = ADC0_In();
}
	

/*------------GameEngine_MainEngine------------
	This function handles the overall game Engine
	Input: none
	Output: none*/
void GameEngine_MainEngine(void)
{
	GameEngine_Attack();
	GameEngine_MovePlayerShip();
	GameEngine_DestroyInvader();
	GameEngine_RefreshScreen();
	GameEngine_CheckGameOver();
	
}


/*------------GameEngine_Attack------------
	This function is responsible for calling the
	Attack routines when there has been a button
	press
	Input: none
	Output: none*/
static void GameEngine_Attack(void)
{
	GameEngine_performFire();
	GameEngine_performSpecialAttack();
	
}
	
/*------------GameEngine_performFire------------
	This function plays the elements of the videogame
	related to a missil Fire.
	Input: none
	Output: none*/
static void GameEngine_performFire(void)
{
	if(GameEngine_FireButton.wasPressed)
	{
		GPIO_TurnOnFireIndicator();
		Sound_Shoot();
		Sprites_SendMissile(GameEngine_ShipPosition);
		GameEngine_FireButton.wasPressed = 0;
	}
	else
	{
		GPIO_TurnOffFireIndicator();
	}
		
}

/*------------GameEngine_performSpecial------------
	This function plays the elements of the videogame
	related to a special attack.
	Input: none
	Output: none*/
static void GameEngine_performSpecialAttack(void)
{
	if(GameEngine_SpecialButton.wasPressed)
	{
		GPIO_TurnOnSpecialIndicator();
		Sound_Shoot();
		//Draw missile
		GameEngine_SpecialButton.wasPressed = 0;
	}
	else
	{
		GPIO_TurnOffSpecialIndicator();
	}
		
}

/*------------GameEngine_MovePlayerShip------------
	This function updates the position of the player
	ship
	Input: none
	Output: none
	*/
static void GameEngine_MovePlayerShip(void)
{
	GameEngine_ShipPosition = Convert(GameEngine_ADCData);
	Sprites_MovePlayerShip(GameEngine_ShipPosition);
}

/*------------GameEngine_RefreshScreen------------
	This function draws the screen at a certain rate, monitored by
	the RefreshScreen flag
	Input: none
	Output: none*/
void GameEngine_RefreshScreen(void)
{
	if(Timer_EnemiesPositionUpdated == 1 )
	{
		Nokia5110_Clear();
		Sprites_Draw();
		Timer_EnemiesPositionUpdated = 0;
	}
}
	/*------------GameEngine_DestroyInvader------------
	This function kills a Space Invader whenever the missile
	has hit it
	Input: none
	Output: none*/
void GameEngine_DestroyInvader(void)
{
	unsigned int i;
	
	if(Missile.life)
	{
		for( i = 0; i < NUMBER_OF_ENEMIES ; i++)
		{
			if((((Missile.y - MISSILE_HEIGHT)< Enemy[i].y) && ((Missile.y - MISSILE_HEIGHT) > (Enemy[i].y - ENEMY_HEIGHT)))
				&& ((Missile.x > Enemy[i].x) && (Missile.x < (Enemy[i].x + ENEMY_WIDTH))) && Enemy[i].life)
			{
				Missile.life = 0;
				Enemy[i].life = 0;
				GameEngine_CreateExplosion = 1;
				Explosion.x = Enemy[i].x;
				Explosion.y = Enemy[i].y;
				Explosion.life = 1;
				break;
			}
		}
	}
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

/*------------GameEngine_CheckGameOver------------
	This function writes game over to the nokia display whenever
	the space invaders have reached the player ship
	Input: none
	Output: none
*/
void GameEngine_CheckGameOver(void)
{
	if((Enemy[NUMBER_OF_ENEMIES -1].y - ENEMY_HEIGHT) > (HEIGHT_OF_DISPLAY - PLAYER_SHIP_HEIGHT))
	{
		Nokia5110_Clear();
		Nokia5110_SetCursor(0,0);
		Nokia5110_OutString((char*)"Game Over");
	}
}

