#include "sprite.h"
#include <SDL/SDL.h>
#include "player.h"
void ft_getCharactSprite(Player *player)
{
    State state = player->state;
    int step = player->step;
    if (player->fire == true)
    {
        step = ft_getFirePosition(player->state);
        state = 5;
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
        case DOWN:
           step =  4;
           break;
        case LEFT:
           step =  6;
           break;
        case RIGTH:
           step =  2;
           break;
    }


    return step;
}
