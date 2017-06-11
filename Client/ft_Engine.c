#include "include\ft_engine.h"
void Engine_init()
{
	_engine.map = malloc(sizeof(Map));
	_engine.fullscreen = 0;
	_engine.WIDTH = 800;
	_engine.HEIGHT = 600;
	_engine.window = SDL_CreateWindow("Wargame #AFTEC", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, _engine.WIDTH, _engine.HEIGHT, 0);
	_engine.screenRenderer = SDL_CreateRenderer(_engine.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	_engine.explodeSurface = IMG_LoadTexture(_engine.screenRenderer, "res/explode.png");
	_engine.characterSurface = IMG_LoadTexture(_engine.screenRenderer, "res/character.png");
	_engine.characterEnnemiSurface = IMG_LoadTexture(_engine.screenRenderer, "res/ennemi.png");
	_engine.mapSurface = IMG_LoadTexture(_engine.screenRenderer, "res/background_dev.png");
	_engine.fogSurface = IMG_LoadTexture(_engine.screenRenderer, "res/fog.png");
	_engine.bulletSurface = IMG_LoadTexture(_engine.screenRenderer, "res/bullet.png");
	_engine.AmmoSurface = IMG_LoadTexture(_engine.screenRenderer, "res/Ammo.png");
	_engine.healthSurface = IMG_LoadTexture(_engine.screenRenderer, "res/Life.png");


	_engine.font = TTF_OpenFont("res/verdana.ttf", 20);
	_engine.colorWhite = (SDL_Color) { 255, 255, 255 };
	_engine.colorWarGame = (SDL_Color) { 52, 22, 20 };
	
	_engine.mousePos.w = 55;
	_engine.mousePos.h = 55;
	
	_engine.mainPlayer = (Player) { .health = 100, .state = DOWN,.ammo = 30, .step = 0, .Pos.x = 800, .Pos.y = 800, .Pos.w = 32, .Pos.h = 32 };
	
	_engine.healthPos = (SDL_Rect) { .x =10, .y = _engine.HEIGHT - 25, .w = 150, .h = 30 };
	
	_engine.ammoPos = (SDL_Rect) { .x = 0, .y = _engine.HEIGHT - 25 - 50, .w = 400, .h = 100 };
	
	_engine.camera = (SDL_Rect) { .y = 0, .x = 0, .w = _engine.WIDTH, .h = _engine.HEIGHT };
	_engine.pCenter = (SDL_Rect) { .x = _engine.WIDTH / 2 - 16, .y = _engine.HEIGHT / 2 - 16, .w = 32, .h = 32 };
}
