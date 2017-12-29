#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "include/main.h"

#define MAX_LENGTH 32
#define FIRE_DELAY 200
#define LAST_UPDATE 12
#define BLOCK_SIZE 32

Engine _engine;
static SDL_Event event;

const Uint8 *keystate;
configuration *mainConfiguration;
bool GetKeyPressEvent(void);
void weapon_AutoReload(void);
void ft_checkPlayerAlive(void);
static uint32_t lastFire = 0;
static uint32_t fireDelay = FIRE_DELAY;
static uint32_t lastUpdate = 0;

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
	SDL_PollEvent(&event);
	if (event.type == SDL_QUIT)
		return false;
	SDL_GetMouseState(&_engine.mousePos.x, &_engine.mousePos.y);
	GetKeyPressEvent();
	if (_engine.mainPlayer.playerBase.state != DEAD) {
		if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
			FireBullet(true);
		else
			FireBullet(false);
	}
	return true;
}

int main(int argc, char *argv[])
{	
	headBullets = NULL;
	keystate = SDL_GetKeyboardState(NULL);
	mainConfiguration = ft_loadConf();
	ft_SDL_init();
	Engine_init();
	ft_nearWall_Init();
	ft_chat_Init();
	menu(mainConfiguration, 0);
	if (!network_CreateConnection(mainConfiguration))
		perror("Create_connection()");
	if (NwkThreadRet < 0)
		menu(mainConfiguration, NwkThreadRet);
	if (mainConfiguration->sound)
		sound_Init(mainConfiguration->music);
	_engine.AnimKillEx.Pos.h = 56;
	_engine.AnimKillEx.Pos.w = 56;
	_engine.AnimKillEx.Step = 0;

	_engine.camera.x = _engine.mainPlayer.playerBase.pos.x - _engine.WIDTH / 2 + 16;
	_engine.camera.y = _engine.mainPlayer.playerBase.pos.y - _engine.HEIGHT / 2 + 16;
	headBullets = NULL;
	
	while (ft_checkEvent())
	{
#ifndef _DEBUG
		if (!checkServerisAlive(mainConfiguration))
				exit(EXIT_FAILURE);
#endif
		_engine.mainPlayer.relativePos.x = _engine.mainPlayer.playerBase.pos.x - _engine.camera.x;
		_engine.mainPlayer.relativePos.y = _engine.mainPlayer.playerBase.pos.y - _engine.camera.y;
		ft_nearWall_Check();
		if (ft_delay(&lastUpdate, LAST_UPDATE)) {
			if (!keystate[SDL_SCANCODE_UP] && !keystate[SDL_SCANCODE_DOWN] && !keystate[SDL_SCANCODE_LEFT] && !keystate[SDL_SCANCODE_RIGHT] && !keystate[SDL_SCANCODE_W] && !keystate[SDL_SCANCODE_S] && !keystate[SDL_SCANCODE_A] && !keystate[SDL_SCANCODE_D])
				checkPlayerPosition();
			ft_ViewGetDegrees(_engine.mousePos.y - _engine.mainPlayer.relativePos.y, _engine.mousePos.x - _engine.mainPlayer.relativePos.x); // Fonction de calcul de degrées de la vue "torche". Les deux paramètres sont des calculs pour mettre l'image de la torche au milieu du joueur.
			ft_getHealthSprite(&_engine.mainPlayer);
			ft_getAmmoSprite(&_engine.mainPlayer);
			
			/*uint32_t i = 0;
			FMOD_Channel_GetPosition(soundChannelMusic, &i, FMOD_TIMEUNIT_MS);
			printf("Music position: %d\n", i)*/
			sound_Grunt_Poll();
		}
		ft_SDL_DrawGame();
		weapon_AutoReload();
		ft_checkPlayerAlive();
	}
	socket_Close();
	network_Clean();
	sound_Close();
	ft_chat_Close();
	ft_SDL_Close();
	return EXIT_SUCCESS;

}

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

