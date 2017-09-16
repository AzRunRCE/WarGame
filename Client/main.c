#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "include\ft_item.h"
#include "include\ft_engine.h"
#include "include\ft_SDL.h"
#include "include\ft_player.h"
#include "include\ft_socket.h"
#include "include\ft_sprite.h"
#include "include\ft_socket.h"
#include "include\ft_engine.h"
#include "include\ft_menu.h"
#include "include\ft_point.h"
#include "include\ft_bullet.h"
#include "include\ft_configuration.h"
#include "include\ft_explode.h"
#include "include/ft_View.h"
#include "include\pb.h"
#include "include\pb_common.h"
#include "include\pb_encode.h"
#include "include\pb_decode.h"
#include "include\unionproto.pb.h"
#include "include\pb_functions.h"
#define MAX_LENGTH 32
#define FIRE_DELAY 200
#define BLOCK_SIZE 32

Engine _engine;
void FireBullet();
SDL_Color colorWhite = { 255, 255, 255 };
SDL_Surface *text = NULL;
SDL_Surface *fontSurface = NULL;
char message[20];
time_t lastTime = 0, lastTimeAnim = 0;
SDL_Rect p = { .x = 200,.y = 200,.w = 4,.h = 4 };


SDL_Event *event;
int actual = 0;
configuration *mainConfiguration;
Explode explode;
void ft_getHealthSprite(Player *player);
void ft_getAmmoSprite(Player *player);
bool ft_delay(int *lastAnim, int  SleepTimeAnim); 
bool ft_getNextExplodeSprite(Explode *explode)
{
	if (explode->Step == 52)
		return false;
	else if (ft_delay(&explode->lastAnim, 50))
	{
		explode->Sprite.x = 256 * (explode->Step % 8);
		explode->Sprite.y = 256 * (explode->Step / 8);
		explode->Sprite.h = 256;
		explode->Sprite.w = 256;
		explode->Step = explode->Step + 1;
		return true;
	}
	return false;
}


