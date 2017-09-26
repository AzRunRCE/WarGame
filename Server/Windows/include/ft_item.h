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
}Item;
#endif // FT_ITEM_H
