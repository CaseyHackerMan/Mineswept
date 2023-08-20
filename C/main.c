#define SDL_MAIN_HANDLED

#include "stdio.h"
#include "SDL.h"
#include "main.h"
#include "graphics.h"

int main(int argc, char* argv[]) {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    int screenWidth = 640;  // Adjust these values as per your requirements
    int screenHeight = 480;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Square Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Rect squareRect;
    squareRect.w = 20;
    squareRect.h = 20;

    Tile myTile;
    myTile.mine = 1;
    myTile.covered = 1;
    myTile.flag = 1;
    myTile.value = 10;

    char running = 1;
    while (running) {
        SDL_Event e;
        while(SDL_PollEvent(&e)){
            switch(e.type){
                case SDL_QUIT:
                    running = 0;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    printf("Tile(%i)\r\n", myTile.value);
                    break;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Set the background color (black)
        SDL_RenderClear(renderer);

        Vector mouse_pos;
        SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
        squareRect.x = mouse_pos.x-10;
        squareRect.y = mouse_pos.y-10;
        SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
        SDL_RenderFillRect(renderer, &squareRect);
        
        SDL_RenderPresent(renderer);
        SDL_Delay(30);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
