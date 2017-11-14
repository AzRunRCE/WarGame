#ifndef FT_MAP_H
#define FT_MAP_H
#include <SDL.h>
#include <stdbool.h>
#include "ft_item.h"


typedef struct _map
{
	Item data[50][50];
	int heigth;
	int width;
}Map;

typedef struct _newMap
{
	SDL_Rect block;
	struct New_Map *next;
}New_Map;

Uint32 obtenirPixel(SDL_Surface *surface, int x, int y);
Item* ft_LoadMap(char * path, Map *map);
#endif /* guard */