#ifndef FT_SDL_H
#define FT_SDL_H
#include "ft_sprite.h"
void ft_getCharactSprite(Player *player,State state,int step);
int ft_getFirePosition(State characterState);
int ft_drawPlayers();
void ft_freeAllSurfaces();
void ft_drawGame();
void SDL_init();
void ft_GetPlayerOrientation(Player *player);
#endif // FT_SDL_H
