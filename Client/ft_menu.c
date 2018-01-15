#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <string.h>
#include <stdlib.h>
#include "include/ft_menu.h"

void initMainMenu(void)
{
	_menu.selectionDone = false;
	_subMenu.subMenuLoaded = false;
	_menu.delayEvent = 0;
	_menu.selection = SELECTION_PLAY;

	_menu.background = IMG_LoadTexture(_engine.screenRenderer, "res/menu.png");
	_menu.WarGameFont = TTF_OpenFont("res/PixelOperator.ttf", 80);
	_menu.WarGameFontTitle = TTF_OpenFont("res/PixelOperator-Bold.ttf", 100);
	_menu.WarGameFontOptions = TTF_OpenFont("res/PixelOperator.ttf", 40);

	// TITLE
	_menu.tempText = TTF_RenderText_Blended(_menu.WarGameFontTitle, "WarGame", _engine.colorWhite);
	_menu.posTitle = (SDL_Rect) { (int32_t)_engine.WIDTH / 2 - _menu.tempText->w / 2, (int32_t)_engine.HEIGHT / 50, (int32_t)_menu.tempText->w, (int32_t)_menu.tempText->h };
	_menu.textureTitle = SDL_CreateTextureFromSurface(_engine.screenRenderer, _menu.tempText);
	SDL_FreeSurface(_menu.tempText);
	
	// PLAY BUTTON
	// - WHITE
	_menu.tempText = TTF_RenderText_Blended(_menu.WarGameFont, "Play", _engine.colorWhite);
	_menu.posPlay = (SDL_Rect) { (int32_t)_engine.WIDTH / 2 - _menu.tempText->w / 2, (int32_t)_engine.HEIGHT / 4, (int32_t)_menu.tempText->w, (int32_t)_menu.tempText->h };
	_menu.texturePlay = SDL_CreateTextureFromSurface(_engine.screenRenderer, _menu.tempText);
	SDL_FreeSurface(_menu.tempText);
	// - RED
	_menu.tempText = TTF_RenderText_Blended(_menu.WarGameFont, "Play", _engine.colorRed);
	_menu.texturePlayBlink = SDL_CreateTextureFromSurface(_engine.screenRenderer, _menu.tempText);
	SDL_FreeSurface(_menu.tempText);

	// OPTIONS BUTTON
	// - WHITE
	_menu.tempText = TTF_RenderText_Blended(_menu.WarGameFont, "Options", _engine.colorWhite);
	_menu.posOptions = (SDL_Rect) { (int32_t)_engine.WIDTH / 2 - _menu.tempText->w / 2, (int32_t)_engine.HEIGHT / 4 + _engine.HEIGHT / 8, (int32_t)_menu.tempText->w, (int32_t)_menu.tempText->h };
	_menu.textureOptions = SDL_CreateTextureFromSurface(_engine.screenRenderer, _menu.tempText);
	SDL_FreeSurface(_menu.tempText);
	// - RED
	_menu.tempText = TTF_RenderText_Blended(_menu.WarGameFont, "Options", _engine.colorRed);
	_menu.textureOptionsBlink = SDL_CreateTextureFromSurface(_engine.screenRenderer, _menu.tempText);
	SDL_FreeSurface(_menu.tempText);

	// QUIT BUTTON
	// - WHITE
	_menu.tempText = TTF_RenderText_Blended(_menu.WarGameFont, "Quit", _engine.colorWhite);
	_menu.posQuit = (SDL_Rect) { (int32_t)_engine.WIDTH / 2 - _menu.tempText->w / 2, (int32_t)_engine.HEIGHT / 4 + (_engine.HEIGHT / 8) * 2, (int32_t)_menu.tempText->w, (int32_t)_menu.tempText->h };
	_menu.textureQuit = SDL_CreateTextureFromSurface(_engine.screenRenderer, _menu.tempText);
	SDL_FreeSurface(_menu.tempText);
	// - RED
	_menu.tempText = TTF_RenderText_Blended(_menu.WarGameFont, "Quit", _engine.colorRed);
	_menu.textureQuitBlink = SDL_CreateTextureFromSurface(_engine.screenRenderer, _menu.tempText);
	SDL_FreeSurface(_menu.tempText);

}

