#ifndef MAIN_H
#define MAIN_H
#include <stdbool.h>
#include "unionproto.pb.h"
#include "ft_item.h"
#include "ft_sdl.h"
#include "ft_player.h"
#include "ft_socket.h"
#include "ft_sprite.h"
#include "ft_engine.h"
#include "ft_menu.h"
#include "ft_point.h"
#include "ft_bullet.h"
#include "ft_configuration.h"
#include "ft_explode.h"
#include "ft_view.h"
#include "pb.h"
#include "pb_common.h"
#include "pb_encode.h"
#include "pb_decode.h"
#include "unionproto.pb.h"
#include "pb_functions.h"
#include "ft_sound.h"
#include "ft_nearwall.h"
#include "ft_playerPosition.h"
#include "ft_delay.h"

#define true 1
#define false 0

void FireBullet(bool MouseButtonLeft);
void ft_getHealthSprite(Player *player);
void ft_getAmmoSprite(Player *player);
//void ft_getNextDyingSprite(Explode *explode);
#endif // MAIN_H
