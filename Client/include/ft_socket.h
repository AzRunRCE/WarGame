#ifndef SOCKET_H
#define SOCKET_H
void *SreamClientData(void *arg);
void *NetworkThreadingListening(void *arg);
int sendMessage(const char *buffer, const int length);
typedef struct
{
	SDL_Rect pos;
	struct BulletElm *next;
	struct BulletElm *previous;
}BulletElm;
#endif // SOCKET_H