void initSubMenu(void)
{
	_subMenu.textInput = false;
	_subMenu.selection = SELECTION_GAME_OPTIONS;

	// TITLES
	strcpy(_subMenu.title[SELECTION_MAIN_OPTIONS].text, "Options");
	strcpy(_subMenu.title[SELECTION_GAME_OPTIONS].text, "Game Options");
	strcpy(_subMenu.title[SELECTION_SOUND_OPTIONS].text, "Sound Options");
	strcpy(_subMenu.title[SELECTION_VIDEO_OPTIONS].text, "Video Options");

	for (uint8_t i = 0; i < SUBMENU_TITLE_MAX; i++)
	{
		// TITLES
		_subMenu.title[i].surface = TTF_RenderText_Blended(_menu.WarGameFontTitle, _subMenu.title[i].text, _engine.colorWhite);
		_subMenu.title[i].rect = (SDL_Rect) { (int32_t)_engine.WIDTH / 2 - _subMenu.title[i].surface->w / 2, (int32_t)_engine.HEIGHT / 50, (int32_t)_subMenu.title[i].surface->w, (int32_t)_subMenu.title[i].surface->h };
		_subMenu.title[i].texture = SDL_CreateTextureFromSurface(_engine.screenRenderer, _subMenu.title[i].surface);
		SDL_FreeSurface(_subMenu.title[i].surface);
		_subMenu.title[i].surface = NULL;

		// BUTTONS
		_subMenu.button[i].surfaceWhite = TTF_RenderText_Blended(_menu.WarGameFont, _subMenu.title[i].text, _engine.colorWhite);
		_subMenu.button[i].surfaceRed = TTF_RenderText_Blended(_menu.WarGameFont, _subMenu.title[i].text, _engine.colorRed);
		_subMenu.button[i].rect = (SDL_Rect) { (int32_t)_engine.WIDTH / 2 - _subMenu.button[i].surfaceWhite->w / 2, (int32_t)_engine.HEIGHT / 4 + (_engine.HEIGHT / 8)*(i - 1), (int32_t)_subMenu.button[i].surfaceWhite->w, (int32_t)_subMenu.button[i].surfaceWhite->h };
		_subMenu.button[i].textureWhite = SDL_CreateTextureFromSurface(_engine.screenRenderer, _subMenu.button[i].surfaceWhite);
		_subMenu.button[i].textureRed = SDL_CreateTextureFromSurface(_engine.screenRenderer, _subMenu.button[i].surfaceRed);
		SDL_FreeSurface(_subMenu.button[i].surfaceWhite);
		SDL_FreeSurface(_subMenu.button[i].surfaceRed);
		_subMenu.button[i].surfaceWhite = NULL;
		_subMenu.button[i].surfaceRed = NULL;
	}

	// BUTTONS OPTIONS
	
	// - GAME
	strcpy(_subMenu.buttonOptions[SELECTION_GAME_SERVER_ADDRESS - SUBMENU_OPTIONS_STEP].text, "Server Address :");
	strncpy(_subMenu.buttonOptions[SELECTION_GAME_SERVER_ADDRESS - SUBMENU_OPTIONS_STEP].textInput, _menu.mainConfiguration->server, strlen(_menu.mainConfiguration->server));
	strcpy(_subMenu.buttonOptions[SELECTION_GAME_PSEUDO - SUBMENU_OPTIONS_STEP].text, "Pseudo :");
	strncpy(_subMenu.buttonOptions[SELECTION_GAME_PSEUDO - SUBMENU_OPTIONS_STEP].textInput, _menu.mainConfiguration->nickname, strlen(_menu.mainConfiguration->nickname));
	
	// - SOUND
	strcpy(_subMenu.buttonOptions[SELECTION_SOUND_SOUND_ACTIVATION - SUBMENU_OPTIONS_STEP].text, "Sound activated (all sounds) :");
	if (_menu.mainConfiguration->sound)
		strcpy(_subMenu.buttonOptions[SELECTION_SOUND_SOUND_ACTIVATION - SUBMENU_OPTIONS_STEP].textInput, "Yes");
	else
		strcpy(_subMenu.buttonOptions[SELECTION_SOUND_SOUND_VOLUME - SUBMENU_OPTIONS_STEP].textInput, "No");
	strcpy(_subMenu.buttonOptions[SELECTION_SOUND_SOUND_VOLUME - SUBMENU_OPTIONS_STEP].text, "Sound volume :");
	sprintf(_subMenu.buttonOptions[SELECTION_SOUND_SOUND_VOLUME - SUBMENU_OPTIONS_STEP].textInput, "%d", _menu.mainConfiguration->soundLevel);
	strcat(_subMenu.buttonOptions[SELECTION_SOUND_SOUND_VOLUME - SUBMENU_OPTIONS_STEP].textInput, "%");

	strcpy(_subMenu.buttonOptions[SELECTION_SOUND_MUSIC_ACTIVATION - SUBMENU_OPTIONS_STEP].text, "Music activated :");
	if (_menu.mainConfiguration->music)
		strcpy(_subMenu.buttonOptions[SELECTION_SOUND_MUSIC_ACTIVATION - SUBMENU_OPTIONS_STEP].textInput, "Yes");
	else
		strcpy(_subMenu.buttonOptions[SELECTION_SOUND_MUSIC_ACTIVATION - SUBMENU_OPTIONS_STEP].textInput, "No");
	strcpy(_subMenu.buttonOptions[SELECTION_SOUND_MUSIC_VOLUME - SUBMENU_OPTIONS_STEP].text, "Music volume :");
	sprintf(_subMenu.buttonOptions[SELECTION_SOUND_MUSIC_VOLUME - SUBMENU_OPTIONS_STEP].textInput, "%d", _menu.mainConfiguration->musicLevel);
	strcat(_subMenu.buttonOptions[SELECTION_SOUND_MUSIC_VOLUME - SUBMENU_OPTIONS_STEP].textInput, "%");

	// - VIDEO
	strcpy(_subMenu.buttonOptions[SELECTION_VIDEO_RESOLUTION - SUBMENU_OPTIONS_STEP].text, "Screen resolution :");
	char temp[10];
	sprintf(temp, "%d", _menu.mainConfiguration->width);
	strncpy(_subMenu.buttonOptions[SELECTION_VIDEO_RESOLUTION - SUBMENU_OPTIONS_STEP].textInput, temp, strlen(temp));
	strcpy(temp, "x");
	strncat(_subMenu.buttonOptions[SELECTION_VIDEO_RESOLUTION - SUBMENU_OPTIONS_STEP].textInput, temp, strlen(temp));
	sprintf(temp, "%d", _menu.mainConfiguration->height);
	strncat(_subMenu.buttonOptions[SELECTION_VIDEO_RESOLUTION - SUBMENU_OPTIONS_STEP].textInput, temp, strlen(temp));
	strcpy(_subMenu.buttonOptions[SELECTION_VIDEO_FULLSCREEN - SUBMENU_OPTIONS_STEP].text, "Fullscreen :");
	if (_engine.fullscreen)
		strcpy(_subMenu.buttonOptions[SELECTION_VIDEO_FULLSCREEN - SUBMENU_OPTIONS_STEP].textInput, "Yes");
	else
		strcpy(_subMenu.buttonOptions[SELECTION_VIDEO_FULLSCREEN - SUBMENU_OPTIONS_STEP].textInput, "No");

	uint8_t positionCount = 0;
	for (uint8_t i = 0; i < SUBMENU_OPTIONS_MAX - 2; i++)
	{

		// BUTTONS

		if (strlen(_subMenu.buttonOptions[i].text) > 1) {
			_subMenu.buttonOptions[i].surfaceTextWhite = TTF_RenderText_Blended(_menu.WarGameFontOptions, _subMenu.buttonOptions[i].text, _engine.colorWhite);
			_subMenu.buttonOptions[i].surfaceTextRed = TTF_RenderText_Blended(_menu.WarGameFontOptions, _subMenu.buttonOptions[i].text, _engine.colorRed);
			_subMenu.buttonOptions[i].rectText = (SDL_Rect) { (int32_t)_engine.WIDTH * 0.05, (int32_t)_engine.HEIGHT*0.4 + (_engine.HEIGHT * 0.08)*(positionCount - 1), (int32_t)_subMenu.buttonOptions[i].surfaceTextWhite->w, (int32_t)_subMenu.buttonOptions[i].surfaceTextWhite->h };
			_subMenu.buttonOptions[i].textureTextWhite = SDL_CreateTextureFromSurface(_engine.screenRenderer, _subMenu.buttonOptions[i].surfaceTextWhite);
			_subMenu.buttonOptions[i].textureTextRed = SDL_CreateTextureFromSurface(_engine.screenRenderer, _subMenu.buttonOptions[i].surfaceTextRed);
			SDL_FreeSurface(_subMenu.buttonOptions[i].surfaceTextWhite);
			SDL_FreeSurface(_subMenu.buttonOptions[i].surfaceTextRed);
			_subMenu.buttonOptions[i].surfaceTextWhite = NULL;
			_subMenu.buttonOptions[i].surfaceTextRed = NULL;
		}
		else
		{
			_subMenu.buttonOptions[i].surfaceTextWhite = NULL;
			_subMenu.buttonOptions[i].surfaceTextRed = NULL;
		}


		if (strlen(_subMenu.buttonOptions[i].textInput) > 1) {
			_subMenu.buttonOptions[i].surfaceTextInput = TTF_RenderText_Blended(_menu.WarGameFontOptions, _subMenu.buttonOptions[i].textInput, _engine.colorWhite);
			_subMenu.buttonOptions[i].rectTextInput = (SDL_Rect) { (int32_t)_engine.WIDTH * 0.01 + _subMenu.buttonOptions[i].rectText.x + _subMenu.buttonOptions[i].rectText.w, (int32_t)_engine.HEIGHT*0.4 + (_engine.HEIGHT * 0.08)*(positionCount - 1), (int32_t)_subMenu.buttonOptions[i].surfaceTextInput->w, (int32_t)_subMenu.buttonOptions[i].surfaceTextInput->h };
			_subMenu.buttonOptions[i].textureTextInput = SDL_CreateTextureFromSurface(_engine.screenRenderer, _subMenu.buttonOptions[i].surfaceTextInput);
			SDL_FreeSurface(_subMenu.buttonOptions[i].surfaceTextInput);
			_subMenu.buttonOptions[i].surfaceTextInput = NULL;
		}
		else
			_subMenu.buttonOptions[i].textureTextInput = NULL;

		switch (i)
		{
		case SELECTION_GAME_PSEUDO - SUBMENU_BUTTON_MAX:
			positionCount = 0;
			break;
		case SELECTION_SOUND_MUSIC_VOLUME - SUBMENU_BUTTON_MAX:
			positionCount = 0;
			break;
		default:
			positionCount++;
			break;
		}
	}

	strcpy(_subMenu.buttonOptions[HINT_ESCAPE_TO_QUIT - SUBMENU_OPTIONS_STEP].text, "Press ESC to discard changes");
	strcpy(_subMenu.buttonOptions[SELECTION_SAVE - SUBMENU_OPTIONS_STEP].text, "Save");
	
	_subMenu.buttonOptions[HINT_ESCAPE_TO_QUIT - SUBMENU_OPTIONS_STEP].surfaceTextWhite = TTF_RenderText_Blended(_menu.WarGameFontOptions, _subMenu.buttonOptions[HINT_ESCAPE_TO_QUIT - SUBMENU_OPTIONS_STEP].text, _engine.colorWhite);
	_subMenu.buttonOptions[HINT_ESCAPE_TO_QUIT - SUBMENU_OPTIONS_STEP].rectText = (SDL_Rect) { (int32_t)_engine.WIDTH * 0.05, (int32_t)_engine.HEIGHT*0.9, (int32_t)_subMenu.buttonOptions[HINT_ESCAPE_TO_QUIT - SUBMENU_OPTIONS_STEP].surfaceTextWhite->w, (int32_t)_subMenu.buttonOptions[HINT_ESCAPE_TO_QUIT - SUBMENU_OPTIONS_STEP].surfaceTextWhite->h };
	_subMenu.buttonOptions[HINT_ESCAPE_TO_QUIT - SUBMENU_OPTIONS_STEP].textureTextWhite = SDL_CreateTextureFromSurface(_engine.screenRenderer, _subMenu.buttonOptions[HINT_ESCAPE_TO_QUIT - SUBMENU_OPTIONS_STEP].surfaceTextWhite);
	SDL_FreeSurface(_subMenu.buttonOptions[HINT_ESCAPE_TO_QUIT - SUBMENU_OPTIONS_STEP].surfaceTextWhite);
	_subMenu.buttonOptions[HINT_ESCAPE_TO_QUIT - SUBMENU_OPTIONS_STEP].surfaceTextWhite = NULL;
	_subMenu.buttonOptions[HINT_ESCAPE_TO_QUIT - SUBMENU_OPTIONS_STEP].surfaceTextRed = NULL;

	_subMenu.buttonOptions[SELECTION_SAVE - SUBMENU_OPTIONS_STEP].surfaceTextWhite = TTF_RenderText_Blended(_menu.WarGameFontOptions, _subMenu.buttonOptions[SELECTION_SAVE - SUBMENU_OPTIONS_STEP].text, _engine.colorWhite);
	_subMenu.buttonOptions[SELECTION_SAVE - SUBMENU_OPTIONS_STEP].surfaceTextRed = TTF_RenderText_Blended(_menu.WarGameFontOptions, _subMenu.buttonOptions[SELECTION_SAVE - SUBMENU_OPTIONS_STEP].text, _engine.colorRed);
	_subMenu.buttonOptions[SELECTION_SAVE - SUBMENU_OPTIONS_STEP].rectText = (SDL_Rect) { (int32_t)_engine.WIDTH * 0.85, (int32_t)_engine.HEIGHT*0.9, (int32_t)_subMenu.buttonOptions[SELECTION_SAVE - SUBMENU_OPTIONS_STEP].surfaceTextWhite->w, (int32_t)_subMenu.buttonOptions[SELECTION_SAVE - SUBMENU_OPTIONS_STEP].surfaceTextWhite->h };
	_subMenu.buttonOptions[SELECTION_SAVE - SUBMENU_OPTIONS_STEP].textureTextWhite = SDL_CreateTextureFromSurface(_engine.screenRenderer, _subMenu.buttonOptions[SELECTION_SAVE - SUBMENU_OPTIONS_STEP].surfaceTextWhite);
	_subMenu.buttonOptions[SELECTION_SAVE - SUBMENU_OPTIONS_STEP].textureTextRed = SDL_CreateTextureFromSurface(_engine.screenRenderer, _subMenu.buttonOptions[SELECTION_SAVE - SUBMENU_OPTIONS_STEP].surfaceTextRed);
	SDL_FreeSurface(_subMenu.buttonOptions[SELECTION_SAVE - SUBMENU_OPTIONS_STEP].surfaceTextWhite);
	SDL_FreeSurface(_subMenu.buttonOptions[SELECTION_SAVE - SUBMENU_OPTIONS_STEP].surfaceTextRed);
	_subMenu.buttonOptions[SELECTION_SAVE - SUBMENU_OPTIONS_STEP].surfaceTextWhite = NULL;
	_subMenu.buttonOptions[SELECTION_SAVE - SUBMENU_OPTIONS_STEP].surfaceTextRed = NULL;

	_subMenu.subMenuLoaded = true;
}

