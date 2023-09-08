#define SDL_MAIN_HANDLED

#include "stdio.h"
#include "SDL.h"
#include "main.h"
#include "graphics.h"

int main(int argc, char* argv[]) {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    int res;
    int screenWidth = 640;  // Adjust these values as per your requirements
    int screenHeight = 480;

    res = SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER);
    if (res) printf(SDL_GetError());
    
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"linear");
    window = SDL_CreateWindow("Mineswept", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    //SDL_WarpMouseInWindow(window, 50, 50); // move mouse

    GameAssets* assets = generate_assets(renderer);

    SDL_Rect squareRect;
    squareRect.w = 20;
    squareRect.h = 20;

    Tile myTile;
    myTile.mine = 1;
    myTile.covered = 1;
    myTile.flag = 1;
    myTile.value = 10;
    SDL_Event e;

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
        SDL_RenderCopy(renderer, assets->empty_tile, NULL, &squareRect);
        squareRect.x += TILE_SIZE;
        SDL_RenderCopy(renderer, assets->covered_tile, NULL, &squareRect);
        squareRect.x += TILE_SIZE;
        SDL_RenderCopy(renderer, assets->flagged_tile, NULL, &squareRect);
        squareRect.x += TILE_SIZE;
        SDL_RenderCopy(renderer, assets->mine_tile, NULL, &squareRect);
        
        SDL_RenderPresent(renderer);
        SDL_Delay(30);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    free_assets(assets);

    SDL_Quit();

    printf("Good ending\r\n");
    return 0;
}
