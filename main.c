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

Engine _engine;

SDL_Color colorWhite = {255, 255, 255};

// MENU
int menu()
{
SDL_Surface *MenuTitle = NULL;
SDL_Surface *Start = NULL;
char textInput[20];
char *composition;
Sint32 cursor;
Sint32 selection_len;
SDL_Rect rect;
rect.h = 100;
rect.w = 100;
rect.x = 0;
rect.y = 0;
SDL_SetTextInputRect(&rect);

    while(true)
    {
        SDL_bool done = SDL_FALSE;
        //SDL_RenderCopy(_engine.screenRenderer, _engine.mapSurface, NULL, NULL);

        MenuTitle = TTF_RenderText_Blended( _engine.font, "WarGame", colorWhite);
        SDL_Rect posText = {_engine.WIDTH/2-MenuTitle->w/2, 0, MenuTitle->w, MenuTitle->h};
        SDL_Texture *texture = SDL_CreateTextureFromSurface(_engine.screenRenderer, MenuTitle);


        SDL_StartTextInput();
        int i;
        while (!done) {
            SDL_RenderClear(_engine.screenRenderer);
            SDL_RenderCopy(_engine.screenRenderer, _engine.menuSurface, NULL, NULL);
            SDL_RenderCopy(_engine.screenRenderer,texture, NULL, &posText);
            SDL_Event event;
            if (SDL_PollEvent(&event)) {
                switch (event.type) {
                    case SDL_QUIT:
                        /* Quit */
                        done = SDL_TRUE;
                        break;
                    case SDL_TEXTINPUT:
                        /* Add new text onto the end of our text */
                        strcat(textInput, event.text.text);
                        break;
                    case SDL_TEXTEDITING:
                    /*
                    Update the composition text.
                    Update the cursor position.
                    Update the selection length (if any).
                    */
                    composition = event.edit.text;
                    cursor = event.edit.start;
                    selection_len = event.edit.length;
                    break;
                    case SDL_KEYDOWN:
                        if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(textInput) > 0)
                        {
                            textInput[strlen(textInput)-1] = 0;
                        }
                    break;

                }
            }
            Start = TTF_RenderText_Blended( _engine.font, textInput, colorWhite);
            SDL_Rect posTextStart = {_engine.WIDTH/2-Start->w/2, _engine.HEIGHT/2-Start->h/2, Start->w, Start->h};
            SDL_Texture *textureTextStart = SDL_CreateTextureFromSurface(_engine.screenRenderer, Start);
            SDL_RenderCopy(_engine.screenRenderer, textureTextStart, NULL, &posTextStart);
            SDL_RenderPresent(_engine.screenRenderer);
            printf("%s\n", textInput);
        }

    }
}

// TEXT

SDL_Surface *text = NULL;
SDL_Surface *fontSurface = NULL;
char message[20];
time_t lastTime = 0, lastTimeAnim = 0;
int const SleepTime = 30;
int const SleepTimeAnim = 200;

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
            int i;
            for (i = 0; i < 15; i++)
            {
                if ( _engine.players[i].Pos.x == 0 &&  _engine.players[i].Pos.y == 0)
                    continue;
                  ft_GetPlayerOrientation(&_engine.players[i]);
                SDL_RenderCopy(_engine.screenRenderer,  _engine.characterEnnemiSurface , &_engine.players[i].sprite, &_engine.players[i].Pos);
            }


          //  SDL_RenderCopy(_engine.screenRenderer, _engine.fogSurface, NULL, NULL);
            SDL_RenderCopy(_engine.screenRenderer,texture, NULL, &posText);
            SDL_RenderPresent(_engine.screenRenderer);
        }

        end();


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
        Uint8 *keystate = SDL_GetKeyboardState(NULL);
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
    if (keystate[SDL_SCANCODE_LALT] && keystate[SDL_SCANCODE_RETURN] )
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
