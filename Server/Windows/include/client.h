#ifndef CLIENT_H
#define CLIENT_H

#include "server.h"
#include <SDL.h>
#include "../unionproto.pb.h"
typedef struct
{
    SOCKADDR_IN sin;
	int id;
}Client;

typedef struct
{
	SDL_Rect dest;
	SDL_Rect pos;
	int dY;
	int dX;
	int x0;
	int x1;
	int y0;
	int y1;
	int sX;
	int sY;
	int err;
	int e2;
	int ownerId;
	int id;
	struct BulletElm *next;
}BulletElm;

#endif /* guard */
