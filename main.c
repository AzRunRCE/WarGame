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
#include "Engine.h"
#include "ft_Menu.h"
#define MAX_LENGTH 32
Engine _engine;

// TEXT
SDL_Color colorWhite = {255, 255, 255};

SDL_Surface *text = NULL;
SDL_Surface *fontSurface = NULL;
char message[20];
time_t lastTime = 0, lastTimeAnim = 0;
int const SleepTime = 30;
int const SleepTimeAnim = 200;
<<<<<<< HEAD
bool tour=true;
Engine _engine;
Player mainPlayer;
char debug[30];
SDL_Color couleurNoire = {0, 0, 0}, couleurBlanche = {255, 255, 255};
int main(int argc, char *argv[])
{
    _engine.WIDTH = 360;
    _engine.HEIGTH = 360;
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
    _engine.police = TTF_OpenFont("res/police.ttf", 10);
    _engine.screenSurface = SDL_SetVideoMode(_engine.WIDTH, _engine.HEIGTH, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("WarGame #AFTEC", NULL);

    mainPlayer.health = 100;
    mainPlayer.characterSurface =  IMG_Load("res/character.png");
    mainPlayer.state = DOWN;
   // mainPlayer.spriteRect =  NULL;
   mainPlayer.step = 0;

    int running = 1;
    _engine.mapSurface =  IMG_Load("res/background.png");
    _engine.fogSurface = IMG_Load("res/fog_260.png");
    _engine.bombSurface = IMG_Load("res/000.png");



    _engine.mapRect.x = _engine.mapSurface->w/2;
    _engine.mapRect.y = _engine.mapSurface->h/2;
    _engine.mapRect.w = 720;
    _engine.mapRect.h = 720;
=======

int main(int argc, char *argv[])
{
    SDL_init();
    Engine_init();
    fontSurface = SDL_GetWindowSurface(_engine.window);
    menu();
    create_connection(&_engine);
        while (GetKeyPressEvent())
        {

            sprintf(message, "%i,%i", _engine.mainPlayer.Pos.x, _engine.mainPlayer.Pos.y);
            text = TTF_RenderText_Blended( _engine.font, message, colorWhite);
            SDL_Rect posText = {0, 0, text->w, text->h};
            SDL_Texture *texture = SDL_CreateTextureFromSurface(_engine.screenRenderer, text);
            _engine.camera.x = _engine.mainPlayer.Pos.x;
            _engine.camera.y = _engine.mainPlayer.Pos.y;
            ft_GetPlayerOrientation(&_engine.mainPlayer);
            SDL_RenderClear(_engine.screenRenderer);
            SDL_RenderCopy(_engine.screenRenderer, _engine.mapSurface, &_engine.camera, NULL);
            SDL_RenderCopy(_engine.screenRenderer, _engine.characterSurface , &_engine.mainPlayer.sprite, &_engine.pCenter);
             SDL_RenderCopy(_engine.screenRenderer, _engine.bulletSurface , NULL, &_engine.pCenter);
            int i;
            for (i = 0; i < 15; i++)
            {
                if ( _engine.players[i].Pos.x == 0 &&  _engine.players[i].Pos.y == 0)
                    continue;
                  ft_GetPlayerOrientation(&_engine.players[i]);
                SDL_RenderCopy(_engine.screenRenderer,  _engine.characterEnnemiSurface , &_engine.players[i].sprite, &_engine.players[i].Pos);
            }

>>>>>>> alpha

          //  SDL_RenderCopy(_engine.screenRenderer, _engine.fogSurface, NULL, NULL);
            SDL_RenderCopy(_engine.screenRenderer,texture, NULL, &posText);
            SDL_RenderPresent(_engine.screenRenderer);
        }

        end();


        return EXIT_SUCCESS;

    }


<<<<<<< HEAD
        ft_getCharactSprite(&mainPlayer);

        SDL_BlitSurface( _engine.mapSurface, &_engine.mapRect, _engine.screenSurface, NULL);
        SDL_BlitSurface(mainPlayer.characterSurface,&mainPlayer.spriteRect, _engine.screenSurface, &mainPlayer.characterScreenRect);
       // SDL_BlitSurface(_engine.fogSurface, NULL, _engine.screenSurface, NULL);
        sprintf(debug, "Position Joueur : x:%d y:%d",  _engine.mapRect.x, _engine.mapRect.y);
        _engine.texteSurface = TTF_RenderText_Shaded(_engine.police, debug, couleurNoire, couleurBlanche);
        SDL_BlitSurface(_engine.texteSurface, NULL, _engine.screenSurface, NULL);
        SDL_PollEvent(&_engine.event);
        FrameDelay();
        SDL_Flip(_engine.screenSurface);
        mainPlayer.fire = false;
        mainPlayer.walk = false;
    }
    SDL_FreeSurface(mainPlayer.characterSurface);
    SDL_Quit();
    return EXIT_SUCCESS;
=======
// KEY CODE https://wiki.libsdl.org/SDL_ScancodeAndKeycode?highlight=%28SDL_SCANCODE%29

void ft_getCharactSprite(Player *player, State state,int step)
{
    player->sprite.x = 32 * step + (step + 1);
    player->sprite.y = 32 * state + (state + 1);
    player->sprite.h = 32;
    player->sprite.w = 32;
>>>>>>> alpha
}

int GetKeyPressEvent()
{
        Uint8 *keystate = SDL_GetKeyboardState(NULL);
        if (SDL_PollEvent(&_engine.event))//close the window
        {
            if (_engine.event.type == SDL_QUIT)
            {
                return 0;
<<<<<<< HEAD
                break;
            case SDL_KEYDOWN:

                switch(_engine.event.key.keysym.sym)
                {
                    case SDLK_UP: // Fleche haut
                        if (_engine.mapRect.y <= -180)
                            return 1;
                        _engine.mapRect.y--;
                        if (keystates[SDLK_RIGHT])
                        {
                            if(tour)
                            {
                                _engine.mapRect.x++;
                                mainPlayer.state = UP_RIGTH;
                                tour = false;
                            }
                            else
                            {
                                _engine.mapRect.y++;
                                tour = true;
                            }

                        }
                        else  if (keystates[SDLK_LEFT])
                        {
                            if(tour)
                            {
                                _engine.mapRect.x--;
                                mainPlayer.state = UP_LEFT;
                                tour = false;
                            }
                            else
                            {
                                _engine.mapRect.y++;
                                tour = true;
                            }
                        }
                        else
                             mainPlayer.state = UP;
                        mainPlayer.walk = true;
                        break;
                    case SDLK_DOWN:
                         if (_engine.mapRect.y >= 720 - 180)
                            return 1; // Fleche bas
                         _engine.mapRect.y++;
                        if (keystates[SDLK_RIGHT])
                        {
                            if(tour)
                            {
                                _engine.mapRect.x++;
                                mainPlayer.state = DOWN_RIGTH;
                                tour = false;
                            }
                            else
                            {
                                _engine.mapRect.y--;
                                tour = true;
                            }
                        }
                        else  if (keystates[SDLK_LEFT])
                        {
                            if(tour)
                            {
                                _engine.mapRect.x--;
                                mainPlayer.state = DOWN_LEFT;
                                tour = false;
                            }
                            else
                            {
                                _engine.mapRect.y--;
                                tour = true;
                            }
                        }
                        else
                            mainPlayer.state = DOWN;
                        mainPlayer.walk = true;
                        break;
                    case SDLK_RIGHT: // Fleche droite
                        if (_engine.mapRect.x >= 540)
                             return 1;
                        _engine.mapRect.x++;
                        if (keystates[SDLK_UP])
                        {
                            if(tour)
                            {
                                _engine.mapRect.y--;
                                mainPlayer.state = UP_RIGTH;
                                tour = false;
                            }
                            else
                            {
                                _engine.mapRect.x--;
                                tour = true;
                            }
                        }
                        else  if (keystates[SDLK_DOWN])
                        {
                            if(tour)
                            {
                                _engine.mapRect.y++;
                                mainPlayer.state = DOWN_RIGTH;
                                tour = false;
                            }
                            else
                            {
                                _engine.mapRect.x--;
                                tour = true;
                            }
                        }
                        else
                            mainPlayer.state = RIGTH;
                        mainPlayer.walk = true;
                        break;
                    case SDLK_LEFT: // Fleche gauche
                        if (_engine.mapRect.x <= -180)
                             return 1;
                         _engine.mapRect.x--;
                        if (keystates[SDLK_UP])
                        {
                            if(tour)
                            {
                                _engine.mapRect.y--;
                                mainPlayer.state = UP_LEFT;
                                tour = false;
                            }
                            else
                            {
                                _engine.mapRect.x++;
                                tour = true;
                            }
                        }
                        else  if (keystates[SDLK_DOWN])
                        {
                        if(tour)
                            {
                                _engine.mapRect.y++;
                                mainPlayer.state = DOWN_LEFT;
                                tour = false;
                            }
                            else
                            {
                                _engine.mapRect.x++;
                                tour = true;
                            }
                        }
                        else
                            mainPlayer.state = LEFT;
                        mainPlayer.walk = true;
                        break;
                    case SDLK_SPACE: // Espace
                        mainPlayer.fire = true;
                        break;
=======
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
    if (keystate[SDL_SCANCODE_LALT] && keystate[SDL_SCANCODE_RETURN] )
        {
            /*if (_engine.fullscreen == 1)
            {*/
                SDL_SetWindowFullscreen(_engine.window,SDL_WINDOW_FULLSCREEN);
               /* _engine.fullscreen = 0;
>>>>>>> alpha
            }
            else
            {
               SDL_SetWindowFullscreen(_engine.window,0);
               _engine.fullscreen = 1;
            }*/
        }
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
