#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "SDL3/SDL.h"
#include "SDL3/SDL_main.h"
#include <SDL/SDL_image.h> // Include SDL_image header

int* gFrameBuffer;
SDL_Window* gSDLWindow;
SDL_Renderer* gSDLRenderer;
SDL_Texture* gSDLTexture;
static int gDone;
int WINDOW_WIDTH = 1920 / 2;
int WINDOW_HEIGHT = 1080 / 2;
int SQUARE_SIZE;
int BOARD_SIZE;

// Chess piece textures (12 pieces, 6 white, 6 black)
SDL_Texture* pieceTextures[12];

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

// Function to load a texture
SDL_Texture* loadTexture(const char* path) {
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) {
        SDL_Log("Unable to load image %s: %s", path, IMG_GetError());
        return NULL;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(gSDLRenderer, surface);
    SDL_FreeSurface(surface);  // Free the surface after converting to texture
    return texture;
}

// Function to draw chessboard and pieces
void draw_board(void) {
    int board_start_x = (WINDOW_WIDTH - BOARD_SIZE) / 2;
    
    // Draw the chessboard
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

    // Draw the chess pieces
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            // Piece type (0-5 are white pieces, 6-11 are black pieces)
            int pieceIndex = -1;
            if (row == 0) {
                if (col == 0 || col == 7) pieceIndex = 0;  // White Rook
                if (col == 1 || col == 6) pieceIndex = 1;  // White Knight
                if (col == 2 || col == 5) pieceIndex = 2;  // White Bishop
                if (col == 3) pieceIndex = 3;               // White Queen
                if (col == 4) pieceIndex = 4;               // White King
            } else if (row == 1) {
                pieceIndex = 5;  // White Pawn
            } else if (row == 6) {
                pieceIndex = 7;  // Black Pawn
            } else if (row == 7) {
                if (col == 0 || col == 7) pieceIndex = 8;  // Black Rook
                if (col == 1 || col == 6) pieceIndex = 9;  // Black Knight
                if (col == 2 || col == 5) pieceIndex = 10; // Black Bishop
                if (col == 3) pieceIndex = 11;             // Black Queen
            }
            if (pieceIndex >= 0) {
                // Draw the piece texture
                SDL_Rect destRect;
                destRect.w = SQUARE_SIZE;
                destRect.h = SQUARE_SIZE;
                destRect.x = (WINDOW_WIDTH - BOARD_SIZE) / 2 + col * SQUARE_SIZE;
                destRect.y = (WINDOW_HEIGHT - BOARD_SIZE) / 2 + row * SQUARE_SIZE;
                SDL_RenderCopy(gSDLRenderer, pieceTextures[pieceIndex], NULL, &destRect);
            }
        }
    }
}

// Update display
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

// Event loop
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
    }
}

int main(int argc, char** argv) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
        return -1;
    }

	if (IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG == 0) {
		SDL_Log("IMG_Init: %s", IMG_GetError());
		return -1;
	}

    SQUARE_SIZE = WINDOW_HEIGHT / 8;
    BOARD_SIZE = SQUARE_SIZE * 8;

    gFrameBuffer = (int*)malloc(WINDOW_WIDTH * WINDOW_HEIGHT * sizeof(int));
    gSDLWindow = SDL_CreateWindow("Chess Board", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
    gSDLRenderer = SDL_CreateRenderer(gSDLWindow, NULL);
    gSDLTexture = SDL_CreateTexture(gSDLRenderer, SDL_PIXELFORMAT_ABGR8888, 
                                    SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);

    if (!gFrameBuffer || !gSDLWindow || !gSDLRenderer || !gSDLTexture) {
        return -1;
    }

    // Load chess piece textures
    pieceTextures[0] = loadTexture("assets/images/wr.png");
    pieceTextures[1] = loadTexture("assets/images/wn.png");
    pieceTextures[2] = loadTexture("assets/images/wb.png");
    pieceTextures[3] = loadTexture("assets/images/wq.png");
    pieceTextures[4] = loadTexture("assets/images/wk.png");
    pieceTextures[5] = loadTexture("assets/images/wp.png");
    pieceTextures[6] = loadTexture("assets/images/br.png");
    pieceTextures[7] = loadTexture("assets/images/bn.png");
    pieceTextures[8] = loadTexture("assets/images/bb.png");
    pieceTextures[9] = loadTexture("assets/images/bq.png");
    pieceTextures[10] = loadTexture("assets/images/bk.png");
    pieceTextures[11] = loadTexture("assets/images/bp.png");

    // Check if any piece failed to load
    for (int i = 0; i < 12; i++) {
        if (!pieceTextures[i]) {
            SDL_Log("Error loading piece texture %d", i);
            return -1;
        }
    }

    draw_board();
    update_display();

    gDone = 0;

    while (!gDone) {
        loop();
    }

    // Cleanup
    for (int i = 0; i < 12; i++) {
        SDL_DestroyTexture(pieceTextures[i]);
    }
    free(gFrameBuffer);
    SDL_DestroyTexture(gSDLTexture);
    SDL_DestroyRenderer(gSDLRenderer);
    SDL_DestroyWindow(gSDLWindow);
    SDL_Quit();
    IMG_Quit();

    return 0;
}
