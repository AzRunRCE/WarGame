#ifndef FT_CHAT_H
#define FT_CHAT_H
#include <SDL.h>

typedef enum _ChatMessageType
{
	PLAYERCONNECTION = 0,
	SERVERMESSAGE = 1
} ChatMessageType;

#define MAX_CHAT_MESSAGE 64
#define MAX_CHAT_MESSAGE_COUNT 5
#define MESSAGE_DELAY 5000

typedef struct _ChatMessage
{
	char message[MAX_CHAT_MESSAGE];
	unsigned int lastUpdate;
} ChatMessage;

ChatMessage chatMessage[5];

unsigned int chatMessageCount;
/**
*  \brief Add message in chat
*
*  \param messageType Message type you want to display
		see ChatMessageType
*  \param *argument For now , you have to pass char *
*/
bool ft_chat_Add(int messageType, char *arguments);

bool ft_chat_Display(void);
void ft_chat_Close(void);

TTF_Font *font;
SDL_Surface *textSurface;
SDL_Texture *textureTextSurface;
SDL_Rect posText;

#endif // FT_CHAT_H
