#ifndef SERVER_H
#define SERVER_H
#define HAVE_STRUCT_TIMESPEC
#include <winsock2.h>
#include <pthread.h>
#define CRLF		"\r\n"
#define PORT	 	1977
#define MAX_CLIENTS 	100
#define BUF_SIZE	1024
typedef int bool;
#define true 1
#define false 0
#include "ft_map.h"
#include "client.h"
#include "ft_packet.h"
void *NetworkThreading(void *arg);
static void init(void);
static void end(void);
static void app(void);
static int init_connection(void);
static void end_connection(int sock);
static ClientPacket read_client(SOCKET sock, SOCKADDR_IN *sin);
static void write_client(SOCKET sock, SOCKADDR_IN *csin, ServerPacket packet);
static void write_client_map(SOCKET sock, SOCKADDR_IN *sin, Map *map);
static void send_message_to_all_clients(int sock, Client *clients, Client *client, int actual, ServerPacket packet, char from_server);
static void remove_client(Client *clients, int to_remove, int *actual);
static int check_if_client_exists(Client *clients, SOCKADDR_IN *csin, int actual);
static Client* get_client(Client *clients, SOCKADDR_IN *csin, int actual);
static void array_remove(Client* arr, size_t size, size_t index, size_t rem_size);
static int get_client_pos(Client *clients, SOCKADDR_IN *csin, int actual);
#endif /* guard */
