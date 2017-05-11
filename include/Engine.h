#ifndef ENGINE_H
#define ENGINE_H
#include "Player.h"

typedef struct Engine Engine;

struct Engine {
    int WIDTH;
    int HEIGHT;
    int fullscreen;
    SDL_Event event;

    SDL_Renderer* screenRenderer;
    SDL_Window* window;

    SDL_Surface *mapSurface;
    SDL_Rect mapRect;
    SDL_Rect mapRectEnnemi;

    SDL_Surface *fogSurface;
    SDL_Rect fogRect;

    SDL_Surface *bombSurface;
    SDL_Surface *characterSurface;
     SDL_Surface *characterEnnemiSurface;
    SDL_Rect spriteRect;
    Player playerList[15];

    int Map[50][50];
};


#endif // ENGINE_H
