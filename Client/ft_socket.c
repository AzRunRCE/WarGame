#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "include\ft_socket.h"
#include "include\ft_client.h"
#include "include\ft_configuration.h"
#include "include\ft_map.h"
#include "include\ft_engine.h"
#include "include\pb.h"
#include "include\pb_common.h"
#include "include\pb_encode.h"
#include "include\pb_decode.h"
#include "include\unionproto.pb.h"
#include "include\pb_functions.h"
#include "include\ft_configuration.h"

#define PORT 1977
#define MAX_BUFFER 4096
#ifdef _WIN32
#define SOCKET_ERRNO	WSAGetLastError()
#else
#define SOCKET_ERRNO	errno
#endif
SOCKET sock;
pthread_t NwkThread;
pthread_t NwkThreadSender;
SOCKADDR_IN *psin;
int clientId;
uint8_t buffer[MAX_BUFFER];
BulletElm* create(BulletMessage *bulletMessage, BulletElm* next);
BulletElm* appendBullet(BulletElm* head, BulletMessage *bulletMessage);

void end()
{
	/*ClientPacket w;
	strcpy(w.clientPlayer.name, "\0");
	write_server(sock, psin, w);*/
#ifdef WIN32
	WSACleanup();
#endif
}

int init_connection(const char *address, SOCKADDR_IN *sin)
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
	
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	struct hostent *hostinfo;

	if (sock == INVALID_SOCKET)
	{
		perror("socket()");
		exit(errno);
	}

	hostinfo = gethostbyname(address);
	if (hostinfo == NULL)
	{
		fprintf(stderr, "Unknown host %s.\n", address);
		exit(EXIT_FAILURE);
	}

	sin->sin_addr = *(IN_ADDR *)hostinfo->h_addr;
	sin->sin_port = htons(PORT);

	return sock;
}

bool readBullets_callback(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
	BulletMessage *bullet = malloc(sizeof(BulletMessage));

	if (!pb_decode(stream, BulletMessage_fields, bullet))
		return false;

	headBullets = appendBullet(headBullets, bullet);
	free(bullet);
	return true;
}

void end_connection(int sock)
{
	closesocket(sock);
}

void dispose(BulletElm *head)
{
	BulletElm *cursor, *tmp;

	if (head != NULL)
	{
		cursor = head->next;
		head->next = NULL;
		while (cursor != NULL)
		{
			tmp = cursor->next;
			free(cursor);
			cursor = tmp;
		}
	}
}

BulletElm* create(BulletMessage *bulletMessage, BulletElm* next)
{

		BulletElm* new_node = (BulletElm*)malloc(sizeof(BulletElm));
		if (new_node == NULL)
		{
			printf("Error creating a new node.\n");
			exit(0);
		}
		new_node->pos = bulletMessage->pos;
		new_node->next = next;

		return new_node;
}

BulletElm* appendBullet(BulletElm* head, BulletMessage *bulletMessage)
{
	/* go to the last node */
	if (head == NULL)
		return create(bulletMessage, NULL);
	BulletElm *cursor = head;
	while (cursor->next != NULL)
		cursor = cursor->next;

	/* create a new node */
	BulletElm* new_node = create(bulletMessage, NULL);
	cursor->next = new_node;

	return head;
}

