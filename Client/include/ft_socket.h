#ifndef SOCKET_H
#define SOCKET_H
#include "main.h"
#include "ft_configuration.h"
void *SreamClientData(void *arg);
void *NetworkThreadingListening(void *arg);
int create_connection(configuration *settings);
int sendMessage(const uint8_t *buffer, const int length);
int write_client(const char *buffer, const int length);
void end();
bool connected;
#endif // SOCKET_H
