#ifndef CLIENT_H
#define CLIENT_H
#include <Winsock2.h>


#define CRLF	 "\r\n"
#define PORT	 1977

#define BUF_SIZE 1024
static void init();
void end();
static void app(const char *address, const char *name);
static int init_connection(const char *address, SOCKADDR_IN *sin);
static void end_connection(int sock);


#endif /* guard */
