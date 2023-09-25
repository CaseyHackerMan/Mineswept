#include <time.h>
#include "graphics.h"
#include "tile.h"
#include "SDL.h"

#ifndef MAIN_H
#define MAIN_H

typedef struct game {
    clock_t timer;
    clock_t start_time;
    int tile_count;
    int mines;
    int flags;
    int correct_flags;
    int covered_tiles;
    int density;
    char playing : 1;
    char won : 1;
} Game;

typedef struct rendering {
    GameAssets* assets;
    SDL_Renderer* renderer;
    SDL_Rect* banner_rect;
    SDL_Rect* field_rect;
} Rendering;

void draw_tile(Rendering* rendering, Tile* tile, int x, int y);
void reveal(Rendering* rendering, Game* game, Minefield* field, Tile* tile, int x, int y);
void flag(Rendering* rendering, Game* game, Tile* tile, int x, int y);
void unflag(Rendering* rendering, Game* game, Tile* tile, int x, int y);
void quick_reveal(Rendering* rendering, Game* game, Minefield* field, Tile* tile, int x, int y);
void quick_flag(Rendering* rendering, Game* game, Minefield* field, Tile* tile, int x, int y);
void draw_field(Rendering* rendering, Minefield* field);
void reset_game(Game* game, Minefield* field);
void draw_field(Rendering* rendering, Minefield* field);
void left_click(Rendering* rendering, Game* game, Minefield* field, int x, int y);
void right_click(Rendering* rendering, Game* game, Minefield* field, int x, int y);
int main(int argc, char* argv[]);

#endif
