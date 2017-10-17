#ifndef CLIENT_H
#define CLIENT_H
#ifdef _WIN32 || _WIN64 /* si vous êtes sous Windows */
#include <winsock2.h>

#elif defined linux || defined __linux || defined __linux__ /* si vous êtes sous linux */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;
#else /* sinon vous êtes sur une plateforme non supportée */
#error not defined for this platform
#endif



#define CRLF	 "\r\n"
#define PORT	 1977

#define BUF_SIZE 1024
static void init();
void end();
static void app(const char *address, const char *name);
static int init_connection(const char *address, SOCKADDR_IN *sin);
static void end_connection(int sock);


#endif /* guard */
