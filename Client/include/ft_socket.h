#ifndef SOCKET_H
#define SOCKET_H
#include <errno.h>
#include "ft_bullet.h"
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
#include <WinSock2.h>
#define SLEEP10MS Sleep(10);
#define SOCKET_ERRNO    WSAGetLastError()
#elif defined linux || defined __linux || defined __linux__
/* si vous êtes sous linux */
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define SOCKET_ERRNO    errno
#define closesocket(s) close(s)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;
#define SLEEP10MS usleep(10000);
#else
/* sinon vous êtes sur une plateforme non supportée */
#error not defined for this platform
#endif

#define MAX_BUFFER 4096
#define CRLF	 "\r\n"
#define PORT	 1977

#define BUF_SIZE 1024

typedef enum
{
	RECVERROR = -10,
	SENDERROR = -11

} SocketError;

void *StreamClientData(void);
void *NetworkThreadingListening(void);
int network_CreateConnection(configuration *settings);
int network_Init(const char *address, SOCKADDR_IN *sin);
int write_client(const uint8_t *buffer, const int length);
void network_Clean(void);
void socket_Close(void);
int checkServerisAlive(configuration *settings);
bool connected;
int NwkThreadRet;
time_t lastUpdateFromServer;
pthread_t NwkThread;
pthread_t NwkThreadSender;
#endif // SOCKET_H
