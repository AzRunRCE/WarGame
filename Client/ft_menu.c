#include <stdio.h>
#include <SDL_image.h>
#include "main.h"
#include "include/ft_menu.h"
#include "include/ft_engine.h"
#include "include/ft_point.h"
#include "include/ft_configuration.h"
#if defined linux || defined __linux || defined __linux__ /* si vous êtes sous linux */
#define _strdup strdup
#endif

void initMenuOptions(Menu *Menu, configuration *settings)
{
	strcpy(Menu->textInputIpAddress, settings->server);
	strcpy(Menu->textInputPseudo, settings->nickname);
	Menu->labelIpAddress = TTF_RenderText_Blended(Menu->WarGameFont, "Server address ", _engine.colorWarGame);
	Menu->labelPseudo = TTF_RenderText_Blended(Menu->WarGameFont, "Pseudo  ", _engine.colorWarGame);
	Menu->labelApply = TTF_RenderText_Blended(Menu->WarGameFont, "Apply", _engine.colorWarGame);
	Menu->labelReturn = TTF_RenderText_Blended(Menu->WarGameFont, "Return", _engine.colorWarGame);
	Menu->posLabelIpAddress = (SDL_Rect) { _engine.WIDTH / 3.5 - Menu->labelIpAddress->w / 2, _engine.HEIGHT / 2.2 + Menu->labelIpAddress->h / 2, Menu->labelIpAddress->w, Menu->labelIpAddress->h };
	Menu->posLabelPseudo = (SDL_Rect) { _engine.WIDTH / 3.5 - Menu->labelPseudo->w, _engine.HEIGHT / 1.8 + Menu->labelPseudo->h / 2, Menu->labelPseudo->w, Menu->labelPseudo->h };
	Menu->posLabelApply = (SDL_Rect) { _engine.WIDTH / 2 + Menu->labelApply->w, _engine.HEIGHT / 1.3 + Menu->labelApply->h / 2, Menu->labelApply->w, Menu->labelApply->h };
	Menu->posLabelReturn = (SDL_Rect) { _engine.WIDTH / 2.5 - Menu->labelReturn->w, _engine.HEIGHT / 1.3 + Menu->labelReturn->h / 2, Menu->labelReturn->w, Menu->labelReturn->h };
	Menu->textureLabelIpAddress = SDL_CreateTextureFromSurface(_engine.screenRenderer, Menu->labelIpAddress);
	Menu->textureLabelPseudo = SDL_CreateTextureFromSurface(_engine.screenRenderer, Menu->labelPseudo);
	Menu->textureLabelApply = SDL_CreateTextureFromSurface(_engine.screenRenderer, Menu->labelApply);
	Menu->textureLabelReturn = SDL_CreateTextureFromSurface(_engine.screenRenderer, Menu->labelReturn);
}

void endMenuOptions(Menu *Menu, configuration *settings)
{
	SDL_DestroyTexture(Menu->textureLabelIpAddress);
	SDL_DestroyTexture(Menu->textureLabelPseudo);
	SDL_DestroyTexture(Menu->textureLabelApply);
	SDL_DestroyTexture(Menu->textureLabelReturn);
	SDL_FreeSurface(Menu->labelIpAddress);
	SDL_FreeSurface(Menu->labelPseudo);
	SDL_FreeSurface(Menu->labelApply);
	SDL_FreeSurface(Menu->labelReturn);
	SDL_FreeSurface(Menu->menuOptionsBackground);
	SDL_FreeSurface(Menu->selectionLeft);
}

