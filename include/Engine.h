#ifndef ENGINE_H
#define ENGINE_H
#include "Player.h"

typedef struct Engine Engine;

struct Engine {
    int WIDTH;
    int HEIGTH;
    SDL_Event event;

    SDL_Surface *screenSurface;

    SDL_Surface *mapSurface;
    SDL_Rect mapRect;

    SDL_Surface *fogSurface;
    SDL_Rect fogRect;

    SDL_Rect *texteSurface;
    TTF_Font *police;

    SDL_Surface *bombSurface;

    Player playerList[15];
    Item ItemList[15];
    int Map[50][50];
};


#endif // ENGINE_H
