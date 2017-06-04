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

Engine _engine;
void FireBullet();
SDL_Color colorWhite = { 255, 255, 255 };
SDL_Surface *text = NULL;
SDL_Surface *fontSurface = NULL;
char message[20];
time_t lastTime = 0, lastTimeAnim = 0;
int const SleepTime = 30;
int const SleepTimeAnim = 200;
SDL_Rect p = { .x = 200,.y = 200,.w = 4,.h = 4 };
int dX = 0;
int dY = 0;
Bullet bulletFired[300];
int actual = 0;
configuration *mainConfiguration;
Explode explode;
int main(int argc, char *argv[])
{
	mainConfiguration = ft_loadConf();
	printf("Version: %d\nNickname: %s\nServer: %s\n", mainConfiguration->version, mainConfiguration->nickname,mainConfiguration->server);
	SDL_init();
	Engine_init();
	fontSurface = SDL_GetWindowSurface(_engine.window);
	menu(mainConfiguration);
	create_connection(mainConfiguration);
	SDL_Rect posText;
	SDL_Texture *texture;
	explode.Pos.h = 7;
	explode.Pos.w = 7;
	explode.Step = 0;
	while (GetKeyPressEvent())
	{
	
		
		ft_getNextExplodeSprite(&explode);

		sprintf(message, "%d,%d %d", _engine.mainPlayer.Pos.x, _engine.mainPlayer.Pos.y, actual);
		
		
		
		text = TTF_RenderText_Blended(_engine.font, message, colorWhite);
		posText = (SDL_Rect) { 0, 0, text->w, text->h };
		texture = SDL_CreateTextureFromSurface(_engine.screenRenderer, text);
		
		
		_engine.camera.x = _engine.mainPlayer.Pos.x;
		_engine.camera.y = _engine.mainPlayer.Pos.y;
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
			SDL_RenderCopy(_engine.screenRenderer, _engine.characterEnnemiSurface, &_engine.players[i].sprite, &_engine.players[i].Pos);
		}
		int j = 0;
		for (j = 0; j < actual; j++)
		{
			if (bulletFired[j].Pos.x > 1000 || bulletFired[j].Pos.y > 1000)
				continue;
			bulletFired[j].Pos.x = bulletFired[j].Pos.x + bulletFired[j].dX;
			bulletFired[j].Pos.y = bulletFired[j].Pos.y + bulletFired[j].dY;
			SDL_RenderCopy(_engine.screenRenderer, _engine.bulletSurface, NULL, &bulletFired[j].Pos);
		}
		SDL_RenderCopy(_engine.screenRenderer, _engine.fogSurface, NULL, NULL);
		SDL_RenderCopy(_engine.screenRenderer, texture, NULL, &posText);
		SDL_RenderPresent(_engine.screenRenderer);
		SDL_DestroyTexture(texture);
		SDL_FreeSurface(text);
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
	explode->Sprite.x = 256 * (explode->Step % 8);
	explode->Sprite.y = 256 * (explode->Step / 8);
	explode->Sprite.h = 256;
	explode->Sprite.w = 256;
	explode->Step = explode->Step +1;
	return true;
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
	if (SDL_PollEvent(&_engine.event))//close the window
	{
		if (_engine.event.type == SDL_QUIT)
			return 0;
	}
	_engine.mainPlayer.fire = false;
	_engine.mainPlayer.walk = false;
	if (keystate[SDL_SCANCODE_LEFT])
	{
		_engine.mainPlayer.Pos.x -= 2;
		_engine.mainPlayer.state = LEFT;
		_engine.mainPlayer.walk = true;
	}
	else if (keystate[SDL_SCANCODE_RIGHT])
	{
		_engine.mainPlayer.Pos.x += 2;
		_engine.mainPlayer.state = RIGHT;
		_engine.mainPlayer.walk = true;
	}
	if (keystate[SDL_SCANCODE_UP])
	{
		if (_engine.mainPlayer.state == LEFT)
		{
			_engine.mainPlayer.state = UP_LEFT;
			_engine.mainPlayer.Pos.y--;
		}

		else if (_engine.mainPlayer.state == RIGHT)
		{
			_engine.mainPlayer.state = UP_RIGHT;
			_engine.mainPlayer.Pos.y--;
		}
		else
		{
			_engine.mainPlayer.state = UP;
			_engine.mainPlayer.Pos.y -= 2;
		}

		_engine.mainPlayer.walk = true;
	}
	else if (keystate[SDL_SCANCODE_DOWN])
	{
		if (_engine.mainPlayer.state == LEFT)
		{
			_engine.mainPlayer.state = DOWN_LEFT;
			_engine.mainPlayer.Pos.y++;
		}
		else  if (_engine.mainPlayer.state == RIGHT)
		{
			_engine.mainPlayer.state = DOWN_RIGHT;
			_engine.mainPlayer.Pos.y++;
		}
		else
		{
			_engine.mainPlayer.state = DOWN;
			_engine.mainPlayer.Pos.y += 2;
		}
		_engine.mainPlayer.walk = true;
	}
	if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
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
	bulletFired[actual].Pos = (SDL_Rect) { .x = _engine.pCenter.x + 16, .y = _engine.pCenter.y + 16, .w = 2, .h = 2 };
	int tmpX = 0;
	int tmpyY = 0;
	tmpX = (_engine.mousePos.x - bulletFired[actual].Pos.x);
	tmpyY = (_engine.mousePos.y - bulletFired[actual].Pos.y);
	int DI = sqrt(tmpX * tmpX + tmpyY * tmpyY);
	double T = DI / 6;
	bulletFired[actual].dX = (_engine.mousePos.x - bulletFired[actual].Pos.x) / T;
	bulletFired[actual].dY = (_engine.mousePos.y - bulletFired[actual].Pos.y) / T;
	actual++;
}

bool AnimDelay(Player *player)
{
	int ActualTimeAnim = SDL_GetTicks();
	if (ActualTimeAnim - player->lastAnim > SleepTimeAnim)
	{
		player->lastAnim = ActualTimeAnim;
		return true;
	}
	else
	{
		return false;
	}

}
int FrameDelay()
{
	time_t ActualTime = time(0); // Get the system time;
	if (ActualTime - lastTime > SleepTime)
	{
		lastTime = ActualTime;
		return 1;
	}
	else
		return 0;
}

