#include "include\ft_bullet.h"
#include "SDL.h"
#include "include/ft_engine.h"
#include "include/ft_bullet.h"



Bullet *initBullet(int x0,int y0,int x1, int y1)
{
	Bullet *bullet = malloc(sizeof(Bullet));
	bullet->x0 = x0;
	bullet->y0 = y0;
	bullet->x1 = x1;
	bullet->y1 = y1;
	bullet->dX = abs(x1 -x0), bullet->sX = x0 < x1 ? 1 : -1;
	bullet->dY = abs(y1 - y0), bullet->sY = y0< y1 ? 1 : -1;
	
	bullet->err = (bullet->dX>bullet->dY ? bullet->dX : -bullet->dY) / 2;

	
	bullet->Pos.h = 6;
	bullet->Pos.w = 6;
	
	return bullet;
}
void drawBullet(Bullet *bullet) {
	bullet->Pos.y = bullet->y0;
	bullet->Pos.x = bullet->x0;
	SDL_Rect RECT = { .x = 0,.y = 0,.h = 6,.w = 6 };
	//bullet->Pos.x -= _engine.camera.x;
	//bullet->Pos.y -= _engine.camera.y;

	SDL_RenderCopy(_engine.screenRenderer, _engine.bulletSurface, &RECT, &bullet->Pos);
	//if (bullet->x0 == bullet->x1 && bullet->y0 == bullet->y1)
	//	return;
	for (size_t i = 0; i < 4; i++)
	{
		bullet->e2 = bullet->err;
		if (bullet->e2 > -bullet->dX)
		{
			bullet->err -= bullet->dY;
			bullet->x0 += bullet->sX;
		}
		if (bullet->e2 < bullet->dY)
		{
			bullet->err += bullet->dX;
			bullet->y0 += bullet->sY;
		}
	}
	
	
}