void reloadSubMenu(uint8_t subMenuSelection)
{
	uint8_t min;
	uint8_t max;
	switch (subMenuSelection)
	{
	case SELECTION_GAME_OPTIONS:
		min = SELECTION_GAME_SERVER_ADDRESS - SUBMENU_OPTIONS_STEP;
		max = SELECTION_GAME_PSEUDO - SUBMENU_OPTIONS_STEP + 1;
		memset(_subMenu.buttonOptions[SELECTION_GAME_SERVER_ADDRESS - SUBMENU_OPTIONS_STEP].textInput, 0, strlen(_subMenu.buttonOptions[SELECTION_GAME_SERVER_ADDRESS - SUBMENU_OPTIONS_STEP].textInput));
		memset(_subMenu.buttonOptions[SELECTION_GAME_PSEUDO - SUBMENU_OPTIONS_STEP].textInput, 0, strlen(_subMenu.buttonOptions[SELECTION_GAME_PSEUDO - SUBMENU_OPTIONS_STEP].textInput));
		strncpy(_subMenu.buttonOptions[SELECTION_GAME_SERVER_ADDRESS - SUBMENU_OPTIONS_STEP].textInput, _menu.mainConfiguration->server, strlen(_menu.mainConfiguration->server));
		strncpy(_subMenu.buttonOptions[SELECTION_GAME_PSEUDO - SUBMENU_OPTIONS_STEP].textInput, _menu.mainConfiguration->nickname, strlen(_menu.mainConfiguration->nickname));
		break;
	case SELECTION_SOUND_OPTIONS:
		min = SELECTION_SOUND_SOUND_ACTIVATION - SUBMENU_OPTIONS_STEP;
		max = SELECTION_SOUND_MUSIC_VOLUME - SUBMENU_OPTIONS_STEP + 1;
		if (_menu.mainConfiguration->sound)
			strcpy(_subMenu.buttonOptions[SELECTION_SOUND_SOUND_ACTIVATION - SUBMENU_OPTIONS_STEP].textInput, "Yes");
		else
			strcpy(_subMenu.buttonOptions[SELECTION_SOUND_SOUND_ACTIVATION - SUBMENU_OPTIONS_STEP].textInput, "No");
		if (_menu.mainConfiguration->music)
			strcpy(_subMenu.buttonOptions[SELECTION_SOUND_MUSIC_ACTIVATION - SUBMENU_OPTIONS_STEP].textInput, "Yes");
		else
			strcpy(_subMenu.buttonOptions[SELECTION_SOUND_MUSIC_ACTIVATION - SUBMENU_OPTIONS_STEP].textInput, "No");
		sprintf(_subMenu.buttonOptions[SELECTION_SOUND_SOUND_VOLUME - SUBMENU_OPTIONS_STEP].textInput, "%d", _menu.mainConfiguration->soundLevel);
		strcat(_subMenu.buttonOptions[SELECTION_SOUND_SOUND_VOLUME - SUBMENU_OPTIONS_STEP].textInput, "%");
		sprintf(_subMenu.buttonOptions[SELECTION_SOUND_MUSIC_VOLUME - SUBMENU_OPTIONS_STEP].textInput, "%d", _menu.mainConfiguration->musicLevel);
		strcat(_subMenu.buttonOptions[SELECTION_SOUND_MUSIC_VOLUME - SUBMENU_OPTIONS_STEP].textInput, "%");
		break;
	case SELECTION_VIDEO_OPTIONS:
		min = SELECTION_VIDEO_RESOLUTION - SUBMENU_OPTIONS_STEP;
		max = SELECTION_VIDEO_FULLSCREEN - SUBMENU_OPTIONS_STEP + 1;
		memset(_subMenu.buttonOptions[SELECTION_VIDEO_RESOLUTION - SUBMENU_OPTIONS_STEP].textInput, 0, strlen(_subMenu.buttonOptions[SELECTION_VIDEO_RESOLUTION - SUBMENU_OPTIONS_STEP].textInput));
		char temp[10];
		sprintf(temp, "%d", _menu.mainConfiguration->width);
		strncpy(_subMenu.buttonOptions[SELECTION_VIDEO_RESOLUTION - SUBMENU_OPTIONS_STEP].textInput, temp, strlen(temp));
		strcpy(temp, "x");
		strncat(_subMenu.buttonOptions[SELECTION_VIDEO_RESOLUTION - SUBMENU_OPTIONS_STEP].textInput, temp, strlen(temp));
		sprintf(temp, "%d", _menu.mainConfiguration->height);
		strncat(_subMenu.buttonOptions[SELECTION_VIDEO_RESOLUTION - SUBMENU_OPTIONS_STEP].textInput, temp, strlen(temp));
		if (_engine.fullscreen)
			strcpy(_subMenu.buttonOptions[SELECTION_VIDEO_FULLSCREEN - SUBMENU_OPTIONS_STEP].textInput, "Yes");
		else
			strcpy(_subMenu.buttonOptions[SELECTION_VIDEO_FULLSCREEN - SUBMENU_OPTIONS_STEP].textInput, "No");
		break;
	}

	for (uint8_t i = min; i < max; i++)
	{
		if (strlen(_subMenu.buttonOptions[i].textInput) > 0)
		{
			SDL_DestroyTexture(_subMenu.buttonOptions[i].textureTextInput);
			_subMenu.buttonOptions[i].surfaceTextInput = TTF_RenderText_Blended(_menu.WarGameFontOptions, _subMenu.buttonOptions[i].textInput, _engine.colorWhite);
			_subMenu.buttonOptions[i].rectTextInput.w = _subMenu.buttonOptions[i].surfaceTextInput->w;
			_subMenu.buttonOptions[i].rectTextInput.h = _subMenu.buttonOptions[i].surfaceTextInput->h;
			_subMenu.buttonOptions[i].textureTextInput = SDL_CreateTextureFromSurface(_engine.screenRenderer, _subMenu.buttonOptions[i].surfaceTextInput);
			SDL_FreeSurface(_subMenu.buttonOptions[i].surfaceTextInput);
			_subMenu.buttonOptions[i].surfaceTextInput = NULL;
		}
	}

}

