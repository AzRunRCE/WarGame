#ifndef FT_SDL_H
#define FT_SDL_H
#include "ft_bullet.h"
#include "ft_item.h"
#include "ft_player.h"
#include "ft_sprite.h"
#include "ft_menu.h"
#include "ft_sound.h"
#include "ft_engine.h"
#include "ft_delay.h"
#include "ft_chat.h"


int ft_drawPlayers();
// void ft_freeAllSurfaces();
void ft_drawGame();
void SDL_init();
void ft_getCharactSprite(Player *player);

#endif // FT_SDL_H
