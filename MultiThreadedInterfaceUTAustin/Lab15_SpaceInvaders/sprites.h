#ifndef SPRITES_H_
#define SPRITES_H_

#define NUMBER_OF_ENEMIES 	4	

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
extern STyp Enemy[NUMBER_OF_ENEMIES];


void Sprites_Init(void);

void Sprites_Move(void);

void Sprites_Draw(void);

#endif
