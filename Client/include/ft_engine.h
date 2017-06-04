#ifndef ENGINE_H
#define ENGINE_H
#include "ft_player.h"


typedef struct Engine Engine;

struct Engine {
    int WIDTH;
    int HEIGHT;
    int fullscreen;
    SDL_Event event;
    SDL_Rect pCenter;
    Player players[16];
    Player mainPlayer;
    SDL_Renderer* screenRenderer;
    SDL_Window* window;

    SDL_Surface *mapSurface;
    SDL_Rect camera;
    TTF_Font *font;
    SDL_Color colorWhite;
    SDL_Color colorWarGame;
    SDL_Surface *fogSurface;
    SDL_Rect fogRect;


    SDL_Surface *bulletSurface;
	SDL_Surface *explodeSurface;
    SDL_Surface *bombSurface;
    SDL_Surface *characterSurface;
    SDL_Surface *characterEnnemiSurface;
    SDL_Surface *menuSurface;
    SDL_Surface *selectionSurface;
    SDL_Rect spriteRect;
    SDL_Rect mousePos;
};

extern Engine _engine;
#endif // ENGINE_H
