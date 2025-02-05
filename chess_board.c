#include <string.h>
#include <math.h>
#include <stdlib.h>
#ifdef EMSCRIPTEN
#include <emscripten/emscripten.h>
#endif
#include "SDL3/SDL.h"
#include "SDL3/SDL_main.h"

int* gFrameBuffer;
SDL_Window* gSDLWindow;
SDL_Renderer* gSDLRenderer;
SDL_Texture* gSDLTexture;
static int gDone;

#define WINDOW_WIDTH (1920)
#define WINDOW_HEIGHT (1080)

int update(void) {
    SDL_Event e;
    if (SDL_PollEvent(&e)) {
        if (e.type == SDL_EVENT_QUIT) {
            return 0;
        }
        if ((e.type == SDL_EVENT_KEY_UP) && (e.key.key == SDLK_ESCAPE)) {
            return 0;
        }
    }

    char* pix;
    int pitch;
    SDL_LockTexture(gSDLTexture, NULL, (void**)&pix, &pitch);
    
    for (int i = 0, sp = 0, dp = 0; i < WINDOW_HEIGHT; i++, dp += WINDOW_WIDTH, sp += pitch) {
        memcpy(pix + sp, gFrameBuffer + dp, WINDOW_WIDTH * 4);
    }
    
    SDL_UnlockTexture(gSDLTexture);
    SDL_RenderTexture(gSDLRenderer, gSDLTexture, NULL, NULL);
    SDL_RenderPresent(gSDLRenderer);
    SDL_Delay(1);
    return 1;
}

void render(Uint64 aTicks) {
    for (int i = 0, c = 0; i < WINDOW_HEIGHT; i++) {
        for (int j = 0; j < WINDOW_WIDTH; j++, c++) {
            gFrameBuffer[c] = (int)(i * i + j * j + aTicks) | 0xff000000;
        }
    }
}

void loop(void) {
    if (!update()) {
        gDone = 1;
        #ifdef EMSCRIPTEN
        emscripten_cancel_main_loop();
        #endif
    } else {
        render(SDL_GetTicks());
    }
}

int main(int argc, char** argv) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
        return -1;
    }

    gFrameBuffer = (int*)malloc(WINDOW_WIDTH * WINDOW_HEIGHT * sizeof(int));
    gSDLWindow = SDL_CreateWindow("SDL3 window", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    gSDLRenderer = SDL_CreateRenderer(gSDLWindow, NULL);
    gSDLTexture = SDL_CreateTexture(gSDLRenderer, SDL_PIXELFORMAT_ABGR8888, 
                                   SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);

    if (!gFrameBuffer || !gSDLWindow || !gSDLRenderer || !gSDLTexture) {
        return -1;
    }

    gDone = 0;

    #ifdef EMSCRIPTEN
    emscripten_set_main_loop(loop, 0, 1);
    #else
    while (!gDone) {
        loop();
    }
    #endif

    free(gFrameBuffer);
    SDL_DestroyTexture(gSDLTexture);
    SDL_DestroyRenderer(gSDLRenderer);
    SDL_DestroyWindow(gSDLWindow);
    SDL_Quit();
    return 0;
}