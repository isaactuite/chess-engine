#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("SDL_Init Error: %s", SDL_GetError());
        return 1;
    }
    SDL_Window* window = SDL_CreateWindow("Test", 800, 600, 0);
    if (!window) {
        SDL_Log("Window Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_Delay(2000); // Keep window open for 2 seconds
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}// test.c

