#ifndef FT_MAP_H
#define FT_MAP_H
#include <SDL.h>

typedef struct
{
	char data[50][50];
	int heigth;
	int width;
}Map;

typedef struct
{
	SDL_Rect block;
	struct New_Map *next;
}New_Map;

Uint32 obtenirPixel(SDL_Surface *surface, int x, int y);
void ft_LoadMap(char * path, Map *map);
#endif /* guard */