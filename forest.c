#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structure.h"

void startForest(struct forest *f)
{
    f->matrice = (struct cell **)malloc(f->lignes * sizeof(struct cell *));
    for (int i = 0; i < f->lignes; i++)
    {
        f->matrice[i] = (struct cell *)malloc(f->colonnes * sizeof(struct cell));
    }
}

void freeForest(struct forest *f)
{
    for (int i = 0; i < f->lignes; i++)
    {
        for (int j = 0; j < f->colonnes; j++)
        {
            free(f->matrice[i][j].type);
        }
        free(f->matrice[i]);
    }
    free(f->matrice);
}
void initNextChanges(struct forest *forest)
{
    forest->nextChanges = malloc(forest->lignes * sizeof(int *));
    for (int i = 0; i < forest->lignes; i++)
    {
        forest->nextChanges[i] = malloc(forest->colonnes * sizeof(int));
        memset(forest->nextChanges[i], 0, forest->colonnes * sizeof(int));
    }
}

void freeNextChanges(struct forest *forest)
{
    for (int i = 0; i < forest->lignes; i++)
    {
        free(forest->nextChanges[i]);
    }
    free(forest->nextChanges);
}