#ifndef FT_ITEM_H
#define FT_ITEM_H
#include <SDL.h>

typedef enum
{
	BLANK = 0,
	WALL = 1,
	PLAYER = 2,
} ItemType;

typedef struct Item {
	ItemType type;
	void *data;
	SDL_Rect *rect;
	struct Item *next;
}Item;

int countItemWall(Item* head);
Item* pushItem(Item* head, Item *new_node);
int checkWallColision(Item *head, int32_t direction);
int browseItemWall(Item* head);
#endif // FT_ITEM_H
