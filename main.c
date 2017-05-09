#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "client.h"
#include "Item.h"
#include "engine.h"
#include "ft_SDL.h"
#include "player.h"
#include "sprite.h"

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

static void end(void)
{
#ifdef WIN32
   WSACleanup();
#endif
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

static int read_server(SOCKET sock, SOCKADDR_IN *sin, char *buffer)
{
   int n = 0;
   size_t sinsize = sizeof *sin;

   if((n = recvfrom(sock, buffer, BUF_SIZE - 1, 0, (SOCKADDR *) sin, &sinsize)) < 0)
   {
      perror("recvfrom()");
      exit(errno);
   }

   buffer[n] = 0;

   return n;
}

static void write_server(SOCKET sock, SOCKADDR_IN *sin, const char *buffer)
{
   if(sendto(sock, buffer, strlen(buffer), 0, (SOCKADDR *) sin, sizeof *sin) < 0)
   {
      perror("sendto()");
      exit(errno);
   }
}



SDL_Color textColor = { 255, 255, 255, 255 }; // white
int lastTime = 0, lastTimeAnim = 0,ActualTime = 0,ActualTimeAnim = 0;
int const SleepTime = 5;
int const SleepTimeAnim = 200;
bool tour=true;
Engine _engine;
Player mainPlayer;
Uint8 *keystate=NULL;
SDL_Color couleurNoire = {0, 0, 0};
SDL_Texture* SurfaceToTexture( SDL_Surface* surf );
SDL_Point mousePosition;
int main(int argc, char *argv[])
{

    char host[] = "127.0.0.1";
    char speudo[] = "client";
    _engine.fullscreen = 0;
    _engine.WIDTH = 400;
    _engine.HEIGHT = 300;
    TTF_Font *police = NULL;
    SDL_Surface *texte = NULL;
    SDL_Texture* blendedTexture;
    //SDL_Color couleurNoire = {0, 0, 0}, couleurBlanche = {255, 255, 255};
    if(SDL_Init(SDL_INIT_VIDEO)== -1)
    {
        fprintf(stderr, "Erreur d'initialisation de SDL_Init : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    if(TTF_Init() == -1)
    {
        fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }


    keystate=SDL_GetKeyboardState(NULL);
    police = TTF_OpenFont("res\verdana.ttf", 15 );
    _engine.window = SDL_CreateWindow("Wargame #AFTEC",
                        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                        _engine.WIDTH, _engine.HEIGHT,
                        0);
        //int h=0;
        //int w=0;
    //SDL_GetWindowSize(&_engine.window, &w, &h);
    _engine.screenRenderer = SDL_CreateRenderer(_engine.window, -1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    mainPlayer.health = 100;
    _engine.characterSurface =  IMG_LoadTexture(_engine.screenRenderer, "res/character.png");
    mainPlayer.state = DOWN;
    mainPlayer.step = 0;
    _engine.mapSurface =  IMG_LoadTexture(_engine.screenRenderer, "res/background.png");
    _engine.fogSurface = IMG_LoadTexture(_engine.screenRenderer, "res/fog.png");
    _engine.mapRect.x = 400;
    _engine.mapRect.y = 400;
    _engine.mapRect.w = _engine.WIDTH;
    _engine.mapRect.h = _engine.HEIGHT;
    _engine.mapRectEnemi.x = 400;
    _engine.mapRectEnemi.y = 400;
    _engine.mapRectEnemi.w = _engine.WIDTH;
    _engine.mapRectEnemi.h = _engine.HEIGHT;
    mainPlayer.characterScreenRect.x = _engine.WIDTH/2 - 16;
    mainPlayer.characterScreenRect.y = _engine.HEIGHT/2 - 16;
    mainPlayer.characterScreenRect.w = 32;
    mainPlayer.characterScreenRect.h = 32;
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    // A REMETTRE SDL_EnableKeyRepeat(10, 5);
//SDL_ShowCursor(SDL_DISABLE);



   //SDL_Surface* solid = TTF_RenderText_Blended( police, "plop", couleurNoire );

	//blendedTexture = SurfaceToTexture( solid );
        init();
        SOCKADDR_IN sin = { 0 };
        SOCKET sock = init_connection(host, &sin);
        char buffer[BUF_SIZE];
        char s_buffer[BUF_SIZE];
        write_server(sock, &sin, speudo);
        while (GetKeyPressEvent())
        {
            //fprintf(stderr, "_engine.mapRect.x : %d\n", _engine.mapRect.x);
            //fprintf(stderr, "_engine.mapRect.y : %d\n", _engine.mapRect.y);
            /*SDL_GetMouseState(&mousePosition.x, &mousePosition.y);
            MousePosition(mousePosition.x, mousePosition.y);*/

            fd_set rdfs;
            FD_ZERO(&rdfs);
            FD_SET(sock, &rdfs);
            sprintf (s_buffer, "%d %d", _engine.mapRect.x - _engine.WIDTH/2 - 16, _engine.mapRect.y - _engine.HEIGHT/2 - 118);
            printf("Buff envoi %s\n", s_buffer);
            write_server(sock, &sin,s_buffer);
            if(FD_ISSET(sock, &rdfs))
            {
                int n = read_server(sock, &sin, buffer);
                printf("Buffer reçu %s\n", buffer);
                if(n == 0)
                {
                    printf("Server disconnected !\n");
                    break;
                }
                sscanf (buffer,"%d %d",&_engine.mapRectEnemi.x,&_engine.mapRectEnemi.y);
                printf("variables reçues : x=%d, y=%d\n", _engine.mapRectEnemi.x, _engine.mapRectEnemi.y);

            }
            ft_GetPlayerOrientation(&mainPlayer);
            SDL_RenderClear(_engine.screenRenderer);
            SDL_RenderCopy(_engine.screenRenderer, _engine.mapSurface, &_engine.mapRect, NULL);
           // SDL_RenderCopy(_engine.screenRenderer, _engine.mapSurface, &_engine.mapRectEnemi, NULL);
            SDL_RenderCopy(_engine.screenRenderer, _engine.characterSurface , &_engine.spriteRect, &mainPlayer.characterScreenRect);
            SDL_Rect enemi;
            enemi.w = 32;
            enemi.h = 32;
            enemi.x =  _engine.mapRectEnemi.x;
            enemi.y =  _engine.mapRectEnemi.y;
            SDL_Rect positionJoueurAbsolue;
            positionJoueurAbsolue.y = _engine.mapRect.y + _engine.HEIGHT/2;
            positionJoueurAbsolue.x = _engine.mapRect.x + _engine.WIDTH/2;
            //printf("y: %d, x: %d\n", positionJoueurAbsolue.y, positionJoueurAbsolue.x);


             SDL_RenderCopy(_engine.screenRenderer, _engine.characterSurface , &_engine.spriteRect, &enemi);
            SDL_RenderCopy(_engine.screenRenderer, _engine.fogSurface, NULL, NULL);
            SDL_RenderPresent(_engine.screenRenderer);
        }
        SDL_DestroyTexture(_engine.mapSurface);
        SDL_DestroyTexture(_engine.characterSurface);
        SDL_DestroyTexture(_engine.fogSurface);
        SDL_DestroyRenderer(_engine.screenRenderer);
        SDL_DestroyWindow(_engine.window);

        SDL_Quit();
        return EXIT_SUCCESS;


    }


// KEY CODE https://wiki.libsdl.org/SDL_ScancodeAndKeycode?highlight=%28SDL_SCANCODE%29

void ft_getCharactSprite(State state,int step)
{
    _engine.spriteRect.x = 32 * step + (step + 1);
    _engine.spriteRect.y = 32 * state + (state + 1);
    _engine.spriteRect.h = 32;
    _engine.spriteRect.w = 32;

}


SDL_Texture* SurfaceToTexture( SDL_Surface* surf )
{
	SDL_Texture* text;

	text = SDL_CreateTextureFromSurface( _engine.screenRenderer, surf );

	SDL_FreeSurface( surf );

	return text;
}

int GetKeyPressEvent()
{
        Uint8 *keystates = SDL_GetKeyboardState(NULL);
        if (SDL_PollEvent(&_engine.event))//close the window
        {
            if (_engine.event.type == SDL_QUIT)
              return 0;
        }
        mainPlayer.fire = false;
        mainPlayer.walk = false;
        if (keystate[SDL_SCANCODE_SPACE] )
          mainPlayer.fire = true;
        else
        {
        if (keystate[SDL_SCANCODE_LEFT] )
        {
          if(_engine.mapRect.x <= 48) return 1;
          _engine.mapRect.x -= 2;
          mainPlayer.state = LEFT;
          mainPlayer.walk = true;
        }
        if (keystate[SDL_SCANCODE_RIGHT] )
        {
            if(_engine.mapRect.x >= 752) return 1;
            _engine.mapRect.x += 2;
            mainPlayer.state = RIGHT;
            mainPlayer.walk = true;
        }

        if (keystate[SDL_SCANCODE_UP] )
        {
            if(_engine.mapRect.y <= 48) return 1;
            _engine.mapRect.y -= 2;
            mainPlayer.state = UP;
            mainPlayer.walk = true;
        }
      /*  if (keystates[SDL_SCANCODE_LALT] && keystates[SDL_SCANCODE_RETURN] )
        {
            if (_engine.fullscreen == 1)
            {
                SDL_SetWindowFullscreen(_engine.window,SDL_WINDOW_FULLSCREEN);
                _engine.fullscreen = 0;
            }
            else
            {
               SDL_SetWindowFullscreen(_engine.window,0);
               _engine.fullscreen = 1;
            }
        }
        if (keystates[SDL_SCANCODE_ESCAPE])
            SDL_SetWindowFullscreen(_engine.window,0);
         }
         if (keystate[SDL_SCANCODE_RIGHT] && keystate[SDL_SCANCODE_UP])
            mainPlayer.state = UP_RIGHT;
        else if (keystate[SDL_SCANCODE_LEFT] && keystate[SDL_SCANCODE_UP])
            mainPlayer.state = UP_LEFT;
        else if (keystate[SDL_SCANCODE_LEFT] && keystate[SDL_SCANCODE_DOWN])
            mainPlayer.state = DOWN_LEFT;
        else if (keystate[SDL_SCANCODE_RIGHT] && keystate[SDL_SCANCODE_DOWN])
            mainPlayer.state = DOWN_RIGHT;*/
  }
    if (keystates[SDL_SCANCODE_LALT] && keystates[SDL_SCANCODE_RETURN] )
        {
            if (_engine.fullscreen == 1)
            {
                SDL_SetWindowFullscreen(_engine.window,SDL_WINDOW_FULLSCREEN);
                _engine.fullscreen = 0;
            }
            else
            {
               SDL_SetWindowFullscreen(_engine.window,0);
               _engine.fullscreen = 1;
            }
        }
    SDL_GetMouseState(&mousePosition.x, &mousePosition.y);
 //  SDL_Log("Mouse position : x=%i y=%i", mousePosition.x, mousePosition.y);
   // SDL_Log("fullscreen : %i", _engine.fullscreen);
    if (mousePosition.x > _engine.WIDTH/2 && (mousePosition.y < 175 && mousePosition.y > 115) )
      {

        mainPlayer.state = RIGHT;
        if (keystate[SDL_SCANCODE_W] )
        {
            if(_engine.mapRect.x >= 752) return 1;
            _engine.mapRect.x += 2;
            mainPlayer.walk = true;
        }
     }
    else if (mousePosition.x > _engine.WIDTH/2 && mousePosition.y > _engine.HEIGHT/2)
        {

        mainPlayer.state = DOWN_RIGHT;
        if (keystate[SDL_SCANCODE_W] )
        {
            if(_engine.mapRect.y >= 752) return 1;
            _engine.mapRect.y += 2;
            _engine.mapRect.x += 1;
            mainPlayer.walk = true;
        }
     }
    else  if (mousePosition.y > _engine.HEIGHT/2 && (mousePosition.x > 175 && mousePosition.x < 225))
     {

        mainPlayer.state = DOWN;
        if (keystate[SDL_SCANCODE_W] )
        {
            if(_engine.mapRect.y >= 752) return 1;
            _engine.mapRect.y += 2;
            mainPlayer.walk = true;
        }
     }
    else if (mousePosition.x < _engine.WIDTH/2 && mousePosition.y > _engine.HEIGHT/2)
     {
        mainPlayer.state= DOWN_LEFT;
        if (keystate[SDL_SCANCODE_W] )
        {
            if(_engine.mapRect.y >= 752) return 1;
            _engine.mapRect.y += 2;
             _engine.mapRect.x -= 1;
            mainPlayer.walk = true;
        }
     }
    else  if (mousePosition.x < _engine.WIDTH/2 && (mousePosition.y < 175 && mousePosition.y > 115))
    {

        mainPlayer.state = LEFT;
        if (keystate[SDL_SCANCODE_W] )
        {
            if(_engine.mapRect.y >= 752) return 1;
           _engine.mapRect.x -= 2;
            mainPlayer.walk = true;
        }
        }
    else  if (mousePosition.x < _engine.WIDTH/2 && mousePosition.y < _engine.HEIGHT/2)

        {
             mainPlayer.state= UP_LEFT;
        if (keystate[SDL_SCANCODE_W] )
        {
            if(_engine.mapRect.y >= 752) return 1;
             _engine.mapRect.y -= 2;
           _engine.mapRect.x -= 1;
            mainPlayer.walk = true;
        }
        }
    else if (mousePosition.y < _engine.HEIGHT/2 && (mousePosition.x > 175 && mousePosition.x < 225))

        {
            mainPlayer.state = UP;
             if (keystate[SDL_SCANCODE_W] )
        {
            if(_engine.mapRect.y <= 48) return 1;
             _engine.mapRect.y -= 2;
            mainPlayer.walk = true;
        }

        }
    else if (mousePosition.x > _engine.WIDTH/2 && mousePosition.y < _engine.HEIGHT/2)
       {
            mainPlayer.state = UP_RIGHT;
             if (keystate[SDL_SCANCODE_W] )
        {
            if(_engine.mapRect.y >= 752) return 1;
             _engine.mapRect.y -= 2;
              _engine.mapRect.x += 1;
            mainPlayer.walk = true;
        }

        }

    if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
        mainPlayer.fire = true;

    return 1;
}


bool AnimDelay()
{
    ActualTimeAnim = SDL_GetTicks();
    if (ActualTimeAnim - lastTimeAnim > SleepTimeAnim)
    {
           lastTimeAnim = ActualTimeAnim;
           return true;
    }
    else
    {
         return false;
    }

}
void FrameDelay()
{
    ActualTime = SDL_GetTicks();
    if (ActualTime - lastTime > SleepTime )
        lastTime = ActualTime;
    else
        SDL_Delay(SleepTime - (ActualTime - lastTime));
}

/*void MousePosition(int lastx, int lasty)
{

}*/
