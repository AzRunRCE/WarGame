#include "include/ft_checkcollision.h"

int checkCollision(SDL_Rect *bullet, SDL_Rect *player)
{
	player->w = 32;

	SDL_bool test = SDL_HasIntersection(bullet, player);
	if (test == SDL_TRUE)
		return true;
	else
		return false;
}