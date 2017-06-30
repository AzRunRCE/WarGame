#ifndef CLIENT_H
#define CLIENT_H

#include "server.h"
#include <SDL.h>
typedef struct
{
    SOCKADDR_IN sin;
    char name[BUF_SIZE];
}Client;

#endif /* guard */
