#ifndef FT_SDL_H
#define FT_SDL_H
#include "ft_sprite.h"
void ft_getCharactSprite(Player *player,State state,int step);
int ft_getFirePosition(State characterState);
int SDL_init();
int SDL_Update();
#endif // FT_SDL_H
