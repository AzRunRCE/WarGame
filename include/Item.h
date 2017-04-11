#ifndef ITEM_H_INCLUDED
#define ITEM_H_INCLUDED

typedef struct Item Item;

struct Item{
    SDL_Rect Sprite;
    char nomItem[720][720]; // Tableau en fonction de la taille de la map
};

#endif // ITEM_H_INCLUDED
