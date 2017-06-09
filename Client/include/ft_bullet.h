#ifndef FT_BULLET_H
#define FT_BULLET_H
#include <SDL.h>

typedef struct
{
    int dY;
    int dX;
	int x0;
	int x1;
	int y0;
	int y1;
	int sX;
	int sY;
    SDL_Rect Pos;
	int err;
	int e2;
}Bullet;


Bullet *initBullet(int x0, int y0, int x1, int y1);
void drawBullet(Bullet *bullet);
#endif // FT_BULLET_H