void menu(configuration *settings)
{
	Menu Menu;
	point pointLeft[3];
	point pointRight[3];
	Menu.textInputIpAddress = malloc(sizeof(char) + MAX_LENGTH + 1);
	Menu.textInputPseudo = malloc(sizeof(char) + MAX_LENGTH + 1);
	Menu.menuSelection = 0;
	Menu.menuOptionsSelection = 0;
	Menu.countBlink = 0;
	Menu.WarGameFont = TTF_OpenFont("res/boston.ttf", 40);
	Menu.menuBackground = IMG_LoadTexture(_engine.screenRenderer, "res/menu.png");
	Menu.menuOptionsBackground = IMG_LoadTexture(_engine.screenRenderer, "res/menu-options.png");
	Menu.selectionRight = IMG_LoadTexture(_engine.screenRenderer, "res/selection-right.png");
	Menu.selectionLeft = IMG_LoadTexture(_engine.screenRenderer, "res/selection-left.png");
	// Position PLAY LEFT
	pointLeft[0] = (point) { _engine.WIDTH / 2 - Menu.selectionLeft->h*2.5, _engine.HEIGHT / 2 - Menu.selectionLeft->h*1.2 };
	// Position PLAY RIGHT
	pointRight[0] = (point) { _engine.WIDTH / 2 + Menu.selectionRight->h*1.5, _engine.HEIGHT / 2 - Menu.selectionRight->h*1.2 };
	// Position OPTIONS LEFT
	pointLeft[1] = (point) { _engine.WIDTH / 2 - Menu.selectionLeft->h*3.5, _engine.HEIGHT / 2 + Menu.selectionLeft->h / 3.5 };
	// Position OPTIONS RIGHT
	pointRight[1] = (point) { _engine.WIDTH / 2 + Menu.selectionRight->h*2.5, _engine.HEIGHT / 2 + Menu.selectionRight->h / 3.5 };
	// Position QUIT LEFT
	pointLeft[2] = (point) { _engine.WIDTH / 2 - Menu.selectionLeft->h*2.5, _engine.HEIGHT / 2 + Menu.selectionLeft->h*1.7 };
	// Position QUIT RIGHT
	pointRight[2] = (point) { _engine.WIDTH / 2 + Menu.selectionRight->h*1.5, _engine.HEIGHT / 2 + Menu.selectionLeft->h*1.7 };
	Menu.ipAddress = NULL;
	Menu.pseudo = NULL;
	Menu.ipAddress = NULL;
	Menu.selectionDone = false;
	Menu.selectionOptionsDone = false;
	Menu.selectionMenuOptionsDone = false;
	while (!Menu.selectionDone) {
		Menu.posSelectionLeft = (SDL_Rect) { pointLeft[Menu.menuSelection].x, pointLeft[Menu.menuSelection].y, Menu.selectionLeft->h, Menu.selectionLeft->h };
		Menu.posSelectionRight = (SDL_Rect) { pointRight[Menu.menuSelection].x, pointRight[Menu.menuSelection].y, Menu.selectionRight->h, Menu.selectionRight->h };
		SDL_RenderClear(_engine.screenRenderer);
		if (Menu.selectionOptionsDone)
			SDL_RenderCopy(_engine.screenRenderer, Menu.menuOptionsBackground, NULL, NULL);
		else {
			SDL_RenderCopy(_engine.screenRenderer, Menu.menuBackground, NULL, NULL);
			SDL_RenderCopy(_engine.screenRenderer, Menu.selectionLeft, NULL, &Menu.posSelectionLeft);
			SDL_RenderCopy(_engine.screenRenderer, Menu.selectionRight, NULL, &Menu.posSelectionRight);
		}
		SDL_Event event;
		if (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				exit(0);
				break;
			case SDL_TEXTINPUT:
				/* Add new text onto the end of our text */
				if (Menu.menuOptionsSelection == 0 && strlen(Menu.textInputIpAddress) < MAX_LENGTH && Menu.selectionOptionsDone && Menu.confirmOptionsForm == 0)
					strcat(Menu.textInputIpAddress, event.text.text);
				else if (Menu.menuOptionsSelection == 1 && strlen(Menu.textInputPseudo) < MAX_LENGTH && Menu.selectionOptionsDone && Menu.confirmOptionsForm == 0)
					strcat(Menu.textInputPseudo, event.text.text);
				break;
				/*case SDL_KEYUP:
					break;*/
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(Menu.textInputIpAddress) > 0 && Menu.confirmOptionsForm == 0 && Menu.menuOptionsSelection == 0)
						Menu.textInputIpAddress[strlen(Menu.textInputIpAddress) - 1] = 0;
				if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(Menu.textInputPseudo) > 0 && Menu.confirmOptionsForm == 0 && Menu.menuOptionsSelection == 1)
					Menu.textInputPseudo[strlen(Menu.textInputPseudo) - 1] = 0;
				if (event.key.keysym.sym == SDLK_RETURN)
				{
					switch (Menu.menuSelection)
					{
					case 0:
						Menu.selectionDone = true;
						break;
					case 1:
						if (!Menu.selectionOptionsDone)
						{
							Menu.confirmOptionsForm = -2;
							initMenuOptions(&Menu, settings);
						}
						Menu.selectionMenuOptionsDone = true;
						break;
					case 2:
						exit(1);
						break;
					}
					switch (Menu.menuOptionsSelection)
					{
					case 0:
					case 1:
						Menu.confirmOptionsForm++;
						if (Menu.confirmOptionsForm == 2)
							Menu.confirmOptionsForm = false;
						if (Menu.selectionOptionsDone)
							Menu.countBlink = 0;
						else {
							Menu.selectionOptionsDone = true;
							Menu.countBlink = -1;
						}
						break;
					case 2:
						Menu.selectionOptionsDone = false;
						Menu.menuOptionsSelection = 0;
						endMenuOptions(&Menu, settings);
						break;
					case 3:			
						settings->server = _strdup(Menu.textInputIpAddress);
						settings->nickname = _strdup(Menu.textInputPseudo);
						ft_saveConf(settings);
						break;
					}
				}
				if (event.key.keysym.sym == SDLK_DOWN)
				{
					if (Menu.menuSelection < 2 && !Menu.selectionOptionsDone)
						Menu.menuSelection++;
					else if (Menu.menuOptionsSelection < 3 && Menu.selectionOptionsDone)
					{
						Menu.confirmOptionsForm = true;
						Menu.menuOptionsSelection++;
						Menu.countBlink = -1;
					}
				}
				if (event.key.keysym.sym == SDLK_UP)
				{
					if (Menu.menuSelection > 0 && !Menu.selectionOptionsDone)
					{
						Menu.menuSelection--;
					}
					else if (Menu.menuOptionsSelection > 0 && Menu.selectionOptionsDone)
					{
						Menu.confirmOptionsForm = true;
						Menu.menuOptionsSelection--;
						Menu.countBlink = -1;

					}
				}
				if (event.key.keysym.sym == SDLK_RIGHT && Menu.menuOptionsSelection < 3 && Menu.menuOptionsSelection >= 2 && Menu.selectionOptionsDone)
				{
					Menu.menuOptionsSelection++;
					Menu.countBlink = -1;

				}
				else if (event.key.keysym.sym == SDLK_LEFT && Menu.menuOptionsSelection <= 3 && Menu.menuOptionsSelection > 2 && Menu.selectionOptionsDone)
				{
					Menu.menuOptionsSelection--;
					Menu.countBlink = -1;
				}
				if (event.key.keysym.sym == SDLK_ESCAPE && Menu.selectionOptionsDone)
				{
					if (Menu.menuOptionsSelection || !Menu.menuOptionsSelection)
						Menu.confirmOptionsForm = -255;
					Menu.selectionOptionsDone = false;
					Menu.menuOptionsSelection = 0;
				}
				break;
			}
		}
		if (Menu.textInputIpAddress[0] != '\0' && Menu.selectionOptionsDone) {
			Menu.ipAddress = TTF_RenderText_Blended(Menu.WarGameFont, Menu.textInputIpAddress, _engine.colorWarGame);
			Menu.posIpAddress = (SDL_Rect) {(int32_t) _engine.WIDTH / 2, (int32_t)_engine.HEIGHT / 2.2 + Menu.ipAddress->h / 2, (int32_t)Menu.ipAddress->w, (int32_t)Menu.ipAddress->h };
			Menu.textureTextIpAdress = SDL_CreateTextureFromSurface(_engine.screenRenderer, Menu.ipAddress);
			SDL_RenderCopy(_engine.screenRenderer, Menu.textureTextIpAdress, NULL, &Menu.posIpAddress);
		}
		if (Menu.textInputPseudo[0] != '\0' && Menu.selectionOptionsDone) {
			Menu.pseudo = TTF_RenderText_Blended(Menu.WarGameFont, Menu.textInputPseudo, _engine.colorWarGame);
			Menu.posPseudo = (SDL_Rect) { _engine.WIDTH / 3.4, _engine.HEIGHT / 1.8 + Menu.pseudo->h / 2, Menu.pseudo->w, Menu.pseudo->h };
			Menu.textureTextPseudo = SDL_CreateTextureFromSurface(_engine.screenRenderer, Menu.pseudo);
			SDL_RenderCopy(_engine.screenRenderer, Menu.textureTextPseudo, NULL, &Menu.posPseudo);
		}
		if (Menu.selectionOptionsDone && Menu.selectionMenuOptionsDone) {
			SDL_RenderCopy(_engine.screenRenderer, Menu.textureLabelApply, NULL, &Menu.posLabelApply);
			SDL_RenderCopy(_engine.screenRenderer, Menu.textureLabelReturn, NULL, &Menu.posLabelReturn);
			switch (Menu.menuOptionsSelection)
			{
			case 0:
				Menu.posOptionsSelectionLeft = (SDL_Rect) { 0, _engine.HEIGHT / 2.2 + Menu.selectionLeft->h / 8, Menu.selectionLeft->h, Menu.selectionLeft->h };
				SDL_RenderCopy(_engine.screenRenderer, Menu.textureLabelPseudo, NULL, &Menu.posLabelPseudo);
				break;
			case 1:
				Menu.posOptionsSelectionLeft = (SDL_Rect) { 0, _engine.HEIGHT / 1.8 + Menu.selectionLeft->h / 8, Menu.selectionLeft->h, Menu.selectionLeft->h };
				SDL_RenderCopy(_engine.screenRenderer, Menu.textureLabelIpAddress, NULL, &Menu.posLabelIpAddress);
				break;
			case 2:
				Menu.posOptionsSelectionLeft = (SDL_Rect) { _engine.WIDTH / 3.5 - Menu.labelApply->w, _engine.HEIGHT / 1.3 + Menu.selectionLeft->h / 8, Menu.selectionLeft->h, Menu.selectionLeft->h };
				break;
			case 3:
				Menu.posOptionsSelectionLeft = (SDL_Rect) { _engine.WIDTH / 2.5 + Menu.labelApply->w, _engine.HEIGHT / 1.3 + Menu.selectionLeft->h / 8, Menu.selectionLeft->h, Menu.selectionLeft->h };
				break;
			}
			if (Menu.countBlink == -1 || Menu.countBlink < 50 || Menu.confirmOptionsForm)
			{
				SDL_RenderCopy(_engine.screenRenderer, Menu.selectionLeft, NULL, &Menu.posOptionsSelectionLeft);
				SDL_RenderCopy(_engine.screenRenderer, Menu.textureLabelIpAddress, NULL, &Menu.posLabelIpAddress);
				SDL_RenderCopy(_engine.screenRenderer, Menu.textureLabelPseudo, NULL, &Menu.posLabelPseudo);
			}
			else if (Menu.countBlink > 80 && !Menu.confirmOptionsForm)
				Menu.countBlink = 0;
			if (Menu.countBlink != -1)
				Menu.countBlink++;
		}
		SDL_RenderPresent(_engine.screenRenderer);
		if (Menu.textInputIpAddress[0] != '\0' && Menu.selectionOptionsDone)
		{
			SDL_DestroyTexture(Menu.textureTextIpAdress);
			SDL_FreeSurface(Menu.ipAddress);
		}
		if (Menu.textInputPseudo[0] != '\0' && Menu.selectionOptionsDone)
		{
			SDL_DestroyTexture(Menu.textureTextPseudo);
			SDL_FreeSurface(Menu.pseudo);
		}
	}
	SDL_FreeSurface(Menu.menuBackground);
	SDL_FreeSurface(Menu.selectionLeft);
	SDL_FreeSurface(Menu.selectionRight);
}

void menuDeath()
{
		SDL_RenderCopy(_engine.screenRenderer, gameoverBackground, NULL, NULL);
}
