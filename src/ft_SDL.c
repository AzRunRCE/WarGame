#include "sprite.h"
#include <SDL2/SDL.h>
#include "Item.h"
#include "engine.h"
#include "ft_SDL.h"
#include "player.h"
#include "sprite.h"

void ft_GetPlayerOrientation(Player *player)
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
    ft_getCharactSprite(state,step);
}

int ft_getFirePosition(State characterState)
{
    int step;
    switch (characterState)
    {
        case UP:
           step =  0;
           break;
        case UP_RIGHT:
           step =  1;
           break;
        case RIGHT:
           step =  2;
           break;
        case DOWN_RIGHT:
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
