#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "include/ft_item.h"
#include "include/ft_engine.h"
#include "include/ft_sdl.h"
#include "include/ft_player.h"
#include "include/ft_socket.h"
#include "include/ft_sprite.h"
#include "include/ft_socket.h"
#include "include/ft_engine.h"
#include "include/ft_menu.h"
#include "include/ft_point.h"
#include "include/ft_bullet.h"
#include "include/ft_configuration.h"
#include "include/ft_explode.h"
#include "include/ft_view.h"
#include "include/pb.h"
#include "include/pb_common.h"
#include "include/pb_encode.h"
#include "include/pb_decode.h"
#include "include/unionproto.pb.h"
#include "include/pb_functions.h"
#include "include/ft_sound.h"
#include "include/ft_nearwall.h"

#define MAX_LENGTH 32
#define FIRE_DELAY 150
#define BLOCK_SIZE 32

Engine _engine;
void FireBullet(bool MouseButtonLeft);
SDL_Color colorWhite = { 255, 255, 255 };
SDL_Surface *text = NULL;
SDL_Surface *fontSurface = NULL;
char message[20];
time_t lastTime = 0, lastTimeAnim = 0;
SDL_Rect p = { .x = 200,.y = 200,.w = 4,.h = 4 };

const Uint8 *keystate;
int actual = 0;
configuration *mainConfiguration;
Explode explode;
int GetKeyPressEvent();
int lastFire = 0;
New_Map newMapLeft;

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



bool ft_checkEvent()
{
	SDL_Event event;
	SDL_PollEvent(&event);
	if (event.type == SDL_QUIT)
		return false;
	SDL_GetMouseState(&_engine.mousePos.x, &_engine.mousePos.y);
	if (_engine.mainPlayer.playerBase.state != DEAD) {
		if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && ft_delay(&lastFire, FIRE_DELAY))
			FireBullet(true);
		else
			FireBullet(false);
	}
	GetKeyPressEvent();
	return true;
}

//int main(int argc, char *argv[])
int main()
{	
	keystate = SDL_GetKeyboardState(NULL);
	mainConfiguration = ft_loadConf();
	SDL_init();
	Engine_init();
	nearWallInit();
	newMapLeft.block.x = 0;
	newMapLeft.block.y = 0;
	newMapLeft.block.w = _engine.WIDTH / 2 - 16;
	newMapLeft.block.h = 1600;
	newMapLeft.next = NULL;
	nearWallInit();
	sound_Init();
	sound_Load("res/fire.wav");
	fontSurface = SDL_GetWindowSurface(_engine.window);
	do
	{
		menu(mainConfiguration);
	} while (!create_connection(mainConfiguration));

	ft_LoadMap("map/first.bmp", _engine.map);
	_engine.AnimKillEx.Pos.h = 56;
	_engine.AnimKillEx.Pos.w = 56;
	_engine.AnimKillEx.Step = 0;

	_engine.camera.x = _engine.mainPlayer.playerBase.pos.x - _engine.WIDTH / 2 + 16;
	_engine.camera.y = _engine.mainPlayer.playerBase.pos.y - _engine.HEIGHT / 2 + 16;
	headBullets = NULL;

	while (ft_checkEvent())
	{
		_engine.mainPlayer.RelativePos.x = _engine.mainPlayer.playerBase.pos.x - _engine.camera.x;
		_engine.mainPlayer.RelativePos.y = _engine.mainPlayer.playerBase.pos.y - _engine.camera.y;
		checkNearWall();

		ft_ViewGetDegrees(_engine.mousePos.y - _engine.mainPlayer.RelativePos.y, _engine.mousePos.x - _engine.mainPlayer.RelativePos.x); // Fonction de calcul de degrées de la vue "torche". Les deux paramètres sont des calculs pour mettre l'image de la torche au milieu du joueur.
		ft_getHealthSprite(&_engine.mainPlayer);
		ft_getAmmoSprite(&_engine.mainPlayer);
		//ft_getNextExplodeSprite(&explode);	
		ft_drawGame();
	}

	end();
	nearWalldelete();
	sound_Close();
	return EXIT_SUCCESS;

}

/*void ft_getNextDyingSprite(Explode *explode)
{
	
}*/
void ft_getHealthSprite(Player *player)
{
	int nb_life = player->playerBase.health / 10;
	_engine.healthRect.x = 0;
	_engine.healthRect.y = 50 * nb_life;
	_engine.healthRect.h = 50;
	_engine.healthRect.w = 300;

}
void ft_getAmmoSprite(Player *player)
{
	_engine.AmmoRect.x = 0;
	_engine.AmmoRect.y = 100 * player->playerBase.ammo;
	_engine.AmmoRect.h = 100;
	_engine.AmmoRect.w = 404;

}

bool ft_delay(int *lastAnim, int  SleepTimeAnim)
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

int a;

