#ifndef FT_ITEM_H
#define FT_ITEM_H
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
#endif // FT_ITEM_H
