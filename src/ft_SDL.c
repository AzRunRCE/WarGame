#include "sprite.h"
#include <SDL/SDL.h>
#include "player.h"
void ft_getCharactSprite(Player *player)
{
    State state = player->state;
    int step = player->step;

    if (player->fire == true)
    {
        step = ft_getFirePosition(state);
        if (AnimDelay())
            state = 9;
        else
            state = 0;
    }
    else
    {
        if (player->walk &&  player->step < 4 && AnimDelay() )
                 player->step++;
        if ( player->walk &&  player->step > 3)
                 player->step = 0;
    }

    player->spriteRect.x = 32 * step + (step + 1);
    player->spriteRect.y = 32 * state + (state + 1);
    player->spriteRect.h = 32;
    player->spriteRect.w = 32;
}


int ft_getFirePosition(State characterState)
{
    int step;
    switch (characterState)
    {
        case UP:
           step =  0;
           break;
        case UP_RIGTH:
           step =  1;
           break;
        case RIGTH:
           step =  2;
           break;
        case DOWN_RIGTH:
           step =  3;
           break;
        case DOWN:
           step =  4;
           break;
        case DOWN_LEFT:
           step =  5;
           break;
        case LEFT:
           step =  6;
           break;
         case UP_LEFT:
           step =  7;
           break;
    }


    return step;
}
