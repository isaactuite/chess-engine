
#include <math.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
int* gFrameBuffer;
SDL_Window* gSDLWindow;
SDL_Renderer* gSDLRenderer;
SDL_Texture* gSDLTexture;
static int gDone;


#define WINDOW_HEIGHT  1440
#define WINDOW_WIDTH 2560

int main(int argc, char** argv){
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)){
        return -1;
    }

    gFrameBuffer = (int*)malloc(WINDOW_WIDTH * WINDOW_HEIGHT * sizeof(int));
    gSDLWindow = SDL_CreateWindow("SDL3 window", WINDOW_WIDTH,WINDOW_HEIGHT, 0);
    gSDLRenderer = SDL_CreateRenderer(gSDLWindow, NULL);
    gSDLTexture = SDL_CreateTexture(gSDLRenderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);

    if (!gFrameBuffer || !gSDLWindow || !gSDLRenderer || !gSDLTexture){
        return -1;
    }

    gDone = 0;

    while (!gDone){
        loop();
    }

    SDL_DestroyTexture(gSDLTexture);
    SDL_DestroyRenderer(gSDLRenderer);
    SDL_DestroyWindow(gSDLWindow);
    SDL_Quit();

    return 0;


}