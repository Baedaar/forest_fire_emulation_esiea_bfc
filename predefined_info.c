#include "predefined_info.h"
#include <string.h>

struct predefinedInfo predefinedInfos[] = {
    {"sol", '+', 0, 0},
    {"arbre", '*', 4, 0},
    {"feuille", ' ', 2, 0},
    {"roche", '#', 5, 0},
    {"herbe", 'x', 3, 0},
    {"eau", '/', 0, 0},
    {"cendres", '-', 1, 0},
    {"cendres eteintes", '@', 0, 0}};

const int numPredefinedInfos = sizeof(predefinedInfos) / sizeof(predefinedInfos[0]);

struct predefinedInfo getInfoForType(const char *type)
{
    struct predefinedInfo info;

    for (int i = 0; i < numPredefinedInfos; i++)
    {
        if (strcmp(type, predefinedInfos[i].type) == 0)
        {
            info = predefinedInfos[i];
            break;
        }
    }

    return info;
}
