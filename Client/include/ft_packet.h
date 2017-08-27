#ifndef PACKET_H
#define PACKET_H

#include "ft_player.h"
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


#endif // PACKET_H
