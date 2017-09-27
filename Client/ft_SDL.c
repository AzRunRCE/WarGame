#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "include\ft_item.h"
#include "include\ft_engine.h"
#include "include\ft_SDL.h"
#include "include\ft_player.h"
#include "include\ft_sprite.h"
#include "include/ft_menu.h"

bool Delay(int *lastAnim, int  SleepTimeAnim)
{
	int ActualTimeAnim = SDL_GetTicks();
	if (ActualTimeAnim - *lastAnim > SleepTimeAnim)
	{
		*lastAnim = ActualTimeAnim;
		return true;
	}
	else
	{
		return false;
	}

}
void ft_GetPlayerOrientation(Player *player)
{
	State state = player->state;
	int step = player->step;
	if (player->fire == true)
	{
		step = ft_getFirePosition(state);
		if (Delay(&player->lastAnim, 200))
			state = 9;
		else
			state = 0;
	}
	else
	{
		if (player->walk &&  player->step < 4 && Delay(&player->lastAnim, 200))
			player->step++;
		if (player->walk &&  player->step > 3)
			player->step = 0;
	}
	ft_getCharactSprite(player, state, step);
}

void SDL_init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
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

int ft_drawPlayers()
{
	for (int i = 0; i < _engine.playersCount; i++)
	{

		if (_engine.players[i].id == -1 || _engine.players[i].id == _engine.mainPlayer.id)
			continue;

		_engine.players[i].Pos.w = 32;
		_engine.players[i].Pos.h = 32;
		SDL_Rect rect = _engine.players[i].Pos;
		rect.x = _engine.players[i].Pos.x - _engine.camera.x;
		rect.y = _engine.players[i].Pos.y - _engine.camera.y;
		ft_GetPlayerOrientation(&_engine.players[i]);
		SDL_RenderCopy(_engine.screenRenderer, _engine.characterEnnemiSurface, &_engine.players[i].sprite, &rect);
	}
	return 1;
}

void ft_freeAllSurfaces()
{
	SDL_FreeSurface(_engine.mapSurface);
	SDL_FreeSurface(_engine.characterSurface);
	SDL_FreeSurface(_engine.explodeSurface);
	SDL_FreeSurface(_engine.characterEnnemiSurface);
	SDL_FreeSurface(_engine.bulletSurface);
	SDL_FreeSurface(_engine.viewSurface);
}
void ft_drawGame()
{
	SDL_RenderClear(_engine.screenRenderer);
	SDL_RenderCopy(_engine.screenRenderer, _engine.mapSurface, &_engine.camera, NULL);
	
	if (_engine.mainPlayer.health > 0)
	{
		_engine.AnimKillEx.Step = 0;
		SDL_RenderCopy(_engine.screenRenderer, _engine.characterSurface, &_engine.mainPlayer.sprite, &_engine.pCenter);
	}
	else
	{
		SDL_Rect rect = _engine.pCenter;
		rect.x -= 12;
		rect.y -= 6;
		rect.h = 41;
		rect.w = 56;
		SDL_RenderCopy(_engine.screenRenderer, _engine.AnimKill, &_engine.AnimKillEx.Sprite, &rect);
		ft_getNextDyingSprite(&_engine.AnimKillEx);
		menuDeath();
	}
		
	
	ft_drawPlayers();
	browserBullets(headBullets, &drawBullet);
	SDL_RenderCopyEx(_engine.screenRenderer, _engine.viewSurface, NULL, &_engine.viewRect, _engine.viewDegrees, NULL, SDL_FLIP_NONE);

	SDL_RenderCopy(_engine.screenRenderer, _engine.AmmoSurface, &_engine.AmmoRect, &_engine.ammoPos);
	SDL_RenderCopy(_engine.screenRenderer, _engine.healthSurface, &_engine.healthRect, &_engine.healthPos);
	SDL_RenderPresent(_engine.screenRenderer);
	ft_freeAllSurfaces();
}