#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <winsock2.h>
#include "include\server.h"
#include "include\client.h"
#include "include\ft_map.h"
#include "include\ft_player.h"
#include "pb.h"
#include "pb_common.h"
#include "pb_encode.h"
#include "pb_decode.h"
#include "unionproto.pb.h"
#include "pb_functions.h"
#define PORT 8080
#define MAX_BUFFER 1500
#define SERVER "127.0.0.1"
#ifdef _WIN32
#define SOCKET_ERRNO	WSAGetLastError()
#else
#define SOCKET_ERRNO	errno
#endif
pthread_t NwkThread;
//ServerGame CurrentGame;

static int init_connection(void)
{
#ifdef _WIN32
	WSADATA WSAData;                    // Contains details of the 
										// Winsock implementation
										// Initialize Winsock. 
	if (WSAStartup(MAKEWORD(1, 1), &WSAData) != 0)
	{
		printf("WSAStartup failed! Error: %d\n", SOCKET_ERRNO);
		return FALSE;
	}
#endif
	/* UDP so SOCK_DGRAM */
	SOCKET sock;
	SOCKADDR_IN sin = { 0 };
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		printf("Allocating socket failed! Error: %d\n", SOCKET_ERRNO);
		return FALSE;
	}


	if (sock == INVALID_SOCKET)
	{
		perror("socket()");
		exit(errno);
	}

	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_port = htons(PORT);
	sin.sin_family = AF_INET;

	if (bind(sock, (SOCKADDR *)&sin, sizeof sin) == SOCKET_ERROR)
	{
		perror("bind()");
		exit(errno);
	}

	return sock;
}
Client clients[MAX_CLIENTS];
SDL_Rect Bullets[250];
Player Players[MAX_CLIENTS];
int actual = 0;
SOCKET sock;
static void app(void)
{
	sock = init_connection();
	char buffer[BUF_SIZE];
	/* the index for the array */
	int max = sock;
	/* an array for all clients */

	if (pthread_create(&NwkThread, NULL, NetworkThreading, NULL) == -1) {
		perror("pthread_create");
		return EXIT_FAILURE;
	}
	fd_set rdfs;

	while (1)
	{
		FD_ZERO(&rdfs);

		/* add STDIN_FILENO
		FD_SET(STDIN_FILENO, &rdfs);*/

		/* add the connection socket */
		FD_SET(sock, &rdfs);

		if (select(max + 1, &rdfs, NULL, NULL, NULL) == -1)
		{
			perror("select()");
			exit(errno);
		}

		/* something from standard input : i.e keyboard
		if(FD_ISSET(STDIN_FILENO, &rdfs))
		{
	   stop process when type on keyboard
	   break;
		}
		else  */
		if (FD_ISSET(sock, &rdfs))
		{
			/* new client */
			SOCKADDR_IN csin = { 0 };

			/* a client is talking */
			uint8_t buffer[MAX_BUFFER];
			int count = read_client(sock, &csin, buffer);
			pb_istream_t stream = pb_istream_from_buffer(buffer, count);
			const pb_field_t *type = decode_unionmessage_type(&stream);

			if (type == ConnectionMessage_fields)
			{
				if (check_if_client_exists(clients, &csin, actual) == 0)
				{
					if (actual != MAX_CLIENTS)
					{
						/*	Client c = { csin };
						CurrentGame.clientId = actual;
						strcpy(c.name, p.clientPlayer.name);

						printf("%s connected", c.name);
						clients[actual] = c;
						write_client_GameMode(sock, &c.sin, &CurrentGame);
						actual++;*/

					}
				}
			}
			//if (check_if_client_exists(clients, &csin, actual) == 0)
			//{
			//	if (actual != MAX_CLIENTS)
			//	{
			//	/*	Client c = { csin };
			//		CurrentGame.clientId = actual;
			//		strcpy(c.name, p.clientPlayer.name);

			//		printf("%s connected", c.name);
			//		clients[actual] = c;
			//		write_client_GameMode(sock, &c.sin, &CurrentGame);
			//		actual++;*/
			//		
			//	}
			//}
			//else
			//{

			///*	Client *client = get_client(clients, &csin, actual);
			//	int clientId = get_client_pos(clients, &csin, actual);

			//	if (client == NULL) continue;
			//	if (p.clientPlayer.name[0] == '\0')
			//	{
			//		int pos = get_client_pos(clients, &csin, actual);
			//		array_remove(clients, MAX_CLIENTS, pos, 1);
			//		printf("player disconnected \n");
			//		actual--;
			//	}
			//	Players[clientId] = p.clientPlayer;
			//	Players[clientId].id = clientId;*/
			//}
		}
	}

	end_connection(sock);
}

