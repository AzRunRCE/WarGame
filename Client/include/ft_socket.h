#ifndef SOCKET_H
#define SOCKET_H
#include "ft_bullet.h"
#include "ft_client.h"
#include "ft_configuration.h"
#include "ft_map.h"
#include "ft_engine.h"
#include "pb.h"
#include "pb_common.h"
#include "pb_encode.h"
#include "pb_decode.h"
#include "unionproto.pb.h"
#include "pb_functions.h"
#include "ft_configuration.h"
#include "ft_menu.h"
#include "ft_chat.h"

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

#define MAX_BUFFER 4096

typedef enum
{
	RECVERROR = -10,
	SENDERROR = -11

} SocketError;

void *StreamClientData(void);
void *NetworkThreadingListening(void);
int create_connection(configuration *settings);
int write_client(const uint8_t *buffer, const int length);
void end();
int checkServerisAlive(configuration *settings);
bool connected;
int NwkThreadRet;
time_t lastUpdateFromServer;
pthread_t NwkThread;
pthread_t NwkThreadSender;
#endif // SOCKET_H