void initMenuOptions(Menu *Menu, configuration *settings)
{
	/*Menu->textInputIpAddress[0] = '\0';
	Menu->textInputPseudo[0] = '\0';
	Menu->textMusic[0] = '\0';
	Menu->textSound[0] = '\0';
	strncpy(Menu->textInputIpAddress, settings->server, strlen(settings->server) + 1);
	strncpy(Menu->textInputPseudo, settings->nickname, strlen(settings->nickname) + 1);
	sprintf(Menu->textMusic, "%d", settings->sound);

	Menu->labelIpAddress = TTF_RenderText_Blended(Menu->WarGameFont, "Server address ", _engine.colorWarGame);
	Menu->labelPseudo = TTF_RenderText_Blended(Menu->WarGameFont, "Pseudo  ", _engine.colorWarGame);
	Menu->labelMusic = TTF_RenderText_Blended(Menu->WarGameFont, "Music ", _engine.colorWarGame);
	Menu->labelSound = TTF_RenderText_Blended(Menu->WarGameFont, "Sound ", _engine.colorWarGame);
	Menu->labelApply = TTF_RenderText_Blended(Menu->WarGameFont, "Apply", _engine.colorWarGame);
	Menu->labelReturn = TTF_RenderText_Blended(Menu->WarGameFont, "Return", _engine.colorWarGame);

	Menu->posLabelIpAddress = (SDL_Rect) { _engine.WIDTH / 3.5 - Menu->labelIpAddress->w / 2, _engine.HEIGHT / 2.2 + Menu->labelIpAddress->h / 2, Menu->labelIpAddress->w, Menu->labelIpAddress->h };
	Menu->posLabelPseudo = (SDL_Rect) { _engine.WIDTH / 3.5 - Menu->labelPseudo->w, _engine.HEIGHT / 1.8 + Menu->labelPseudo->h / 2, Menu->labelPseudo->w, Menu->labelPseudo->h };
	Menu->posLabelApply = (SDL_Rect) { _engine.WIDTH / 2 + Menu->labelApply->w, _engine.HEIGHT / 1.3 + Menu->labelApply->h / 2, Menu->labelApply->w, Menu->labelApply->h };
	Menu->posLabelReturn = (SDL_Rect) { _engine.WIDTH / 2.5 - Menu->labelReturn->w, _engine.HEIGHT / 1.3 + Menu->labelReturn->h / 2, Menu->labelReturn->w, Menu->labelReturn->h };
	Menu->textureLabelIpAddress = SDL_CreateTextureFromSurface(_engine.screenRenderer, Menu->labelIpAddress);
	Menu->textureLabelPseudo = SDL_CreateTextureFromSurface(_engine.screenRenderer, Menu->labelPseudo);
	Menu->textureLabelApply = SDL_CreateTextureFromSurface(_engine.screenRenderer, Menu->labelApply);
	Menu->textureLabelReturn = SDL_CreateTextureFromSurface(_engine.screenRenderer, Menu->labelReturn);*/
}

