#include "tile.h"
#include "stdio.h"
#include "graphics.h"

char check_tile(Minefield* field, int x, int y) {
    return x >= 0 && x < field->width && y >= 0 && y < field->height;
}

Tile* get_tile(Minefield* field, int x, int y) {
    Tile* tile = field->arr + field->width*y + x;
    printf("Tile(%i,%i): %02X\r\n",x,y,*(char*)tile);
    return tile;
}

char get_neighbors(Minefield* field, int x, int y, Vector neighbors[9]) {
    char n = 0;
    for (int j = MAX(0,y-1); j < MIN(y+2, field->height); j++)
        for (int i = MAX(0,x-1); i < MIN(x+2, field->width); i++)
            neighbors[n++] = (Vector) {i,j};
    // printf("neigh: [");
    // for (int i = 0; i < n; i++) {
    //     printf("{%i,%i}, ", neighbors[i].x, neighbors[i].y);
    // }
    // printf("]\r\n");
    return n;
}

int gen_field(Minefield* field, int density) {
    int m = 0;
    Tile* end = field->arr + field->height*field->width*sizeof(Tile);
    for (Tile* curr = field->arr; curr < end; curr++) {
        if (!(curr->drawn && curr->covered && !curr->flag)) {
            curr->drawn = 0;
            curr->covered = 1;
            curr->flag = 0;
        }
        curr->value = 0;
        m += curr->mine = rand() % 100 < density;
    }
    return m;
}