static int check_if_client_exists(Client *clients, SOCKADDR_IN *csin, int actual)
{
	int i = 0;
	for (i = 0; i < actual; i++)
	{
		if (clients[i].sin.sin_addr.s_addr == csin->sin_addr.s_addr
			&& clients[i].sin.sin_port == csin->sin_port)
		{
			return 1;
		}
	}

	return 0;
}

static Client* get_client(Client *clients, SOCKADDR_IN *csin, int actual)
{
	int i = 0;
	for (i = 0; i < actual; i++)
	{
		if (clients[i].sin.sin_addr.s_addr == csin->sin_addr.s_addr
			&& clients[i].sin.sin_port == csin->sin_port)
		{
			return &clients[i];
		}
	}

	return NULL;
}

static int get_client_pos(Client *clients, SOCKADDR_IN *csin, int actual)
{
	int i = 0;
	for (i = 0; i < actual; i++)
	{
		if (clients[i].sin.sin_addr.s_addr == csin->sin_addr.s_addr
			&& clients[i].sin.sin_port == csin->sin_port)
		{
			return i;
		}
	}

	return 0;
}
static void array_remove(Client* arr, size_t size, size_t index, size_t rem_size)
{
	int* begin = arr + index;                        // beginning of segment to remove
	int* end = arr + index + rem_size;               // end of segment to remove
	size_t trail_size = size - index - rem_size;       // size of the trailing items after segment

	memcpy(begin,                                  // move data to beginning
		end,                                    // from end of segment
		trail_size * sizeof(Client));

	memset(begin + trail_size,                       // from the new end of the array
		0,                                      // set everything to zero
		rem_size * sizeof(Client));

}

static void remove_client(Client *clients, int to_remove, int *actual)
{
	/* we remove the client in the array */
	memmove(clients + to_remove, clients + to_remove + 1, (*actual - to_remove) * sizeof(Client));
	/* number client - 1 */
	(*actual)--;
}

//static void send_message_to_all_clients(int sock, Client *clients, Client *sender, int actual, ServerPacket packet, char from_server)
//{
//	int i = 0;
//	char message[BUF_SIZE];
//	message[0] = 0;
//
//	for (i = 0; i < actual; i++)
//	{
//		/* we don't send message to the sender */
//		if (sender != &clients[i])
//		{
//			if (from_server == 0)
//			{
//				//  strncpy(message, sender->name, BUF_SIZE - 1);
//				 // strncat(message, " : ", sizeof message - strlen(message) - 1);
//			}
//			// strncat(message, buffer, sizeof message - strlen(message) - 1);
//			write_client(sock, &clients[i].sin, packet);
//		}
//	}
//
//}


static void end_connection(int sock)
{
	closesocket(sock);
}

static int read_client(SOCKET sock, SOCKADDR_IN *sin, char *buffer)
{
	int n = 0;
	size_t sinsize = sizeof *sin;

	if ((n = recvfrom(sock, buffer, MAX_BUFFER, 0, (SOCKADDR *)sin, &sinsize)) < 0)
	{
		perror("recvfrom()");
		exit(errno);
	}


	return n;
}

static int write_client(SOCKET sock, SOCKADDR_IN *sin, const uint8_t *buffer, const int length)
{
	int n = 0;
	if ((n = sendto(sock, buffer, length, 0, (SOCKADDR *)sin, sizeof *sin))< 0)
	{
		perror("send()");
		exit(errno);
	}
	return n;
}

void *NetworkThreading(void *arg)
{
	/*while (true)
	{
		ServerPacket *packet;
		packet = malloc(sizeof(ServerPacket));

		memcpy(packet->bullets, Bullets, sizeof Bullets);
		memcpy(packet->players, Players, sizeof Players);
		send_message_to_all_clients(sock, clients, NULL, actual, *packet, 0);
		Sleep(10);
	}
	pthread_exit(NULL);*/
}

int main(int argc, char **argv)
{
	//ft_LoadMap("map/first.bmp", &CurrentGame.map);


	app();

	//end();

	return EXIT_SUCCESS;
}
