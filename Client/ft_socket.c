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
SOCKADDR_IN *psin;
SOCKET sock;
pthread_t NwkThread;
pthread_t NwkThreadSender;



static void init(void)
{
#ifdef WIN32
	WSADATA wsa;
	int err = WSAStartup(MAKEWORD(2, 2), &wsa);
	if (err < 0)
	{
		puts("WSAStartup failed !");
		exit(EXIT_FAILURE);
	}
#endif
}

void end()
{
	ClientPacket w;
	strcpy(w.clientPlayer.name, "\0");
	write_server(sock, psin, w);
#ifdef WIN32
	WSACleanup();
#endif
}



int create_connection(configuration *settings)
{
	char host[] = "127.0.0.1";
	char pseudo[] = "client";
	init();
	psin = malloc(sizeof(SOCKADDR_IN));
	psin->sin_family = AF_INET;
	sock = init_connection(host, psin);
	ClientPacket w;
	strcpy(w.clientPlayer.name, settings->nickname);
	size_t sinsize = sizeof *psin;
	int n = 0;
	write_server(sock, psin, w);
	_engine.currentGame = malloc(sizeof(ServerGame));
	if ((n = recvfrom(sock, _engine.currentGame, sizeof(ServerGame), 0, (SOCKADDR *)psin, &sinsize)) < 0)
	{
		perror("recvfrom()");
	}
	_engine.map = &_engine.currentGame->map;
	_engine.mainPlayer.id = _engine.currentGame->clientId;
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
	if (pthread_create(&NwkThreadSender, NULL, SreamClientData, NULL) == -1) {
		perror("pthread_create");
		return EXIT_FAILURE;
	}
}

static int init_connection(const char *address, SOCKADDR_IN *sin)
{
	/* UDP so SOCK_DGRAM */
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
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
	sin->sin_family = AF_INET;

	return sock;
}

static void end_connection(int sock)
{
	closesocket(sock);
}

static ServerPacket read_server(SOCKET sock, SOCKADDR_IN *sin)
{
	int n = 0;
	size_t sinsize = sizeof *sin;
	ServerPacket pck;
	if ((n = recvfrom(sock, &pck, sizeof(pck), 0, (SOCKADDR *)sin, &sinsize)) < 0)
	{
		perror("recvfrom()");
		//exit(errno);
	}
	// buffer[n] = 0;
	return pck;
}

static void write_server(SOCKET sock, SOCKADDR_IN *sin, ClientPacket pck)
{
	if (sendto(sock, &pck, sizeof(pck), 0, (SOCKADDR *)sin, sizeof *sin) < 0)
	{
		perror("sendto()");
		exit(errno);
	}
}

void *NetworkThreadingListening(void *arg)
{
	while (true)
	{
		ServerPacket packet;
		packet = read_server(sock, psin);
		pthread_mutex_lock(&_engine.mutex); /* On verrouille le mutex */
		pthread_cond_wait(&_engine.condition, &_engine.mutex); /* On attend que la condition soit remplie */
		memcpy(_engine.bullets, packet.bullets, sizeof _engine.bullets);
		memcpy(_engine.players, packet.players, sizeof _engine.players);
		pthread_mutex_unlock(&_engine.mutex); /* On déverrouille le mutex */
	}
	pthread_exit(NULL);
}

void *SreamClientData(void *arg)
{
	while (true)
	{
		ClientPacket pck;
		pck.clientPlayer = _engine.mainPlayer;
		if (_engine.mainPlayer.Pos.x <= _engine.WIDTH / 2 - 16 || _engine.mainPlayer.Pos.y <= _engine.HEIGHT / 2 - 16 || _engine.mainPlayer.Pos.x + _engine.WIDTH / 2 + 16 >= _engine.mapSurface->h || _engine.mainPlayer.Pos.y + _engine.HEIGHT / 2 + 16 >= _engine.mapSurface->h)
		{
				pck.clientPlayer.Pos.x += _engine.pCenter.x - _engine.WIDTH / 2 + 16;
				pck.clientPlayer.Pos.y += _engine.pCenter.y - _engine.HEIGHT / 2 + 16;
		
		}
		strcpy(pck.clientPlayer.name, "Jack");
		write_server(sock, psin, pck);
		Sleep(15);
	}
	pthread_exit(NULL);
}
