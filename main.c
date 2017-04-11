#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
// NO NEED RIGHT NOW #include <SDL/SDL_ttf.h>

int main(int argc, char *argv[])
{
//    Player Player;
  //  Player.Health = 100;

    SDL_Surface *ecran = NULL, *character = NULL, *bomb = NULL;
    SDL_Surface *map = NULL, *circle = NULL,*fog = NULL;
    SDL_Rect characterPos,positionMap,sprite, bombPos;
    int lastTime = 0, ActualTime = 0;
    SDL_Event event;
    int running = 1;
    SDL_Color couleurNoire = {0, 0, 0}, couleurBlanche = {255, 255, 255};
    TTF_Init();
    SDL_Init(SDL_INIT_VIDEO);
     //TTF_Font *police = TTF_OpenFont("angelina.ttf", 65);
     /* sprintf(temps, "Temps : %d", compteur);
     texte = TTF_RenderText_Shaded(police, temps, couleurNoire, couleurBlanche);*/

    ecran = SDL_SetVideoMode(717, 717, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("WarGame #AFTEC", NULL);

    map =  IMG_Load("res/background.bmp");
    character =  IMG_Load("res/character.png");
    bomb = IMG_Load("res/000.png");
    //fog = IMG_Load("res/fog.png");
    circle =  IMG_Load("res/circle.bmp");
   // SDL_SetColorKey(circle, SDL_SRCCOLORKEY, SDL_MapRGB(circle->format, 0, 162, 232));
   // SDL_SetColorKey(fog, SDL_SRCCOLORKEY, SDL_MapRGB(fog->format, 0, 162, 232));
    SDL_SetColorKey(circle, SDL_SRCCOLORKEY, SDL_MapRGB(circle->format, 160, 160, 164));
   // SDL_SetColorKey(map, SDL_SRCCOLORKEY, SDL_MapRGB(circle->format, 0, 162, 232));
    bombPos.x = 0;
    bombPos.y = 0;
    positionMap.x = 0;
    positionMap.y = 0;
    sprite.x = 1;
    sprite.y = 1;
    sprite.w = 32;
    sprite.h = 32;

    characterPos.x = ecran->w / 2 - character->w / 2;
    characterPos.y = ecran->h / 2 - character->h / 2;

    SDL_EnableKeyRepeat(10, 5);

    while (running)
    {
     //   SDL_BlitSurface(map, NULL, ecran, &positionMap);
       // SDL_BlitSurface(character, &sprite, ecran, &characterPos);
        SDL_PollEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                running = 0;
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_UP: // Fleche haut
                        characterPos.y--;
                        break;
                    case SDLK_DOWN: // Fleche bas
                        characterPos.y++;
                        break;
                    case SDLK_RIGHT: // Fleche droite
                        characterPos.x++;
                        break;
                    case SDLK_LEFT: // Fleche gauche
                        characterPos.x--;
                        printf("Gauche");
                        break;
                    case SDLK_SPACE: // Espace
                        SDL_BlitSurface(bomb, NULL, ecran, &characterPos);
                        printf("Espace");
                        break;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                characterPos.x = event.button.x;
                characterPos.y = event.button.y;
                break;
        }

        ActualTime = SDL_GetTicks();
    if (ActualTime - lastTime > 10)
        {
       //     positioncharacter.x++;
        lastTime = ActualTime;
        }
       else
           {

            SDL_Delay(10 - (ActualTime - lastTime));
       }


       // SDL_FillRect(circle, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));




      // SDL_CreateRGBSurface(SDL_SWSURFACE,800,400,32,0,0,0,0);


        //DL_BlitSurface(bomb, NULL, ecran, &characterPos);
        SDL_BlitSurface(map, NULL, ecran, &positionMap);
       // SDL_FillRect(map, NULL, SDL_MapRGB(map->format, 255, 255, 255));

       // SDL_FillRect(circle, &ecran->clip_rect, SDL_MapRGB(ecran->format, 1, 0, 0));

       // SDL_BlitSurface(fog, NULL, ecran, &positionMap);
        SDL_Rect circlePos = characterPos;
        circlePos.x = characterPos.x - 135;
        circlePos.y = characterPos.y - 135;
        SDL_BlitSurface(circle, NULL, ecran, &circlePos);
        SDL_BlitSurface(character, &sprite, ecran, &characterPos);
        SDL_Flip(ecran);

    }

    SDL_FreeSurface(character);
    SDL_Quit();

    return EXIT_SUCCESS;
}
