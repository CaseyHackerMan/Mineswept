#include "graphics.h"
#include "stdio.h"
#include "string.h"
#include "SDL.h"
#include <sys/stat.h>


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

long read_num(FILE* file, int offset, char size) {
    long res = 0;
    fseek(file, offset, SEEK_SET);
    fread(&res, size, 1, file);
    return res;
}

SDL_Surface* load_bmp(SDL_Renderer* renderer, const char* path) {
    FILE* asset = fopen(path,"rb");
    if (asset == NULL) return NULL;
    
    struct stat sb;
    if (stat(path, &sb) == -1) return NULL;
    if (read_num(asset, 0, 2) != 19778) return NULL;
    
    int w = read_num(asset, 18, 4);
    int h = read_num(asset, 22, 4);
    int pix = read_num(asset, 10, 4);
    int depth = read_num(asset, 28, 2);
    int file_size = read_num(asset, 2, 4);
    if (sb.st_size != file_size) return NULL;

    SDL_Surface* surf = SDL_CreateRGBSurfaceWithFormat(0,w,h,depth,SDL_PIXELFORMAT_BGR24);
    if (surf == NULL) return NULL;
    fseek(asset, pix, SEEK_SET);
    fread(surf->pixels, depth, w*h, asset);
    
    fclose(asset);
    return surf;
}

SDL_Texture* asset_to_texture(SDL_Renderer* renderer, const char* name) {
    char path[50] = "../assets/";
    strncat(path, name, 40);
    SDL_Surface* surf = load_bmp(renderer, path);
    if (surf == NULL) {
        printf("ERROR: failed to load asset \"%s\"\r\n", name);
        exit(1);
    }
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
    return tex;
}

GameAssets* generate_assets(SDL_Renderer* renderer) {
    GameAssets* assets = (GameAssets*) malloc(sizeof(GameAssets));
    assets->covered_tile = asset_to_texture(renderer,"covered_tile.bmp");
    assets->flagged_tile = asset_to_texture(renderer,"flagged_tile.bmp");
    assets->mine_tile = asset_to_texture(renderer,"mine_tile.bmp");
    assets->n_tiles[0] = asset_to_texture(renderer,"empty_tile.bmp");
    assets->n_tiles[1] = asset_to_texture(renderer,"one_tile.bmp");
    assets->n_tiles[2] = asset_to_texture(renderer,"two_tile.bmp");
    assets->n_tiles[3] = asset_to_texture(renderer,"three_tile.bmp");
    assets->n_tiles[4] = asset_to_texture(renderer,"four_tile.bmp");
    assets->n_tiles[5] = asset_to_texture(renderer,"five_tile.bmp");
    assets->n_tiles[6] = asset_to_texture(renderer,"six_tile.bmp");
    assets->n_tiles[7] = asset_to_texture(renderer,"seven_tile.bmp");
    assets->n_tiles[8] = asset_to_texture(renderer,"eight_tile.bmp");
    assets->n_tiles[9] = asset_to_texture(renderer,"nine_tile.bmp");
    return assets;
}

void free_assets(GameAssets* assets) {
    SDL_DestroyTexture(assets->covered_tile);
    SDL_DestroyTexture(assets->flagged_tile);
    SDL_DestroyTexture(assets->mine_tile);
    for (int i = 0; i < 10; i++)
        SDL_DestroyTexture(assets->n_tiles[i]);
    free(assets);
}