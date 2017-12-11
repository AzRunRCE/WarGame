#ifndef FT_CHAT_H
#define FT_CHAT_H

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
bool ft_chat_add(int messageType, char *arguments);

bool ft_chat_display(void);

#endif // FT_CHAT_H
