#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <SDL.h>
#include "include/ft_nearwall.h"
#include "include/ft_engine.h"
#ifdef _WIN32 || _WIN64
/* si vous êtes sous Windows */
#define SIZEOFNEARWALL sizeof(nearWall)
#elif defined linux || defined __linux || defined __linux__
/* si vous êtes sous linux */
#define SIZEOFNEARWALL sizeof(nearWall)/sizeof(nearWall[0])
#else
/* sinon vous êtes sur une plateforme non supportée */
#error not defined for this platform
#endif


void nearWallInit()
{
	for (int i = 0; i < SIZEOFNEARWALL; i++)
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
	for (int i = 0; i < SIZEOFNEARWALL; i++)
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
	for (int i = 0; i < SIZEOFNEARWALL; i++)
		free(nearWall[i]);
}
