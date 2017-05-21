#ifndef PLAYER_H
#define PLAYER_H
#include "sprite.h"
#include "main.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
typedef struct Player Player;

struct Player {
    char name[16];
    int health;
    bool fire;
    State state;
    int step;
    bool walk;
    int lastAnim;
    SDL_Rect Pos;
    SDL_Rect sprite;
};


#endif // PLAYER_H
