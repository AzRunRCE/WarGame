#ifndef FT_PACKET_H
#define FT_PACKET_H

#include "ft_state.h"
#include "server.h"
#include "ft_player.h"
#include <SDL.h>


typedef struct
{
	int Y;
	int X;
	State state;
	char name[256];
	bool walk;
	bool fire;
	int clientNum;
}ClientPacket;

typedef struct
{
	Player players[16];
	SDL_Rect bullets[250];
}ServerPacket;

#endif /* guard */
