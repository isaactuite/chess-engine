#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "SDL3/SDL.h"
#include "SDL3/SDL_main.h"
#include "include\SDL3_image\SDL_image.h"
#include "defs.h"
#include "game_logic.h"


// Array to hold piece textures
ChessPiece pieces[12];  // 6 piece types * 2 colors
void draw_pieces();
void draw_board();
void update_display();
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

int get_tile_x(float x){
    if (x>=board_start_x && x<=board_start_x+BOARD_SIZE){
        printf("board_start_x: %d\n",board_start_x);
        float x_relative = x-board_start_x;
        printf("relative_x", x_relative);
        return x_relative/SQUARE_SIZE;
        
    }
}
int get_tile_y(float y){
        return y/SQUARE_SIZE;
}

void select_square(float x, float y){
    if (x>=board_start_x && x<=board_start_x+BOARD_SIZE){
        selected_x=get_tile_x(x);
        selected_y=get_tile_y(y);
        printf("selected_x: %d", selected_x);
        printf("selected_y: %d", selected_y);

        draw_board();
        legal_moves(selected_x, selected_y);
        
        update_display();
    }

}
void handle_mouse_event(SDL_Event *e) {
    if (e->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        printf("\nMouse button %d released at (%.2f, %.2f)\n", 
               e->button.button, e->button.x, e->button.y);
        
        if (is_selected_piece == 1){
            for (int i=0; i<highlighted_squares_x[0]; i++){

                if ((highlighted_squares_x[i+1]==get_tile_x(e->button.x)) && (highlighted_squares_y[i+1]==get_tile_y(e->button.y))){
                    board[get_tile_x(e->button.x)][get_tile_y(e->button.y)] = board[selected_x][selected_y];
                    board[selected_x][selected_y] = 'o';
                    selected_x = -1;
                    selected_y = -1;
                    memset(highlighted_squares_x, -1, sizeof(highlighted_squares_x));
                    memset(highlighted_squares_y, -1, sizeof(highlighted_squares_y));
                    draw_board();
                    draw_pieces();
                    update_display();
                    is_selected_piece = 0;
                }

            }
            is_selected_piece=0;
        }
        if (is_selected_piece == 0){
        select_square(e->button.x, e->button.y);
        is_selected_piece = 1;
        }


    }
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
    board_start_x = (WINDOW_WIDTH - BOARD_SIZE) / 2;
    printf("\ndrawing board. selected tile: %d, %d\n", selected_x, selected_y);
    // Draw board squares
    for (int y = 0; y < WINDOW_HEIGHT; y++) {
        for (int x = 0; x < WINDOW_WIDTH; x++) {
            int board_x = (x - board_start_x);
            int board_y = y;
            if (((x >= (selected_x * SQUARE_SIZE)+board_start_x) && (x < (selected_x + 1) * SQUARE_SIZE+board_start_x))
                && ((y >= selected_y * SQUARE_SIZE) && (y < (selected_y + 1) * SQUARE_SIZE))) {
                gFrameBuffer[y * WINDOW_WIDTH + x] = 0x806666FF;  // Highlight color
            }else if (board_x >= 0 && board_x < BOARD_SIZE && board_y < BOARD_SIZE) {
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

void draw_piece(int piece_char, int square_x, int square_y) {

    int piece_index;
    switch (piece_char){
        case 'p':
            piece_index=6;
            break;
        case 'r':
            piece_index=7;
            break;
        case 'n':
            piece_index=8;
            break;
        case 'b':
            piece_index=9;
            break;
        case 'q':
            piece_index=10;
            break;
        case 'k':
            piece_index=11;
            break;
        case 'P':
            piece_index=0;
            break;
        case 'R':
            piece_index=1;
            break;
        case 'N':
            piece_index=2;
            break;
        case 'B':
            piece_index=3;
            break;
        case 'Q':
            piece_index=4;
            break;
        case 'K':
            piece_index=5;
            break;
        default:
            return;
    }
    SDL_FRect dest = {
        board_start_x + square_x * SQUARE_SIZE,
        square_y * SQUARE_SIZE,
        SQUARE_SIZE,
        SQUARE_SIZE
    };
    SDL_RenderTexture(gSDLRenderer, pieces[piece_index].texture, NULL, &dest);
}

void draw_pieces() {
    // Draw board position

    for (int i=0;i<8; i++){
        for (int j=0;j<8;j++){
            draw_piece(board[i][j], i, j);
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
        if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN || e.type == SDL_EVENT_MOUSE_BUTTON_UP) {
            handle_mouse_event(&e);
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