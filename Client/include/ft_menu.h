#ifndef FT_MENU_H
#define FT_MENU_H
#define MAX_LENGTH 32
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdbool.h>
#include "ft_engine.h"
#include "ft_point.h"
#include "ft_configuration.h"
#include "ft_delay.h"

typedef enum _menuType
{
	MENU_MAIN = 0,
	MENU_OPTIONS = 1,
	MENU_GAME = 2,
	MENU_SOUND = 3,
	MENU_VIDEO = 4
} MenuType;

typedef struct Menu {
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
	SDL_Surface *music;
	SDL_Surface *sound;
	SDL_Surface *labelPseudo;
    SDL_Surface *labelIpAddress;
	SDL_Surface *labelMusic;
	SDL_Surface *labelSound;
	SDL_Surface *labelApply;
	SDL_Surface *labelReturn;
    SDL_Rect posIpAddress;
	SDL_Rect posPseudo;
	SDL_Rect posMusic;
	SDL_Rect posSound;
	SDL_Rect posLabelPseudo;
    SDL_Rect posLabelIpAddress;
	SDL_Rect posLabelMusic;
	SDL_Rect posLabelSound;
    SDL_Rect posSelectionRight;
    SDL_Rect posSelectionLeft;
    SDL_Rect posOptionsSelectionLeft;
	SDL_Rect posLabelApply;
	SDL_Rect posLabelReturn;
    SDL_Texture *textureTextIpAdress;
	SDL_Texture *textureTextPseudo;
	SDL_Texture *textureTextMusic;
	SDL_Texture *textureTextSound;
    SDL_Texture *textureLabelIpAddress;
	SDL_Texture *textureLabelPseudo;
	SDL_Texture *textureLabelMusic;
	SDL_Texture *textureLabelSound;
	SDL_Texture *textureLabelApply;
	SDL_Texture *textureLabelReturn;
    TTF_Font *WarGameFont;
    char textInputIpAddress[MAX_LENGTH];
	char textInputPseudo[MAX_LENGTH];
	char textMusic[MAX_LENGTH];
	char textSound[MAX_LENGTH];
    bool selectionDone;
    bool selectionOptionsDone;

}Menu;

typedef struct MenuDeath {
	TTF_Font *font;
	char textCoolDown[3];
	char textRespawn[15];
	char respawnSentence[25];
	SDL_Color black;
	SDL_Surface *textSurface;
	SDL_Texture *textTexture;
	SDL_Rect textPos;
} MenuDeath;

static MenuDeath _menuDeath;

void menu(configuration *settings, int errcode);
void endMenuOptions(Menu *Menu);
void menuDeath(void);
void init_menuDeath(void);
#endif // FT_MENU_H
