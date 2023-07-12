#include <SDL2/SDL.h>

int main(int argc, char* argv[]) {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    int screenWidth = 640;  // Adjust these values as per your requirements
    int screenHeight = 480;
    int squareSize = 200;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Square Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Set the background color (black)

    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // Set the square color (white)

    SDL_Rect squareRect;
    squareRect.x = (screenWidth - squareSize) / 2;  // Center the square horizontally
    squareRect.y = (screenHeight - squareSize) / 2;  // Center the square vertically
    squareRect.w = squareSize;
    squareRect.h = squareSize;

    SDL_RenderFillRect(renderer, &squareRect);

    SDL_RenderPresent(renderer);

    SDL_Delay(2000);  // Pause for 2 seconds

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
