
#ifndef SPRITE_H_INCLUDED
#define SPRITE_H_INCLUDED

typedef enum
{
    UP_IDLE= 0,
    UP_RIGTH_IDLE= 0,
    RIGTH_IDLE= 0,
    DOWN_RIGTH_IDLE= 0,
    DOWN_IDLE= 0,
    DOWN_LEFT_IDLE= 0,
    LEFT_IDLE= 0,
    UP_LEFT_IDLE = 0,

    UP = 1,
    UP_RIGTH = 1,
    RIGTH = 2,
    DOWN_RIGTH = 2,
    DOWN = 3,
    DOWN_LEFT = 3,
    LEFT = 4,
    UP_LEFT = 4,
    FIRE =5,
    UP_FIRE = 5,
    UP_RIGTH_FIRE = 5,
    RIGTH_FIRE = 5,
    DOWN_RIGTH_FIRE = 5,
    DOWN_FIRE = 5,
    DOWN_LEFT_FIRE = 5,
    LEFT_FIRE = 5,
    UP_LEFT_FIRE = 5
} State;
#endif // SPRITE_H_INCLUDED
