#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "Item.h"
#include "engine.h"
#include "ft_SDL.h"
#include "player.h"
#include "sprite.h"
#include "Packet.h"
#include "Socket.h"

// TEXT
SDL_Window *ecran = NULL;
SDL_Surface *text = NULL;
TTF_Font *font = NULL;
SDL_Color colorWhite = {255, 255, 255};
SDL_Surface *fontSurface = NULL;
char message[20];
time_t lastTime = 0, lastTimeAnim = 0;
int const SleepTime = 30;
int const SleepTimeAnim = 200;
bool tour=true;
Engine _engine;
Uint8 *keystate=NULL;
SDL_Point mousePosition;





int main(int argc, char *argv[])
{



    _engine.fullscreen = 0;
    _engine.WIDTH = 400;
    _engine.HEIGHT = 300;

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
    font = TTF_OpenFont("res/verdana.ttf", 20);



    keystate=SDL_GetKeyboardState(NULL);
    _engine.window = SDL_CreateWindow("Wargame #AFTEC",
                        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                        _engine.WIDTH, _engine.HEIGHT,
                        0);
    _engine.screenRenderer = SDL_CreateRenderer(_engine.window, -1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    _engine.menuSurface =               IMG_LoadTexture(_engine.screenRenderer, "res/menu.png");
    //menu();
    _engine.characterSurface =          IMG_LoadTexture(_engine.screenRenderer, "res/character.png");
    _engine.characterEnnemiSurface =    IMG_LoadTexture(_engine.screenRenderer, "res/ennemi.png");
    _engine.mapSurface =                IMG_LoadTexture(_engine.screenRenderer, "res/background.png");
    _engine.fogSurface =                IMG_LoadTexture(_engine.screenRenderer, "res/fog.png");

    fontSurface = SDL_GetWindowSurface(_engine.window);


    _engine.mainPlayer.health = 100;
    _engine.mainPlayer.state = DOWN;
    _engine.mainPlayer.step = 0;
    _engine.mainPlayer.Pos.x = 48;
    _engine.mainPlayer.Pos.y = 48;
    _engine.mainPlayer.Pos.w = 32;
    _engine.mainPlayer.Pos.h = 32;
    _engine.enemiPlayer = _engine.mainPlayer;
    SDL_Rect pCenter = {_engine.WIDTH/2 - 16, _engine.HEIGHT/2 - 16,32,32};
    _engine.camera.x = 0;
    _engine.camera.y = 0;
    _engine.camera.w = _engine.WIDTH;
    _engine.camera.h = _engine.HEIGHT;
   create_connection(&_engine);
        while (GetKeyPressEvent())
        {


            _engine.camera.x = _engine.mainPlayer.Pos.x;
            _engine.camera.y = _engine.mainPlayer.Pos.y;
            ft_GetPlayerOrientation(&_engine.mainPlayer);
            ft_GetPlayerOrientation(&_engine.enemiPlayer);
             sprintf(message, "%i,%i", _engine.mainPlayer.Pos.x, _engine.mainPlayer.Pos.y);
            text = TTF_RenderText_Blended(font, message, colorWhite);

            SDL_Rect posText = {0, 0, text->w, text->h};
            SDL_Texture *texture = SDL_CreateTextureFromSurface(_engine.screenRenderer, text);

            SDL_RenderClear(_engine.screenRenderer);
            SDL_RenderCopy(_engine.screenRenderer, _engine.mapSurface, &_engine.camera, NULL);
            SDL_RenderCopy(_engine.screenRenderer, _engine.characterSurface , &_engine.mainPlayer.sprite, &pCenter);
            SDL_RenderCopy(_engine.screenRenderer,  _engine.characterEnnemiSurface , &_engine.enemiPlayer.sprite, &_engine.enemiPlayer.Pos);
            SDL_RenderCopy(_engine.screenRenderer, _engine.fogSurface, NULL, NULL);
            SDL_RenderCopy(_engine.screenRenderer,texture, NULL, &posText);
           // SDL_RenderCopy(_engine.screenRenderer, _engine.menuSurface, NULL, NULL);
            SDL_RenderPresent(_engine.screenRenderer);
        }
        TTF_CloseFont(font);
        SDL_FreeSurface(text);
        SDL_DestroyTexture(_engine.mapSurface);
        SDL_DestroyTexture(_engine.characterSurface);
        SDL_DestroyTexture(_engine.fogSurface);
        SDL_DestroyRenderer(_engine.screenRenderer);
        SDL_DestroyWindow(_engine.window);
        end();

        SDL_Quit();
        return EXIT_SUCCESS;

    }


// KEY CODE https://wiki.libsdl.org/SDL_ScancodeAndKeycode?highlight=%28SDL_SCANCODE%29

void ft_getCharactSprite(Player *player, State state,int step)
{
    player->sprite.x = 32 * step + (step + 1);
    player->sprite.y = 32 * state + (state + 1);
    player->sprite.h = 32;
    player->sprite.w = 32;
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
            {
                return 0;
            }

        }
        _engine.mainPlayer.fire = false;
        _engine.mainPlayer.walk = false;
        if (keystate[SDL_SCANCODE_SPACE] )
          _engine.mainPlayer.fire = true;
        else
        {
            if (keystate[SDL_SCANCODE_LEFT] &   _engine.mainPlayer.Pos.x >= 50)
            {
            _engine.mainPlayer.Pos.x -= 2;
                _engine.mainPlayer.state = LEFT;
                _engine.mainPlayer.walk = true;
            }
            if (keystate[SDL_SCANCODE_RIGHT] & _engine.mainPlayer.Pos.x <= 750)
            {
                _engine.mainPlayer.Pos.x += 2;
                _engine.mainPlayer.state = RIGHT;
                _engine.mainPlayer.walk = true;
            }
            if (keystate[SDL_SCANCODE_UP] &   _engine.mainPlayer.Pos.y >= 50 )
            {
                _engine.mainPlayer.Pos.y -= 2;
                _engine.mainPlayer.state = UP;
                _engine.mainPlayer.walk = true;
            }
            if (keystate[SDL_SCANCODE_DOWN] &   _engine.mainPlayer.Pos.y <= 750 )
            {
                _engine.mainPlayer.Pos.y += 2;
                _engine.mainPlayer.state = DOWN;
                _engine.mainPlayer.walk = true;
            }

  }
    if (keystates[SDL_SCANCODE_LALT] && keystates[SDL_SCANCODE_RETURN] )
        {
            /*if (_engine.fullscreen == 1)
            {*/
                SDL_SetWindowFullscreen(_engine.window,SDL_WINDOW_FULLSCREEN);
               /* _engine.fullscreen = 0;
            }
            else
            {
               SDL_SetWindowFullscreen(_engine.window,0);
               _engine.fullscreen = 1;
            }*/
        }
    SDL_GetMouseState(&mousePosition.x, &mousePosition.y);
    if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
        _engine.mainPlayer.fire = true;
    return 1;
}


bool AnimDelay(Player *player)
{
    int ActualTimeAnim = SDL_GetTicks();
    if (ActualTimeAnim - player->lastAnim > SleepTimeAnim)
    {
           player->lastAnim = ActualTimeAnim;
           return true;
    }
    else
    {
         return false;
    }

}
int FrameDelay()
{
   time_t ActualTime = time(0); // Get the system time;
    if (ActualTime - lastTime > SleepTime )
  {
       lastTime = ActualTime;
        return 1;
  }
    else
       return 0;
}

/*void MousePosition(int lastx, int lasty)
{

}*/
