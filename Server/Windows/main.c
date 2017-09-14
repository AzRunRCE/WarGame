#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <winsock2.h>
#include "include\server.h"
#include "include\client.h"
#include "pb.h"
#include "pb_common.h"
#include "pb_encode.h"
#include "pb_decode.h"
#include "unionproto.pb.h"
#include "pb_functions.h"
#define PORT 8080
#define MAX_BUFFER 4096
#define SERVER "127.0.0.1"
#ifdef _WIN32
#define SOCKET_ERRNO	WSAGetLastError()
#else
#define SOCKET_ERRNO	errno
#endif
pthread_t NwkThread;
//ServerGame CurrentGame;
uint8_t currentGameBuffer[MAX_BUFFER];
pthread_cond_t condition = PTHREAD_COND_INITIALIZER; /* Création de la condition */
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; /* Création du mutex */

Client clients[MAX_CLIENTS];
BulletElm *headBulletList;
Player Players[MAX_CLIENTS];
int actual = 0;
SOCKET sock;
typedef void(*callback)(BulletElm* head_bulletList);
bool list_bullet;
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


BulletElm *initBullet(BulletElm* bullet)
{
	bullet->x0 = bullet->pos.x;
	bullet->y0 = bullet->pos.y;
	bullet->x1 = bullet->dest.x;
	bullet->y1 = bullet->dest.y;
	bullet->dX = abs(bullet->x1 - bullet->x0), bullet->sX = bullet->x0 < bullet->x1 ? 1 : -1;
	bullet->dY = abs(bullet->y1 - bullet->y0), bullet->sY = bullet->y0< bullet->y1 ? 1 : -1;
	bullet->err = (bullet->dX>bullet->dY ? bullet->dX : -bullet->dY) / 2;
	bullet->pos.h = 6;
	bullet->pos.w = 6;

	return bullet;
}
void incrementBullet(BulletElm *bullet) {
	

	bullet->pos.y = bullet->y0;
	bullet->pos.x = bullet->x0;

	for (size_t i = 0; i < 3; i++)
	{
		bullet->e2 = bullet->err;
		if (bullet->e2 > -bullet->dX)
		{
			bullet->err -= bullet->dY;
			bullet->x0 += bullet->sX;
		}
		if (bullet->e2 < bullet->dY)
		{
			bullet->err += bullet->dX;
			bullet->y0 += bullet->sY;
		}
	}


}

void checkbulletExceed(BulletElm *bullet)
{
	if (bullet !=NULL && bullet->next != NULL)
	{
		BulletElm* bulletNext = bullet->next;
		if (bulletNext->x0 > 1600 || bulletNext->x0 < 0 || bulletNext->y0 > 1600 || bulletNext->y0 < 0)
		{
			bullet->next = bulletNext->next;
			free(bulletNext);
		}
	}
}
bool listBullets_callback(pb_ostream_t *stream, const pb_field_t *field, void * const *arg)
{
		BulletElm* cursor = headBulletList;
		int count = 0;
		while (cursor != NULL)
		{
			if (!pb_encode_tag_for_field(stream, field))
				return false;
			BulletMessage* tmpBulletMsg = malloc(sizeof(BulletMessage));
			tmpBulletMsg->dest = cursor->dest;
			tmpBulletMsg->pos = cursor->pos;
			tmpBulletMsg->id = count;
			tmpBulletMsg->ownerId = cursor->ownerId;
			if (!pb_encode_submessage(stream, BulletMessage_fields, tmpBulletMsg))
				return false;
			free(tmpBulletMsg);
			count++;
			cursor = cursor->next;
		}
		//printf("cursorCount: %d\n", count); // Debug print for count
	return true;
}

bool listPlayers_callback(pb_ostream_t *stream, const pb_field_t *field, void * const *arg)
{
	list_bullet = true;
	for (int i =0; i < actual;i++)
	{	
	
		
		/* This encodes the header for the field, based on the constant info
		* from pb_field_t. */
		if (!pb_encode_tag_for_field(stream, field))
			return false;

		/* This encodes the data for the field, based on our FileInfo structure. */
		if (!pb_encode_submessage(stream, Player_fields, &Players[i]))
			return false;
	}

	return true;
}

BulletElm* create(BulletMessage *bulletMsg, BulletElm* next)
{
	BulletElm* new_node = (BulletElm*)malloc(sizeof(BulletElm));
	if (new_node == NULL)
	{
		printf("Error creating a new node.\n");
		exit(0);
	}
	
	new_node->next = next;
	new_node->ownerId = bulletMsg->ownerId;
	new_node->pos = bulletMsg->pos;
	new_node->dest = bulletMsg->dest;
	new_node->dest.h = new_node->dest.w = new_node->pos.h = new_node->pos.w = 0;

	new_node = initBullet(new_node);
	return new_node;
}

void browse(BulletElm* head, callback f)
{
	BulletElm* cursor = head;
	if (cursor != NULL && (cursor->x0 > 1600 || cursor->x0 < 0 || cursor->y0 > 1600 || cursor->y0 < 0))
	{
		if (cursor->next != NULL)
		{
			headBulletList = cursor->next;
			free(cursor);
			cursor = NULL;
		}
		else
		{
			free(cursor);
			cursor = NULL;
			headBulletList = NULL;
		}

	}
	while (cursor != NULL)
	{
		f(cursor);
		cursor = cursor->next;
	}
}
BulletElm* pushBullet(BulletElm* head, BulletMessage *bulletMsg)
{
	if (head == NULL)	{
		head = create(bulletMsg, NULL);
		return head;
	}
	/* go to the last node */
	BulletElm *cursor = head;
	while (cursor->next != NULL)
		cursor = cursor->next;

		
	/* create a new node */
	BulletElm* new_node = create(bulletMsg, NULL);
	cursor->next = new_node;

	return head;
}

