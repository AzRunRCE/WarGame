#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include "Item.h"
#include "engine.h"
#include "ft_SDL.h"
#include "player.h"
#include "sprite.h"

int lastTime = 0, lastTimeAnim = 0,ActualTime = 0,ActualTimeAnim = 0;
int const SleepTime = 15;
int const SleepTimeAnim = 100;
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

    _engine.screenSurface = SDL_SetVideoMode(_engine.WIDTH, _engine.HEIGTH, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("WarGame #AFTEC", NULL);

    mainPlayer.health = 100;
    mainPlayer.characterSurface =  IMG_Load("res/character.png");
    mainPlayer.state = DOWN;
   // mainPlayer.spriteRect =  NULL;

    int running = 1;
    _engine.mapSurface =  IMG_Load("res/background.png");
    _engine.fogSurface = IMG_Load("res/fog.png");
    _engine.bombSurface = IMG_Load("res/000.png");



    _engine.mapRect.x = _engine.mapSurface->w/2;
    _engine.mapRect.y = _engine.mapSurface->h/2;
    _engine.mapRect.w = _engine.WIDTH;
    _engine.mapRect.h = _engine.HEIGTH;

    mainPlayer.characterScreenRect.x = _engine.screenSurface->w/2 - 16;
    mainPlayer.characterScreenRect.y = _engine.screenSurface->h/2 - 16;

    SDL_EnableKeyRepeat(10, 5);

    while (GetKeyPressEvent(&_engine.event))
    {
        if (mainPlayer.walk && mainPlayer.step < 3 )
            mainPlayer.step++;
        else
            mainPlayer.step = 1;
        ft_getCharactSprite(&mainPlayer);

        SDL_BlitSurface( _engine.mapSurface, &_engine.mapRect, _engine.screenSurface, NULL);
        SDL_BlitSurface(mainPlayer.characterSurface,&mainPlayer.spriteRect, _engine.screenSurface, &mainPlayer.characterScreenRect);
        SDL_BlitSurface(_engine.fogSurface, NULL, _engine.screenSurface, NULL);

        SDL_PollEvent(&_engine.event);
        FrameDelay();
        SDL_Flip(_engine.screenSurface);
        mainPlayer.fire = false;
        mainPlayer.walk = false;
    }
    SDL_FreeSurface(mainPlayer.characterSurface);
    SDL_Quit();
    return EXIT_SUCCESS;
}

int GetKeyPressEvent(SDL_Event *ev)
{
    SDL_Event event = (*ev);
    switch(event.type)
    {
        case SDL_QUIT:
            return 0;
            break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
                case SDLK_UP: // Fleche haut
                        //characterPos.y--;
                    _engine.mapRect.y--;
                    mainPlayer.state = UP;
                    mainPlayer.walk = true;
                    break;
                case SDLK_DOWN: // Fleche bas
                        //characterPos.y++;
                    _engine.mapRect.y++;
                     mainPlayer.state = DOWN;
                     mainPlayer.walk = true;
                    break;
                case SDLK_RIGHT: // Fleche droite
                        //characterPos.x++;
                    _engine.mapRect.x++;
                     mainPlayer.state = RIGTH;
                     mainPlayer.walk = true;
                    break;
                case SDLK_LEFT: // Fleche gauche
                        //characterPos.x--;
                    _engine.mapRect.x--;
                    mainPlayer.state = LEFT;
                    mainPlayer.walk = true;
                    break;
                case SDLK_SPACE: // Espace
                    mainPlayer.fire = true;

                    //SDL_BlitSurface(bomb, NULL, _engine.screen, &characterPos);
                    break;
            }
            break;
        case SDL_MOUSEBUTTONUP:
               // _engine.mapRect.x = event.button.x;
                //_engine.mapRect.y = event.button.y;
            break;
    }
    return 1;
}


bool AnimDelay()
{
    ActualTimeAnim = SDL_GetTicks();
    if (ActualTimeAnim - lastTimeAnim > SleepTimeAnim * 300 )
      {
           lastTimeAnim = ActualTimeAnim;
           return true;
      }
    else
        return false;
}
void FrameDelay()
{
    ActualTime = SDL_GetTicks();
    if (ActualTime - lastTime > SleepTime )
        lastTime = ActualTime;
    else
        SDL_Delay(SleepTime - (ActualTime - lastTime));
}
