#ifndef FT_NEARWALL_H
#define FT_NEARWALL_H
#include "ft_engine.h"
typedef enum
{
	NONE = 0,
	LEFTRIGHT = 1,
	UPDOWN = 2,
	BOTH = 3
} NearWallMode;

int nWallMode;
static SDL_Rect nearWall[4];
void ft_nearWall_Init(void);
void ft_nearWall_Check(void);

#endif // FT_NEARWALL_H