static void app(void)
{
	bool status = true;
	sock = init_connection();
	char buffer[BUF_SIZE];
	/* the index for the array */
	int max = sock;
	/* an array for all clients */

	while (true)
	{
		SOCKADDR_IN csin = { 0 };
		/* a client is talking */
		uint8_t buffer[MAX_BUFFER];
		int count = read_client(sock, &csin, buffer);
		pb_istream_t stream = pb_istream_from_buffer(buffer, count);
		const pb_field_t *type = decode_unionmessage_type(&stream);

		if (type == ConnectionMessage_fields && check_if_client_exists(clients, &csin, actual) == 0)
		{
			ConnectionMessage connectionMessage;
			status = decode_unionmessage_contents(&stream, ConnectionMessage_fields, &connectionMessage);
			if (actual != MAX_CLIENTS && status)
			{
				Client c = { csin };
				c.id = actual;
				clients[actual] = c;
				strncpy(Players[actual].name, connectionMessage.name, sizeof(Players[actual].name));
				connectionMessage.name[sizeof(connectionMessage.name) - 1] = '\0';
				ConnectionCallbackMessage callBackMessage;
				callBackMessage.clientId = actual;
				strncpy(callBackMessage.motd, "Bienvenue sur mon serveur", sizeof(callBackMessage.motd));
				callBackMessage.motd[sizeof(callBackMessage.motd) - 1] = '\0';
				callBackMessage.sucess = true;
				uint8_t callback_buffer[ConnectionCallbackMessage_size];
				pb_ostream_t output = pb_ostream_from_buffer(callback_buffer, sizeof(callback_buffer));
				status = encode_unionmessage(&output, ConnectionCallbackMessage_fields, &callBackMessage);
				write_client(sock, &csin, callback_buffer, output.bytes_written);
				printf("%s connected", connectionMessage.name);
				actual++;

			}

			/*	int pos = get_client_pos(clients, &csin, actual);
				array_remove(clients, MAX_CLIENTS, pos, 1);
				printf("player disconnected \n");
				actual--;*/
		}
		else if (type == BulletMessage_fields)
		{
			BulletMessage *bulletMsg = malloc(sizeof(BulletMessage));
			status = decode_unionmessage_contents(&stream, BulletMessage_fields, bulletMsg);
			printf("BulletMessage name:%s x:%d y:%d\n",Players[bulletMsg->ownerId].name, bulletMsg->pos.x, bulletMsg->pos.y);
			headBulletList = pushBullet(headBulletList, bulletMsg);
			free(bulletMsg);
	
		}
		else if (type == Player_fields)
		{
			Player PlayerMessage;			
			status = decode_unionmessage_contents(&stream, Player_fields, &PlayerMessage);
			Client *client = get_client(clients, &csin, actual);
			if (client == NULL) continue;
			memcpy(&Players[PlayerMessage.id], &PlayerMessage, sizeof(PlayerMessage));
			GameDataMessage *gameDataMessage = malloc(sizeof(GameDataMessage));
			gameDataMessage->GameMode = 1;
			gameDataMessage->playersCount = actual;
			gameDataMessage->players.funcs.encode = &listPlayers_callback;
			gameDataMessage->bullets.funcs.encode = &listBullets_callback;
			pb_ostream_t output = pb_ostream_from_buffer(currentGameBuffer, MAX_BUFFER);
			bool status = encode_unionmessage(&output, GameDataMessage_fields, gameDataMessage);
			int n = write_client(sock, &csin, currentGameBuffer, output.bytes_written);
			free(gameDataMessage);
		}

		browse(headBulletList, &checkbulletExceed);
		browse(headBulletList, &incrementBullet);
		
		
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

static void send_message_to_all_clients(int sock, Client *clients, Client *sender, int actual, uint8_t *buffer,int length)
{
	for (int i = 0; i < actual; i++)
	{
		if (sender != &clients[i])
		{
			write_client(sock, &clients[i].sin, buffer,length);
		}
	}

}


static void end_connection(int sock)
{
	closesocket(sock);
}

static int read_client(SOCKET sock, SOCKADDR_IN *sin, uint8_t *buffer)
{
	int n = 0;
	size_t sinsize = sizeof *sin;

	if ((n = recvfrom(sock, buffer, MAX_BUFFER, 0, (SOCKADDR *)sin, &sinsize)) < 0)
	{
		perror("recvfrom()");
		
	}


	return n;
}

static int write_client(SOCKET sock, SOCKADDR_IN *sin, const uint8_t *buffer, const int length)
{
	int n = 0;
	if ((n = sendto(sock, buffer, length, 0, (SOCKADDR *)sin, sizeof *sin))< 0)
	{
		perror("send()");
		
	}
	return n;
}


int main(int argc, char **argv)
{
	//ft_LoadMap("map/first.bmp", &CurrentGame.map);


	app();

	//end();

	return EXIT_SUCCESS;
}
