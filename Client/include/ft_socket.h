#ifndef SOCKET_H
#define SOCKET_H
#include "main.h"
void *SreamClientData(void *arg);
void *NetworkThreadingListening(void *arg);
int sendMessage(const char *buffer, const int length);
void end();
bool connected;
#endif // SOCKET_H
