#include "graphics.h"
#include "stdio.h"

GameAssets* generate_assets(SDL_Renderer* renderer) {
    int res;
    //if (res) printf(SDL_GetError());
    SDL_Color n_colors[] = {BLUE, DARK_GREEN, RED, DARK_BLUE, DARK_RED, CYAN, BLACK, DARK_GREY, PINK};
    GameAssets* assets = (GameAssets*) malloc(sizeof(GameAssets));

    
    SDL_Surface* empty_tile = SDL_CreateRGBSurface(0, TILE_SIZE, TILE_SIZE, 32, BLACK);
    SDL_Surface* covered_tile = SDL_CreateRGBSurface(0, TILE_SIZE, TILE_SIZE, 32, BLACK);
    if (empty_tile == NULL) printf(SDL_GetError());
    if (covered_tile == NULL) printf(SDL_GetError());

    SDL_FillRect(empty_tile, &(SDL_Rect) {1,1,TILE_SIZE-2,TILE_SIZE-2}, SDL_MapRGBA(empty_tile->format,LIGHT_GREY));

    // probably inefficient
    SDL_FillRect(covered_tile, &(SDL_Rect) {1,1,TILE_SIZE-2,TILE_SIZE-2}, SDL_MapRGBA(empty_tile->format,DARK_GREY));
    SDL_FillRect(covered_tile, &(SDL_Rect) {3,3,TILE_SIZE-6,TILE_SIZE-6}, SDL_MapRGBA(empty_tile->format,GREY));

    assets->empty_tile = SDL_CreateTextureFromSurface(renderer, empty_tile);
    if (assets->empty_tile == NULL) printf(SDL_GetError());

    assets->covered_tile = SDL_CreateTextureFromSurface(renderer, covered_tile);
    if (assets->covered_tile == NULL) printf(SDL_GetError());
    
    assets->flagged_tile = SDL_CreateTextureFromSurface(renderer, empty_tile);
    assets->mine_tile = SDL_CreateTextureFromSurface(renderer, empty_tile);
    for (int i = 0; i < 9; i++) {
        assets->n_tiles[i] = SDL_CreateTextureFromSurface(renderer, empty_tile);
    }

    SDL_FreeSurface(empty_tile);
    return assets;
}

void free_assets(GameAssets* assets) {
    SDL_DestroyTexture(assets->covered_tile);
    SDL_DestroyTexture(assets->empty_tile);
    SDL_DestroyTexture(assets->flagged_tile);
    SDL_DestroyTexture(assets->mine_tile);
    for (int i = 0; i < 9; i++)
        SDL_DestroyTexture(assets->n_tiles[i]);
    free(assets);
}

Vector* add_vec(Vector* v1, Vector* v2, Vector* res) {
    res->x = v1->x + v2->x;
    res->y = v1->y + v2->y;
    return res;
}

Vector* sub_vec(Vector* v1, Vector* v2, Vector* res) {
    res->x = v1->x - v2->x;
    res->y = v1->y - v2->y;
    return res;
}

Vector* mul_vec(Vector* v, float a, Vector* res) {
    res->x = v->x*a;
    res->y = v->y*a;
    return res;
}

Vector* div_vec(Vector* v, float a, Vector* res) {
    res->x = v->x/a;
    res->y = v->y/a;
    return res;
}