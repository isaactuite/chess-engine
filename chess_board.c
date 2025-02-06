#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "SDL3/SDL.h"
#include "SDL3/SDL_main.h"
#include "include\SDL3_image\SDL_image.h"
#include "defs.h"

typedef struct {
    SDL_Texture* texture;
    int width;
    int height;
} ChessPiece;

int* gFrameBuffer;
SDL_Window* gSDLWindow;
SDL_Renderer* gSDLRenderer;
SDL_Texture* gSDLTexture;
static int gDone;
int WINDOW_WIDTH = 1920 / 2;
int WINDOW_HEIGHT = 1080 / 2;
int SQUARE_SIZE;
int BOARD_SIZE;

// Array to hold piece textures
ChessPiece pieces[12];  // 6 piece types * 2 colors

// Load a piece texture and store its dimensions
ChessPiece load_piece(const char* path) {
    ChessPiece piece = {0};
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) {
        SDL_Log("Failed to load image %s: %s", path, SDL_GetError());
        return piece;
    }
    piece.texture = SDL_CreateTextureFromSurface(gSDLRenderer, surface);
    if (!piece.texture) {
        SDL_Log("Failed to create texture from surface: %s", SDL_GetError());
    }
    piece.width = surface->w;
    piece.height = surface->h;
    SDL_DestroySurface(surface);
    return piece;
}

void load_pieces() {
    const char* piece_files[] = {
        "assets/images/wp.png", "assets/images/wr.png", "assets/images/wn.png",
        "assets/images/wb.png", "assets/images/wq.png", "assets/images/wk.png",
        "assets/images/bp.png", "assets/images/br.png", "assets/images/bn.png",
        "assets/images/bb.png", "assets/images/bq.png", "assets/images/bk.png"
    };
    
    for (int i = 0; i < 12; i++) {
        pieces[i] = load_piece(piece_files[i]);
        if (!pieces[i].texture) {
            SDL_Log("Failed to load piece: %s", piece_files[i]);
        }
    }
}

void cleanup_pieces() {
    for (int i = 0; i < 12; i++) {
        if (pieces[i].texture) {
            SDL_DestroyTexture(pieces[i].texture);
        }
    }
}

void resize_window(int width, int height) {
    WINDOW_WIDTH = width;
    WINDOW_HEIGHT = height;
    SQUARE_SIZE = WINDOW_HEIGHT / 8;
    BOARD_SIZE = SQUARE_SIZE * 8;
    
    free(gFrameBuffer);
    gFrameBuffer = (int*)malloc(WINDOW_WIDTH * WINDOW_HEIGHT * sizeof(int));
    if (!gFrameBuffer) {
        SDL_Log("Failed to allocate framebuffer");
        return;
    }
    SDL_DestroyTexture(gSDLTexture);
    gSDLTexture = SDL_CreateTexture(gSDLRenderer, SDL_PIXELFORMAT_ABGR8888, 
                                   SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!gSDLTexture) {
        SDL_Log("Failed to create texture: %s", SDL_GetError());
    }
}

void draw_board(void) {
    int board_start_x = (WINDOW_WIDTH - BOARD_SIZE) / 2;
    
    // Draw board squares
    for (int y = 0; y < WINDOW_HEIGHT; y++) {
        for (int x = 0; x < WINDOW_WIDTH; x++) {
            int board_x = (x - board_start_x);
            int board_y = y;
            
            if (board_x >= 0 && board_x < BOARD_SIZE && board_y < BOARD_SIZE) {
                int square_x = board_x / SQUARE_SIZE;
                int square_y = board_y / SQUARE_SIZE;
                int is_black = (square_x + square_y) % 2;
                int color = is_black ? 0xFF374E6F : 0xFFB3DEF5;
                gFrameBuffer[y * WINDOW_WIDTH + x] = color;
            } else {
                gFrameBuffer[y * WINDOW_WIDTH + x] = 0xFF808080;
            }
        }
    }
}

void draw_piece(int piece_index, int square_x, int square_y) {
    if (!pieces[piece_index].texture) return;
    
    int board_start_x = (WINDOW_WIDTH - BOARD_SIZE) / 2;
    SDL_FRect dest = {
        board_start_x + square_x * SQUARE_SIZE,
        square_y * SQUARE_SIZE,
        SQUARE_SIZE,
        SQUARE_SIZE
    };
    SDL_RenderTexture(gSDLRenderer, pieces[piece_index].texture, NULL, &dest);
}

void draw_pieces() {
    // Draw initial board position
    // Pawns
    for (int i = 0; i < 8; i++) {
        draw_piece(0, i, 6);  // White pawns
        draw_piece(6, i, 1);  // Black pawns
    }
    
    // Other pieces
    int piece_order[] = {1, 2, 3, 4, 5, 3, 2, 1};  // rook, knight, bishop, queen, king, bishop, knight, rook
    for (int i = 0; i < 8; i++) {
        draw_piece(piece_order[i], i, 7);        // White pieces
        draw_piece(piece_order[i] + 6, i, 0);    // Black pieces
    }
}

int highlight_squares(Piece selected_piece, int square_x, int square_y, int legal_moves){
    int squares_to_highlight = legal_moves(selected_piece);

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
    
    // Draw pieces on top
    draw_pieces();
    
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
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return -1;
    }
    
    SQUARE_SIZE = WINDOW_HEIGHT / 8;
    BOARD_SIZE = SQUARE_SIZE * 8;
    
    gFrameBuffer = (int*)malloc(WINDOW_WIDTH * WINDOW_HEIGHT * sizeof(int));
    if (!gFrameBuffer) {
        SDL_Log("Failed to allocate framebuffer");
        SDL_Quit();
        return -1;
    }

    gSDLWindow = SDL_CreateWindow("Chess Board", WINDOW_WIDTH, WINDOW_HEIGHT, 
                                SDL_WINDOW_RESIZABLE);
    if (!gSDLWindow) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        free(gFrameBuffer);
        SDL_Quit();
        return -1;
    }

    gSDLRenderer = SDL_CreateRenderer(gSDLWindow, NULL);
    if (!gSDLRenderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(gSDLWindow);
        free(gFrameBuffer);
        SDL_Quit();
        return -1;
    }

    gSDLTexture = SDL_CreateTexture(gSDLRenderer, SDL_PIXELFORMAT_ABGR8888, 
                                   SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!gSDLTexture) {
        SDL_Log("Failed to create texture: %s", SDL_GetError());
        SDL_DestroyRenderer(gSDLRenderer);
        SDL_DestroyWindow(gSDLWindow);
        free(gFrameBuffer);
        SDL_Quit();
        return -1;
    }

    load_pieces();
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

    cleanup_pieces();
    free(gFrameBuffer);
    SDL_DestroyTexture(gSDLTexture);
    SDL_DestroyRenderer(gSDLRenderer);
    SDL_DestroyWindow(gSDLWindow);
    SDL_Quit();
    return 0;
}