#include "ft_Menu.h"
#include "Engine.h"
#include "point.h"


void menu()
{
    Menu Menu;
    point pointLeft[3];
    point pointRight[3];
    Menu.menuSelection = 0;
    Menu.WarGameFont = TTF_OpenFont("res/boston.ttf", 60);

    Menu.menuBackground = IMG_LoadTexture(_engine.screenRenderer, "res/menu.png");
    Menu.menuOptionsBackground = IMG_LoadTexture(_engine.screenRenderer, "res/menu-options.png");
    Menu.selectionRight = IMG_LoadTexture(_engine.screenRenderer, "res/selection-right.png");
    Menu.selectionLeft = IMG_LoadTexture(_engine.screenRenderer, "res/selection-left.png");
    // Position PLAY LEFT
    pointLeft[0].x = _engine.WIDTH/2-Menu.selectionLeft->h*2.5;
    pointLeft[0].y = _engine.HEIGHT/2-Menu.selectionLeft->h*1.2;
    // Position PLAY RIGHT
    pointRight[0].x = _engine.WIDTH/2+Menu.selectionRight->h*1.5;
    pointRight[0].y = _engine.HEIGHT/2-Menu.selectionRight->h*1.2;

    // Position OPTIONS LEFT
    pointLeft[1].x = _engine.WIDTH/2-Menu.selectionLeft->h*3.5;
    pointLeft[1].y = _engine.HEIGHT/2+Menu.selectionLeft->h/3.5;
    // Position OPTIONS RIGHT
    pointRight[1].x = _engine.WIDTH/2+Menu.selectionRight->h*2.5;
    pointRight[1].y = _engine.HEIGHT/2+Menu.selectionRight->h/3.5;

     // Position QUIT LEFT
    pointLeft[2].x = _engine.WIDTH/2-Menu.selectionLeft->h*2.5;
    pointLeft[2].y = _engine.HEIGHT/2+Menu.selectionLeft->h*1.7;
    // Position QUIT RIGHT
    pointRight[2].x = _engine.WIDTH/2+Menu.selectionRight->h*1.5;
    pointRight[2].y = _engine.HEIGHT/2+Menu.selectionLeft->h*1.7;




    Menu.ipAddress = NULL;
    Menu.textInput[0] = '\0';
    Menu.selectionDone = false;
    Menu.selectionOptionsDone = false;
    SDL_StartTextInput();
    while (!Menu.selectionDone) {
        Menu.posSelectionLeft = (SDL_Rect) {pointLeft[Menu.menuSelection].x,pointLeft[Menu.menuSelection].y, Menu.selectionLeft->h, Menu.selectionLeft->h};
        Menu.posSelectionRight = (SDL_Rect) {pointRight[Menu.menuSelection].x,pointRight[Menu.menuSelection].y, Menu.selectionRight->h, Menu.selectionRight->h};
        SDL_RenderClear(_engine.screenRenderer);
        if (Menu.menuSelection == 1 && Menu.selectionOptionsDone == true)
                SDL_RenderCopy(_engine.screenRenderer, Menu.menuOptionsBackground, NULL, NULL);
        else {
        SDL_RenderCopy(_engine.screenRenderer, Menu.menuBackground, NULL, NULL);
        SDL_RenderCopy(_engine.screenRenderer, Menu.selectionLeft, NULL, &Menu.posSelectionLeft);
        SDL_RenderCopy(_engine.screenRenderer, Menu.selectionRight, NULL, &Menu.posSelectionRight);
        }
        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    /* Quit */
                    Menu.selectionDone = true;
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
                    {
                        switch (Menu.menuSelection)
                        {
                        case 0:
                           Menu.selectionDone = true;
                           break;
                        case 1:
                            Menu.selectionOptionsDone = true;
                            break;
                        case 2:
                            exit(1);
                            break;
                        }
                    }
                    if (event.key.keysym.sym == SDLK_DOWN)
                        if (Menu.menuSelection < 2 && Menu.selectionOptionsDone == false)
                            Menu.menuSelection++;
                    if (event.key.keysym.sym == SDLK_UP && Menu.selectionOptionsDone == false)
                        if (Menu.menuSelection > 0)
                            Menu.menuSelection--;
                    if (event.key.keysym.sym == SDLK_ESCAPE && Menu.selectionOptionsDone == true)
                        Menu.selectionOptionsDone = false;
                break;

            }
        }
        if(Menu.menuSelection == 1 && Menu.textInput[0] != '\0' && Menu.selectionOptionsDone == true) {
            Menu.ipAddress = TTF_RenderText_Blended( Menu.WarGameFont, Menu.textInput, _engine.colorWarGame);
            Menu.posIpAddress.x = _engine.WIDTH/2-Menu.ipAddress->w/2;
            Menu.posIpAddress.y = _engine.HEIGHT/2-Menu.ipAddress->h/2;
            Menu.posIpAddress.w = Menu.ipAddress->w;
            Menu.posIpAddress.h = Menu.ipAddress->h;
            SDL_Texture *textureTextStart = SDL_CreateTextureFromSurface(_engine.screenRenderer, Menu.ipAddress);
            SDL_RenderCopy(_engine.screenRenderer, textureTextStart, NULL, &Menu.posIpAddress);
        }
        printf("sel=%d\n", Menu.menuSelection);
        printf("seldone=%d\n", Menu.selectionDone);
        SDL_RenderPresent(_engine.screenRenderer);
        //printf("%d\n",selectionDone);
    }
}
