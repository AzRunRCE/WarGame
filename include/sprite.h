
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
    UP_RIGTH = 2,
    RIGTH = 3,
    DOWN_RIGTH = 4,
    DOWN = 5,
    DOWN_LEFT = 6,
    LEFT = 7,
    UP_LEFT = 8,
    FIRE =9,
    UP_FIRE = 9,
    UP_RIGTH_FIRE = 9,
    RIGTH_FIRE = 9,
    DOWN_RIGTH_FIRE = 9,
    DOWN_FIRE = 9,
    DOWN_LEFT_FIRE = 9,
    LEFT_FIRE = 9,
    UP_LEFT_FIRE = 9
} State;
#endif // SPRITE_H_INCLUDED
