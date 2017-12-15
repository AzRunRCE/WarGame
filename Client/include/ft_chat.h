#ifndef FT_CHAT_H
#define FT_CHAT_H
#include <SDL.h>

typedef enum _ChatMessageType
{
	PLAYERCONNECTION = 0,
	SERVERMESSAGE = 1
} ChatMessageType;

typedef enum _DisplayType
{
	DISPLAY_OFF = 0,
	DISPLAY_ON = 1,
	DISPLAY_HISTORY = 2
} DisplayType;

#define MAX_CHAT_MESSAGE 64
#define MAX_CHAT_MESSAGE_ARRAY 15
#define MAX_CHAT_MESSAGE_COUNT 5
#define MESSAGE_DELAY 5000

typedef struct _ChatMessage
{
	char message[MAX_CHAT_MESSAGE];
	uint32_t lastUpdate;
	uint8_t display;
} ChatMessage;

ChatMessage chatMessage[MAX_CHAT_MESSAGE_ARRAY];

static uint8_t chatMessageCount;
/**
*  \brief Add message in chat
*
*  \param messageType Message type you want to display
		see ChatMessageType
*  \param *argument For now , you have to pass char *
*/
bool ft_chat_Add(uint8_t messageType, char *arguments);
void ft_chat_Remove(uint8_t messagePosition);
void ft_chat_History_Set(uint8_t messagePosition);
void ft_chat_History_Show(void);

bool ft_chat_Display(void);
void ft_chat_Init(void);
void ft_chat_Close(void);

TTF_Font *font;
SDL_Surface *textSurface;
SDL_Texture *textureTextSurface;
SDL_Rect posText;

#endif // FT_CHAT_H
