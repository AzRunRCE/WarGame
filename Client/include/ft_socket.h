#ifndef SOCKET_H
#define SOCKET_H
#include "main.h"
#include "ft_configuration.h"

#ifdef _WIN32 || _WIN64
/* si vous êtes sous Windows */
#include <pthread_VC.h>
#define SLEEP10MS Sleep(10);
#define SOCKET_ERRNO    WSAGetLastError()
#elif defined linux || defined __linux || defined __linux__
/* si vous êtes sous linux */
#include <pthread.h>
#define SLEEP10MS usleep(10000);
#define SOCKET_ERRNO    errno
#else
/* sinon vous êtes sur une plateforme non supportée */
#error not defined for this platform
#endif
#define _WINSOCK_DEPRECATED_NO_WARNINGS

typedef enum
{
	RECVERROR = -10,
	SENDERROR = -11

} SocketError;

void *SreamClientData(void *arg);
void *NetworkThreadingListening(void *arg);
int create_connection(configuration *settings);
int write_client(const uint8_t *buffer, const int length);
void end();
bool connected;
int NwkThreadRet;
pthread_t NwkThread;
pthread_t NwkThreadSender;
#endif // SOCKET_H
