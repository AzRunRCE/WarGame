#ifndef FT_MENU_H
#define FT_MENU_H
#define MAX_LENGTH 32
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "main.h"

typedef struct Menu Menu;

struct Menu {
    int menuSelection;
    int menuOptionsSelection;
    int countBlink;
    SDL_Surface *menuBackground;
    SDL_Surface *menuOptionsBackground;
    SDL_Surface *selectionRight;
    SDL_Surface *selectionLeft;
    SDL_Surface *ipAddress;
    SDL_Surface *labelIpAddress;
    SDL_Rect posIpAddress;
    SDL_Rect posLabelIpAddress;
    SDL_Rect posSelectionRight;
    SDL_Rect posSelectionLeft;
    SDL_Rect posOptionsSelectionLeft;
    SDL_Texture *textureTextIpAdress;
    SDL_Texture *textureLabelIpAddress;
    TTF_Font *WarGameFont;
    char textInput[MAX_LENGTH + 1];
    bool selectionDone;
    bool selectionOptionsDone;

};

void menu();
#endif // FT_MENU_H
