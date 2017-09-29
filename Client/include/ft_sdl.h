#ifndef FT_SDL_H
#define FT_SDL_H
#include "ft_sprite.h"

int ft_drawPlayers();
void ft_freeAllSurfaces();
void ft_drawGame();
void SDL_init();
void ft_getCharactSprite(Player *player);

#endif // FT_SDL_H
