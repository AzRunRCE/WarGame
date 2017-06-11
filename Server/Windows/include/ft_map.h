
#ifndef FT_MAP_H
#define FT_MAP_H

typedef struct
{
	char data[50][50];
	int heigth;
	int width;
}Map;

Map *ft_LoadMap(char * path, Map *map);

#endif /* guard */