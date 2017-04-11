#ifndef PLAYER_H
#define PLAYER_H
#include "sprite.h"
#include "main.h"
typedef struct Player Player;

struct Player {
    char name[16];
    int health;
    bool fire;
    SDL_Surface *characterSurface;
    SDL_Rect characterScreenRect;
    SDL_Rect spriteRect;
    State state;
    int step;
    bool walk;
};


#endif // PLAYER_H
