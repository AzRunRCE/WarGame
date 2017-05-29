#ifndef ITEM_H_INCLUDED
#define ITEM_H_INCLUDED

typedef struct Item Item;
typedef enum ItemValue ItemValue;

struct Item{
    SDL_Rect Sprite;
    char label[720]; // Tableau en fonction de la taille de la map
};

enum ItemValue
{
    BOMB, SWORD, KNIFE
};
#endif // ITEM_H_INCLUDED
