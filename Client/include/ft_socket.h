#ifndef SOCKET_H
#define SOCKET_H
#include "main.h"
#include "ft_configuration.h"
void *SreamClientData(void *arg);
void *NetworkThreadingListening(void *arg);
int sendMessage(const char *buffer, const int length);
int create_connection(configuration *settings);
void end();
bool connected;
#endif // SOCKET_H
