#ifndef FT_ITEM_H
#define FT_ITEM_H
#include <SDL.h>
typedef enum ItemType
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

Item* pushItem(Item* head, Item *new_node);
int countItemWall(Item* head);
#endif // FT_ITEM_H
