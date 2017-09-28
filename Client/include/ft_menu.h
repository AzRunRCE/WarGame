#ifndef FT_MENU_H
#define FT_MENU_H
#define MAX_LENGTH 32
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "main.h"
#include "ft_configuration.h"
typedef struct Menu Menu;

struct Menu {
    int menuSelection;
    int menuOptionsSelection;
	int selectionMenuOptionsDone;
    int countBlink;
	int confirmOptionsForm;
	SDL_Texture *menuBackground;
	SDL_Texture *menuOptionsBackground;
    SDL_Surface *selectionLeft;
	SDL_Surface *selectionRight;
    SDL_Surface *ipAddress;
	SDL_Surface *pseudo;
	SDL_Surface *labelPseudo;
    SDL_Surface *labelIpAddress;
	SDL_Surface *labelApply;
	SDL_Surface *labelReturn;
    SDL_Rect posIpAddress;
	SDL_Rect posPseudo;
	SDL_Rect posLabelPseudo;
    SDL_Rect posLabelIpAddress;
    SDL_Rect posSelectionRight;
    SDL_Rect posSelectionLeft;
    SDL_Rect posOptionsSelectionLeft;
	SDL_Rect posLabelApply;
	SDL_Rect posLabelReturn;
    SDL_Texture *textureTextIpAdress;
	SDL_Texture *textureTextPseudo;
    SDL_Texture *textureLabelIpAddress;
	SDL_Texture *textureLabelPseudo;
	SDL_Texture *textureLabelApply;
	SDL_Texture *textureLabelReturn;
    TTF_Font *WarGameFont;
    char *textInputIpAddress;
	char *textInputPseudo;
    bool selectionDone;
    bool selectionOptionsDone;

};

SDL_Texture *gameoverBackground;

void menu(configuration *settings);
void menuDeath();
#endif // FT_MENU_H
