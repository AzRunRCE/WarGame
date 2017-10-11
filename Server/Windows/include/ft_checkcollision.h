#ifndef FT_CHECKCOLLISION_H
#define FT_CHECKCOLLISION_H
#define true 1
#define false 0

#include <SDL.h>

int checkCollision(SDL_Rect *bullet, SDL_Rect *player);

#endif // FT_CHECKCOLLISION_H