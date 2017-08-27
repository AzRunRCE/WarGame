#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "ft_sprite.h"
#include "main.h"

typedef struct Player {
	char name[16];
	bool calibred;
	int health;
	int ammo;
	int step;
	bool fire;
	State state;
	bool walk;
	int lastAnim;
	int fireIdle;
	int id;
	SDL_Rect Pos;
	SDL_Rect sprite;
}Player;

#endif // PLAYER_H