int create_connection(configuration *settings)
{
	configuration *mainConfiguration;
	mainConfiguration = ft_loadConf();
	bool status = false;
	psin = malloc(sizeof(SOCKADDR_IN));
	psin->sin_family = AF_INET;
	sock = init_connection(mainConfiguration->server,psin);
	
	uint8_t buffer[MAX_BUFFER];

	ConnectionMessage connectionMessage;
	strncpy(_engine.mainPlayer.name, settings->nickname, sizeof(_engine.mainPlayer.name));
	strncpy(connectionMessage.name, settings->nickname,sizeof(connectionMessage.name));
	connectionMessage.name[sizeof(connectionMessage.name) - 1] = '\0';


	pb_ostream_t output = pb_ostream_from_buffer(buffer, sizeof(buffer));
	status = encode_unionmessage(&output, ConnectionMessage_fields, &connectionMessage);
	int c = write_client(buffer,output.bytes_written);
	
	/*_engine.map = &_engine.currentGame->map;
	_engine.mainPlayer.id = _engine.currentGame->clientId;*/
	

	//FILE * fp;

	//fp = fopen("file.txt", "w+");
	////int j = 0; j <_engine.map->width; j++)
	//for (int i = 0; i < _engine.map->heigth; i++)
	//{
	//	for (int j = 0; j <_engine.map->width; j++)
	//	{
	//		fprintf(fp, "%c", _engine.map->data[i][j]);
	//	}
	//	fprintf(fp, "\n");
	//}



	//fclose(fp);

	if (pthread_create(&NwkThread, NULL, NetworkThreadingListening, NULL) == -1) {
		perror("pthread_create");
		return EXIT_FAILURE;
	}
	
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
 int write_client(const char *buffer, const int length)
{
	int n = 0;
	if ((n = sendto(sock, buffer, length, 0, (SOCKADDR *)psin, sizeof *psin))< 0)
	{
		perror("send()");
		exit(errno);
	}
	return n;
}

int sendMessage(const char *buffer, const int length)
{
	return write_client(buffer, length);
}

bool readPlayers_callback(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
	Player PlayerInfo;
	if (!pb_decode(stream, Player_fields, &PlayerInfo))
		return false;
	
	
	if (_engine.mainPlayer.id != PlayerInfo.id)
	{
		_engine.players[PlayerInfo.id] = PlayerInfo;
		_engine.players[PlayerInfo.id].Pos.x -= _engine.camera.x;
		_engine.players[PlayerInfo.id].Pos.y -= _engine.camera.y;

	}
	return true;
}


void *NetworkThreadingListening(void *arg)
{
	uint8_t buffer[MAX_BUFFER];
	bool status = true;
	while (true)
	{
		int count = read_client(sock, psin, buffer);

		pb_istream_t stream = pb_istream_from_buffer(buffer, count);
		const pb_field_t *type = decode_unionmessage_type(&stream);
		if (type == ConnectionCallbackMessage_fields)
		{
			ConnectionCallbackMessage callback;	
			 status = decode_unionmessage_contents(&stream, ConnectionCallbackMessage_fields, &callback);
			if (callback.sucess)
			{
				printf("ConnectionCallbackMessage motd:%s", callback.motd);
			_engine.mainPlayer.id = callback.clientId;
			if (pthread_create(&NwkThreadSender, NULL, SreamClientData, NULL) == -1) {
				perror("pthread_create");
				return EXIT_FAILURE;
			}
			}
		}
		
		else if (type == GameDataMessage_fields)
		{
			dispose(headBullets);
			GameDataMessage gameData;
			gameData.players.funcs.decode = &readPlayers_callback;
			gameData.bullets.funcs.decode = &readBullets_callback;
			status = decode_unionmessage_contents(&stream, GameDataMessage_fields, &gameData);
			
			_engine.playersCount = gameData.playersCount;
		}
		/*else if (type == Player_fields)
		{
			Player player;
			player.bullets.funcs.decode = &readPlayerBullets_callback;

			status = decode_unionmessage_contents(&stream, Player_fields, &player);
		}*/
	}
	pthread_exit(NULL);
}

void *SreamClientData(void *arg)
{
	while (true)
	{
		Player *PlayerMessage = malloc(sizeof(Player));
		memcpy(PlayerMessage, &_engine.mainPlayer, sizeof(Player));
		if (_engine.mainPlayer.Pos.x <= _engine.WIDTH / 2 - 16 || _engine.mainPlayer.Pos.y <= _engine.HEIGHT / 2 - 16 || _engine.mainPlayer.Pos.x + _engine.WIDTH / 2 + 16 >= _engine.mapSurface->h || _engine.mainPlayer.Pos.y + _engine.HEIGHT / 2 + 16 >= _engine.mapSurface->h)
		{
				PlayerMessage->Pos.x += _engine.pCenter.x - _engine.WIDTH / 2 + 16;
				PlayerMessage->Pos.y += _engine.pCenter.y - _engine.HEIGHT / 2 + 16;
		}
		PlayerMessage->Pos.w = 32;
		PlayerMessage->Pos.h = 32;

		pb_ostream_t output = pb_ostream_from_buffer(buffer, sizeof(buffer));
		bool status = encode_unionmessage(&output, Player_fields, PlayerMessage);
		int c = write_client(buffer, output.bytes_written);
		write_client(buffer, output.bytes_written);
		Sleep(15);
		free(PlayerMessage);
	}
	pthread_exit(NULL);
}
