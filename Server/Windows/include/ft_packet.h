#ifndef FT_PACKET_H
#define FT_PACKET_H

#include "ft_state.h"
#include "server.h"
#include "ft_player.h"
#include <SDL.h>
#include "ft_map.h"

typedef struct
{
	int clientId;
	Map map;
}ServerGame;

typedef struct
{
	Player clientPlayer;
}ClientPacket;


typedef struct
{
	Player players[16];
	SDL_Rect bullets[250];
}ServerPacket;

#endif /* guard */
