#include "ft_Menu.h"
#include "Engine.h"



void menu()
{
    Menu Menu;
    Menu.ipAddress = NULL;
    Menu.textInput[0] = '\0';
    Menu.selectionDone = SDL_FALSE;
    SDL_StartTextInput();
    while (!Menu.selectionDone) {
        SDL_RenderClear(_engine.screenRenderer);
        SDL_RenderCopy(_engine.screenRenderer, _engine.menuSurface, NULL, NULL);
        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    /* Quit */
                    Menu.selectionDone = SDL_TRUE;
                    break;
                case SDL_TEXTINPUT:
                    /* Add new text onto the end of our text */

                    if (strlen(Menu.textInput) < MAX_LENGTH)
                    {
                         strcat(Menu.textInput, event.text.text);

                    }

                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(Menu.textInput) > 0)
                        Menu.textInput[strlen(Menu.textInput)-1] = 0;
                    if (event.key.keysym.sym == SDLK_RETURN)
                        Menu.selectionDone = SDL_TRUE;
                break;

            }
        }
        if(Menu.textInput[0] != '\0') {
            Menu.ipAddress = TTF_RenderText_Blended( _engine.font, Menu.textInput, _engine.colorWhite);
            Menu.posIpAddress.x = _engine.WIDTH/2-Menu.ipAddress->w/2;
            Menu.posIpAddress.y = _engine.HEIGHT/2-Menu.ipAddress->h/2;
            Menu.posIpAddress.w = Menu.ipAddress->w;
            Menu.posIpAddress.h = Menu.ipAddress->h;
            SDL_Texture *textureTextStart = SDL_CreateTextureFromSurface(_engine.screenRenderer, Menu.ipAddress);
            SDL_RenderCopy(_engine.screenRenderer, textureTextStart, NULL, &Menu.posIpAddress);
        }
        SDL_RenderPresent(_engine.screenRenderer);
        //printf("%d\n",selectionDone);
    }
}
