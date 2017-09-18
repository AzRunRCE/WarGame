#ifndef FT_MAP_H
#define FT_MAP_H
#include <SDL.h>

typedef struct
{
	char data[50][50];
	int heigth;
	int width;
}Map;

Uint32 obtenirPixel(SDL_Surface *surface, int x, int y);
Map *ft_LoadMap(char * path, Map *map);
#endif /* guard */