void endMenuOptions(Menu *Menu)
{
	SDL_DestroyTexture(Menu->textureLabelIpAddress);
	SDL_DestroyTexture(Menu->textureLabelPseudo);
	SDL_DestroyTexture(Menu->textureLabelMusic);
	SDL_DestroyTexture(Menu->textureLabelSound);
	SDL_DestroyTexture(Menu->textureLabelApply);
	SDL_DestroyTexture(Menu->textureLabelReturn);
	SDL_FreeSurface(Menu->labelIpAddress);
	SDL_FreeSurface(Menu->labelPseudo);
	SDL_FreeSurface(Menu->labelMusic);
	SDL_FreeSurface(Menu->labelSound);
	SDL_FreeSurface(Menu->labelApply);
	SDL_FreeSurface(Menu->labelReturn);
	SDL_FreeSurface(Menu->menuOptionsBackground);
	SDL_FreeSurface(Menu->selectionLeft);
}

void mainMenu(int errcode)
/* TODO: handling errorcode, for example when server is not recheable or disconnected */
{
	initMainMenu();
	while (!_menu.selectionDone)
	{
		SDL_RenderCopy(_engine.screenRenderer, _menu.background, NULL, NULL);
		SDL_RenderCopy(_engine.screenRenderer, _menu.textureTitle, NULL, &_menu.posTitle);
		SDL_RenderCopy(_engine.screenRenderer, _menu.texturePlay, NULL, &_menu.posPlay);
		SDL_RenderCopy(_engine.screenRenderer, _menu.textureOptions, NULL, &_menu.posOptions);
		SDL_RenderCopy(_engine.screenRenderer, _menu.textureQuit, NULL, &_menu.posQuit);
		if (SDL_PollEvent(&_engine.event))
		{
			if (_engine.event.type == SDL_QUIT)
				exit(EXIT_SUCCESS);
			else if (_engine.event.type == SDL_KEYDOWN)
			{
				if (_engine.keystate[SDL_SCANCODE_UP] && _menu.selection > 0)
					_menu.selection--;
				else if (_engine.keystate[SDL_SCANCODE_DOWN] && _menu.selection < 2)
					_menu.selection++;
			}
		}
		switch (_menu.selection)
		{
		case SELECTION_PLAY:
			if (_engine.keystate[SDL_SCANCODE_RETURN])
				_menu.selectionDone = true;
			SDL_RenderCopy(_engine.screenRenderer, _menu.texturePlayBlink, NULL, &_menu.posPlay);
			break;
		case SELECTION_OPTIONS:
			if (_engine.keystate[SDL_SCANCODE_RETURN])
				subMenu(SELECTION_MAIN_OPTIONS);
			SDL_RenderCopy(_engine.screenRenderer, _menu.textureOptionsBlink, NULL, &_menu.posOptions);
			break;
		case SELECTION_QUIT:
			if (_engine.keystate[SDL_SCANCODE_RETURN])
				exit(EXIT_SUCCESS);
			SDL_RenderCopy(_engine.screenRenderer, _menu.textureQuitBlink, NULL, &_menu.posQuit);
			break;
		}
		SDL_RenderPresent(_engine.screenRenderer);
	}


	/*Menu Menu;
	point pointLeft[3];
	point pointRight[3];
	Menu.menuSelection = 0;
	Menu.menuOptionsSelection = 0;
	Menu.countBlink = 0;
	Menu.WarGameFont = TTF_OpenFont("res/boston.ttf", 40);
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
				// Add new text onto the end of our text
				if (Menu.menuOptionsSelection == 0 && strlen(Menu.textInputIpAddress) < MAX_LENGTH && Menu.selectionOptionsDone && Menu.confirmOptionsForm == 0)
					strcat(Menu.textInputIpAddress, event.text.text);
				else if (Menu.menuOptionsSelection == 1 && strlen(Menu.textInputPseudo) < MAX_LENGTH && Menu.selectionOptionsDone && Menu.confirmOptionsForm == 0)
					strcat(Menu.textInputPseudo, event.text.text);
				break;
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
						endMenuOptions(&Menu);
						break;
					case 3:
						strncpy(settings->server, Menu.textInputIpAddress, strlen(Menu.textInputIpAddress) + 1);
						strncpy(settings->nickname, Menu.textInputPseudo, strlen(Menu.textInputPseudo) + 1);
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
		if (Menu.textMusic[0] != '\0' && Menu.selectionOptionsDone) {
			Menu.music = TTF_RenderText_Blended(Menu.WarGameFont, Menu.textMusic, _engine.colorWarGame);
			Menu.posMusic = (SDL_Rect) { _engine.WIDTH / 3.4, _engine.HEIGHT / 2.6 + Menu.music->h / 2, Menu.music->w, Menu.music->h };
			Menu.textureTextMusic = SDL_CreateTextureFromSurface(_engine.screenRenderer, Menu.music);
			SDL_RenderCopy(_engine.screenRenderer, Menu.textureTextPseudo, NULL, &Menu.posMusic);
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
	SDL_FreeSurface(Menu.selectionRight);*/
}

