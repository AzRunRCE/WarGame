#ifndef ENGINE_H
#define ENGINE_H
#include "ft_map.h"
#include "ft_player.h"
#include <pthread.h>
#include "unionproto.pb.h"
#include "ft_socket.h"
typedef struct {
	pthread_cond_t condition; 
	pthread_mutex_t mutex; 
    int WIDTH;
    int HEIGHT;
    int fullscreen;
	int playersCount;
    SDL_Event event;
    Player players[16];
    Player mainPlayer;
    SDL_Renderer* screenRenderer;
    SDL_Window* window;
	Map *map;
    
    TTF_Font *font;

    SDL_Color colorWhite;
    SDL_Color colorWarGame;
	
	SDL_Rect pCenter;
	SDL_Rect camera;
	SDL_Rect bullets[250];
	SDL_Rect viewRect;
	SDL_Rect healthRect;
	SDL_Rect healthPos;
	SDL_Rect AmmoRect;
	SDL_Rect ammoPos;
	#pragma region  SDL_Surface
	double viewDegrees;

	SDL_Surface *mapSurface;
	SDL_Surface *viewSurface;
	SDL_Surface *AmmoSurface;
	SDL_Surface *healthSurface;
	SDL_Surface *bulletSurface;
	SDL_Surface *explodeSurface;
	SDL_Surface *bombSurface;
	SDL_Surface *characterSurface;
	SDL_Surface *characterEnnemiSurface;
	SDL_Surface *menuSurface;
	SDL_Surface *selectionSurface;

#pragma endregion

    SDL_Rect spriteRect;
    SDL_Rect mousePos;
}Engine;
extern Engine _engine;
BulletElm* headBullets;
#endif // ENGINE_H
