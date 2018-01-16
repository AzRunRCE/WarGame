#ifndef FT_EXPL_H
#define FT_EXPL_H
#include <SDL.h>

typedef struct _Explode
{
	SDL_Rect Pos;
	SDL_Rect Sprite;

	uint8_t Step;
	uint32_t lastAnim;
}Explode;





#endif // FT_EXPL_H
