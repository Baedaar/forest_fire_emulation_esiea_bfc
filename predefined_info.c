#include "predefined_info.h"
#include <string.h>

// Tableau de structures contenant les informations pré-définies
struct predefinedInfo predefinedInfos[] = {
    {"sol", '+', 0, 0},
    {"arbre", '*', 4, 0},
    {"feuille", ' ', 2, 0},
    {"roche", '#', 5, 0},
    {"herbe", 'x', 3, 0},
    {"eau", '/', 0, 0},
    {"cendres", '-', 1, 0},
    {"cendres eteintes", '@', 0, 0}
};

// Le nombre d'éléments dans le tableau predefinedInfos
const int numPredefinedInfos = sizeof(predefinedInfos) / sizeof(predefinedInfos[0]);


// Fonction pour obtenir les informations pré-définies en fonction du type
struct predefinedInfo getInfoForType(const char *type) {
    struct predefinedInfo info;

    // Recherche dans le tableau d'informations pré-définies
    for (int i = 0; i < numPredefinedInfos; i++) {
        if (strcmp(type, predefinedInfos[i].type) == 0) {
            info = predefinedInfos[i];
            break; // Sortie de la boucle lorsque le type est trouvé
        }
    }

    return info;
}
