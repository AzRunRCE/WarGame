#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <Item.h>
// NO NEED RIGHT NOW #include <SDL/SDL_ttf.h>

int main(int argc, char *argv[])
{
// NO NEED RIGHT NOW  Player Player;
// NO NEED RIGHT NOW  Player.Health = 100;

    SDL_Surface *ecran = NULL, *character = NULL, *bomb = NULL;
    SDL_Surface *map = NULL,*fog = NULL;
    SDL_Rect characterPos,positionMap,sprite, bombPos, spriteMap, positionDeplacementMap;
    int lastTime = 0, ActualTime = 0;
    int ecranWidth = 250, ecranHeight = 250;
    SDL_Event event;
    int running = 1;
    SDL_Color couleurNoire = {0, 0, 0}, couleurBlanche = {255, 255, 255};
    TTF_Init();
    SDL_Init(SDL_INIT_VIDEO);
     //TTF_Font *police = TTF_OpenFont("angelina.ttf", 65);
     /* sprintf(temps, "Temps : %d", compteur);
     texte = TTF_RenderText_Shaded(police, temps, couleurNoire, couleurBlanche);*/

    ecran = SDL_SetVideoMode(ecranWidth, ecranHeight, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("WarGame #AFTEC", NULL);

    map =  IMG_Load("res/background.png");
    character =  IMG_Load("res/character.png");
    bomb = IMG_Load("res/000.png");
    fog = IMG_Load("res/fog.png");
    bombPos.x = 0;
    bombPos.y = 0;
    positionMap.x = 0;
    positionMap.y = 0;
    sprite.x = 1;
    sprite.y = 1;
    sprite.w = 32;
    sprite.h = 32;
    spriteMap.x = map->w/2;
    spriteMap.y = map->h/2;
    spriteMap.w = ecranWidth;
    spriteMap.h = ecranHeight;

    characterPos.x = ecran->w/2;
    characterPos.y = ecran->h/2;

    SDL_EnableKeyRepeat(10, 5);

    while (running)
    {
        positionDeplacementMap.y = characterPos.y - spriteMap.h/2;
        positionDeplacementMap.x = characterPos.x - spriteMap.w/2;
        SDL_BlitSurface(map, &spriteMap, ecran, &positionDeplacementMap);
        SDL_BlitSurface(character, &sprite, ecran, &characterPos);
        SDL_BlitSurface(fog, NULL, ecran, &positionMap);
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
                        //characterPos.y--;
                        spriteMap.y--;
                        break;
                    case SDLK_DOWN: // Fleche bas
                        //characterPos.y++;
                        spriteMap.y++;
                        break;
                    case SDLK_RIGHT: // Fleche droite
                        //characterPos.x++;
                        spriteMap.x++;
                        break;
                    case SDLK_LEFT: // Fleche gauche
                        //characterPos.x--;
                        spriteMap.x--;
                        break;
                    case SDLK_SPACE: // Espace
                        SDL_BlitSurface(bomb, NULL, ecran, &characterPos);
                        break;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                spriteMap.x = event.button.x;
                spriteMap.y = event.button.y;
                break;
        }

        ActualTime = SDL_GetTicks();
    if (ActualTime - lastTime > 5)
        {
            lastTime = ActualTime;
        }
       else
           {

            SDL_Delay(5 - (ActualTime - lastTime));
       }

        SDL_Flip(ecran);

    }
    SDL_FreeSurface(character);
    SDL_Quit();

    return EXIT_SUCCESS;
}
