#include "include/ft_engine.h"
#include "include/ft_chat.h"
#include "include/ft_delay.h"

chatMessageCount = 0;

bool ft_chat_Add(int messageType, char *argument)
{
	if (strlen(argument) > MAX_CHAT_MESSAGE || chatMessageCount >= 5)
		return false;
	char text[MAX_CHAT_MESSAGE];
	memset(&text, 0, sizeof(text));
	if (messageType == PLAYERCONNECTION)
	{
		strncpy(text, argument, strlen(argument));
		strncat(text, " joined the game !", strlen(" joined the game !"));
		strncpy(chatMessage[chatMessageCount].message, text, strlen(text));
		chatMessage[chatMessageCount].lastUpdate = SDL_GetTicks();
	}
	else if (messageType == SERVERMESSAGE)
	{
		strncpy(text, "[SERVER] ", strlen("[SERVER] "));
		strncat(text, argument, strlen(argument));
		strncpy(chatMessage[chatMessageCount].message, text, strlen(text));
		chatMessage[chatMessageCount].lastUpdate = SDL_GetTicks();
	}
	chatMessageCount++;
	return true;
}

void ft_chat_Remove(unsigned int messagePosition)
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

bool ft_chat_Display(void)
{
	if (chatMessageCount == 0)
		return false;
	font = TTF_OpenFont("res/verdana.ttf", 15);

	for (int i = 0; i < chatMessageCount; i++)
	{
		textSurface = TTF_RenderText_Blended(font, chatMessage[i].message, _engine.colorWhite);
		textureTextSurface = SDL_CreateTextureFromSurface(_engine.screenRenderer, textSurface);
		posText = (SDL_Rect) { 10, i*20 + 10, textSurface->w, textSurface->h };
		SDL_RenderCopy(_engine.screenRenderer, textureTextSurface, NULL, &posText);
		if (ft_delay(&chatMessage[i].lastUpdate, MESSAGE_DELAY))
			ft_chat_Remove(i);
		TTF_CloseFont(font);
		SDL_FreeSurface(textSurface);
		SDL_DestroyTexture(textureTextSurface);
	}
	return true;
}

void ft_chat_Close(void)
{
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textureTextSurface);
}