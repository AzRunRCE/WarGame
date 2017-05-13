#ifndef SERVER_H
#define SERVER_H

#ifdef WIN32

#include <winsock2.h>

#elif defined (linux)

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

#else

#error not defined for this platform

#endif

#define CRLF		"\r\n"
#define PORT	 	1977
#define MAX_CLIENTS 	100

#define BUF_SIZE	1024

#include "client.h"

static void init(void);
static void end(void);
static void app(void);
static int init_connection(void);
static void end_connection(int sock);
static int read_client(SOCKET sock, SOCKADDR_IN *csin, char *buffer);
static void write_client(SOCKET sock, SOCKADDR_IN *csin, const char *buffer);
static void send_message_to_all_clients(int sock, Client *clients, Client *client, int actual, const char *buffer, char from_server);
static void remove_client(Client *clients, int to_remove, int *actual);
static int check_if_client_exists(Client *clients, SOCKADDR_IN *csin, int actual);
static Client* get_client(Client *clients, SOCKADDR_IN *csin, int actual);
static int get_client_pos(Client *clients, SOCKADDR_IN *csin, int actual);
static void ft_drop_element(Client *clients,int at, int actual );
static void array_remove (Client* arr, size_t size, size_t index, size_t rem_size);

#endif /* guard */
