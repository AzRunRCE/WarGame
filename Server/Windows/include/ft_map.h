#ifndef FT_MAP_H
#define FT_MAP_H
#include <SDL.h>
#include "ft_item.h"
typedef struct Map
{
	Item data[50][50];
	int heigth;
	int width;
}Map;

Uint32 obtenirPixel(SDL_Surface *surface, int x, int y);
void ft_LoadMap(char * path, Map *map);
static Map map;

#endif /* guard */
