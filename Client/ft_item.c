#include <stdlib.h>
#include "include/ft_engine.h"
#define BLOCK_SIZE 32

int countItemWall(Item* head)
{
	int total = 0;
	if (head == NULL) {
		return 0;
	}
	/* go to the last node */
	Item *cursor = head;
	while (cursor != NULL)
	{
		total++;
		if (cursor->next == NULL)
			break;
		cursor = cursor->next;
	}
	return total;
}

int browseItemWall(Item* head)
{
	if (head == NULL) {
		return 0;
	}
	/* go to the last node */
	Item *cursor = head;
	while (cursor != NULL)
	{
		SDL_Rect rect = *cursor->rect;
		rect.x -= _engine.camera.x;
		rect.y -= _engine.camera.y;
		SDL_RenderCopy(_engine.screenRenderer, _engine.redSurface, cursor->rect, &rect);
		if (cursor->next == NULL)
			break;
		cursor = cursor->next;
	}
	return 1;
}

Item* pushItem(Item* head, Item *new_node)
{
	if (head == NULL) {
		head = new_node;
		return head;
	}
	/* go to the last node */
	Item *cursor = head;
	while (cursor->next != NULL)
		cursor = cursor->next;
	cursor->next = new_node;
	return head;
}

int checkWallColision(Item *head, int32_t direction)
{

	if (head == NULL) {
		return -1;
	}
	SDL_Rect playerBaseposTEMP;
	playerBaseposTEMP = _engine.mainPlayer.playerBase.pos;
	switch (direction)
	{
	case UP:
		playerBaseposTEMP.y--;
		break;
	case DOWN:
		playerBaseposTEMP.y++;
		break;
	case LEFT:
		playerBaseposTEMP.x--;
		break;
	case RIGHT:
		playerBaseposTEMP.x++;
		break;
	case -1:
		break;
	default:
		return -2; /*Ne devrait pas arriver*/
	}

	/* go to the last node */
	Item *cursor = head;
	while (cursor != NULL)
	{
		if (SDL_HasIntersection(cursor->rect, &playerBaseposTEMP))
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