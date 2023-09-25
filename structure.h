#ifndef FORREST 
#define FORREST

// Structure définissant chaque attribut d'une cellule composant la forêt.
struct cell
{
    char *type;
    char symbol;
    int degree;
    int state;
    int isSelected;
    int decremented;
};



// Structure pour créer la forêt.
struct forest
{
    int rows;
    int columns;
    struct cell **matrice;
};


// Déclaration de fonctions allouant et libérant de la mémoire pour la fôret et les cellules.
void initializeForest(struct forest *f);
void freeForest(struct forest *f);





#endif