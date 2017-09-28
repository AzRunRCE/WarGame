#ifndef FT_BULLET_H
#define FT_BULLET_H
#include <SDL.h>

typedef struct _BulletElm
{
	SDL_Rect pos;
	struct _BulletElm *next;
//	struct BulletElm *previous;
}BulletElm;

typedef void(*callback)(BulletElm* bullet);
void browserBullets(BulletElm* head, callback f);
void drawBullet(BulletElm *bullet);
#endif // FT_BULLET_H
