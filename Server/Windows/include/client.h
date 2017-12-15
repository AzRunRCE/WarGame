#ifndef CLIENT_H
#define CLIENT_H

#include <SDL.h>
#include <errno.h>
#include "unionproto.pb.h"
#define SLEEPDISCONNECTTHREADING 100
#ifdef _WIN32 || _WIN64 /* si vous êtes sous Windows */
#include <winsock2.h>
#define SOCKET_ERRNO WSAGetLastError()
#define SLEEP100MS Sleep(SLEEPDISCONNECTTHREADING)
#elif defined linux || defined __linux || defined __linux__ /* si vous êtes sous linux */
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
#define SLEEP100MS usleep(SLEEPDISCONNECTTHREADING*1000)

#else /* sinon vous êtes sur une plateforme non supportée */
#error not defined for this platform
#endif
#define MAX_CLIENTS 4
#define MAX_BUFFER 4096
#define PORT	 	1977
#define SERVER "127.0.0.1"
typedef struct _Client
{
    SOCKADDR_IN sin;
	uint8_t id;
	time_t lastUpdate;
}Client;

Client clients[MAX_CLIENTS];
Player Players[MAX_CLIENTS];

int check_if_client_exists(Client *clients, SOCKADDR_IN *csin, int playerCount);
Client* get_client(Client *clients, SOCKADDR_IN *csin, int playerCount);
int get_client_pos(Client *clients, SOCKADDR_IN *csin, int actual);
void array_remove(Client* arr, size_t size, size_t index, size_t rem_size);
void remove_client(Client *clients, int to_remove, int *actual);
void send_message_to_all_clients(int sock, Client *clients, Client *sender, int actual, uint8_t *buffer, int length);
void end_connection(int sock);
int read_client(SOCKET sock, SOCKADDR_IN *sin, uint8_t *buffer);
int write_client(SOCKET sock, SOCKADDR_IN *sin, const uint8_t *buffer, const int length);
//int playerCount;
#endif /* guard */
