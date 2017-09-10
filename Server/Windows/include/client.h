#ifndef CLIENT_H
#define CLIENT_H

#include "server.h"
#include <SDL.h>
#include "..\unionproto.pb.h"
typedef struct
{
    SOCKADDR_IN sin;
	int id;
}Client;

typedef struct
{
	SDL_Rect dest;
	SDL_Rect pos;
	int ownerId;
	int id;
	struct BulletElm *next;
	struct BulletElm *previous;
}BulletElm;

#endif /* guard */
