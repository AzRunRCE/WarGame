#ifndef FT_DISCONNECT_H
#define FT_DISCONNECT_H

#include <SDL.h>

#ifdef _WIN32 || _WIN64
/* si vous êtes sous Windows */
#include <pthread_VC.h>
#define SOCKET_ERRNO    WSAGetLastError()
#elif defined linux || defined __linux || defined __linux__
/* si vous êtes sous linux */
#include <pthread.h>
#else
/* sinon vous êtes sur une plateforme non supportée */
#error not defined for this platform
#endif

pthread_t DisconnectThread;
int threadStartDisconnect(void);
void *DisconnectThreading(void);
static uint8_t playerCount;

#endif /* FT_DISCONNECT_H */