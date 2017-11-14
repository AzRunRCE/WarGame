#include "include/ft_map.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define BLOCK_SIZE 32
#define BLACK 255
#define WHITE 0

Item *headItemList = NULL;


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


int ft_LoadMap(char * path, Map *map)
{
	SDL_Surface *mapBMP = NULL;
	mapBMP = SDL_LoadBMP(path);
	if (!mapBMP)
	{
		printf("Erreur de chargement de l'image : %s", SDL_GetError());
		return -1;
	}
	map->heigth = mapBMP->h;

	map->width = mapBMP->w;


	for (int j = 0; j < mapBMP->w; j++)
	{
		for (int i = 0; i < mapBMP->h; i++)
		{
			if ((int)obtenirPixel(mapBMP, j, i) == BLACK)
			{
				map->data[i][j].type = WALL;
			}
			else
				map->data[i][j].type = BLANK;
			
		}
	}

	int i = 0;
	int j = 0;
	while (j < map->heigth)
	{
		while (i < map->width)
		{
			Item *item = malloc(sizeof(Item));
			item->rect = malloc(sizeof(SDL_Rect));
			while ((int)obtenirPixel(mapBMP, j, i) != BLACK && i < map->width)
				i++;
			item->next = NULL;
			item->type = WALL;
			item->rect->h = 0;
			item->rect->w = 32;
			item->rect->x = j * 32;
			item->rect->y = i * 32;
			bool findWall = false;
			while ((int)obtenirPixel(mapBMP, j, i) == BLACK  && i < map->width)
			{
				findWall = true;
				item->rect->h += 32;
				i++;
			}
			if (findWall)
				headItemList = pushItem(headItemList, item);
		}
		j++;
		i = 0;
	}
	printf("%d items\n", countItemWall(headItemList));
	return headItemList;
}

