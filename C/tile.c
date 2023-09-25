#include "tile.h"
#include "stdio.h"
#include "graphics.h"

char check_tile(Minefield* field, int x, int y) {
    return x >= 0 && x < field->width && y >= 0 && y < field->height;
}

Tile* get_tile(Minefield* field, int x, int y) {
    return field->arr + field->width*y + x;
}

char get_neighbors(Minefield* field, int x, int y, Vector neighbors[9]) {
    char n = 0;
    for (int j = MAX(0,y-1); j < MIN(y+2, field->height); j++)
        for (int i = MAX(0,x-1); i < MIN(x+2, field->width); i++)
            neighbors[n++] = (Vector) {i,j};
    printf("neigh: [");
    for (int i = 0; i < n; i++) {
        printf("{%i,%i}, ", neighbors[i].x, neighbors[i].y);
    }
    printf("]\r\n");
    return n;
}
