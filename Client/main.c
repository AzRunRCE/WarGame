#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "include\ft_item.h"
#include "include\ft_engine.h"
#include "include\ft_SDL.h"
#include "include\ft_player.h"
#include "include\ft_sprite.h"
#include "include\ft_packet.h"
#include "include\ft_socket.h"
#include "include\ft_engine.h"
#include "include\ft_menu.h"
#include "include\ft_point.h"
#include "include\ft_bullet.h"
#include "include\ft_configuration.h"
#include "include\ft_explode.h"
#define MAX_LENGTH 32
#define FIRE_DELAY 200

Engine _engine;
void FireBullet();
SDL_Color colorWhite = { 255, 255, 255 };
SDL_Surface *text = NULL;
SDL_Surface *fontSurface = NULL;
char message[20];
time_t lastTime = 0, lastTimeAnim = 0;
SDL_Rect p = { .x = 200,.y = 200,.w = 4,.h = 4 };
int dX = 0;
int dY = 0;
Bullet *bulletFired[300];
int actual = 0;
configuration *mainConfiguration;
Explode explode;
int main(int argc, char *argv[])
{
	mainConfiguration = ft_loadConf();
	printf("Version: %d\nNickname: %s\nServer: %s\n", mainConfiguration->version, mainConfiguration->nickname, mainConfiguration->server);
	SDL_init();
	Engine_init();
	fontSurface = SDL_GetWindowSurface(_engine.window);
	menu(mainConfiguration);
	create_connection(mainConfiguration);
	SDL_Rect posText;
	SDL_Texture *texture;
	explode.Pos.h = 255;
	explode.Pos.w = 255;
	explode.Step = 0;
	FireBullet();
	while (GetKeyPressEvent())
	{
		ft_getNextExplodeSprite(&explode);

		// If the character is near the wall, show the character position compared to the screen instead of camera position
		if (_engine.mainPlayer.Pos.x <= _engine.WIDTH / 2 - 16 || _engine.mainPlayer.Pos.y <= _engine.HEIGHT / 2 - 16 || _engine.mainPlayer.Pos.x + _engine.WIDTH / 2 + 16 >= _engine.mapSurface->h || _engine.mainPlayer.Pos.y + _engine.HEIGHT / 2 + 16 >= _engine.mapSurface->h)
			sprintf(message, "%d,%d %d", _engine.pCenter.x + _engine.mainPlayer.Pos.x - _engine.WIDTH / 2 + 16, _engine.pCenter.y + _engine.mainPlayer.Pos.y - _engine.HEIGHT / 2 + 16, actual);
		else
			sprintf(message, "%d,%d %d", _engine.mainPlayer.Pos.x, _engine.mainPlayer.Pos.y, actual);
		text = TTF_RenderText_Blended(_engine.font, message, colorWhite);
		posText = (SDL_Rect) { 0, 0, text->w, text->h };
		texture = SDL_CreateTextureFromSurface(_engine.screenRenderer, text);
		_engine.camera.x = _engine.mainPlayer.Pos.x - _engine.WIDTH / 2 + 16;
		_engine.camera.y = _engine.mainPlayer.Pos.y - _engine.HEIGHT / 2 + 16;
		ft_GetPlayerOrientation(&_engine.mainPlayer);
		SDL_RenderClear(_engine.screenRenderer);
		SDL_RenderCopy(_engine.screenRenderer, _engine.mapSurface, &_engine.camera, NULL);
		SDL_RenderCopy(_engine.screenRenderer, _engine.characterSurface, &_engine.mainPlayer.sprite, &_engine.pCenter);
		SDL_GetMouseState(&explode.Pos.x, &explode.Pos.y);
		SDL_RenderCopy(_engine.screenRenderer, _engine.explodeSurface, &explode.Sprite, &explode.Pos);
		int i;
		for (i = 0; i < 15; i++)
		{

			if (_engine.players[i].Pos.x == 0 && _engine.players[i].Pos.y == 0)
				continue;
			ft_GetPlayerOrientation(&_engine.players[i]);
			_engine.players[i].Pos.x -= _engine.camera.x;
			_engine.players[i].Pos.y -= _engine.camera.y;
			SDL_RenderCopy(_engine.screenRenderer, _engine.characterEnnemiSurface, &_engine.players[i].sprite, &_engine.players[i].Pos);
		}
		int j = 0;
		for (j = 0; j < actual; j++)
		{
			drawBullet(bulletFired[j]);
		}
		SDL_RenderCopy(_engine.screenRenderer, _engine.fogSurface, NULL, NULL);
		SDL_RenderCopy(_engine.screenRenderer, texture, NULL, &posText);
		SDL_RenderPresent(_engine.screenRenderer);
		SDL_DestroyTexture(texture);
		SDL_FreeSurface(text);
		SDL_FreeSurface(_engine.mapSurface);
		SDL_FreeSurface(_engine.characterSurface);
		SDL_FreeSurface(_engine.explodeSurface);
		SDL_FreeSurface(_engine.characterEnnemiSurface);
		SDL_FreeSurface(_engine.bulletSurface);
		SDL_FreeSurface(_engine.fogSurface);
	}

	end();


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

bool ft_getNextExplodeSprite(Explode *explode)
{
	if (explode->Step == 52)
		return false;
	else if (Delay(&explode->lastAnim, 50))
	{
		explode->Sprite.x = 256 * (explode->Step % 8);
		explode->Sprite.y = 256 * (explode->Step / 8);
		explode->Sprite.h = 256;
		explode->Sprite.w = 256;
		explode->Step = explode->Step + 1;
		return true;
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
	if (SDL_PollEvent(&_engine.event))
	{
		if (_engine.event.type == SDL_QUIT)
			return 0;
	}
	_engine.mainPlayer.fire = false;
	_engine.mainPlayer.walk = false;
	if (keystate[SDL_SCANCODE_LEFT] && _engine.pCenter.x + _engine.mainPlayer.Pos.x - _engine.WIDTH / 2 + 16 > 0)
	{

		if (_engine.mainPlayer.Pos.x <= _engine.WIDTH / 2 - 16 || _engine.pCenter.x + _engine.mainPlayer.Pos.x + 32 > _engine.mapSurface->h)
			_engine.pCenter.x -= 2;
		else
			_engine.mainPlayer.Pos.x -= 2;
		_engine.mainPlayer.state = LEFT;
		_engine.mainPlayer.walk = true;
	}
	else if (keystate[SDL_SCANCODE_RIGHT] && _engine.pCenter.x + _engine.mainPlayer.Pos.x - _engine.WIDTH / 2 + 32 < _engine.mapSurface->h)
	{
		if (_engine.pCenter.x < _engine.WIDTH / 2 - 16 || _engine.mainPlayer.Pos.x + _engine.WIDTH / 2 + 16 >= _engine.mapSurface->h)
			_engine.pCenter.x += 2;
		else
			_engine.mainPlayer.Pos.x += 2;
		_engine.mainPlayer.state = RIGHT;
		_engine.mainPlayer.walk = true;
	}
	if (keystate[SDL_SCANCODE_UP] && _engine.pCenter.y + _engine.mainPlayer.Pos.y - _engine.HEIGHT / 2 + 16 > 0)
	{
		if (_engine.mainPlayer.state == LEFT)
		{
			_engine.mainPlayer.state = UP_LEFT;
			if (_engine.mainPlayer.Pos.y <= _engine.HEIGHT / 2 - 16 || _engine.pCenter.y + _engine.mainPlayer.Pos.y + 32 >= _engine.mapSurface->h)
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
				_engine.pCenter.y -= 2;
			else
				_engine.mainPlayer.Pos.y -= 2;
		}

		_engine.mainPlayer.walk = true;
	}
	else if (keystate[SDL_SCANCODE_DOWN] && _engine.pCenter.y + _engine.mainPlayer.Pos.y - _engine.HEIGHT / 2 + 32 < _engine.mapSurface->h)
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
				_engine.pCenter.y += 2;
			else
				_engine.mainPlayer.Pos.y += 2;
		}
		_engine.mainPlayer.walk = true;
	}
	if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT) && Delay(&_engine.mainPlayer.fireIdle, FIRE_DELAY))
	{
		FireBullet();
	}

	return 1;
}

void FireBullet()
{
	if (actual > 299)
		actual = 0;
	explode.Step = 0;
	SDL_GetMouseState(&_engine.mousePos.x, &_engine.mousePos.y);
	_engine.mainPlayer.fire = true;
	bulletFired[actual] = initBullet(_engine.pCenter.x + 16, _engine.pCenter.y + 16, 600, 600);
	actual++;
}

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
