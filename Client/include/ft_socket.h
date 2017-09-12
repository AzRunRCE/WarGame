#ifndef SOCKET_H
#define SOCKET_H
void *SreamClientData(void *arg);
void *NetworkThreadingListening(void *arg);
int sendMessage(const char *buffer, const int length);

#endif // SOCKET_H
