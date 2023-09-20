#define SDL_MAIN_HANDLED

#include "stdio.h"
#include "SDL.h"
#include "main.h"
#include "graphics.h"
#include <time.h>

void draw_tile(Rendering* rendering, Minefield* field, Vector ind) {
    Tile tile = field->arr[field->width*ind.y + ind.x];
    GameAssets* assets = rendering->assets;
    SDL_Texture* tex;

    if (tile.covered) {
        if (tile.flag) tex = assets->flagged_tile;
        else tex = assets->covered_tile;
    } else {
        if (tile.mine) tex = assets->mine_tile;
        else tex = assets->n_tiles[tile.value-1];
    }
    SDL_Rect dest_rect;
    Vector* dest = (Vector*) &dest_rect;
    mul_vec(&ind, TILE_SIZE, dest);
    add_vec(dest, &rendering->origin, dest);
    dest->y += BANNER_HEIGHT;
    dest_rect.w = TILE_SIZE; 
    dest_rect.h = TILE_SIZE; 
    SDL_RenderCopy(rendering->renderer, tex, NULL, &dest_rect);
    SDL_RenderPresent(rendering->renderer);
    SDL_Delay(5);
}

void gen_field(Rendering* rendering, Game* game, int density) {
    return;
}

int main(int argc, char* argv[]) {
    int res;
    int screenWidth = 640;  // Adjust these values as per your requirements
    int screenHeight = 480;

    res = SDL_Init(0);
    if (res) printf(SDL_GetError());
    
    // SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"linear");
    SDL_Window* window = SDL_CreateWindow("Mineswept", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_RESIZABLE);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Surface* root = SDL_GetWindowSurface(window);
    //SDL_WarpMouseInWindow(window, 50, 50); // move mouse

    GameAssets* assets = generate_assets(renderer);
    if (assets == NULL) {
        printf("ERROR: failed to generate assets\r\n");
        return 1;
    }

    SDL_Rect squareRect;
    squareRect.w = 24;
    squareRect.h = 24;

    Tile myTile;
    myTile.mine = 1;
    myTile.covered = 1;
    myTile.flag = 1;
    myTile.value = 10;
    SDL_Event e;

    Minefield field;
    field.width = 25;
    field.height = 20;
    field.arr = (Tile*) malloc(sizeof(Tile)*field.width*field.height);

    char running = 1;
    while (running) {
        while(SDL_PollEvent(&e)){
            switch(e.type){
                case SDL_QUIT:
                    running = 0;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    printf("X: %i, Y: %i\r\n", e.button.x, e.button.y);
                    break;
            }
        }
        SDL_SetRenderDrawColor(renderer, WHITE);  // Set the background color (black)
        SDL_RenderClear(renderer);

        Vector mouse_pos;
        SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
        squareRect.x = mouse_pos.x-TILE_SIZE;
        squareRect.y = mouse_pos.y-TILE_SIZE;
        // SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
        // SDL_RenderFillRect(renderer, &squareRect);
        SDL_RenderCopy(renderer, assets->covered_tile, NULL, &squareRect);
        squareRect.x += TILE_SIZE;
        SDL_RenderCopy(renderer, assets->flagged_tile, NULL, &squareRect);
        squareRect.x += TILE_SIZE;
        SDL_RenderCopy(renderer, assets->mine_tile, NULL, &squareRect);
        squareRect.x += TILE_SIZE;
        for (int i = 0; i < 10; i++) {
            SDL_RenderCopy(renderer, assets->n_tiles[i], NULL, &squareRect);
            squareRect.x += TILE_SIZE;
        }

        
        SDL_RenderPresent(renderer);
        SDL_Delay(30);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    free(field.arr);
    free_assets(assets);

    SDL_Quit();

    printf("Good ending\r\n");
    return 0;
}
