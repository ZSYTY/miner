#ifndef GAME_H
#define GAME_H

#include "graphics.h"
#include "extgraph.h"

typedef enum
{
    SMALL,
    MEDIUM,
    LARGE
} goldType;

typedef struct
{
    double x, y;
    goldType type;
} gold;

void initGame();

#endif