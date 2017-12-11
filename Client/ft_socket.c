#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <string.h>
#include <time.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "include/ft_socket.h"

#define h_addr h_addr_list[0] /* for backward compatibility */
#define PORT 1977

SOCKET sock;
SOCKADDR_IN *psin;
int clientId;
BulletElm* create(BulletMessage *bulletMessage, BulletElm* next);
BulletElm* appendBullet(BulletElm* head, BulletMessage *bulletMessage);
int NwkThreadRet = 0;

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

bool readBullets_callback(pb_istream_t *stream, void **arg)
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
	psin = malloc(sizeof(SOCKADDR_IN));
	psin->sin_family = AF_INET;

	if ((sock = init_connection(settings->server, psin)) == false)
		return false;

	ConnectionMessage connectionMessage = ConnectionMessage_init_zero;
	strncpy(_engine.mainPlayer.name, settings->nickname, strlen(settings->nickname));
	strncpy(_engine.mainPlayer.playerBase.name, settings->nickname, strlen(settings->nickname));
	strncpy(connectionMessage.name, settings->nickname, strlen(settings->nickname));

	uint8_t buffer[MAX_BUFFER];
	pb_ostream_t output = pb_ostream_from_buffer(buffer, sizeof(buffer));
	if (!encode_unionmessage(&output, ConnectionMessage_fields, &connectionMessage))
		fprintf(stderr, "Encoding failed: %s\n", PB_GET_ERROR(&output));
	write_client(buffer, output.bytes_written);
	if (pthread_create(&NwkThread, NULL, NetworkThreadingListening, NULL) == -1) {
		perror("pthread_create");
		return false;
	}
	return true;
}
int read_client(const uint8_t *readBuffer)
{
	int n = 0;
	size_t sinsize = sizeof *psin;

	if ((n = recvfrom(sock, readBuffer, MAX_BUFFER, 0, (SOCKADDR *)psin, &sinsize)) < 0)
	{
		perror("recvfrom()");
		return RECVERROR;
	}
	return n;
}
int write_client(const uint8_t *writeBuffer, const int length)
{
	int n = 0;
	if ((n = sendto(sock, writeBuffer, length, 0, (SOCKADDR *)psin, sizeof *psin)) < 0)
	{
		perror("write_client()");
		return SENDERROR;
	}
	return n;
}

bool readPlayers_callback(pb_istream_t *stream, void **arg)
{
	PlayerBase pMessage = PlayerBase_init_zero;
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


void *NetworkThreadingListening(void)
{
	while (true)
	{
		uint8_t buffer[MAX_BUFFER];
		memset(&buffer, 0, strlen(buffer));
		lastUpdateFromServer = time(NULL);
		int count = read_client(&buffer);
		if (count < 0) /* This mean there is an error, we need to kill the thread ! */
		{
			NwkThreadRet = count;
			pthread_exit(&count);
		}
		pb_istream_t stream = pb_istream_from_buffer(buffer, count);
		const pb_field_t *type = decode_unionmessage_type(&stream);
		if (type == ConnectionCallbackMessage_fields)
		{
			ConnectionCallbackMessage callback = ConnectionCallbackMessage_init_zero;
			if (!decode_unionmessage_contents(&stream, ConnectionCallbackMessage_fields, &callback))
				fprintf(stderr, "Decoding failed: %s\n", PB_GET_ERROR(&stream));
			if (callback.sucess)
			{
				printf("[SERVER] %s\n", callback.motd);
				ft_chat_Add(SERVERMESSAGE, &callback.motd);
				_engine.mainPlayer.playerBase.id = callback.clientId;
				if (pthread_create(&NwkThreadSender, NULL, StreamClientData, NULL) == -1) {
					perror("pthread_create");
				}
			}
		}

		else if (type == GameDataMessage_fields)
		{
			dispose(headBullets);
			GameDataMessage gameData = GameDataMessage_init_zero;
			gameData.players.funcs.decode = &readPlayers_callback;
			gameData.bullets.funcs.decode = &readBullets_callback;
			if (!decode_unionmessage_contents(&stream, GameDataMessage_fields, &gameData))
				fprintf(stderr, "Decoding failed: %s\n", PB_GET_ERROR(&stream));
			_engine.playersCount = gameData.playersCount;
		}
		else if (type == SpawnCallbackMessage_fields)
		{
			SpawnCallbackMessage spawnCallbackMsg = SpawnCallbackMessage_init_zero;
			if (!decode_unionmessage_contents(&stream, SpawnCallbackMessage_fields, &spawnCallbackMsg))
				fprintf(stderr, "Decoding failed: %s\n", PB_GET_ERROR(&stream));
			else if (_engine.mainPlayer.playerBase.id == spawnCallbackMsg.id)
			{
				_engine.mainPlayer.playerBase.pos.x = spawnCallbackMsg.x;
				_engine.mainPlayer.playerBase.pos.x = spawnCallbackMsg.y;
				_engine.camera.x = _engine.mainPlayer.playerBase.pos.x - _engine.WIDTH / 2 + 16;
				_engine.camera.y = _engine.mainPlayer.playerBase.pos.y - _engine.HEIGHT / 2 + 16;
			}
		}
	}
	pthread_exit(NULL);
}

void *StreamClientData(void)
{
	while (true)
	{
		uint8_t writebuffer[MAX_BUFFER];
		PlayerBase pMessage = PlayerBase_init_zero;
		memcpy(&pMessage, &_engine.mainPlayer.playerBase, sizeof(PlayerBase));
		pb_ostream_t output = pb_ostream_from_buffer(writebuffer, sizeof(writebuffer));
		if (!encode_unionmessage(&output, PlayerBase_fields, &pMessage))
			fprintf(stderr, "Encoding failed: %s\n", PB_GET_ERROR(&output));
		writebuffer[output.bytes_written] = '\0';
		write_client(writebuffer, output.bytes_written);

		SLEEP10MS;
	}
	pthread_exit(NULL);
}

int checkServerisAlive(configuration *settings)
{
	time_t actualTime = time(NULL);
	while (lastUpdateFromServer != 0 && actualTime - lastUpdateFromServer > 2) {
		printf("ERROR: No answer from server for 2 sec.\n");
		pthread_kill(NwkThread, 1);
		menu(settings, NwkThreadRet);
		lastUpdateFromServer = time(NULL);
		if (!create_connection(settings)) {
			perror("create_connection()");
			return false;
		}
	}
	return true;
}