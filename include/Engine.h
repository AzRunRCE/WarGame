#ifndef ENGINE_H
#define ENGINE_H
#include "Player.h"


typedef struct Engine Engine;

struct Engine {
    int WIDTH;
    int HEIGHT;
    int fullscreen;
    SDL_Event event;
    Player  enemiPlayer;
    Player mainPlayer;
    SDL_Renderer* screenRenderer;
    SDL_Window* window;

    SDL_Surface *mapSurface;
    SDL_Rect camera;


    SDL_Surface *fogSurface;
    SDL_Rect fogRect;

    SDL_Surface *bombSurface;
    SDL_Surface *characterSurface;
    SDL_Surface *characterEnnemiSurface;
    SDL_Surface *menuSurface;
    SDL_Rect spriteRect;
};

extern Engine _engine;
#endif // ENGINE_H
