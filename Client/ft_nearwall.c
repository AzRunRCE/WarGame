#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <SDL.h>
#include "include/ft_nearwall.h"
#include "include/ft_engine.h"


void nearWallInit()
{
	for (int i = 0; i < 4; i++)
		nearWall[i] = malloc(sizeof(SDL_Rect));
	
	nearWall[0]->x = 0;
	nearWall[0]->y = 0;
	nearWall[0]->w = 800 / 2 - 16;
	nearWall[0]->h = 1600;

	nearWall[1]->x = 0;
	nearWall[1]->y = 0;
	nearWall[1]->w = 1600;
	nearWall[1]->h = 600 / 2 - 16;

	nearWall[2]->x = 1600 - 800 / 2 + 16;
	nearWall[2]->y = 0;
	nearWall[2]->w = 800 / 2;
	nearWall[2]->h = 1600;

	nearWall[3]->x = 0;
	nearWall[3]->y = 1600 - 600 / 2 + 16;
	nearWall[3]->w = 1600;
	nearWall[3]->h = 600 / 2;
}


void checkNearWall()
{
	int nWallModeTmp1 = NONE;
	int nWallModeTmp2 = NONE;
	nWallMode = NONE;
	for (int i = 0; i < 4; i++)
	{
		if (SDL_HasIntersection(&_engine.mainPlayer.playerBase.pos, nearWall[i]))
		{
			if (!(i % 2))
				nWallModeTmp1 = LEFTRIGHT;
			else
				nWallModeTmp2 = UPDOWN;
		}
	}
	nWallMode = nWallModeTmp1 + nWallModeTmp2;
}

void nearWalldelete()
{
	for (int i = 0; i < 4; i++)
		free(nearWall[i]);
}