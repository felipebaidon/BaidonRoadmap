#ifndef SPRITES_H_
#define SPRITES_H_

#define NUMBER_OF_ENEMIES 	3
#define PLAYER_SHIP_HEIGHT	8
#define HEIGHT_OF_DISPLAY 	47 

// enemy ship that starts at the top of the screen (arms/mouth closed)
// width=16 x height=10
extern const unsigned char SmallEnemy30PointA[];


struct State {
  unsigned long x;      // x coordinate
  unsigned long y;      // y coordinate
  const unsigned char *image; // ptr->image
  long life;            // 0=dead, 1=alive
};

typedef struct State STyp;

extern STyp Sprites_PlayerShip;
extern STyp Enemy[NUMBER_OF_ENEMIES];
extern STyp Missile; 


void Sprites_Init(unsigned long shipPosition);

void Sprites_Move(void);
void Sprites_MovePlayerShip(unsigned long position);
void Sprites_SendMissile(unsigned long position);

void Sprites_Draw(void);

#endif
