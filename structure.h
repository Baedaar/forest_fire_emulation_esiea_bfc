#ifndef FORREST
#define FORREST

struct cell
{
    char *type;
    char symbole;
    int degree;
    int etat;
    int isSelected;
    int decremented;
};

struct forest
{
    int lignes;
    int colonnes;
    struct cell **matrice;
    int **nextChanges;
};

void startForest(struct forest *f);
void freeForest(struct forest *f);
void initNextChanges(struct forest *forest);
void freeNextChanges(struct forest *forest);

#endif