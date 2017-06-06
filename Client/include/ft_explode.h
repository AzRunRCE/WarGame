#ifndef FT_EXPL_H
#define FT_EXPL_H
#include <SDL.h>

typedef struct
{
	SDL_Rect Pos;
	SDL_Rect Sprite;

	int Step;
	int Delay;
	int lastAnim;
}Explode;





#endif // FT_EXPL_H
