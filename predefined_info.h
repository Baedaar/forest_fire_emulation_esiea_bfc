#ifndef PREDEFINED_INFO_H
#define PREDEFINED_INFO_H

#include "structure.h"

struct predefinedInfo
{
    char type[20];
    char symbole;
    int degree;
    int etat;
};

struct predefinedInfo getInfoForType(const char *type);

#endif
