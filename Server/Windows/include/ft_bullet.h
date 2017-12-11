#ifndef FT_BULLET_H
#define FT_BULLET_H

#include <SDL.h>

typedef struct _BulletElm
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
#endif /* FT_BULLET_H */
