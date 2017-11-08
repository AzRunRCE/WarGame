#ifndef SOCKET_H
#define SOCKET_H
#include "include/ft_bullet.h"
#include "include/ft_client.h"
#include "include/ft_configuration.h"
#include "include/ft_map.h"
#include "include/ft_engine.h"
#include "include/pb.h"
#include "include/pb_common.h"
#include "include/pb_encode.h"
#include "include/pb_decode.h"
#include "include/unionproto.pb.h"
#include "include/pb_functions.h"
#include "include/ft_configuration.h"

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

typedef enum
{
	RECVERROR = -10,
	SENDERROR = -11

} SocketError;

void *SreamClientData(void);
void *NetworkThreadingListening(void);
int create_connection(configuration *settings);
int write_client(const uint8_t *buffer, const int length);
void end();
bool connected;
int NwkThreadRet;
pthread_t NwkThread;
pthread_t NwkThreadSender;
#endif // SOCKET_H
