#ifndef MAIN_H
#define MAIN_H
#include "../include/unionproto.pb.h"
#include "ft_bullet.h"
#include "ft_map.h"
#include "ft_explode.h"
#define true 1
#define false 0

BulletElm* headBullets;
void FireBullet();
void ft_getHealthSprite(Player *player);
void ft_getAmmoSprite(Player *player);
bool ft_delay(int *lastAnim, int  SleepTimeAnim);
void ft_getNextDyingSprite(Explode *explode);
#endif // MAIN_H
