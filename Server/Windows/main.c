#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
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
#include <SDL_timer.h>
#include "include/server.h"
#include "include/client.h"
#include "include/ft_map.h"
#include "include/pb.h"
#include "include/ft_state.h"
#include "include/pb_common.h"
#include "include/pb_encode.h"
#include "include/pb_decode.h"
#include "include/unionproto.pb.h"
#include "include/pb_functions.h"
#include "include/ft_item.h"
#define MAX_BUFFER 4096
#define SERVER "127.0.0.1"
#define BLOCK_SIZE 32
#ifdef WIN32
#define SOCKET_ERRNO	WSAGetLastError()
#else
#define SOCKET_ERRNO	errno
#endif


Client clients[MAX_CLIENTS];
BulletElm *headBulletList;
Item *headItemList = NULL;
Player Players[MAX_CLIENTS];
int actual = 0;
SOCKET sock;
typedef void(*callback)(BulletElm* head_bulletList);
void updatePlayer(PlayerBase *playerBase);
bool playerIsAlive(PlayerBase *playerBase);
bool list_bullet;
Map *map;
int lastInc = 0;

 int init_connection(void)
{
#ifdef _WIN32
	WSADATA WSAData;                    // Contains details of the 
										// Winsock implementation
										// Initialize Winsock. 
	if (WSAStartup(MAKEWORD(1, 1), &WSAData) != 0)
	{
		printf("WSAStartup failed! Error: %d\n", SOCKET_ERRNO);
		return false;
	}
#endif
	/* UDP so SOCK_DGRAM */
	SOCKET sock;
	SOCKADDR_IN sin = { 0 };
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		printf("Allocating socket failed! Error: %d\n", SOCKET_ERRNO);
		return false;
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
BulletElm* remove_front(BulletElm* head)
{
	if (head == NULL)
		return NULL;
	BulletElm *front = head;
	head = head->next;
	front->next = NULL;
	/* is this the last node in the list */
	if (front == head)
		head = NULL;
	free(front);
	return head;
}
BulletElm* remove_back(BulletElm* head)
{
	if (head == NULL)
		return NULL;

	BulletElm *cursor = head;
	BulletElm *back = NULL;
	while (cursor->next != NULL)
	{
		back = cursor;
		cursor = cursor->next;
	}
	if (back != NULL)
		back->next = NULL;

	/* if this is the last node in the list*/
	if (cursor == head)
		head = NULL;

	free(cursor);

	return head;
}
BulletElm* remove_any(BulletElm* head, BulletElm* nd)
{
	BulletElm* cursor = head;
	/* if the node is the first node */
	if (nd == head)
	{
		head = remove_front(head);
		return head;
	}

	/* if the node is the last node */
	if (nd->next == NULL)
	{
		head = remove_back(head);
		return head;
	}
	while (cursor != NULL)
	{
		if (cursor->next == nd)
			break;
		cursor = cursor->next;
	}
	if (cursor != NULL)
	{
		BulletElm* tmp = cursor->next;
		cursor->next = tmp->next;
		tmp->next = NULL;
		free(tmp);
	}
	
	return head;
}

void incrementBullet(BulletElm* headBullets)
{
	BulletElm* bullet = headBullets;
	while (bullet != NULL)
	{
		Item *currentItem = &map->data[(int)bullet->pos.y / BLOCK_SIZE][(int)(bullet->pos.x) / BLOCK_SIZE];
		BulletElm* next = bullet->next;

		if (currentItem->type == WALL)
			headBulletList = remove_any(headBulletList, bullet);
		else if (currentItem->type != BLANK)
		{
			Player *player = (Player*)currentItem->data;
			if (player->playerBase.id != bullet->ownerId && playerIsAlive(&player->playerBase) && checkCollision(&bullet->pos, &player->playerBase.pos))
			{
					player->playerBase.health -= 10;
					headBulletList = remove_any(headBulletList, bullet);
			}
		}
		if (bullet != NULL)
		{

			bullet->pos.y = bullet->y0;
			bullet->pos.x = bullet->x0;
			bullet->e2 = bullet->err;
			for (size_t i = 0; i < 3; i++)
			{
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
	
		bullet = next;
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
			BulletMessage tmpBulletMsg;
			tmpBulletMsg.dest = cursor->dest;
			tmpBulletMsg.pos = cursor->pos;
			tmpBulletMsg.id = count;
			tmpBulletMsg.ownerId = cursor->ownerId;
			if (!pb_encode_submessage(stream, BulletMessage_fields, &tmpBulletMsg))
				return false;
			count++;
			cursor = cursor->next;
		}
	return true;
}

bool listPlayers_callback(pb_ostream_t *stream, const pb_field_t *field, void * const *arg)
{
	for (int i =0; i < actual;i++)
	{	
	
		
		/* This encodes the header for the field, based on the constant info
		* from pb_field_t. */
		if (!pb_encode_tag_for_field(stream, field))
			return false;

		/* This encodes the data for the field, based on our FileInfo structure. */
		if (!pb_encode_submessage(stream, PlayerBase_fields, &Players[i].playerBase))
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

Item* pushItem(Item* head, Item *new_node)
{
	if (head == NULL) {
		head = new_node;
		return head;
	}
	/* go to the last node */
	Item *cursor = head;
	while (cursor->next != NULL)
		cursor = cursor->next;
	cursor->next = new_node;
	return head;
}

int countItemWall(Item* head)
{
	int total = 0;
	if (head == NULL) {
		return 0;
	}
	/* go to the last node */
	Item *cursor = head;
	while (cursor != NULL)
	{
		total++;
		if (cursor->next == NULL)
			break;
		cursor = cursor->next;
	}
	return total;
}

void app(void)
{
	bool status = true;
	sock = init_connection();
	/* the index for the array */
	int max = sock;
	/* an array for all clients */
	ConnectionCallbackMessage *callBackMessage = calloc(1, sizeof(ConnectionCallbackMessage));
	strncpy(callBackMessage->motd, "Bienvenue sur mon serveur", sizeof(callBackMessage->motd));
	callBackMessage->motd[sizeof(callBackMessage->motd) - 1] = '\0';
	callBackMessage->sucess = true;

	SpawnList[0].x = 1002;
	SpawnList[0].y = 624;
	SpawnList[1].x = 1203;
	SpawnList[1].y = 572;
	SpawnList[2].x = 1282;
	SpawnList[2].y = 930;
	SpawnList[3].x = 1030;
	SpawnList[3].y = 316;
	SpawnList[4].x = 636;
	SpawnList[4].y = 284;
	SpawnList[5].x = 636;
	SpawnList[5].y = 479;
	SpawnList[6].x = 558;
	SpawnList[6].y = 623;
	SpawnList[7].x = 490;
	SpawnList[7].y = 876;
	SpawnList[8].x = 386;
	SpawnList[8].y = 1176;
	SpawnList[9].x = 1192;
	SpawnList[9].y = 1202;

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
				Players[actual].playerBase.health = 100;
				Players[actual].playerBase.id = actual;
				callBackMessage->clientId = actual;
				
				uint8_t callback_buffer[ConnectionCallbackMessage_size];
				pb_ostream_t output = pb_ostream_from_buffer(callback_buffer, sizeof(callback_buffer));
				status = encode_unionmessage(&output, ConnectionCallbackMessage_fields, callBackMessage);
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

			BulletMessage bulletMsg;
			status = decode_unionmessage_contents(&stream, BulletMessage_fields, &bulletMsg);
			if (playerIsAlive(&Players[bulletMsg.ownerId])) {
				printf("BulletMessage name:%s x:%d y:%d\n", Players[bulletMsg.ownerId].name, bulletMsg.pos.x, bulletMsg.pos.y);
				headBulletList = pushBullet(headBulletList, &bulletMsg);
			}
		}
		else if (type == SpawnMessage_fields)
		{
			SpawnMessage spawnMsg;
			status = decode_unionmessage_contents(&stream, SpawnMessage_fields, &spawnMsg);
			printf("Spawn request: %s\n", Players[spawnMsg.id].name);
			Players[spawnMsg.id].playerBase.health = 100;
			Players[spawnMsg.id].playerBase.ammo = 30;
			int random;
			srand(time(NULL)); // initialisation de rand
			random = (rand() % (9 + 1));		
			SpawnCallbackMessage spawnCallbackMsg;
			uint8_t spCallbackBuffer[MAX_BUFFER];
			spawnCallbackMsg.id = spawnMsg.id;
			spawnCallbackMsg.x = SpawnList[random].x;
			spawnCallbackMsg.y = SpawnList[random].y;
			pb_ostream_t output = pb_ostream_from_buffer(spCallbackBuffer, sizeof(spCallbackBuffer));
			encode_unionmessage(&output, SpawnCallbackMessage_fields, &spawnCallbackMsg);
			write_client(sock, &csin, spCallbackBuffer, output.bytes_written);

		}
		else if (type == PlayerBase_fields)
		{
			PlayerBase pMessage;
			status = decode_unionmessage_contents(&stream, PlayerBase_fields, &pMessage);
			Client *client = get_client(clients, &csin, actual);
			if (client == NULL) continue;
			if (Players[pMessage.id].playerBase.pos.y || Players[pMessage.id].playerBase.pos.x) {
				map->data[(int)Players[pMessage.id].playerBase.pos.y / BLOCK_SIZE][(int)(Players[pMessage.id].playerBase.pos.x) / BLOCK_SIZE].type = BLANK;
				map->data[(int)Players[pMessage.id].playerBase.pos.y / BLOCK_SIZE][(int)(Players[pMessage.id].playerBase.pos.x) / BLOCK_SIZE].data = NULL;
				updatePlayer(&pMessage);
				map->data[(int)pMessage.pos.y / BLOCK_SIZE][(int)(pMessage.pos.x) / BLOCK_SIZE].type = PLAYER;
				map->data[(int)pMessage.pos.y / BLOCK_SIZE][(int)(pMessage.pos.x) / BLOCK_SIZE].data = &Players[pMessage.id];
			}
			GameDataMessage gameDataMessage;
			uint8_t currentGameBuffer[MAX_BUFFER];
			gameDataMessage.gameMode = 1;
			gameDataMessage.playersCount = actual;
			gameDataMessage.players.funcs.encode = &listPlayers_callback;
			gameDataMessage.bullets.funcs.encode = &listBullets_callback;
			pb_ostream_t output = pb_ostream_from_buffer(currentGameBuffer, sizeof(currentGameBuffer));
			encode_unionmessage(&output, GameDataMessage_fields, &gameDataMessage);
			write_client(sock, &csin, currentGameBuffer, output.bytes_written);
			
		}

		if (headBulletList != NULL && ft_delay(&lastInc, 5))
		{
			incrementBullet(headBulletList);
		}
	}
	end_connection(sock);
}


void updatePlayer(PlayerBase *playerBase){
	if (playerIsAlive(playerBase)){
		Players[playerBase->id].playerBase.orientation = playerBase->orientation;
		Players[playerBase->id].playerBase.pos = playerBase->pos;
	}
	Players[playerBase->id].playerBase.state = playerBase->state;
	// Update player state even if he is dead !
}
bool playerIsAlive(PlayerBase *playerBase) {
	return playerBase->state != DEAD;
}

 int check_if_client_exists(Client *clients, SOCKADDR_IN *csin, int actual)
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

Client* get_client(Client *clients, SOCKADDR_IN *csin, int actual)
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

int get_client_pos(Client *clients, SOCKADDR_IN *csin, int actual)
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
void array_remove(Client* arr, size_t size, size_t index, size_t rem_size)
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

 void remove_client(Client *clients, int to_remove, int *actual)
{
	/* we remove the client in the array */
	memmove(clients + to_remove, clients + to_remove + 1, (*actual - to_remove) * sizeof(Client));
	/* number client - 1 */
	(*actual)--;
}

 void send_message_to_all_clients(int sock, Client *clients, Client *sender, int actual, uint8_t *buffer,int length)
{
	for (int i = 0; i < actual; i++)
	{
		if (sender != &clients[i])
		{
			write_client(sock, &clients[i].sin, buffer,length);
		}
	}

}
 
 void end_connection(int sock)
{
	closesocket(sock);
}

 int read_client(SOCKET sock, SOCKADDR_IN *sin, uint8_t *buffer)
{
	int n = 0;
	size_t sinsize = sizeof *sin;

	if ((n = recvfrom(sock, buffer, MAX_BUFFER, 0, (SOCKADDR *)sin, &sinsize)) < 0)
	{
		perror("recvfrom()");
		
	}


	return n;
}

 int write_client(SOCKET sock, SOCKADDR_IN *sin, const uint8_t *buffer, const int length)
{
	int n = 0;
	if ((n = sendto(sock, buffer, length, 0, (SOCKADDR *)sin, sizeof *sin))< 0)
	{
		perror("send()");
		
	}
	return n;
}

 bool ft_delay(int *last, int SleepTimeAnim)
{
	int ActualTimeAnim = SDL_GetTicks();
	if (ActualTimeAnim - *last > SleepTimeAnim)
	{
		*last = ActualTimeAnim;
		return true;
	}
	else
		return false;
}



 int main(int argc, char **argv)
{
	map = malloc(sizeof(Map));
	ft_LoadMap("map/first.bmp", map);;


	int i = 0;
	int j = 0;
	while (j < map->heigth)
	{
		while (i < map->width)
		{
			Item *item = malloc(sizeof(Item));
			item->rect = malloc(sizeof(SDL_Rect));
			while (map->data[i][j].type != WALL && i < map->width)
				i++;
			item->next = NULL;
			item->type = WALL;
			item->rect->h = 32;
			item->rect->w = 0;
			item->rect->x = i * 32;
			item->rect->y = j * 32;
			bool findWall = false;
			while (map->data[i][j].type == WALL  && i < map->width)
			{
				findWall = true;
				item->rect->w += 32;
				i++;
			}
			if (findWall)
				headItemList = pushItem(headItemList, item);
		}
		j++;
		i = 0;
	}
	printf("%d", countItemWall(headItemList));
	app();

	//end();

	return EXIT_SUCCESS;
}
