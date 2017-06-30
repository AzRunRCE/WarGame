#ifndef PLAYER_H
#define PLAYER_H
#include "ft_state.h"
#include "include\server.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

typedef struct Player {
	char name[16];
	int health;
	int ammo;
	int step;
	bool fire;
	State state;
	bool walk;
	int lastAnim;
	int fireIdle;
	SDL_Rect Pos;
	SDL_Rect sprite;
	SDL_Rect bullets[30];
}Player;


#endif // PLAYER_H
