#ifndef PREDEFINED_INFO_H
#define PREDEFINED_INFO_H

#include "structure.h"

// Structure pour stocker les informations pré-définies pour chaque type
struct predefinedInfo {
    char type[20]; // Le type
    char symbol;   // Le symbole
    int degree;    // Le degré
    int state;     // L'état
};


// Fonction pour obtenir les informations pré-définies en fonction du type
struct predefinedInfo getInfoForType(const char *type);

#endif
