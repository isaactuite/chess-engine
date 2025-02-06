#include "SDL3/SDL.h"
#include "SDL3/SDL_ige.h"

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("SDL Init Error: %s", SDL_GetError());
        return 1;
    }

    if (IMG_Init(IMG_INIT_PNG) == 0) {
        SDL_Log("IMG_Init Error: %s", IMG_GetError());
        return 1;
    }

    SDL_Log("SDL3_image initialized successfully!");
    IMG_Quit();
    SDL_Quit();
    return 0;
}