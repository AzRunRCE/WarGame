#ifndef PACKET_H
#define PACKET_H
#include "sprite.h"
#include "main.h"


typedef struct
{
    int Y;
    int X;
    State state;
    char name[256];
    bool walk;
    bool fire;
}Packet;

#endif // PACKET_H
