#ifndef ENGINE_H
#define ENGINE_H

typedef struct Engine Engine;

struct Engine {
    Player[15] playerList;
    Item[15] ItemList;
    int[50][50] Map;
};


#endif // ENGINE_H
