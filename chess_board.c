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
int WINDOW_WIDTH = 1920 / 2;
int WINDOW_HEIGHT = 1080 / 2;
int SQUARE_SIZE;
int BOARD_SIZE;

void resize_window(int width, int height) {
    WINDOW_WIDTH = width;
    WINDOW_HEIGHT = height;
    SQUARE_SIZE = WINDOW_HEIGHT / 8;
    BOARD_SIZE = SQUARE_SIZE * 8;
    
    free(gFrameBuffer);
    gFrameBuffer = (int*)malloc(WINDOW_WIDTH * WINDOW_HEIGHT * sizeof(int));
    SDL_DestroyTexture(gSDLTexture);
    gSDLTexture = SDL_CreateTexture(gSDLRenderer, SDL_PIXELFORMAT_ABGR8888, 
                                   SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);
}

void draw_board(void) {
    int board_start_x = (WINDOW_WIDTH - BOARD_SIZE) / 2;
    
    for (int y = 0; y < WINDOW_HEIGHT; y++) {
        for (int x = 0; x < WINDOW_WIDTH; x++) {
            int board_x = (x - board_start_x);
            int board_y = y;
            
            if (board_x >= 0 && board_x < BOARD_SIZE && board_y < BOARD_SIZE) {
                int square_x = board_x / SQUARE_SIZE;
                int square_y = board_y / SQUARE_SIZE;
                int is_white = (square_x + square_y) % 2;
                int color = is_white ? 0xFF374E6F : 0xFFB3DEF5;
                gFrameBuffer[y * WINDOW_WIDTH + x] = color;
            } else {
                gFrameBuffer[y * WINDOW_WIDTH + x] = 0xFF808080;
            }
        }
    }
}

void update_display(void) {
    char* pix;
    int pitch;
    SDL_LockTexture(gSDLTexture, NULL, (void**)&pix, &pitch);
    for (int i = 0, sp = 0, dp = 0; i < WINDOW_HEIGHT; i++, dp += WINDOW_WIDTH, sp += pitch) {
        memcpy(pix + sp, gFrameBuffer + dp, WINDOW_WIDTH * 4);
    }
    SDL_UnlockTexture(gSDLTexture);
    SDL_RenderTexture(gSDLRenderer, gSDLTexture, NULL, NULL);
    SDL_RenderPresent(gSDLRenderer);
}

int update(void) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_EVENT_QUIT || 
            (e.type == SDL_EVENT_KEY_UP && e.key.key == SDLK_ESCAPE)) {
            return 0;
        }
        if (e.type == SDL_EVENT_WINDOW_RESIZED) {
            resize_window(e.window.data1, e.window.data2);
            draw_board();
            update_display();
        }
    }
    return 1;
}

void loop(void) {
    if (!update()) {
        gDone = 1;
        #ifdef EMSCRIPTEN
        emscripten_cancel_main_loop();
        #endif
    }
}

int main(int argc, char** argv) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
        return -1;
    }

    SQUARE_SIZE = WINDOW_HEIGHT / 8;
    BOARD_SIZE = SQUARE_SIZE * 8;
    
    gFrameBuffer = (int*)malloc(WINDOW_WIDTH * WINDOW_HEIGHT * sizeof(int));
    gSDLWindow = SDL_CreateWindow("Chess Board", WINDOW_WIDTH, WINDOW_HEIGHT, 
                                SDL_WINDOW_RESIZABLE);
    gSDLRenderer = SDL_CreateRenderer(gSDLWindow, NULL);
    gSDLTexture = SDL_CreateTexture(gSDLRenderer, SDL_PIXELFORMAT_ABGR8888, 
                                   SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);

    if (!gFrameBuffer || !gSDLWindow || !gSDLRenderer || !gSDLTexture) {
        return -1;
    }

    draw_board();
    update_display();

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