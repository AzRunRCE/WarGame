#include "ft_Engine.h"
#include "Engine.h"
void Engine_init()
{
    _engine.fullscreen = 0;
    _engine.WIDTH = 400;
    _engine.HEIGHT = 300;
    _engine.window = SDL_CreateWindow("Wargame #AFTEC",SDL_WINDOWPOS_CENTERED,
                                      SDL_WINDOWPOS_CENTERED,_engine.WIDTH, _engine.HEIGHT,0);
    _engine.screenRenderer = SDL_CreateRenderer(_engine.window, -1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    _engine.menuSurface =               IMG_LoadTexture(_engine.screenRenderer, "res/menu.png");
    _engine.characterSurface =          IMG_LoadTexture(_engine.screenRenderer, "res/character.png");
    _engine.characterEnnemiSurface =    IMG_LoadTexture(_engine.screenRenderer, "res/ennemi.png");
    _engine.mapSurface =                IMG_LoadTexture(_engine.screenRenderer, "res/background.png");
    _engine.fogSurface =                IMG_LoadTexture(_engine.screenRenderer, "res/fog.png");
    _engine.selectionSurface =          IMG_LoadTexture(_engine.screenRenderer, "res/selection.png");
    _engine.font = TTF_OpenFont("res/verdana.ttf", 20);
    _engine.colorWhite = (SDL_Color) {255, 255, 255};


    //_engine.enemiPlayer = (Player) {.health = 100,.state = DOWN, .step = 0,.Pos.x = 48,.Pos.y = 48,.Pos.w = 32,.Pos.h = 32};
    _engine.mainPlayer = (Player) {.health = 100,.state = DOWN, .step = 0,.Pos.x = 48,.Pos.y = 48,.Pos.w = 32,.Pos.h = 32};
    _engine.camera = (SDL_Rect) {.y = 0,.x=0,.w = _engine.WIDTH,.h = _engine.HEIGHT };
    _engine.pCenter =(SDL_Rect) {.x = _engine.WIDTH/2 - 16,.y = _engine.HEIGHT/2 - 16,.w = 32, .h = 32};
}
