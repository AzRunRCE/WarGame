#include <SDL.h>
#include "include/ft_engine.h"
#include "include/ft_chat.h"
#include "include/ft_delay.h"

bool ft_chat_Add(uint8_t messageType, char *argument)
{
	if (strlen(argument) > MAX_CHAT_MESSAGE)
		return false;
	if (chatMessageCount >= MAX_CHAT_MESSAGE_ARRAY)
	{
		ft_chat_Remove(chatMessageCount);
	}
	char text[MAX_CHAT_MESSAGE];
	memset(&text, 0, sizeof(text));
	if (messageType == PLAYERCONNECTION)
	{
		strncpy(text, argument, strlen(argument));
		strncat(text, " joined the game !", strlen(" joined the game !"));
		strncpy(chatMessage[chatMessageCount].message, text, strlen(text));
		chatMessage[chatMessageCount].lastUpdate = SDL_GetTicks();
		chatMessage[chatMessageCount].display = DISPLAY_ON;
	}
	else if (messageType == SERVERMESSAGE)
	{
		strncpy(text, "[SERVER] ", strlen("[SERVER] "));
		strncat(text, argument, strlen(argument));
		strncpy(chatMessage[chatMessageCount].message, text, strlen(text));
		chatMessage[chatMessageCount].lastUpdate = SDL_GetTicks();
		chatMessage[chatMessageCount].display = DISPLAY_ON;
	}
	chatMessageCount++;
	return true;
}

void ft_chat_Remove(uint8_t messagePosition)
{
	if (messagePosition == 0 && chatMessageCount == 1) /* If this is the first message and we have only 1 message */
	{
		memset(&chatMessage[messagePosition].message, 0, MAX_CHAT_MESSAGE);
		chatMessage[messagePosition].lastUpdate = 0;
	}
	else
	{
		for (int i = messagePosition; i < chatMessageCount; i++)
			memmove(&chatMessage[i], &chatMessage[i + 1], sizeof(ChatMessage));
		memset(&chatMessage[chatMessageCount - 1].message, 0, MAX_CHAT_MESSAGE);
		chatMessage[chatMessageCount - 1 ].lastUpdate = 0;
	}
	chatMessageCount--;
}

void ft_chat_History_Set(uint8_t messagePosition)
{
	chatMessage[messagePosition].display = DISPLAY_OFF;
}

void ft_chat_History_Show(void)
{
	for (uint8_t i = 0; i < chatMessageCount; i++)
		chatMessage[i].display = DISPLAY_HISTORY;
}
void ft_chat_History_Hide(void)
{
	for (uint8_t i = 0; i < chatMessageCount; i++)
		chatMessage[i].display = DISPLAY_OFF;
}

bool ft_chat_Display(void)
{
	if (chatMessageCount == 0)
		return false;
	bool displayHistory = false;

	uint16_t messageMaxWidth = 0;

	for (uint8_t i = 0; i < chatMessageCount; i++)
	{
		if (chatMessage[i].display == DISPLAY_ON || chatMessage[i].display == DISPLAY_HISTORY)
		{
			textSurface = TTF_RenderText_Blended(font, chatMessage[i].message, _engine.colorWhite);
			textureTextSurface = SDL_CreateTextureFromSurface(_engine.screenRenderer, textSurface);
			if (textSurface->w > messageMaxWidth)
				messageMaxWidth = textSurface->w;
			posText = (SDL_Rect) { 10, i * 20 + 10, textSurface->w, textSurface->h };
			SDL_RenderCopy(_engine.screenRenderer, textureTextSurface, NULL, &posText);
			if (chatMessage[i].display == DISPLAY_ON && ft_delay(&chatMessage[i].lastUpdate, MESSAGE_DELAY))
				ft_chat_History_Set(i);
			else if (!displayHistory && chatMessage[i].display == DISPLAY_HISTORY)
				displayHistory = true;

			SDL_FreeSurface(textSurface);
			SDL_DestroyTexture(textureTextSurface);
		}
	}
	if (displayHistory)
	{
		SDL_SetRenderDrawColor(_engine.screenRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		uint16_t xMax;
		uint16_t yMax;
		uint16_t a;
		uint16_t b;
		for (uint8_t i = 0; i < 3; i++)
		{
			xMax = messageMaxWidth + 20 + i;
			yMax = (chatMessageCount + 1) * 20 + i;

			SDL_RenderDrawLine(_engine.screenRenderer, 0, yMax, xMax, yMax);
			SDL_RenderDrawLine(_engine.screenRenderer, xMax, 0, xMax, yMax);

			a = (chatMessageCount + 2) * 20 + i;
			SDL_RenderDrawLine(_engine.screenRenderer, 0, a, xMax, a);
			SDL_RenderDrawLine(_engine.screenRenderer, xMax, yMax, xMax, a);
		}


	}
	return true;
}

void ft_chat_Init(void)
{
	font = TTF_OpenFont("res/verdana.ttf", 15);
	chatMessageCount = 0;
}

void ft_chat_Close(void)
{
	TTF_CloseFont(font);
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textureTextSurface);
}