bool GetKeyPressEvent(void)
{
	if (_engine.mainPlayer.playerBase.state != DEAD)
	{
		_engine.mainPlayer.playerBase.state = IDLE;
		if ((keystate[SDL_SCANCODE_LEFT] || keystate[SDL_SCANCODE_A])
			&& !checkWallColision(headItemList2, LEFT)
			)
		{
			_engine.mainPlayer.playerBase.pos.x -= 2;
			if (nWallMode == NONE || nWallMode == UPDOWN)
				_engine.camera.x -= 2;
			_engine.mainPlayer.playerBase.orientation = LEFT;
			_engine.mainPlayer.playerBase.state = WALK;

		}
		else if ((keystate[SDL_SCANCODE_RIGHT] || keystate[SDL_SCANCODE_D])
			&& !checkWallColision(headItemList2, RIGHT)
			)
		{
			_engine.mainPlayer.playerBase.pos.x += 2;
			if (nWallMode == NONE || nWallMode == UPDOWN)
				_engine.camera.x += 2;
			_engine.mainPlayer.playerBase.orientation = RIGHT;
			_engine.mainPlayer.playerBase.state = WALK;
		}
		if ((keystate[SDL_SCANCODE_UP] || keystate[SDL_SCANCODE_W])
			&& !checkWallColision(headItemList2, UP)
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
			&& !checkWallColision(headItemList2, DOWN)
			)
		{
			if (_engine.mainPlayer.playerBase.orientation == LEFT)
			{
				_engine.mainPlayer.playerBase.orientation = DOWN_LEFT;
				_engine.mainPlayer.playerBase.pos.y++;
				if (nWallMode == NONE || nWallMode == LEFTRIGHT)
					_engine.camera.y++;
			}
			else if (_engine.mainPlayer.playerBase.orientation == RIGHT)
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
#ifdef _DEBUG
		else if (_engine.mainPlayer.playerBase.ammo < 30 && keystate[SDL_SCANCODE_R])
			_engine.mainPlayer.playerBase.ammo = 30;
#endif
		else if (keystate[SDL_SCANCODE_LALT] && keystate[SDL_SCANCODE_RETURN] && ft_delay(&_engine.cooldownFullscreen, 500))
		{
			if (!_engine.fullscreen)
				_engine.fullscreen = true;
			else
				_engine.fullscreen = false;
			SDL_SetWindowFullscreen(_engine.window, _engine.fullscreen);
		}
		else if (keystate[SDL_SCANCODE_H])
			ft_chat_History_Show();
		else if (keystate[SDL_SCANCODE_J])
			ft_chat_History_Hide();
	}
	else if (_engine.mainPlayer.playerBase.state = DEAD)
	{
		if (!_engine.cooldownDeath && keystate[SDL_SCANCODE_SPACE])
			/* When spawn cooldown is done and space pressed */
		{
			_engine.mainPlayer.playerBase.ammo = 30;
			_engine.cooldownDeath = 10;
			SpawnMessage spMessage;
			uint8_t buffer[SpawnMessage_size];
			spMessage.id = _engine.mainPlayer.playerBase.id;
			pb_ostream_t output = pb_ostream_from_buffer(buffer, sizeof(buffer));
			if (!encode_unionmessage(&output, SpawnMessage_fields, &spMessage))
				fprintf(stderr, "Encoding failed: %s\n", PB_GET_ERROR(&output));
			write_client(buffer, output.bytes_written);
		}
	}
	return true;
}




void FireBullet(bool MouseButtonLeft)
{
	if (_engine.mainPlayer.playerBase.ammo > 10 && _engine.mainPlayer.playerBase.ammo < 20)
	{
		fireDelay = FIRE_DELAY + 75 - _engine.mainPlayer.playerBase.ammo * 3;
	}
	else if (_engine.mainPlayer.playerBase.ammo > 3 && _engine.mainPlayer.playerBase.ammo <= 10)
	{
		fireDelay = FIRE_DELAY + 200 - _engine.mainPlayer.playerBase.ammo * 3;
	}
	else if (fireDelay != FIRE_DELAY && _engine.mainPlayer.playerBase.ammo >= 20)
		fireDelay = FIRE_DELAY;

	if (_engine.mainPlayer.playerBase.ammo > 1 && MouseButtonLeft && ft_delay(&lastFire, fireDelay)) {
		_engine.mainPlayer.playerBase.ammo -= 2;
		sound_Play_Fire(&soundChannelMainPlayer);

		_engine.mainPlayer.playerBase.state = FIRE;
		uint8_t buffer[MAX_BUFFER];

		BulletMessage bulletMessage;
		bulletMessage.pos.x = _engine.mainPlayer.playerBase.pos.x + 8;
		bulletMessage.pos.y = _engine.mainPlayer.playerBase.pos.y + 8;

		bulletMessage.pos.h = bulletMessage.pos.w = 6;
		bulletMessage.dest = _engine.mousePos;
		bulletMessage.dest.x += _engine.camera.x;
		bulletMessage.dest.y += _engine.camera.y;

		bulletMessage.ownerId = _engine.mainPlayer.playerBase.id;
		pb_ostream_t output = pb_ostream_from_buffer(buffer, sizeof(buffer));
		if (!encode_unionmessage(&output, BulletMessage_fields, &bulletMessage))
			fprintf(stderr, "Encoding failed: %s\n", PB_GET_ERROR(&output));
		buffer[output.bytes_written] = '\0';
		write_client(buffer, output.bytes_written);
	}
	if (ft_delay(&lastFire, fireDelay))
	{
		if (_engine.mainPlayer.playerBase.ammo < 3 && _engine.mainPlayer.playerBase.ammo > 0)
			_engine.mainPlayer.playerBase.ammo = 0;
		else if (_engine.mainPlayer.playerBase.ammo == 0)
			_engine.mainPlayer.playerBase.ammo = 2;
	}
	
		

}

uint32_t lastReload;

void weapon_AutoReload(void)
{
	if ((event.type != SDL_MOUSEBUTTONDOWN || event.button.button != SDL_BUTTON_LEFT)
		&& _engine.mainPlayer.playerBase.ammo < 30
		&& ft_delay(&lastReload, (uint32_t)fireDelay * 2)
		)
		_engine.mainPlayer.playerBase.ammo += 2;
}


void ft_checkPlayerAlive(void)
{
	if (_engine.mainPlayer.playerBase.health < 1)
		_engine.mainPlayer.playerBase.state = DEAD;
	else
		_engine.mainPlayer.playerBase.state = IDLE;

}