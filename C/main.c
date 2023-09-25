#define SDL_MAIN_HANDLED

#include "stdio.h"
#include "SDL.h"
#include "main.h"
#include "graphics.h"
#include <time.h>

void draw_tile(Rendering* rendering, Tile* tile, int x, int y) {
    // printf("X=%i Y=%i Tile(c=%i, m=%i, f=%i, v=%i)\r\n", x,y,tile->covered,tile->mine,tile->flag,tile->value);
    GameAssets* assets = rendering->assets;
    SDL_Texture* tex;

    if (tile->covered) {
        if (tile->flag) tex = assets->flagged_tile;
        else tex = assets->covered_tile;
    } else {
        if (tile->mine) tex = assets->mine_tile;
        else tex = assets->n_tiles[tile->value];
    }
    SDL_Rect dest_rect;
    Vector* dest = (Vector*) &dest_rect;
    dest_rect.x = x*TILE_SIZE;
    dest_rect.y = y*TILE_SIZE;
    add_vec(dest, &rendering->origin, dest);
    dest->y += BANNER_HEIGHT;
    dest_rect.w = TILE_SIZE; 
    dest_rect.h = TILE_SIZE; 
    SDL_RenderCopy(rendering->renderer, tex, NULL, &dest_rect);
    SDL_RenderPresent(rendering->renderer);
    SDL_Delay(5);
}

void gen_field(Rendering* rendering, Game* game, Minefield* field, int density) {
    Tile* curr = field->arr;
    for (int y = 0; y < field->height; y++) {
        for (int x = 0; x < field->width; x++) {
            *(char*) curr = 2 + (rand() % 100 < density);
            game->mines += curr->mine;
            draw_tile(rendering, curr, x, y);
            curr++;
        }
    }
}

void reset_game(Game* game) {
    game->won = 0;
    game->timer = 0;
    game->flags = 0;
    game->mines = 0;
    game->playing = 1;
    game->correct_flags = 0;
    game->covered_tiles = game->tile_count;
    game->start_time = time(NULL);
}

char get_neighbors(Minefield* field, int x, int y, Vector neighbors[9]) {
    char n = 0;
    for (int j = MAX(0,y-1); j < MIN(y+2, field->height); j++)
        for (int i = MAX(0,x-1); i < MIN(x+2, field->width); i++)
            neighbors[n++] = (Vector) {i,j};
    printf("neigh: [");
    for (int i = 0; i < n; i++) {
        printf("{%i,%i}, ", neighbors[i].x, neighbors[i].y);
    }
    printf("]\r\n");
    return n;
}

char check_tile(Minefield* field, int x, int y) {
    return x >= 0 && x < field->width && y >= 0 && y < field->height;
}

Tile* get_tile(Minefield* field, int x, int y) {
    return field->arr + field->width*y + x;
}

void quick_reveal(Rendering* rendering, Game* game, Minefield* field, Tile* tile, int x, int y) {
    Vector neighs[9];
    Tile* tiles[9];
    char n = get_neighbors(field, x, y, neighs);
    int n_flagged = 0;
    for (int i = 0; i < n; i++) {
        tiles[i] = get_tile(field, neighs[i].x, neighs[i].y);
        n_flagged += tiles[i]->flag;
    }

    if (n_flagged >= tile->value)
        for (int i = 0; i < n; i++)
            if (tiles[i]->covered && !tiles[i]->flag)
                reveal(rendering, game, field, tiles[i], neighs[i].x, neighs[i].y);
}

void quick_flag(Rendering* rendering, Game* game, Minefield* field, Tile* tile, int x, int y) {
    Vector neighs[9];
    Tile* tiles[9];
    char n = get_neighbors(field, x, y, neighs);
    int n_covered = 0;
    for (int i = 0; i < n; i++) {
        tiles[i] = get_tile(field, neighs[i].x, neighs[i].y);
        n_covered += tiles[i]->covered;
    }

    if (n_covered <= tile->value)
        for (int i = 0; i < n; i++)
            if (tiles[i]->covered && !tiles[i]->flag)
                flag(rendering, game, tiles[i], neighs[i].x, neighs[i].y);
}

void reveal(Rendering* rendering, Game* game, Minefield* field, Tile* tile, int x, int y) {
    if (!tile->covered) return;

    tile->covered = 0;
    game->covered_tiles--;

    Vector neighs[9];
    char n = get_neighbors(field, x, y, neighs);
    for (int i = 0; i < n; i++) 
        tile->value += get_tile(field, neighs[i].x, neighs[i].y)->mine;
    
    draw_tile(rendering, tile, x, y);

    if (tile->mine) {
        game->playing = 0;
        game->won = 0;
        return;
    }
    if (game->covered_tiles == game->mines) {
        game->playing = 0;
        game->won = 1;
    }

    if (tile->value == 0)
        for (int i = 0; i < n; i++) {
            Tile* t = get_tile(field, neighs[i].x, neighs[i].y);
            reveal(rendering, game, field, t, neighs[i].x, neighs[i].y);
        }
}

