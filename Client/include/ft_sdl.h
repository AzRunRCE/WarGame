#ifndef FT_SDL_H
#define FT_SDL_H
#include "include/ft_bullet.h"
#include "include/ft_item.h"
#include "include/ft_player.h"
#include "include/ft_sprite.h"
#include "include/ft_menu.h"
#include "include/ft_sound.h"
#include "include/ft_engine.h"
#include "include/ft_delay.h"


int ft_drawPlayers();
// void ft_freeAllSurfaces();
void ft_drawGame();
void SDL_init();
void ft_getCharactSprite(Player *player);

#endif // FT_SDL_H
