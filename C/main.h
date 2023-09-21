#include <time.h>
#include "graphics.h"

#ifndef MAIN_H
#define MAIN_H

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

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

Tile* get_tile(Minefield* field, int x, int y);
char get_neighbors(Minefield* field, int x, int y, Vector neighbors[9]);
void draw_tile(Rendering* rendering, Tile* tile, int x, int y);
void reveal(Rendering* rendering, Game* game, Minefield* field, Tile* tile, int x, int y);
void flag(Game* game, Tile* tile, int x, int y);
void unflag(Game* game, Tile* tile, int x, int y);
void reset_game(Game* game);
void gen_field(Rendering* rendering, Game* game, Minefield* field, int density);
void left_click(Rendering* rendering, Game* game, Minefield* field, int x, int y);
void right_click(Rendering* rendering, Game* game, Minefield* field, int x, int y);
int main(int argc, char* argv[]);

#endif