int main(int argc, char *argv[])
{
	event = malloc(sizeof(SDL_Event));
	mainConfiguration = ft_loadConf();
	printf("Version: %d\nNickname: %s\nServer: %s\n", mainConfiguration->version, mainConfiguration->nickname, mainConfiguration->server);
	SDL_init();
	Engine_init();
	fontSurface = SDL_GetWindowSurface(_engine.window);
	menu(mainConfiguration);
	create_connection(mainConfiguration);
	ft_LoadMap("map/first.bmp", _engine.map);
	SDL_Rect posText;
	SDL_Texture *texture;
	explode.Pos.h = 255;
	explode.Pos.w = 255;
	explode.Step = 0;

	while (GetKeyPressEvent())
	{
		ft_getNextExplodeSprite(&explode);
		_engine.PlayerRealPos.x = (_engine.mainPlayer.Pos.x + 16);
		_engine.PlayerRealPos.y = (_engine.mainPlayer.Pos.y + 16);
		if (_engine.mainPlayer.Pos.x <= _engine.WIDTH / 2 - 16 || _engine.mainPlayer.Pos.y <= _engine.HEIGHT / 2 - 16 || _engine.mainPlayer.Pos.x + _engine.WIDTH / 2 + 16 >= _engine.mapSurface->h || _engine.mainPlayer.Pos.y + _engine.HEIGHT / 2 + 16 >= _engine.mapSurface->h)
		{
			_engine.PlayerRealPos.x = (_engine.pCenter.x + _engine.mainPlayer.Pos.x - _engine.WIDTH / 2 + 32);
			_engine.PlayerRealPos.y = (_engine.pCenter.y + _engine.mainPlayer.Pos.y - _engine.HEIGHT / 2 + 32);
		}
		// If the character is near the wall, show the character position compared to the screen instead of camera position
		if (_engine.mainPlayer.Pos.x <= _engine.WIDTH / 2 - 16 || _engine.mainPlayer.Pos.y <= _engine.HEIGHT / 2 - 16 || _engine.mainPlayer.Pos.x + _engine.WIDTH / 2 + 16 >= _engine.mapSurface->h || _engine.mainPlayer.Pos.y + _engine.HEIGHT / 2 + 16 >= _engine.mapSurface->h)
			sprintf(message, "%d,%d %d %d %d %c", _engine.pCenter.x + _engine.mainPlayer.Pos.x - _engine.WIDTH / 2 + 16, _engine.pCenter.y + _engine.mainPlayer.Pos.y - _engine.HEIGHT / 2 + 16, actual, _engine.PlayerRealPos.x / BLOCK_SIZE, _engine.PlayerRealPos.y / BLOCK_SIZE, _engine.map->data[(int)_engine.PlayerRealPos.y / BLOCK_SIZE][(int)_engine.PlayerRealPos.x / BLOCK_SIZE]);
		else
			sprintf(message, "%d,%d %d %d %d %c", _engine.mainPlayer.Pos.x, _engine.mainPlayer.Pos.y, actual, _engine.PlayerRealPos.x / BLOCK_SIZE, _engine.PlayerRealPos.y / BLOCK_SIZE, _engine.map->data[(int)_engine.PlayerRealPos.y / BLOCK_SIZE][(int)_engine.PlayerRealPos.x / BLOCK_SIZE]);
		text = TTF_RenderText_Blended(_engine.font, message, colorWhite);
		posText = (SDL_Rect) { 0, 0, text->w, text->h };
//		texture = SDL_CreateTextureFromSurface(_engine.screenRenderer, text);
		_engine.camera.x = _engine.mainPlayer.Pos.x - _engine.WIDTH / 2 + 16;
		_engine.camera.y = _engine.mainPlayer.Pos.y - _engine.HEIGHT / 2 + 16;
		ft_GetPlayerOrientation(&_engine.mainPlayer);
		SDL_RenderClear(_engine.screenRenderer);
		SDL_RenderCopy(_engine.screenRenderer, _engine.mapSurface, &_engine.camera, NULL);
		SDL_RenderCopy(_engine.screenRenderer, _engine.characterSurface, &_engine.mainPlayer.sprite, &_engine.pCenter);
		
		SDL_GetMouseState(&_engine.mousePos.x, &_engine.mousePos.y);
		ft_ViewGetDegrees(_engine.mousePos.y - _engine.pCenter.y, _engine.mousePos.x - _engine.pCenter.x); // Fonction de calcul de degrées de la vue "torche". Les deux paramètres sont des calculs pour mettre l'image de la torche au milieu du joueur.
		
		//	SDL_RenderCopy(_engine.screenRenderer, _engine.explodeSurface, &explode.Sprite, &explode.Pos);
		browserBullets(headBullets, &drawBullet);
		int i;
		for (i = 0; i < _engine.playersCount; i++)
		{

			if (_engine.players[i].id == -1 || _engine.players[i].id == _engine.mainPlayer.id)
				continue;

			_engine.players[i].Pos.w = 32;
			_engine.players[i].Pos.h = 32;

			ft_GetPlayerOrientation(&_engine.players[i]);
			SDL_RenderCopy(_engine.screenRenderer, _engine.characterEnnemiSurface, &_engine.players[i].sprite, &_engine.players[i].Pos);
			//printf("%d %d\n", _engine.players[i].Pos.x, _engine.players[i].Pos.y);
			SDL_RenderCopy(_engine.screenRenderer, _engine.characterEnnemiSurface, &_engine.players[i].sprite, &_engine.players[i].Pos);
		}
	
		//SDL_RenderCopyEx(_engine.screenRenderer, _engine.viewSurface, NULL, &_engine.viewRect, _engine.viewDegrees, NULL, SDL_FLIP_NONE);
		ft_getHealthSprite(&_engine.mainPlayer);
		ft_getAmmoSprite(&_engine.mainPlayer);
		SDL_RenderCopy(_engine.screenRenderer, _engine.healthSurface, &_engine.healthRect, &_engine.healthPos);
		SDL_RenderCopy(_engine.screenRenderer, _engine.AmmoSurface, &_engine.AmmoRect, &_engine.ammoPos);
	//	SDL_RenderCopy(_engine.screenRenderer, texture, NULL, &posText);
		SDL_RenderPresent(_engine.screenRenderer);
	//	SDL_DestroyTexture(texture);
		SDL_FreeSurface(text);
		SDL_FreeSurface(_engine.mapSurface);
		SDL_FreeSurface(_engine.characterSurface);
		SDL_FreeSurface(_engine.explodeSurface);
		SDL_FreeSurface(_engine.characterEnnemiSurface);
		SDL_FreeSurface(_engine.bulletSurface);
		SDL_FreeSurface(_engine.viewSurface);
		
	}

	end();
	free(event);

	return EXIT_SUCCESS;

}


