
#ifndef SPRITE_H_INCLUDED
#define SPRITE_H_INCLUDED

typedef enum
{
    UP = 1,
    UP_RIGHT = 2,
    RIGHT = 3,
    DOWN_RIGHT = 4,
    DOWN = 5,
    DOWN_LEFT = 6,
    LEFT = 7,
    UP_LEFT = 8,
} View_Orientation;

typedef enum
{
	IDLE=0,
	WALK=1,
	FIRE=2
} Player_State;
#endif // SPRITE_H_INCLUDED
