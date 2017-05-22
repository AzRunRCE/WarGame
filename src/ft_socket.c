#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "socket.h"
#include "client.h"
#include "engine.h"
SOCKADDR_IN sin;
SOCKET sock;
pthread_t NwkThread;
pthread_t NwkThreadSender;

static void init(void)
{
#ifdef WIN32
   WSADATA wsa;
   int err = WSAStartup(MAKEWORD(2, 2), &wsa);
   if(err < 0)
   {
      puts("WSAStartup failed !");
      exit(EXIT_FAILURE);
   }
#endif
}

void end()
{
 Packet w;
    strcpy(w.name,"\0");
    write_server(sock, &sin,  w);
#ifdef WIN32
   WSACleanup();
#endif
}


int create_connection()
{


    char host[] = "127.0.0.1";
    char pseudo[] = "client";
    init();
    sin.sin_family = 0;
    sock = init_connection(host, &sin);
    Packet w;
    strcpy(w.name,"Bertrand");
    write_server(sock, &sin,  w);
    if(pthread_create(&NwkThread, NULL, NetworkThreadingListening, NULL) == -1) {
        perror("pthread_create");
        return EXIT_FAILURE;
    }
   if(pthread_create(&NwkThreadSender, NULL, SreamClientData, NULL) == -1) {
       perror("pthread_create");
       return EXIT_FAILURE;
    }
}

static int init_connection(const char *address, SOCKADDR_IN *sin)
{
   /* UDP so SOCK_DGRAM */
   SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
   struct hostent *hostinfo;

   if(sock == INVALID_SOCKET)
   {
      perror("socket()");
      exit(errno);
   }

   hostinfo = gethostbyname(address);
   if (hostinfo == NULL)
   {
      fprintf (stderr, "Unknown host %s.\n", address);
      exit(EXIT_FAILURE);
   }

   sin->sin_addr = *(IN_ADDR *) hostinfo->h_addr;
   sin->sin_port = htons(PORT);
   sin->sin_family = AF_INET;

   return sock;
}

static void end_connection(int sock)
{
   closesocket(sock);
}

static Packet read_server(SOCKET sock, SOCKADDR_IN *sin)
{
    int n = 0;
    size_t sinsize = sizeof *sin;
    Packet pck;
    if((n = recvfrom(sock, &pck, sizeof(pck), 0, (SOCKADDR *) sin, &sinsize)) < 0)
    {
        perror("recvfrom()");
        exit(errno);
    }
  // buffer[n] = 0;
    return pck;
}

static void write_server(SOCKET sock, SOCKADDR_IN *sin,Packet pck)
{
   if(sendto(sock, &pck, sizeof(pck), 0, (SOCKADDR *) sin, sizeof *sin) < 0)
   {
      perror("sendto()");
      exit(errno);
   }
}

void *NetworkThreadingListening(void *arg)
{
    while(true)
    {
        Packet p;
        p = read_server(sock, &sin);
        _engine.players[p.clientNum].fire = p.fire;
        _engine.players[p.clientNum].state = p.state;
        _engine.players[p.clientNum].walk = p.walk;
          _engine.players[p.clientNum].Pos.h =32;
           _engine.players[p.clientNum].Pos.w =32;
        _engine.players[p.clientNum].Pos.x =  p.X -  _engine.camera.x +  _engine.WIDTH/2 - 16;
        _engine.players[p.clientNum].Pos.y =  p.Y-  _engine.camera.y +  _engine.HEIGHT/2 - 16;
    }
    pthread_exit(NULL);
}


 void *SreamClientData(void *arg)
{
   while(true)
   {
            Packet pck = {.X = _engine.mainPlayer.Pos.x,.Y = _engine.mainPlayer.Pos.y,
             .state = _engine.mainPlayer.state, .fire = _engine.mainPlayer.fire,.walk = _engine.mainPlayer.walk};
            strcpy(pck.name,"Jack");
            write_server(sock, &sin,pck);
            usleep(18000);
   }
    pthread_exit(NULL);
}
