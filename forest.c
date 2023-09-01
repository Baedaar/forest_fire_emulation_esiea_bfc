#include <stdio.h>
#include <stdlib.h>
#include "structure.h"


// Fonction permettant de l'allocation de mémoire pour la forêt.
void initializeForest(struct forest *f) {
    f -> matrice = (struct cell**)malloc(f -> rows * sizeof(struct cell *));
        for (int i = 0; i < f -> rows; i++)
        {
            f->matrice[i] = (struct cell *)malloc(f->columns * sizeof(struct cell));
        }
         
}

// Fonction permettant de libérer la mémoire.
void freeForest(struct forest *f) {
    for (int i = 0; i < f->rows; i++) {
        for (int j = 0; j < f->columns; j++) {
            free(f->matrice[i][j].type); // Libérer la mémoire allouée pour le membre "type"
        }
        free(f->matrice[i]); // Libérer la mémoire pour chaque ligne de cellules
    }
    free(f->matrice); // Libérer la mémoire pour la grille
}