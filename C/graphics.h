#include "SDL.h"

#ifndef GRAPHICS_H
#define GRAPHICS_H

#define TILE_SIZE 24
#define BANNER_HEIGHT 50
#define FIELD_POS (Vector) {0, BANNERHEIGHT}
#define TILE_RECT (SDL_Rect) {0, 0, TILE_SIZE, TILE_SIZE}
#define PIX_FORMAT 

#define WHITE      255, 255, 255, 255
#define PINK       255, 0, 255, 255
#define RED        255, 0, 0, 255
#define YELLOW     255, 255, 0, 255
#define GREEN      0, 100, 0, 255
#define CYAN       0, 139, 139, 255
#define BLUE       0, 0, 255, 255
#define GREY       153, 153, 153, 255
#define DARK_GREEN 0, 100, 0, 255
#define DARK_BLUE  0, 0, 139, 255
#define DARK_RED   139, 0, 0, 255
#define LIGHT_GREY 217, 217, 217, 255
#define DARK_GREY  102, 102, 102, 255
#define BLACK      0, 0, 0, 255

typedef struct vector {
    int x, y;
} Vector;

typedef struct assets {
    SDL_Texture* covered_tile;
    SDL_Texture* flagged_tile;
    SDL_Texture* mine_tile;
    SDL_Texture* n_tiles[10];
} GameAssets;

Vector* add_vec(Vector* v1, Vector* v2, Vector* res);
Vector* sub_vec(Vector* v1, Vector* v2, Vector* res);
Vector* mul_vec(Vector* v, float a, Vector* res);
Vector* div_vec(Vector* v, float a, Vector* res);

void free_assets(GameAssets* ptr);
GameAssets* generate_assets();

#endif