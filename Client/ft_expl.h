typedef struct Item Item;
typedef enum ItemValue ItemValue;

struct Item {
	SDL_Rect Sprite;
	char label[720]; // Tableau en fonction de la taille de la map
};
