#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "include\ft_item.h"
#include "include\ft_engine.h"
#include "include\ft_sdl.h"
#include "include\ft_player.h"
#include "include\ft_sprite.h"

void ft_GetPlayerOrientation(Player *player)
{
	State state = player->state;
	int step = player->step;
	if (player->fire == true)
	{
		step = ft_getFirePosition(state);
		if (AnimDelay(player))
			state = 9;
		else
			state = 0;
	}
	else
	{
		if (player->walk &&  player->step < 4 && AnimDelay(player))
			player->step++;
		if (player->walk &&  player->step > 3)
			player->step = 0;
	}
	ft_getCharactSprite(player, state, step);
}
int SDL_Update()
{
	SDL_RenderClear(_engine.screenRenderer);
	SDL_RenderCopy(_engine.screenRenderer, _engine.mapSurface, &_engine.camera, NULL);
	SDL_RenderCopy(_engine.screenRenderer, _engine.characterSurface, &_engine.mainPlayer.sprite, &_engine.pCenter);
	//            SDL_RenderCopy(_engine.screenRenderer,  _engine.characterEnnemiSurface , &_engine.enemiPlayer.sprite, &_engine.enemiPlayer.Pos);
	SDL_RenderCopy(_engine.screenRenderer, _engine.fogSurface, NULL, NULL);
	//
	SDL_RenderPresent(_engine.screenRenderer);
}
int SDL_init()
{
	if (SDL_Init(SDL_INIT_VIDEO) == -1)
	{
		fprintf(stderr, "Erreur d'initialisation de SDL_Init : %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	if (TTF_Init() == -1)
	{
		fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
		exit(EXIT_FAILURE);
	}
}

int ft_getFirePosition(State characterState)
{
	int step;
	switch (characterState)
	{
	case UP:
		step = 0;
		break;
	case UP_RIGHT:
		step = 1;
		break;
	case RIGHT:
		step = 2;
		break;
	case DOWN_RIGHT:
		step = 3;
		break;
	case DOWN:
		step = 4;
		break;
	case DOWN_LEFT:
		step = 5;
		break;
	case LEFT:
		step = 6;
		break;
	case UP_LEFT:
		step = 7;
		break;
	}


	return step;
}