// KEY CODE https://wiki.libsdl.org/SDL_ScancodeAndKeycode?highlight=%28SDL_SCANCODE%29

void ft_getCharactSprite(Player *player, State state, int step)
{
	player->sprite.x = 32 * step + (step + 1);
	player->sprite.y = 32 * state + (state + 1);
	player->sprite.h = 32;
	player->sprite.w = 32;
}

void ft_getHealthSprite(Player *player)
{
	int nb_life = player->health / 10;
	_engine.healthRect.x = 0;
	_engine.healthRect.y = 50 * nb_life;
	_engine.healthRect.h = 50;
	_engine.healthRect.w = 300;

}
void ft_getAmmoSprite(Player *player)
{
	_engine.AmmoRect.x = 0;
	_engine.AmmoRect.y = 100 * player->ammo;
	_engine.AmmoRect.h = 100;
	_engine.AmmoRect.w = 404;

}

bool ft_delay(int *lastAnim, int  SleepTimeAnim)
{
	int ActualTimeAnim = SDL_GetTicks();
	int a = *lastAnim;
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





SDL_Texture* SurfaceToTexture(SDL_Surface* surf)
{
	SDL_Texture* text;

	text = SDL_CreateTextureFromSurface(_engine.screenRenderer, surf);

	SDL_FreeSurface(surf);

	return text;
}

int GetKeyPressEvent()
{
	
	Uint8 *keystate = SDL_GetKeyboardState(NULL);
	
	if (SDL_PollEvent(event))
	{
		if (event->type == SDL_QUIT)
			return 0;
	}
	_engine.mainPlayer.fire = false;
	_engine.mainPlayer.walk = false;

	int posX = _engine.mainPlayer.Pos.x + 16;
	int posY = _engine.mainPlayer.Pos.y + 16;
	if (_engine.mainPlayer.Pos.x <= _engine.WIDTH / 2 - 16 || _engine.mainPlayer.Pos.y <= _engine.HEIGHT / 2 - 16 || _engine.mainPlayer.Pos.x + _engine.WIDTH / 2 + 16 >= _engine.mapSurface->h || _engine.mainPlayer.Pos.y + _engine.HEIGHT / 2 + 16 >= _engine.mapSurface->h)
	{
		// Décalage car la map 50x50 commence (tableau char) a 0 et la position a 1
		posX = _engine.pCenter.x + _engine.mainPlayer.Pos.x - _engine.WIDTH / 2 + 32;
		posY = _engine.pCenter.y + _engine.mainPlayer.Pos.y - _engine.HEIGHT / 2 + 32;
	}
	
	if (keystate[SDL_SCANCODE_LEFT] && _engine.pCenter.x + _engine.mainPlayer.Pos.x - _engine.WIDTH / 2 + 16 > 0 
		&& _engine.map->data[(int)posY/BLOCK_SIZE][(int)(posX - 8)/BLOCK_SIZE]
		)
	{
		if (_engine.mainPlayer.Pos.x <= _engine.WIDTH / 2 - 16 || _engine.pCenter.x + _engine.mainPlayer.Pos.x + 32 > _engine.mapSurface->h)
			_engine.pCenter.x -= 2;
		else
			_engine.mainPlayer.Pos.x -= 2;
		_engine.mainPlayer.state = LEFT;
		_engine.mainPlayer.walk = true;
		
	}
	else if (keystate[SDL_SCANCODE_RIGHT] && _engine.pCenter.x + _engine.mainPlayer.Pos.x - _engine.WIDTH / 2 + 32 < _engine.mapSurface->h 
		&& _engine.map->data[(int)posY/BLOCK_SIZE][(int)(posX + 8)/BLOCK_SIZE]
		)
	{
	 
		if (_engine.pCenter.x < _engine.WIDTH / 2 - 16 || _engine.mainPlayer.Pos.x + _engine.WIDTH / 2 + 16 >= _engine.mapSurface->h)
			_engine.pCenter.x += 2;
		else
			_engine.mainPlayer.Pos.x += 2;
		_engine.mainPlayer.state = RIGHT;
		_engine.mainPlayer.walk = true;
	}
	if (keystate[SDL_SCANCODE_UP] && _engine.pCenter.y + _engine.mainPlayer.Pos.y - _engine.HEIGHT / 2 + 16 > 0
		&& _engine.map->data[(int)(posY - 8)/BLOCK_SIZE][(int)posX/BLOCK_SIZE]
		)
	{
		if (_engine.mainPlayer.state == LEFT)
		{
			_engine.mainPlayer.state = UP_LEFT;
			if (_engine.mainPlayer.Pos.y <= _engine.HEIGHT / 2 - 16 || _engine.pCenter.y + _engine.mainPlayer.Pos.y + 32 >= _engine.mapSurface->h )
				_engine.pCenter.y--;
			else
				_engine.mainPlayer.Pos.y--;
		}

		else if (_engine.mainPlayer.state == RIGHT)
		{
			_engine.mainPlayer.state = UP_RIGHT;
			if (_engine.mainPlayer.Pos.y <= _engine.HEIGHT / 2 - 16 || _engine.pCenter.y + _engine.mainPlayer.Pos.y + 32 >= _engine.mapSurface->h)
				_engine.pCenter.y--;
			else 
				_engine.mainPlayer.Pos.y--;
		}
		else
		{
			_engine.mainPlayer.state = UP;
			if (_engine.mainPlayer.Pos.y <= _engine.HEIGHT / 2 - 16 || _engine.pCenter.y + _engine.mainPlayer.Pos.y + 32 >= _engine.mapSurface->h) 
				_engine.pCenter.y-=2;
			else 
				_engine.mainPlayer.Pos.y -=2;
		}

		_engine.mainPlayer.walk = true;
	}
	else if (keystate[SDL_SCANCODE_DOWN] && _engine.pCenter.y + _engine.mainPlayer.Pos.y - _engine.HEIGHT / 2 + 32 < _engine.mapSurface->h 
		&& _engine.map->data[(int)(posY + 8)/BLOCK_SIZE][(int)posX/BLOCK_SIZE]
		)
	{
		if (_engine.mainPlayer.state == LEFT)
		{
			_engine.mainPlayer.state = DOWN_LEFT;
			if (_engine.pCenter.y < _engine.HEIGHT / 2 - 16 || _engine.mainPlayer.Pos.y + _engine.HEIGHT / 2 + 16 >= _engine.mapSurface->h)
				_engine.pCenter.y++;
			else
				_engine.mainPlayer.Pos.y++;
		}
		else  if (_engine.mainPlayer.state == RIGHT)
		{
			_engine.mainPlayer.state = DOWN_RIGHT;
			if (_engine.pCenter.y < _engine.HEIGHT / 2 - 16 || _engine.mainPlayer.Pos.y + _engine.HEIGHT / 2 + 16 >= _engine.mapSurface->h)
				_engine.pCenter.y++;
			else
				_engine.mainPlayer.Pos.y++;
		}
		else
		{
			_engine.mainPlayer.state = DOWN;
			if (_engine.pCenter.y < _engine.HEIGHT / 2 - 16 || _engine.mainPlayer.Pos.y + _engine.HEIGHT / 2 + 16 >= _engine.mapSurface->h)
				_engine.pCenter.y +=2;
			else
				_engine.mainPlayer.Pos.y+=2;
		}
		_engine.mainPlayer.walk = true;
	}
	if (SDL_GetMouseState(NULL, NULL) && SDL_BUTTON(SDL_BUTTON_LEFT) && ft_delay(&_engine.mainPlayer.fireIdle, FIRE_DELAY))
	{
	
		FireBullet();
	}
	return 1;
}




void FireBullet()
{

	if (_engine.mainPlayer.ammo > 0)
		_engine.mainPlayer.ammo -= 1;
	else
	{
		_engine.mainPlayer.ammo = 30;
	}
	SDL_GetMouseState(&_engine.mousePos.x, &_engine.mousePos.y);
	_engine.mainPlayer.fire = true;
	uint8_t buffer[BulletMessage_size];

	BulletMessage bulletMessage;
	bulletMessage.pos.x = _engine.PlayerRealPos.x;
	bulletMessage.pos.y = _engine.PlayerRealPos.y;
	bulletMessage.pos.w = 6;
	bulletMessage.pos.h = 6;

	bulletMessage.pos.h = bulletMessage.pos.w = 6;
	bulletMessage.dest = _engine.mousePos;
	bulletMessage.dest.x += _engine.camera.x;
	bulletMessage.dest.y += _engine.camera.y;

	bulletMessage.ownerId = _engine.mainPlayer.id;
	pb_ostream_t output = pb_ostream_from_buffer(buffer, sizeof(buffer));
	bool status = encode_unionmessage(&output, BulletMessage_fields, &bulletMessage);
	int c = sendMessage(buffer, output.bytes_written);
	
}

