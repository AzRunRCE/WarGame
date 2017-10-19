#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "include/ft_item.h"
#include "include/ft_engine.h"
#include "include/ft_sdl.h"
#include "include/ft_player.h"
#include "include/ft_sprite.h"
#include "include/ft_menu.h"
#include "include/ft_sound.h"



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


int ft_drawPlayers()
{
	for (int i = 0; i < _engine.playersCount; i++)
	{

		_engine.players[i].sprite.h = _engine.players[i].sprite.w = 32;
		if (_engine.players[i].playerBase.id == -1 || _engine.players[i].playerBase.id == _engine.mainPlayer.playerBase.id)
			continue;

		_engine.players[i].playerBase.pos.w = 32;
		_engine.players[i].playerBase.pos.h = 32;
		SDL_Rect rect = _engine.players[i].playerBase.pos;
		rect.x = _engine.players[i].playerBase.pos.x - _engine.camera.x;
		rect.y = _engine.players[i].playerBase.pos.y - _engine.camera.y;
		ft_getCharactSprite(&_engine.players[i]);
		if (_engine.players[i].playerBase.health > 0)
		{
			
			SDL_RenderCopy(_engine.screenRenderer, _engine.characterEnnemiSurface, &_engine.players[i].sprite, &rect);
		}
		else
		{
			if (_engine.players[i].deathAnimationStep != 10)
			{
				if (ft_delay(&_engine.players[i].lastAnim, 100))
				{
					_engine.players[i].sprite.x = 56 * (_engine.players[i].deathAnimationStep % 5) + (_engine.players[i].deathAnimationStep % 5 + 1);
					_engine.players[i].sprite.y = 41 * (_engine.players[i].deathAnimationStep / 5) + (_engine.players[i].deathAnimationStep / 5 + 1);	
					_engine.players[i].deathAnimationStep += 1;
				}
				_engine.players[i].sprite.h = 41;
				_engine.players[i].sprite.w = 56;
				rect.x -= 12;
				rect.y -= 6;
				rect.h = 41;
				rect.w = 56;
				SDL_RenderCopy(_engine.screenRenderer, _engine.AnimKill, &_engine.players[i].sprite, &rect);
			}
		
		}
		if (_engine.players[i].playerBase.state == FIRE)
			sound_Play(soundChannelEnemies);
	}
	return 1;
}


void ft_drawGame()
{
	SDL_RenderClear(_engine.screenRenderer);
	SDL_RenderCopy(_engine.screenRenderer, _engine.mapSurface, &_engine.camera, NULL);
	
	
	if (_engine.mainPlayer.playerBase.health > 0)
	{
		ft_getCharactSprite(&_engine.mainPlayer);
		SDL_RenderCopy(_engine.screenRenderer, _engine.characterSurface, &_engine.mainPlayer.sprite, &_engine.mainPlayer.RelativePos);
		_engine.mainPlayer.deathAnimationStep = 0;
	}
	else
	{

		if (_engine.mainPlayer.deathAnimationStep != 10)
		{
			if (ft_delay(&_engine.mainPlayer.lastAnim, 100))
			{
				_engine.mainPlayer.sprite.x = 56 * (_engine.mainPlayer.deathAnimationStep % 5) + (_engine.mainPlayer.deathAnimationStep % 5 + 1);
				_engine.mainPlayer.sprite.y = 41 * (_engine.mainPlayer.deathAnimationStep / 5) + (_engine.mainPlayer.deathAnimationStep / 5 + 1);
				_engine.mainPlayer.sprite.h = 41;
				_engine.mainPlayer.sprite.w = 56;
				_engine.mainPlayer.deathAnimationStep += 1;
			}
			SDL_Rect rect = _engine.mainPlayer.RelativePos;
			rect.x -= 12;
			rect.y -= 6;
			rect.h = 41;
			rect.w = 56;
			SDL_RenderCopy(_engine.screenRenderer, _engine.AnimKill, &_engine.mainPlayer.sprite, &rect);
		}
		else
			menuDeath();
	}
		
	
	ft_drawPlayers();
	browserBullets(headBullets, &drawBullet);
	if (_engine.mainPlayer.playerBase.state != DEAD)
		SDL_RenderCopyEx(_engine.screenRenderer, _engine.viewSurface, NULL, &_engine.viewRect, _engine.viewDegrees, NULL, SDL_FLIP_NONE);

	SDL_RenderCopy(_engine.screenRenderer, _engine.AmmoSurface, &_engine.AmmoRect, &_engine.ammoPos);
	SDL_RenderCopy(_engine.screenRenderer, _engine.healthSurface, &_engine.healthRect, &_engine.healthPos);
	SDL_RenderPresent(_engine.screenRenderer);
	//ft_freeAllSurfaces();
}

void ft_getCharactSprite(Player *player)
{
		player->sprite.h = player->sprite.w = 32;
		switch (player->playerBase.state)
		{
		case IDLE:
			if (ft_delay(&player->lastAnim, 200))
			{
				player->sprite.y = 298;
				player->sprite.x = 32 * player->playerBase.orientation + player->playerBase.orientation + 1;
			}
			break;
		case FIRE:
			player->sprite.y = 265;
			player->sprite.x = 32 * player->playerBase.orientation + player->playerBase.orientation + 1;
			break;
		case WALK:
			if (ft_delay(&player->lastAnim, 100))
			{
				player->sprite.x = 32 * player->walkAnimationStep + player->walkAnimationStep + 1;
				player->sprite.y = 32 * player->playerBase.orientation + player->playerBase.orientation + 1;
				if (player->walkAnimationStep < 3)
					player->walkAnimationStep++;
				else
					player->walkAnimationStep = 0;
			}
			break;
		default:
			break;
		}
}