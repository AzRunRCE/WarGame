#ifndef MAIN_H
#define MAIN_H
#include "../include/unionproto.pb.h"
#include "ft_bullet.h"
#include "ft_map.h"
#define true 1
#define false 0

BulletElm* headBullets;
void FireBullet();
void ft_getHealthSprite(Player *player);
void ft_getAmmoSprite(Player *player);
bool ft_delay(int *lastAnim, int  SleepTimeAnim);
#endif // MAIN_H
