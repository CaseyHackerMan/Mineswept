#include "graphics.h"

#ifndef TILE_H
#define TILE_H

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

typedef struct tile {
    unsigned char mine    : 1;
    unsigned char covered : 1;
    unsigned char flag    : 1;
    unsigned char drawn   : 1;
    unsigned char value   : 4;
} Tile;

typedef struct minefield {
    Tile* arr;
    int width;
    int height;
} Minefield;

char check_tile(Minefield* field, int x, int y);
Tile* get_tile(Minefield* field, int x, int y);
char get_neighbors(Minefield* field, int x, int y, Vector neighbors[9]);
int gen_field(Minefield* field, int density);

#endif
