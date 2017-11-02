#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#ifdef _WIN32 || _WIN64
/* si vous êtes sous Windows */
#include <pthread_VC.h>
#elif defined linux || defined __linux || defined __linux__
/* si vous êtes sous linux */
#include <pthread.h>
#else
/* sinon vous êtes sur une plateforme non supportée */
#error not defined for this platform
#endif
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "include/ft_socket.h"
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
#if defined linux || defined __linux || defined __linux__
#define Sleep sleep
#endif

#define h_addr h_addr_list[0] /* for backward compatibility */
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
		return false;
	}
#endif
	/* UDP so SOCK_DGRAM */

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	struct hostent *hostinfo;

	if (sock == INVALID_SOCKET)
	{
		perror("socket()");
		return false;
	}

	hostinfo = gethostbyname(address);
	if (hostinfo == NULL)
	{
		fprintf(stderr, "Unknown host %s.\n", address);
		return false;
	}

	sin->sin_addr = *(IN_ADDR *)hostinfo->h_addr;
	sin->sin_port = htons(PORT);

	return sock;
}

bool readBullets_callback(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
	BulletMessage bullet;

	if (!pb_decode(stream, BulletMessage_fields, &bullet))
		return false;

	headBullets = appendBullet(headBullets, &bullet);
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

	if ((sock = init_connection(mainConfiguration->server, psin)) == false)
		return false;

	uint8_t buffer[MAX_BUFFER];

	ConnectionMessage connectionMessage;
	strncpy(_engine.mainPlayer.name, settings->nickname, strlen(settings->nickname) + 1);
	strncpy(connectionMessage.name, settings->nickname, strlen(settings->nickname) + 1);


	pb_ostream_t output = pb_ostream_from_buffer(buffer, sizeof(buffer));
	status = encode_unionmessage(&output, ConnectionMessage_fields, &connectionMessage);

	int c = write_client(buffer, output.bytes_written);
	if (pthread_create(&NwkThread, NULL, NetworkThreadingListening, NULL) == -1) {
		perror("pthread_create");
		return false;
	}
	return true;
}
int read_client(SOCKET sock, SOCKADDR_IN *sin, uint8_t *buffer)
{
	int n = 0;
	size_t sinsize = sizeof *sin;

	if ((n = recvfrom(sock, buffer, MAX_BUFFER, 0, (SOCKADDR *)sin, &sinsize)) < 0)
	{
		perror("recvfrom()");
		//exit(-1);
	}


	return n;
}
int write_client(const char *buffer, const int length)
{
	int n = 0;
	if ((n = sendto(sock, buffer, length, 0, (SOCKADDR *)psin, sizeof *psin)) < 0)
		perror("write_client()");
	return n;
}

int sendMessage(const uint8_t *buffer, const int length)
{
	return write_client(buffer, length);
}

bool readPlayers_callback(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
	PlayerBase pMessage;
	if (!pb_decode(stream, PlayerBase_fields, &pMessage))
		return false;
	if (_engine.mainPlayer.playerBase.id != pMessage.id)
	{
		_engine.players[pMessage.id].playerBase = pMessage;
		if (_engine.players[pMessage.id].playerBase.state != DEAD)
			_engine.players[pMessage.id].deathAnimationStep = 0;
	}
	else
		_engine.mainPlayer.playerBase.health = pMessage.health;
	return true;
}


void *NetworkThreadingListening(void *arg)
{
	while (true)
	{
		uint8_t buffer[MAX_BUFFER];
		int count = read_client(sock, psin, buffer);

		pb_istream_t stream = pb_istream_from_buffer(buffer, count);
		const pb_field_t *type = decode_unionmessage_type(&stream);
		if (type == ConnectionCallbackMessage_fields)
		{
			ConnectionCallbackMessage callback;
			decode_unionmessage_contents(&stream, ConnectionCallbackMessage_fields, &callback);
			if (callback.sucess)
			{
				printf("Connection success motd:%s", callback.motd);
				_engine.mainPlayer.playerBase.id = callback.clientId;
				if (pthread_create(&NwkThreadSender, NULL, SreamClientData, NULL) == -1) {
					perror("pthread_create");
				}
			}
		}

		else if (type == GameDataMessage_fields)
		{
			dispose(headBullets);
			GameDataMessage gameData;
			gameData.players.funcs.decode = &readPlayers_callback;
			gameData.bullets.funcs.decode = &readBullets_callback;
			decode_unionmessage_contents(&stream, GameDataMessage_fields, &gameData);
			_engine.playersCount = gameData.playersCount;
		}
		else if (type == SpawnCallbackMessage_fields)
		{
			SpawnCallbackMessage spawnCallbackMsg;
			if (decode_unionmessage_contents(&stream, SpawnCallbackMessage_fields, &spawnCallbackMsg) && _engine.mainPlayer.playerBase.id == spawnCallbackMsg.id) {
				_engine.mainPlayer.playerBase.pos.x = spawnCallbackMsg.x;
				_engine.mainPlayer.playerBase.pos.x = spawnCallbackMsg.y;
				_engine.camera.x = _engine.mainPlayer.playerBase.pos.x - _engine.WIDTH / 2 + 16;
				_engine.camera.y = _engine.mainPlayer.playerBase.pos.y - _engine.HEIGHT / 2 + 16;
			}
		}
	}
	pthread_exit(NULL);
}

void *SreamClientData(void *arg)
{
	while (true)
	{
		uint8_t buffer[PlayerBase_size];
		PlayerBase pMessage;
		memcpy(&pMessage, &_engine.mainPlayer.playerBase, sizeof(PlayerBase));
		/*if (_engine.mainPlayer.playerBase.pos.x <= _engine.WIDTH / 2 - 16 || _engine.mainPlayer.playerBase.pos.y <= _engine.HEIGHT / 2 - 16 || _engine.mainPlayer.playerBase.pos.x + _engine.WIDTH / 2 + 16 >= _engine.mapSurface->h || _engine.mainPlayer.playerBase.pos.y + _engine.HEIGHT / 2 + 16 >= _engine.mapSurface->h)
		{
			pMessage.pos.x += _engine.pCenter.x - _engine.WIDTH / 2 + 16;
			pMessage.pos.y += _engine.pCenter.y - _engine.HEIGHT / 2 + 16;
		}*/

		pb_ostream_t output = pb_ostream_from_buffer(buffer, sizeof(buffer));
		if (encode_unionmessage(&output, PlayerBase_fields, &pMessage))
			write_client(buffer, output.bytes_written);
		Sleep(10);
	}
	pthread_exit(NULL);
}
