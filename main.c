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
int main(int argc, char *argv[])
{
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
    _engine.fogSurface = IMG_LoadTexture(_engine.screenRenderer, "res/fog_260.png");
    _engine.mapRect.x = 400;
    _engine.mapRect.y = 400;
    _engine.mapRect.w = _engine.WIDTH;
    _engine.mapRect.h = _engine.HEIGHT;
    mainPlayer.characterScreenRect.x = _engine.WIDTH/2 - 16;
    mainPlayer.characterScreenRect.y = _engine.HEIGHT/2 - 16;
    mainPlayer.characterScreenRect.w = 32;
    mainPlayer.characterScreenRect.h = 32;
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    // A REMETTRE SDL_EnableKeyRepeat(10, 5);




   //SDL_Surface* solid = TTF_RenderText_Blended( police, "plop", couleurNoire );

	//blendedTexture = SurfaceToTexture( solid );
    while (GetKeyPressEvent())
    {
        ft_GetPlayerOrientation(&mainPlayer);
        SDL_RenderClear(_engine.screenRenderer);
        SDL_RenderCopy(_engine.screenRenderer, _engine.mapSurface, &_engine.mapRect, NULL);
        SDL_RenderCopy(_engine.screenRenderer, _engine.characterSurface , &_engine.spriteRect, &mainPlayer.characterScreenRect);
        //SDL_RenderCopy(_engine.screenRenderer, _engine.fogSurface, NULL, NULL);
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
          if(_engine.mapRect.x <= 45) return 1;
          _engine.mapRect.x -= 2;
          mainPlayer.state = LEFT;
          mainPlayer.walk = true;
        }
        if (keystate[SDL_SCANCODE_RIGHT] )
        {
            if(_engine.mapRect.x >= 755) return 1;
            _engine.mapRect.x += 2;
            mainPlayer.state = RIGHT;
            mainPlayer.walk = true;
        }
        if (keystate[SDL_SCANCODE_DOWN] )
        {
            if(_engine.mapRect.y >= 745) return 1;
            _engine.mapRect.y += 2;
            mainPlayer.state = DOWN;
            mainPlayer.walk = true;
        }
        if (keystate[SDL_SCANCODE_UP] )
        {
            if(_engine.mapRect.y <= 45) return 1;
            _engine.mapRect.y -= 2;
            mainPlayer.state = UP;
            mainPlayer.walk = true;
        }
        if (keystates[SDL_SCANCODE_F])
            SDL_SetWindowFullscreen(_engine.window,SDL_WINDOW_FULLSCREEN);
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
            mainPlayer.state = DOWN_RIGHT;

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
