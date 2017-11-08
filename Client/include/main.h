#ifndef MAIN_H
#define MAIN_H
#include <stdbool.h>
#include "../include/unionproto.pb.h"
#include "include/ft_item.h"
#include "include/ft_sdl.h"
#include "include/ft_player.h"
#include "include/ft_socket.h"
#include "include/ft_sprite.h"
#include "include/ft_engine.h"
#include "include/ft_menu.h"
#include "include/ft_point.h"
#include "include/ft_bullet.h"
#include "include/ft_configuration.h"
#include "include/ft_explode.h"
#include "include/ft_view.h"
#include "include/pb.h"
#include "include/pb_common.h"
#include "include/pb_encode.h"
#include "include/pb_decode.h"
#include "include/unionproto.pb.h"
#include "include/pb_functions.h"
#include "include/ft_sound.h"
#include "include/ft_nearwall.h"
#include "include/ft_playerPosition.h"
#include "include/ft_delay.h"

#define true 1
#define false 0

void FireBullet(bool MouseButtonLeft);
void ft_getHealthSprite(Player *player);
void ft_getAmmoSprite(Player *player);
//void ft_getNextDyingSprite(Explode *explode);
#endif // MAIN_H
