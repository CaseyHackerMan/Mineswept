#include <time.h>
#include "graphics.h"

#ifndef MAIN_H
#define MAIN_H

typedef struct tile {
    char mine    : 1;
    char covered : 1;
    char flag    : 1;
    char value   : 4;
} Tile;

typedef struct minefield {
    Tile* arr;
    int width;
    int height;
} Minefield;

typedef struct game {
    clock_t timer;
    clock_t start_time;
    Minefield* field;
    int tile_count;
    int mines;
    int flags;
    int correct_flags;
    int covered_tiles;
    char playing : 1;
    char won : 1;
} Game;

typedef struct rendering {
    GameAssets* assets;
    SDL_Renderer* renderer;
    Vector origin;
} Rendering;

void draw_tile(Rendering* rendering, Minefield* field, Vector ind);
void gen_field(Rendering* rendering, Game* game, int density);
int main(int argc, char* argv[]);

#endif