void flag(Rendering* rendering, Game* game, Tile* tile, int x, int y) {
    tile->flag = 1;
    game->flags++;
    if (tile->mine) {
        game->correct_flags++;
        char all_mines_flagged = game->correct_flags == game->mines;
        char all_flags_correct = game->flags == game->correct_flags;
        if (all_mines_flagged && all_flags_correct) {
            game->playing = 0;
            game->won = 1;
        }
    }
    draw_tile(rendering, tile, x, y);
}

void unflag(Rendering* rendering, Game* game, Tile* tile, int x, int y) {
    tile->flag = 0;
    game->flags--;
    if (tile->mine) game->correct_flags--;
    else {
        char all_mines_flagged = game->correct_flags == game->mines;
        char all_flags_correct = game->flags == game->correct_flags;
        if (all_mines_flagged && all_flags_correct) {
            game->playing = 0;
            game->won = 1;
        }
    }
    draw_tile(rendering, tile, x, y);
}

void left_click(Rendering* rendering, Game* game, Minefield* field, int x, int y) {
    if (check_tile(field, x, y)) {
        Tile* tile = get_tile(field, x, y);
        if (tile->flag) return;
        if (tile->covered) reveal(rendering, game, field, tile, x, y);
        else quick_reveal(rendering, game, field, tile, x, y);
    }
}

void right_click(Rendering* rendering, Game* game, Minefield* field, int x, int y) {
    if (check_tile(field, x, y)) {
        Tile* tile = get_tile(field, x, y);
        if (tile->covered) {
            if (tile->flag) unflag(rendering, game, tile, x, y);
            else flag(rendering, game, tile, x, y);
        }
        else quick_flag(rendering, game, field, tile, x, y);
    }
}

int main(int argc, char* argv[]) {
    srand(time(NULL));

    Minefield field;
    field.width = 25;
    field.height = 20;
    field.arr = (Tile*) malloc(sizeof(Tile)*field.width*field.height);

    Rendering rendering;
    rendering.origin = (Vector) {BORDER_WIDTH,BORDER_WIDTH};

    SDL_Rect bannerRect;
    bannerRect.x = rendering.origin.x;
    bannerRect.y = rendering.origin.y;
    bannerRect.w = field.width*TILE_SIZE;
    bannerRect.h = BANNER_HEIGHT;

    int res;
    int screenWidth = bannerRect.w + BORDER_WIDTH*2;
    int screenHeight = field.height*TILE_SIZE + BANNER_HEIGHT + BORDER_WIDTH*2;

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

    SDL_Rect squareRect = TILE_RECT;
    
    Tile myTile;
    myTile.mine = 1;
    myTile.covered = 1;
    myTile.flag = 1;
    myTile.value = 10;
    SDL_Event e;

    rendering.renderer = renderer;
    rendering.assets = assets;

    Game game;
    game.tile_count = field.width*field.height;
    reset_game(&game);

    SDL_SetRenderDrawColor(renderer, WHITE);  // Set the background color (black)
    SDL_RenderClear(renderer);
    gen_field(&rendering, &game, &field, 15);

    char running = 1;
    while (running) {
        int sel_x, sel_y;
        
        while(SDL_PollEvent(&e)){
            switch(e.type){
                case SDL_QUIT:
                    running = 0;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    sel_x = (e.button.x - rendering.origin.x)/TILE_SIZE;
                    sel_y = (e.button.y - rendering.origin.y - BANNER_HEIGHT)/TILE_SIZE;
                    printf("X: %i, Y: %i\r\n", sel_x, sel_y);
                    if (game.playing) {
                        if (e.button.button == SDL_BUTTON_LEFT)
                            left_click(&rendering, &game, &field, sel_x, sel_y);
                        else if (e.button.button == SDL_BUTTON_RIGHT)
                            right_click(&rendering, &game, &field, sel_x, sel_y);
                    }
                    break;
                case SDL_KEYDOWN:
                    if (e.key.keysym.sym == SDLK_SPACE)
                        reset_game(&game);
                        gen_field(&rendering, &game, &field, 10);
            }
        }



        // Vector mouse_pos;
        // SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
        if (game.playing)
            SDL_SetRenderDrawColor(renderer, WHITE);
        else if (game.won)
            SDL_SetRenderDrawColor(renderer, GREEN);
        else
            SDL_SetRenderDrawColor(renderer, RED);
        
        SDL_RenderFillRect(renderer, &bannerRect);
        // SDL_RenderCopy(renderer, assets->covered_tile, NULL, &squareRect);
        // squareRect.x += TILE_SIZE;
        // SDL_RenderCopy(renderer, assets->flagged_tile, NULL, &squareRect);
        // squareRect.x += TILE_SIZE;
        // SDL_RenderCopy(renderer, assets->mine_tile, NULL, &squareRect);
        // squareRect.x += TILE_SIZE;
        // for (int i = 0; i < 10; i++) {
        //     SDL_RenderCopy(renderer, assets->n_tiles[i], NULL, &squareRect);
        //     squareRect.x += TILE_SIZE;
        // }

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
