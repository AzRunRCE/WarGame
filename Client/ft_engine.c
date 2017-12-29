#include "include/ft_engine.h"
#include "include/ft_menu.h"

void Engine_init()
{
	_engine.map = malloc(sizeof(Map));
	_engine.fullscreen = 0;
	_engine.WIDTH = 800;
	_engine.HEIGHT = 600;
	_engine.window = SDL_CreateWindow("Wargame #AFTEC", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, _engine.WIDTH, _engine.HEIGHT, SDL_WINDOW_SHOWN);
	_engine.screenRenderer = SDL_CreateRenderer(_engine.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	_engine.playersCount = 0;
	_engine.bloodSurface = IMG_LoadTexture(_engine.screenRenderer, "res/blood.png");
	_engine.explodeSurface = IMG_LoadTexture(_engine.screenRenderer, "res/explode.png");
	_engine.AnimKill = IMG_LoadTexture(_engine.screenRenderer, "res/AnimationKill.png");
	_engine.characterSurface = IMG_LoadTexture(_engine.screenRenderer, "res/character.png");
	_engine.characterEnnemiSurface = IMG_LoadTexture(_engine.screenRenderer, "res/enemy.png");
	_engine.mapSurface = IMG_LoadTexture(_engine.screenRenderer, "res/background_dev.png");
	_engine.viewSurface = IMG_LoadTexture(_engine.screenRenderer, "res/view.png");
	_engine.gameoverBackground = IMG_LoadTexture(_engine.screenRenderer, "res/gameover.png");
	_engine.bulletSurface = IMG_LoadTexture(_engine.screenRenderer, "res/bullet.png");
	_engine.AmmoSurface = IMG_LoadTexture(_engine.screenRenderer, "res/Ammo.png");
	_engine.healthSurface = IMG_LoadTexture(_engine.screenRenderer, "res/Life.png");
	_engine.redSurface = IMG_LoadTexture(_engine.screenRenderer, "res/red.png");
	//_engine.mutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_init(&_engine.mutex, NULL);
	//_engine.condition = PTHREAD_COND_INITIALIZER;

	_engine.font = TTF_OpenFont("res/verdana.ttf", 20);
	_engine.colorWhite = (SDL_Color) { 255, 255, 255 };
	_engine.colorWarGame = (SDL_Color) { 52, 22, 20 };
	
	_engine.mousePos.w = 55;
	_engine.mousePos.h = 55;
	_engine.fullscreen = false;
	_engine.cooldownFullscreen = 0;
	SDL_SetWindowResizable(_engine.window, true);
	_engine.mainPlayer = (Player) Player_init_default;
	_engine.mainPlayer.playerBase.state = 0;
	_engine.healthPos = (SDL_Rect) { .x =10, .y = _engine.HEIGHT - 25, .w = 150, .h = 30 };
	
	_engine.ammoPos = (SDL_Rect) { .x = 0, .y = _engine.HEIGHT - 25 - 50, .w = 400, .h = 100 };
	
	_engine.camera = (SDL_Rect) { .y = 0, .x = 0, .w = _engine.WIDTH, .h = _engine.HEIGHT };
	_engine.viewRect = (SDL_Rect) { .w = 1800, .h = 1800 };
	_engine.cooldownDeath = 10;
	_engine.lastCooldownDeath = 0;
	init_menuDeath();
	headItemList2 = ft_LoadMap("map/first.bmp", _engine.map);
}
