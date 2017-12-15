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

typedef struct _SDL
{
	TTF_Font *font;
	SDL_Surface *pseudoSurface;
	SDL_Texture *texturePseudoSurface;
	SDL_Rect posPseudo;
} SDL;

SDL sdl;
int ft_SDL_DrawPlayers(void);
void ft_SDL_DrawGame(void);
void ft_SDL_init(void);
void ft_SDL_Close(void);
void ft_getCharactSprite(Player *player);

#endif // FT_SDL_H
