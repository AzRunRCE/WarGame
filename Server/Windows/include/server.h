#ifndef SERVER_H
#define SERVER_H
#ifdef _WIN32 || _WIN64 /* si vous êtes sous Windows */
#include <winsock2.h> 
#elif defined linux || defined __linux || defined __linux__ /* si vous êtes sous linux */
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

#else /* sinon vous êtes sur une plateforme non supportée */
#error not defined for this platform

#endif
#include <stdbool.h>
#include <sys/types.h>
#include "include/ft_map.h"
#include "include/pb.h"
#include "include/ft_state.h"
#include "include/pb_common.h"
#include "include/pb_encode.h"
#include "include/pb_decode.h"
#include "include/unionproto.pb.h"
#include "include/pb_functions.h"
#include "include/ft_item.h"
#include "include/ft_checkcollision.h"
#include "include/client.h"

#define CRLF		"\r\n"
#define PORT	 	1977
#define SERVER "127.0.0.1"
#define BUF_SIZE	1024
#define true 1
#define false 0
SDL_Rect SpawnList[9];
void end(void);
void app(void);
int init_connection(void);
void end_connection(int sock);
int read_client(SOCKET sock, SOCKADDR_IN *sin, uint8_t *buffer);
int write_client(SOCKET sock, SOCKADDR_IN *sin, const uint8_t *buffer, const int length);
void remove_client(Client *clients, int to_remove, int *actual);
int check_if_client_exists(Client *clients, SOCKADDR_IN *csin, int actual);
Client* get_client(Client *clients, SOCKADDR_IN *csin, int actual);
void array_remove(Client* arr, size_t size, size_t index, size_t rem_size);
int get_client_pos(Client *clients, SOCKADDR_IN *csin, int actual);
bool ft_delay(int *lastAnim, int SleepTimeAnim);
#endif /* guard */
