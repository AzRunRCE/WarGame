
#ifndef FT_MAP_H
#define FT_MAP_H

typedef struct
{
	int **data;
	int heigth;
	int width;
}Map;

Map *ft_LoadMap(char * path, Map *map);

#endif /* guard */