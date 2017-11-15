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

int checkCollisionWall(Item *head, BulletElm* bullet)
/* Function to check collisons between walls and bullet */
{

	if (head == NULL) {
		return -1;
	}
	/* go to the last node */
	Item *cursor = head;
	while (cursor != NULL)
	{
		if (SDL_HasIntersection(cursor->rect, &bullet->pos))
		{
			return 1;
			break;
		}
		if (cursor->next == NULL)
		{
			return 0;
			break;
		}
		cursor = cursor->next;
	}
}

int checkCollisionPlayer(BulletElm* bullet, int playerCount)
{
	SDL_Rect currentPlayerPos;
	if (playerCount < 2)
		return 0;
	for (int i = 0; i < playerCount; i++)
	{
		currentPlayerPos = Players[i].playerBase.pos;
		currentPlayerPos.w = 20;
		currentPlayerPos.h = 24;
		if (SDL_HasIntersection(&currentPlayerPos, &bullet->pos) && Players[i].playerBase.id != bullet->ownerId && Players[i].playerBase.state != DEAD)
		{
			Players[i].playerBase.health -= 10;
			return 1;
			break;
		}
	}
	return 0;
}