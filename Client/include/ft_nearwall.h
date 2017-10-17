#ifndef FT_NEARWALL_H
#define FT_NEARWALL_H
#include <SDL.h>
typedef enum
{
	NONE = 0,
	LEFTRIGHT = 1,
	UPDOWN = 2,
	BOTH = 3
} NearWallMode;

int nWallMode;
SDL_Rect *nearWall[3];
void nearWallInit();
void nearWalldelete();


#endif // FT_NEARWALL_H