void subMenu(uint8_t subMenuSelection)
{
	_subMenu.selectionDone = false;
	if (!_subMenu.subMenuLoaded)
		initSubMenu();
	switch (subMenuSelection)
	{
	case SELECTION_MAIN_OPTIONS:
		_subMenu.selection = SELECTION_GAME_OPTIONS;
		_subMenu.selectionMin = SELECTION_GAME_OPTIONS;
		_subMenu.selectionMax = SELECTION_VIDEO_OPTIONS;
		break;
	case SELECTION_GAME_OPTIONS:
		_subMenu.selection = SELECTION_GAME_SERVER_ADDRESS;
		_subMenu.selectionMin = SELECTION_GAME_SERVER_ADDRESS;
		_subMenu.selectionMax = SELECTION_GAME_PSEUDO;
		break;
	case SELECTION_SOUND_OPTIONS:
		_subMenu.selection = SELECTION_SOUND_SOUND_ACTIVATION;
		_subMenu.selectionMin = SELECTION_SOUND_SOUND_ACTIVATION;
		_subMenu.selectionMax = SELECTION_SOUND_MUSIC_VOLUME;
		break;
	case SELECTION_VIDEO_OPTIONS:
		_subMenu.selection = SELECTION_VIDEO_RESOLUTION;
		_subMenu.selectionMin = SELECTION_VIDEO_RESOLUTION;
		_subMenu.selectionMax = SELECTION_VIDEO_FULLSCREEN;
		break;
	}
	while (!_subMenu.selectionDone) {
		SDL_RenderCopy(_engine.screenRenderer, _menu.background, NULL, NULL);
		SDL_RenderCopy(_engine.screenRenderer, _subMenu.title[subMenuSelection].texture, NULL, &_subMenu.title[subMenuSelection].rect);
		SDL_RenderCopy(_engine.screenRenderer, _subMenu.buttonOptions[HINT_ESCAPE_TO_QUIT - SUBMENU_OPTIONS_STEP].textureTextWhite, NULL, &_subMenu.buttonOptions[HINT_ESCAPE_TO_QUIT - SUBMENU_OPTIONS_STEP].rectText);
		if (SDL_PollEvent(&_engine.event))
		{
			if (_engine.event.type == SDL_QUIT)
				exit(EXIT_SUCCESS);
			else if (_engine.event.type == SDL_KEYDOWN)
			{
				if (_engine.keystate[SDL_SCANCODE_UP] && _subMenu.selection > _subMenu.selectionMin && _subMenu.selection <= _subMenu.selectionMax)
					_subMenu.selection--;
				else if (_engine.keystate[SDL_SCANCODE_DOWN] && _subMenu.selection < _subMenu.selectionMax)
					_subMenu.selection++;
				else if (_engine.keystate[SDL_SCANCODE_UP] && subMenuSelection > SELECTION_MAIN_OPTIONS && _subMenu.selection == SELECTION_SAVE)
					_subMenu.selection = _subMenu.selectionMax;
				else if (_engine.keystate[SDL_SCANCODE_DOWN] && subMenuSelection > SELECTION_MAIN_OPTIONS && _subMenu.selection == _subMenu.selectionMax)
					_subMenu.selection = SELECTION_SAVE;
				else if (subMenuSelection != SELECTION_MAIN_OPTIONS && _engine.keystate[SDL_SCANCODE_ESCAPE])
				{
					subMenu(SELECTION_MAIN_OPTIONS);
					_menu.mainConfiguration = ft_loadConf();
					reloadSubMenu(subMenuSelection);
				}
				else if (subMenuSelection == SELECTION_MAIN_OPTIONS && _engine.keystate[SDL_SCANCODE_ESCAPE])
					_subMenu.selectionDone = true;
				else if (subMenuSelection == SELECTION_MAIN_OPTIONS && _engine.keystate[SDL_SCANCODE_RETURN])
					subMenu(_subMenu.selection);
				else if (subMenuSelection != SELECTION_MAIN_OPTIONS && (_engine.keystate[SDL_SCANCODE_RETURN] || _engine.keystate[SDL_SCANCODE_LEFT] || _engine.keystate[SDL_SCANCODE_RIGHT]))
				{
					switch (_subMenu.selection)
					{
					case SELECTION_SOUND_SOUND_VOLUME:
						if (_engine.keystate[SDL_SCANCODE_LEFT])
						{
							if (_menu.mainConfiguration->soundLevel > 10)
								_menu.mainConfiguration->soundLevel -= 10;
							else
								_menu.mainConfiguration->soundLevel = 100;
						}
						else
						{
							if (_menu.mainConfiguration->soundLevel < 100)
								_menu.mainConfiguration->soundLevel += 10;
							else
								_menu.mainConfiguration->soundLevel = 10;
						}
						break;
					case SELECTION_SOUND_MUSIC_VOLUME:
						if (_engine.keystate[SDL_SCANCODE_LEFT])
						{
							if (_menu.mainConfiguration->musicLevel > 10)
								_menu.mainConfiguration->musicLevel -= 10;
							else
								_menu.mainConfiguration->musicLevel = 100;
						}
						else
						{
							if (_menu.mainConfiguration->musicLevel < 100)
								_menu.mainConfiguration->musicLevel += 10;
							else
								_menu.mainConfiguration->musicLevel = 10;
						}
						break;
					case SELECTION_SOUND_MUSIC_ACTIVATION:
						if (_menu.mainConfiguration->music)
							_menu.mainConfiguration->music = false;
						else
							_menu.mainConfiguration->music = true;
						break;
					case SELECTION_SOUND_SOUND_ACTIVATION:
						if (_menu.mainConfiguration->sound)
							_menu.mainConfiguration->sound = false;
						else
							_menu.mainConfiguration->sound = true;
						break;
					case SELECTION_VIDEO_RESOLUTION:
						if (_engine.keystate[SDL_SCANCODE_LEFT])
						{
							if (_menu.mainConfiguration->width > 800)
								_menu.mainConfiguration->width -= 192;
							else
								_menu.mainConfiguration->width = 1184;
						}
						else
						{
							if (_menu.mainConfiguration->width < 1184)
								_menu.mainConfiguration->width += 192;
							else
								_menu.mainConfiguration->width = 800;
						}
						_menu.mainConfiguration->height = _menu.mainConfiguration->width * (3.0 / 4.0);
						break;
					case SELECTION_GAME_SERVER_ADDRESS:
						if (_engine.keystate[SDL_SCANCODE_RETURN])
						{
							if (_subMenu.textInput)
							{
								SDL_StopTextInput();
								_subMenu.textInput = false;
							}
							else
							{
								SDL_StartTextInput();
								_subMenu.textInput = true;
							}
						}
						break;
					case SELECTION_SAVE:
						if (_engine.keystate[SDL_SCANCODE_RETURN])
							ft_saveConf(_menu.mainConfiguration);
						break;
					}
					reloadSubMenu(subMenuSelection);
				}
				else if (_subMenu.textInput && _engine.keystate[SDL_SCANCODE_BACKSPACE])
				{
					if (_subMenu.selection == SELECTION_GAME_SERVER_ADDRESS && strlen(_menu.mainConfiguration->server) > 0)
						_menu.mainConfiguration->server[strlen(_menu.mainConfiguration->server) - 1] = '\0';
					else if (_subMenu.selection == SELECTION_GAME_PSEUDO && strlen(_menu.mainConfiguration->nickname) > 0)
						_menu.mainConfiguration->nickname[strlen(_menu.mainConfiguration->nickname) - 1] = '\0';
					reloadSubMenu(subMenuSelection);
				}
			}
			else if (_engine.event.type == SDL_TEXTINPUT)
			{
				switch (_subMenu.selection)
				{
				case SELECTION_GAME_SERVER_ADDRESS:
					if (strlen(_menu.mainConfiguration->server) < MAX_LENGTH)
						strcat(_menu.mainConfiguration->server, _engine.event.text.text);
					printf("server: %s\n", _menu.mainConfiguration->server);
					break;
				}
				reloadSubMenu(subMenuSelection);
			}
		}
		switch (subMenuSelection)
		{
		case SELECTION_MAIN_OPTIONS:
			for (uint8_t i = SELECTION_GAME_OPTIONS; i < SUBMENU_OPTIONS_STEP; i++)
				SDL_RenderCopy(_engine.screenRenderer, _subMenu.button[i].textureWhite, NULL, &_subMenu.button[i].rect);
			SDL_RenderCopy(_engine.screenRenderer, _subMenu.button[_subMenu.selection].textureRed, NULL, &_subMenu.button[_subMenu.selection].rect);
			break;
		case SELECTION_GAME_OPTIONS:
		case SELECTION_SOUND_OPTIONS:
		case SELECTION_VIDEO_OPTIONS:
			for (uint8_t i = _subMenu.selectionMin - SUBMENU_OPTIONS_STEP; i < _subMenu.selectionMax - SUBMENU_OPTIONS_STEP + 1; i++)
			{
				SDL_RenderCopy(_engine.screenRenderer, _subMenu.buttonOptions[i].textureTextWhite, NULL, &_subMenu.buttonOptions[i].rectText);
				if (strlen(_subMenu.buttonOptions[i].textInput) > 0)
					SDL_RenderCopy(_engine.screenRenderer, _subMenu.buttonOptions[i].textureTextInput, NULL, &_subMenu.buttonOptions[i].rectTextInput);
			}
			SDL_RenderCopy(_engine.screenRenderer, _subMenu.buttonOptions[SELECTION_SAVE - SUBMENU_OPTIONS_STEP].textureTextWhite, NULL, &_subMenu.buttonOptions[SELECTION_SAVE - SUBMENU_OPTIONS_STEP].rectText);
			SDL_RenderCopy(_engine.screenRenderer, _subMenu.buttonOptions[_subMenu.selection - SUBMENU_OPTIONS_STEP].textureTextRed, NULL, &_subMenu.buttonOptions[_subMenu.selection - SUBMENU_OPTIONS_STEP].rectText);
			break;
		}
		printf("SubMenu selection: %d\n", _subMenu.selection);
		printf("TextInput: %d\n", _subMenu.textInput);
		SDL_RenderPresent(_engine.screenRenderer);
	}
}

