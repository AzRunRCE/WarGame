#ifndef FT_CHECKCOLLISION_H
#define FT_CHECKCOLLISION_H
#define true 1
#define false 0
#include <SDL.h>
#include <stdbool.h>
#include "ft_item.h"
#include "ft_state.h"
#include "ft_bullet.h"

bool checkCollision(SDL_Rect *bullet, SDL_Rect *player);
int checkCollisionWall(Item *head, BulletElm* bullet);
bool checkCollisionPlayer(BulletElm* bullet, int playerCount);

#endif // FT_CHECKCOLLISION_H