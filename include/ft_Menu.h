#ifndef FT_MENU_H
#define FT_MENU_H
#define MAX_LENGTH 32
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

typedef struct Menu Menu;

struct Menu {
    SDL_Surface *menuBackground;
    SDL_Surface *ipAddress;
    SDL_Rect posIpAddress;
    char textInput[MAX_LENGTH + 1];
    SDL_bool selectionDone;

};

void menu();
#endif // FT_MENU_H
