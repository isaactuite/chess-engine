#include "SDL3/SDL.h"
#include "SDL3/SDL_image.h"
#include "defs.h"

int* gFrameBuffer;
SDL_Window* gSDLWindow;
SDL_Renderer* gSDLRenderer;
SDL_Texture* gSDLTexture;
int gDone;

int WINDOW_WIDTH = 1920 / 2;
int WINDOW_HEIGHT = 1080 / 2;

int SQUARE_SIZE;
int BOARD_SIZE;

float board_start_x;

int selected_x = 100;
int selected_y = 100;

int castling_rights_QS = 1;
int castling_rights_KS = 1;
int castling_rights_qs = 1;
int castling_rights_ks = 1;

int turn_color =0; //0=white, 1=black
int turn_count = 0; //pair values

int en_passant_x = 0;
int en_passant_y = 0;

int is_selected_piece = 0;

int highlighted_squares_x[32] = {-1};
int highlighted_squares_y[32] = {-1};

int squares_under_attack_x[200] = {-1};
int squares_under_attack_y[200] = {-1};

int bking_x = 4;
int bking_y = 0;

int wking_x = 4;
int wking_y = 7;

int piece_logic_for_moving = 1;

char board[8][8] = {
    {'r', 'p', 'o', 'o', 'o', 'o', 'P', 'R'},  // File A
    {'n', 'p', 'o', 'o', 'o', 'o', 'P', 'N'},  // File B
    {'b', 'p', 'o', 'o', 'o', 'o', 'P', 'B'},  // File C
    {'q', 'p', 'o', 'o', 'o', 'o', 'P', 'Q'},  // File D
    {'k', 'p', 'o', 'o', 'o', 'o', 'P', 'K'},  // File E
    {'b', 'p', 'o', 'o', 'o', 'o', 'P', 'B'},  // File F
    {'n', 'p', 'o', 'o', 'o', 'o', 'P', 'N'},  // File G
    {'r', 'p', 'o', 'o', 'o', 'o', 'P', 'R'}   // File H
};

char fake_board[8][8] = {
    {'r', 'p', 'o', 'o', 'o', 'o', 'P', 'R'},  // File A
    {'n', 'p', 'o', 'o', 'o', 'o', 'P', 'N'},  // File B
    {'b', 'p', 'o', 'o', 'o', 'o', 'P', 'B'},  // File C
    {'q', 'p', 'o', 'o', 'o', 'o', 'P', 'Q'},  // File D
    {'k', 'p', 'o', 'o', 'o', 'o', 'P', 'K'},  // File E
    {'b', 'p', 'o', 'o', 'o', 'o', 'P', 'B'},  // File F
    {'n', 'p', 'o', 'o', 'o', 'o', 'P', 'N'},  // File G
    {'r', 'p', 'o', 'o', 'o', 'o', 'P', 'R'}   // File H
};