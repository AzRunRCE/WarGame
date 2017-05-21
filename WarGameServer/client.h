#ifndef CLIENT_H
#define CLIENT_H

#include "server.h"

typedef struct
{
    SOCKADDR_IN sin;
    char name[BUF_SIZE];
}Client;

typedef enum
{
    UP_IDLE= 0,
    UP_RIGHT_IDLE= 0,
    RIGHT_IDLE= 0,
    DOWN_RIGHT_IDLE= 0,
    DOWN_IDLE= 0,
    DOWN_LEFT_IDLE= 0,
    LEFT_IDLE= 0,
    UP_LEFT_IDLE = 0,

    UP = 1,
    UP_RIGHT = 2,
    RIGHT = 3,
    DOWN_RIGHT = 4,
    DOWN = 5,
    DOWN_LEFT = 6,
    LEFT = 7,
    UP_LEFT = 8,
    FIRE = 9,
    UP_FIRE = 9,
    UP_RIGHT_FIRE = 9,
    RIGHT_FIRE = 9,
    DOWN_RIGHT_FIRE = 9,
    DOWN_FIRE = 9,
    DOWN_LEFT_FIRE = 9,
    LEFT_FIRE = 9,
    UP_LEFT_FIRE = 9
} State;
typedef int bool;
#define true 1
#define false 0


typedef struct
{
    int Y;
    int X;
    State state;
    char name[256];
    bool walk;
    bool fire;
}Packet;

#endif /* guard */