int GetKeyPressEvent()
{
	if (_engine.mainPlayer.playerBase.health > 0)
	{
		_engine.mainPlayer.playerBase.state = IDLE;
		int playerPosX = _engine.mainPlayer.playerBase.pos.x + 16;
		int playerPosY = _engine.mainPlayer.playerBase.pos.y + 16;

		if ((keystate[SDL_SCANCODE_LEFT] || keystate[SDL_SCANCODE_A])
			&& _engine.map->data[(int)playerPosY / BLOCK_SIZE][(int)(playerPosX - 8) / BLOCK_SIZE]
			)
		{
			_engine.mainPlayer.playerBase.pos.x -= 2;
			if (nWallMode == NONE || nWallMode == UPDOWN)
				_engine.camera.x -= 2;
			_engine.mainPlayer.playerBase.orientation = LEFT;
			_engine.mainPlayer.playerBase.state = WALK;

		}
		else if ((keystate[SDL_SCANCODE_RIGHT] || keystate[SDL_SCANCODE_D])
			&& _engine.map->data[(int)playerPosY / BLOCK_SIZE][(int)(playerPosX + 8) / BLOCK_SIZE]
			)
		{
			_engine.mainPlayer.playerBase.pos.x += 2;
			if (nWallMode == NONE || nWallMode == UPDOWN)
				_engine.camera.x += 2;
			_engine.mainPlayer.playerBase.orientation = RIGHT;
			_engine.mainPlayer.playerBase.state = WALK;
		}
		if ((keystate[SDL_SCANCODE_UP] || keystate[SDL_SCANCODE_W])
			&& _engine.map->data[(int)(playerPosY - 8) / BLOCK_SIZE][(int)playerPosX / BLOCK_SIZE]
			)
		{
			if (_engine.mainPlayer.playerBase.orientation == LEFT)
			{
				_engine.mainPlayer.playerBase.orientation = UP_LEFT;
				_engine.mainPlayer.playerBase.pos.y--;
				if (nWallMode == NONE || nWallMode == LEFTRIGHT)
					_engine.camera.y--;
			}

			else if (_engine.mainPlayer.playerBase.orientation == RIGHT)
			{
				_engine.mainPlayer.playerBase.orientation = UP_RIGHT;
				_engine.mainPlayer.playerBase.pos.y--;
				if (nWallMode == NONE || nWallMode == LEFTRIGHT)
					_engine.camera.y--;
			}
			else
			{
				_engine.mainPlayer.playerBase.orientation = UP;
				_engine.mainPlayer.playerBase.pos.y -= 2;
				if (nWallMode == NONE || nWallMode == LEFTRIGHT)
					_engine.camera.y -= 2;
			}

			_engine.mainPlayer.playerBase.state = WALK;
		}
		else if ((keystate[SDL_SCANCODE_DOWN] || keystate[SDL_SCANCODE_S])
			&& _engine.map->data[(int)(playerPosY + 8) / BLOCK_SIZE][(int)playerPosX / BLOCK_SIZE]
			)
		{
			if (_engine.mainPlayer.playerBase.orientation == LEFT)
			{
				_engine.mainPlayer.playerBase.orientation = DOWN_LEFT;
				_engine.mainPlayer.playerBase.pos.y++;
				if (nWallMode == NONE || nWallMode == LEFTRIGHT)
					_engine.camera.y++;
			}
			else  if (_engine.mainPlayer.playerBase.orientation == RIGHT)
			{
				_engine.mainPlayer.playerBase.orientation = DOWN_RIGHT;
				_engine.mainPlayer.playerBase.pos.y++;
				if (nWallMode == NONE || nWallMode == LEFTRIGHT)
					_engine.camera.y++;
			}
			else
			{
				_engine.mainPlayer.playerBase.orientation = DOWN;
				_engine.mainPlayer.playerBase.pos.y += 2;
				if (nWallMode == NONE || nWallMode == LEFTRIGHT)
					_engine.camera.y += 2;
			}
			_engine.mainPlayer.playerBase.state = WALK;
		}
		else if (_engine.mainPlayer.playerBase.ammo < 30 && keystate[SDL_SCANCODE_R])
			_engine.mainPlayer.playerBase.ammo = 30;
	}
	else
	{
		_engine.mainPlayer.playerBase.state = DEAD;
		if (keystate[SDL_SCANCODE_SPACE])
		{
			_engine.mainPlayer.playerBase.ammo = 30;
			SpawnMessage spMessage;
			uint8_t buffer[SpawnMessage_size];
			spMessage.id = _engine.mainPlayer.playerBase.id;
			pb_ostream_t output = pb_ostream_from_buffer(buffer, sizeof(buffer));
			if (encode_unionmessage(&output, SpawnMessage_fields, &spMessage))
				write_client(buffer, output.bytes_written);
		}
	}
	return 1;
}




void FireBullet(bool MouseButtonLeft)
{
	if (_engine.mainPlayer.playerBase.ammo > 3 && MouseButtonLeft) {
		_engine.mainPlayer.playerBase.ammo -= 3;
		sound_Play(soundChannelMainPlayer);

		_engine.mainPlayer.playerBase.state = FIRE;
		uint8_t buffer[BulletMessage_size];

		BulletMessage bulletMessage;
		bulletMessage.pos.x = _engine.mainPlayer.playerBase.pos.x + 8;
		bulletMessage.pos.y = _engine.mainPlayer.playerBase.pos.y + 8;
		bulletMessage.pos.w = 6;
		bulletMessage.pos.h = 6;

		bulletMessage.pos.h = bulletMessage.pos.w = 6;
		bulletMessage.dest = _engine.mousePos;
		bulletMessage.dest.x += _engine.camera.x;
		bulletMessage.dest.y += _engine.camera.y;

		bulletMessage.ownerId = _engine.mainPlayer.playerBase.id;
		pb_ostream_t output = pb_ostream_from_buffer(buffer, sizeof(buffer));
		encode_unionmessage(&output, BulletMessage_fields, &bulletMessage);
		sendMessage(buffer, output.bytes_written);
	}
	else if (_engine.mainPlayer.playerBase.ammo == 3 && ft_delay(&lastFire, FIRE_DELAY))
		_engine.mainPlayer.playerBase.ammo = 0;
	else if (_engine.mainPlayer.playerBase.ammo == 0 && ft_delay(&lastFire, FIRE_DELAY))
		_engine.mainPlayer.playerBase.ammo = 3;
}
