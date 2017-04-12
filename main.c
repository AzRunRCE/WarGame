#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "Item.h"
#include "engine.h"
#include "ft_SDL.h"
#include "player.h"
#include "sprite.h"

int lastTime = 0, lastTimeAnim = 0,ActualTime = 0,ActualTimeAnim = 0;
int const SleepTime = 5;
int const SleepTimeAnim = 200;
bool tour=true;
Engine _engine;
Player mainPlayer;
int main(int argc, char *argv[])
{
    _engine.WIDTH = 260;
    _engine.HEIGTH = 260;
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

    //_engine.screenSurface = SDL_SetVideoMode(_engine.WIDTH, _engine.HEIGTH, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    //SDL_WM_SetCaption("WarGame #AFTEC", NULL);
         /* Create the window where we will draw. */
        _engine.window = SDL_CreateWindow("Wargame #AFTEC",
                        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                        512, 512,
                        0);
        int h=0;
        int w=0;
        SDL_GetWindowSize(&_engine.window, &w, &h);
/* We must call SDL_CreateRenderer in order for draw calls to affect this window. */
        _engine.screenRenderer = SDL_CreateRenderer(_engine.window, -1, 0);
/* Give us time to see the window. */
        SDL_Delay(5000);

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
    _engine.mapRect.w = _engine.WIDTH;
    _engine.mapRect.h = _engine.HEIGTH;

    mainPlayer.characterScreenRect.x = _engine.WIDTH/2 - 16;
    mainPlayer.characterScreenRect.y = _engine.HEIGTH/2 - 16;

    // A REMETTRE SDL_EnableKeyRepeat(10, 5);

    while (GetKeyPressEvent())
    {



        ft_getCharactSprite(&mainPlayer);

        SDL_BlitSurface( _engine.mapSurface, &_engine.mapRect, _engine.window, NULL);
        SDL_BlitSurface(mainPlayer.characterSurface,&mainPlayer.spriteRect, _engine.window, &mainPlayer.characterScreenRect);
        SDL_BlitSurface(_engine.fogSurface, NULL, _engine.window, NULL);

        SDL_PollEvent(&_engine.event);
        FrameDelay();
       //SDL_Flip(_engine.screenSurface);
        SDL_RenderPresent(_engine.screenRenderer);
        mainPlayer.fire = false;
        mainPlayer.walk = false;
    }
    SDL_FreeSurface(mainPlayer.characterSurface);
    SDL_Quit();
    return EXIT_SUCCESS;
}

int GetKeyPressEvent()
{
    Uint8 *keystates = SDL_GetKeyboardState(NULL);
     switch(_engine.event.type)
        {
            case SDL_QUIT:
                return 0;
                break;
            case SDL_KEYDOWN:
                switch(_engine.event.key.keysym.sym)
                {
                    case SDLK_UP: // Fleche haut
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
                    case SDLK_DOWN: // Fleche bas
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
                        //characterPos.x--;
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
            }
            break;
        }


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