void init_menuDeath(void) {
	_menuDeath.font = TTF_OpenFont("res/verdana.ttf", 40);
	_menuDeath.black = (SDL_Color) { 255, 255, 255 };
	char textRespawn[] = "Respawn in ";
	char respawnSentence[] = "Press SPACE to respawn !";
	strncpy(_menuDeath.textRespawn, textRespawn, strlen(textRespawn));
	strncpy(_menuDeath.respawnSentence, respawnSentence, strlen(respawnSentence));
}

void menuDeath(void)
{

	SDL_RenderCopy(_engine.screenRenderer, _engine.gameoverBackground, NULL, NULL);
	sprintf(_menuDeath.textCoolDown, "%d", _engine.cooldownDeath);
	strcat(_menuDeath.textRespawn, _menuDeath.textCoolDown);

	if (_engine.cooldownDeath > 0 ) {
		_menuDeath.textSurface = TTF_RenderText_Blended(_menuDeath.font, _menuDeath.textRespawn, _menuDeath.black);
		_menuDeath.textTexture = SDL_CreateTextureFromSurface(_engine.screenRenderer, _menuDeath.textSurface);
		_menuDeath.textPos = (SDL_Rect) { .x = _engine.WIDTH / 2 - _menuDeath.textSurface->w / 2, .y = _engine.HEIGHT / 2, .w = _menuDeath.textSurface->w, .h = _menuDeath.textSurface->h };
		SDL_RenderCopy(_engine.screenRenderer, _menuDeath.textTexture, NULL, &_menuDeath.textPos);
		if (ft_delay(&_engine.lastCooldownDeath, 1000))
			_engine.cooldownDeath--;
	}
	else if (!_engine.cooldownDeath) {
		_menuDeath.textSurface = TTF_RenderText_Blended(_menuDeath.font, _menuDeath.respawnSentence, _menuDeath.black);
		_menuDeath.textTexture = SDL_CreateTextureFromSurface(_engine.screenRenderer, _menuDeath.textSurface);
		_menuDeath.textPos = (SDL_Rect) { .x = _engine.WIDTH / 2 - _menuDeath.textSurface->w / 2, .y = _engine.HEIGHT / 2, .w = _menuDeath.textSurface->w, .h = _menuDeath.textSurface->h };
		SDL_RenderCopy(_engine.screenRenderer, _menuDeath.textTexture, NULL, &_menuDeath.textPos);
	}
	_menuDeath.textRespawn[strlen(_menuDeath.textRespawn) - strlen(_menuDeath.textCoolDown)] = '\0';
	SDL_FreeSurface(_menuDeath.textSurface);
	SDL_DestroyTexture(_menuDeath.textTexture);

}
