#ifndef FT_MENU_H
#define FT_MENU_H
#define MAX_LENGTH 32
#define SUBMENU_TITLE_MAX 4
#define SUBMENU_BUTTON_MAX SUBMENU_TITLE_MAX
#define SUBMENU_OPTIONS_MAX SELECTION_SAVE - SELECTION_GAME_SERVER_ADDRESS + 1
#define SUBMENU_OPTIONS_STEP SELECTION_GAME_SERVER_ADDRESS
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdbool.h>
#include "ft_engine.h"
#include "ft_point.h"
#include "ft_configuration.h"
#include "ft_delay.h"

typedef enum _MenuType
{
	MENU_MAIN = 0,
	MENU_OPTIONS = 1,
	MENU_GAME = 2,
	MENU_SOUND = 3,
	MENU_VIDEO = 4
} MenuType;

typedef enum _MenuSelection
{
	SELECTION_PLAY = 0,
	SELECTION_OPTIONS = 1,
	SELECTION_QUIT = 2
} MenuSelection;

typedef enum _SubMenuSelection
{
	SELECTION_MAIN_OPTIONS = 0,
	SELECTION_GAME_OPTIONS = 1,
	SELECTION_SOUND_OPTIONS = 2,
	SELECTION_VIDEO_OPTIONS = 3

} SubMenuSelection;

typedef enum _SubMenuOptionsSelection
{
	SELECTION_GAME_SERVER_ADDRESS = 4,
	SELECTION_GAME_PSEUDO = 5,
	SELECTION_SOUND_SOUND_ACTIVATION = 6,
	SELECTION_SOUND_MUSIC_ACTIVATION = 7,
	SELECTION_SOUND_SOUND_VOLUME = 8,
	SELECTION_SOUND_MUSIC_VOLUME = 9,
	SELECTION_VIDEO_RESOLUTION = 10,
	SELECTION_VIDEO_FULLSCREEN = 11,
	HINT_ESCAPE_TO_QUIT = 12,
	SELECTION_SAVE = 13
}SubMenuOptionsSelection;

typedef struct _Menu {
	uint8_t selection;
	bool selectionDone;
	uint32_t delayEvent;
    int menuSelection;
    int menuOptionsSelection;
	int selectionMenuOptionsDone;
    int countBlink;
	int confirmOptionsForm;
	configuration *mainConfiguration;
	SDL_Surface *tempText;

	SDL_Texture *background;
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

	SDL_Rect posTitle;
	SDL_Rect posPlay;
	SDL_Rect posOptions;
	SDL_Rect posQuit;


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

	SDL_Texture *textureTitle;
	SDL_Texture *texturePlay;
	SDL_Texture *texturePlayBlink;
	SDL_Texture *textureOptions;
	SDL_Texture *textureOptionsBlink;
	SDL_Texture *textureQuit;
	SDL_Texture *textureQuitBlink;

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
	TTF_Font *WarGameFontOptions;
	TTF_Font *WarGameFontTitle;
    char textInputIpAddress[MAX_LENGTH];
	char textInputPseudo[MAX_LENGTH];
	char textMusic[MAX_LENGTH];
	char textSound[MAX_LENGTH];
}Menu;

typedef struct _TextTitle {
	char text[MAX_LENGTH];
	SDL_Rect rect;
	SDL_Surface *surface;
	SDL_Texture *texture;
}TextTitle;

typedef struct _TextButton {
	char text[MAX_LENGTH];
	SDL_Rect rect;
	SDL_Surface *surfaceWhite;
	SDL_Surface *surfaceRed;
	SDL_Texture *textureWhite;
	SDL_Texture *textureRed;
}TextButton;

typedef struct _ButtonOptions {
	char text[MAX_LENGTH];
	char textInput[MAX_LENGTH];
	SDL_Rect rectText;
	SDL_Rect rectTextInput;
	SDL_Surface *surfaceTextWhite;
	SDL_Surface *surfaceTextRed;
	SDL_Texture *textureTextWhite;
	SDL_Texture *textureTextRed;
	SDL_Surface *surfaceTextInput;
	SDL_Texture *textureTextInput;
}ButtonOptions;


typedef struct _SubMenu {
	uint8_t selection;
	uint8_t selectionMin;
	uint8_t selectionMax;
	bool selectionDone;
	bool subMenuLoaded;
	TextTitle title[SUBMENU_TITLE_MAX];
	TextButton button[SUBMENU_BUTTON_MAX];
	ButtonOptions buttonOptions[SUBMENU_OPTIONS_MAX];

}SubMenu;


Menu _menu;
SubMenu _subMenu;

typedef struct _MenuDeath {
	TTF_Font *font;
	char textCoolDown[3];
	char textRespawn[15];
	char respawnSentence[25];
	SDL_Color black;
	SDL_Surface *textSurface;
	SDL_Texture *textTexture;
	SDL_Rect textPos;
} MenuDeath;

MenuDeath _menuDeath;

void initMainMenu(void);
void initSubMenu(void);

void mainMenu(int errcode);
void subMenu(uint8_t subMenuSelection);
void endMenuOptions(Menu *Menu);
void reloadSubMenu(uint8_t subMenuSelection);
void menuDeath(void);
void init_menuDeath(void);
#endif // FT_MENU_H
