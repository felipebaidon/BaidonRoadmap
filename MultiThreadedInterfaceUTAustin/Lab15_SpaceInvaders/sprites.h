#ifndef SPRITES_H_
#define SPRITES_H_

#define NUMBER_OF_ENEMIES 	3
#define PLAYER_SHIP_HEIGHT	8
#define PLAYER_SHIP_WIDTH   18
#define HEIGHT_OF_DISPLAY 	47
#define ENEMY_HEIGHT	 10
#define ENEMY_WIDTH		 16

// enemy ship that starts at the top of the screen (arms/mouth closed)
// width=16 x height=10
extern const unsigned char SmallEnemy30PointA[];


struct State {
  signed long x;      // x coordinate
  signed long y;      // y coordinate
  const unsigned char *image; // ptr->image
  long life;            // 0=dead, 1=alive
};

typedef struct State STyp;

extern STyp PlayerShip;
extern STyp Enemy[NUMBER_OF_ENEMIES];
extern STyp PlayerMissile;
extern STyp Explosion;


void Sprites_Init(unsigned long shipPosition);

void Sprites_Move(void);
void Sprites_MovePlayerShip(unsigned long position);
void Sprites_SendShipMissile(unsigned long position);
void Sprites_SendInvaderMissile(signed long invaderHorizontalPos, signed long invaderVerticalPos);

void Sprites_Draw(void);

#endif
