#ifndef FT_CHECKCOLLISION_H
#define FT_CHECKCOLLISION_H
#define true 1
#define false 0
#include <SDL.h>
#include "ft_item.h"
#include "client.h"
#include "ft_state.h"

int checkCollision(SDL_Rect *bullet, SDL_Rect *player);
int checkCollisionWall(Item *head, BulletElm* bullet);
int checkCollisionPlayer(BulletElm* bullet, int playerCount);

#endif // FT_CHECKCOLLISION_H