#include "include/ft_map.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL.h>
#define BLOCK_SIZE 32
#define BLACK 255
#define WHITE 0





/*obtenirPixel : permet de récupérer la couleur d'un pixel*/
Uint32 obtenirPixel(SDL_Surface *surface, int x, int y)
{
	int nbOctetsParPixel = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * nbOctetsParPixel;
	switch (nbOctetsParPixel)
	{
	case 1:
		return *p;

	case 2:
		return *(Uint16 *)p;

	case 3:
		/*Suivant l'architecture de la machine*/
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			return p[0] << 16 | p[1] << 8 | p[2];
		else
			return p[0] | p[1] << 8 | p[2] << 16;

	case 4:
		return *(Uint32 *)p;

		/*Ne devrait pas arriver, mais évite les erreurs*/
	default:
		return 0;
	}
}


void ft_LoadMap(char * path, Map *map)
{
	SDL_Surface *mapBMP = SDL_LoadBMP(path);
	map->heigth = mapBMP->h;

	map->width = mapBMP->w;

	int rows, columns;
	/* initialize rows and columns to the desired value */

	/*map->data = (int**)malloc(mapBMP->h * sizeof(int*));
	for (int i = 0;i<mapBMP->h;i++)
	{
	map->data[i] = (int*)malloc(mapBMP->w * sizeof(int));
	}*/
	//
	//int i = 0; i < mapBMP->h; i++)
	for (int j = 0; j < mapBMP->w; j++)
	{
		for (int i = 0; i < mapBMP->h; i++)
		{
			if ((int)obtenirPixel(mapBMP, j, i) == BLACK)
			{
				map->data[i][j] = 0;
			}
			else
				map->data[i][j] = 1;

		}
	}
}

