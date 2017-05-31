#include "include\ft_Menu.h"
#include "include\ft_engine.h"
#include "include\ft_point.h"
#include <stdio.h>
#include "main.h"


void menu()
{
	Menu Menu;
	point pointLeft[3];
	point pointRight[3];
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

	Menu.labelIpAddress = TTF_RenderText_Blended(Menu.WarGameFont, "Server address =", _engine.colorWarGame);
	Menu.labelPseudo = TTF_RenderText_Blended(Menu.WarGameFont, "Pseudo =", _engine.colorWarGame);
	Menu.posLabelIpAddress = (SDL_Rect) { _engine.WIDTH / 3.5 - Menu.labelIpAddress->w / 2, _engine.HEIGHT / 1.8 + Menu.labelIpAddress->h / 2, Menu.labelIpAddress->w, Menu.labelIpAddress->h };
	Menu.posLabelPseudo = (SDL_Rect) { _engine.WIDTH / 3.5 - Menu.labelPseudo->w, _engine.HEIGHT / 1.5 + Menu.labelPseudo->h / 2, Menu.labelPseudo->w, Menu.labelPseudo->h };
	Menu.textureLabelIpAddress = SDL_CreateTextureFromSurface(_engine.screenRenderer, Menu.labelIpAddress);
	Menu.textureLabelPseudo = SDL_CreateTextureFromSurface(_engine.screenRenderer, Menu.labelPseudo);

	Menu.ipAddress = NULL;
	Menu.textInput[0] = '\0';
	Menu.selectionDone = false;
	Menu.selectionOptionsDone = false;
	Menu.selectionMenuOptionsDone = false;
	while (!Menu.selectionDone) {
		Menu.posSelectionLeft = (SDL_Rect) { pointLeft[Menu.menuSelection].x, pointLeft[Menu.menuSelection].y, Menu.selectionLeft->h, Menu.selectionLeft->h };
		Menu.posSelectionRight = (SDL_Rect) { pointRight[Menu.menuSelection].x, pointRight[Menu.menuSelection].y, Menu.selectionRight->h, Menu.selectionRight->h };
		SDL_RenderClear(_engine.screenRenderer);
		if (Menu.menuSelection == 1 && Menu.selectionOptionsDone == true)
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
				if (strlen(Menu.textInput) < MAX_LENGTH && Menu.selectionOptionsDone == true)
					strcat(Menu.textInput, event.text.text);
				break;
			case SDL_KEYUP:
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(Menu.textInput) > 0 && Menu.menuOptionsSelection == 0)
					Menu.textInput[strlen(Menu.textInput) - 1] = 0;
				if (event.key.keysym.sym == SDLK_RETURN)
				{
					switch (Menu.menuSelection)
					{
					case 0:
						Menu.selectionDone = true;
						break;
					case 1:
						if (Menu.selectionOptionsDone)
							Menu.countBlink = 0;
						else {
							Menu.selectionOptionsDone = true;
							Menu.countBlink = -1;
						}
						break;
					case 2:
						exit(1);
						break;
					}
					if (Menu.selectionOptionsDone == true)
						Menu.selectionMenuOptionsDone = true;
				}
				if (event.key.keysym.sym == SDLK_DOWN)
				{
					if (Menu.menuSelection < 2 && Menu.selectionOptionsDone == false)
						Menu.menuSelection++;
					else if (Menu.menuOptionsSelection < 1 && Menu.selectionOptionsDone == true)
					{
						Menu.menuOptionsSelection++;
						Menu.countBlink = -1;
					}
				}
				if (event.key.keysym.sym == SDLK_UP)
				{
					if (Menu.menuSelection > 0 && Menu.selectionOptionsDone == false)
					{
						Menu.menuSelection--;
					}
					else if (Menu.menuOptionsSelection > 0 && Menu.selectionOptionsDone == true)
					{
						Menu.countBlink = -1;
						Menu.menuOptionsSelection--;
					
					}
				}
				if (event.key.keysym.sym == SDLK_ESCAPE && Menu.selectionOptionsDone == true)
					Menu.selectionOptionsDone = false;

				break;
			}
		}
		if (Menu.menuSelection == 1 && Menu.textInput[0] != '\0' && Menu.selectionOptionsDone == true) {
			Menu.ipAddress = TTF_RenderText_Blended(Menu.WarGameFont, Menu.textInput, _engine.colorWarGame);
			Menu.posIpAddress = (SDL_Rect) { _engine.WIDTH / 2, _engine.HEIGHT / 1.8 + Menu.ipAddress->h / 2, Menu.ipAddress->w, Menu.ipAddress->h };
			Menu.textureTextIpAdress = SDL_CreateTextureFromSurface(_engine.screenRenderer, Menu.ipAddress);
			SDL_RenderCopy(_engine.screenRenderer, Menu.textureTextIpAdress, NULL, &Menu.posIpAddress);
		}
		if (Menu.menuSelection == 1 && Menu.selectionOptionsDone == true) {
			SDL_StartTextInput();
			switch (Menu.menuOptionsSelection)
			{
			case 0:
				Menu.posOptionsSelectionLeft = (SDL_Rect) { 0, _engine.HEIGHT / 1.8 + Menu.selectionLeft->h / 8, Menu.selectionLeft->h, Menu.selectionLeft->h };
				break;
			case 1:
				Menu.posOptionsSelectionLeft = (SDL_Rect) { 0, _engine.HEIGHT / 1.5 + Menu.selectionLeft->h / 8, Menu.selectionLeft->h, Menu.selectionLeft->h };
				SDL_StopTextInput();
				break;
			}
			/*SDL_RenderCopy(_engine.screenRenderer, Menu.selectionLeft, NULL, &Menu.posOptionsSelectionLeft);
			SDL_RenderCopy(_engine.screenRenderer, Menu.textureLabelIpAddress, NULL, &Menu.posLabelIpAddress);*/
			SDL_RenderCopy(_engine.screenRenderer, Menu.textureLabelPseudo, NULL, &Menu.posLabelPseudo);
			if (Menu.countBlink == -1)
			{
				SDL_RenderCopy(_engine.screenRenderer, Menu.selectionLeft, NULL, &Menu.posOptionsSelectionLeft);
				SDL_RenderCopy(_engine.screenRenderer, Menu.textureLabelIpAddress, NULL, &Menu.posLabelIpAddress);
			}
			else if (Menu.countBlink < 50) {
				SDL_RenderCopy(_engine.screenRenderer, Menu.selectionLeft, NULL, &Menu.posOptionsSelectionLeft);
				SDL_RenderCopy(_engine.screenRenderer, Menu.textureLabelIpAddress, NULL, &Menu.posLabelIpAddress);
			}
			else if (Menu.countBlink > 80)
				Menu.countBlink = 0;
			if (Menu.countBlink != -1 && Menu.menuOptionsSelection == 0)
				Menu.countBlink++;
		}
		SDL_RenderPresent(_engine.screenRenderer);
	}
}
