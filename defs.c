#include "SDL3/SDL.h"
#include "SDL3/SDL_image.h"
#include "defs.h"
#include <stdio.h>
int* gFrameBuffer;
SDL_Window* gSDLWindow;
SDL_Renderer* gSDLRenderer;
SDL_Texture* gSDLTexture;
SDL_Texture* checkmateTexture = NULL;

int gDone;

int WINDOW_WIDTH = 2560/2;
int WINDOW_HEIGHT = 1440/2;

int SQUARE_SIZE;
int BOARD_SIZE;

float board_start_x;

int selected_x = 100;
int selected_y = 100;

int castling_rights_QS = 1;
int castling_rights_KS = 1;
int castling_rights_qs = 1;
int castling_rights_ks = 1;
int sim_castling_rights_QS = 1;
int sim_castling_rights_KS = 1;
int sim_castling_rights_qs = 1;
int sim_castling_rights_ks = 1;

int turn_color =0; //0=white, 1=black
int bot_color;
int player_color = -1;



int turn_count = 0;

int en_passant_x = -1;
int en_passant_y = -1;
int bot_en_passant_x = -1;
int bot_en_passant_y = -1;

int is_selected_piece = 0;

int highlighted_squares_x[32] = {-1};
int highlighted_squares_y[32] = {-1};

int squares_under_attack_x[400] = {-1};
int squares_under_attack_y[400] = {-1};

int bking_x = 4;
int bking_y = 0;

int wking_x = 4;
int wking_y = 7;

int bot_bking_x = 4;
int bot_bking_y = 0;

int bot_wking_x = 4;
int bot_wking_y = 7;

int flipped = 0;
int piece_logic_for_moving = 1;
int sim_counter = 1;

#define MAX_MOVES 256

Move killer_moves[MAX_DEPTH][2];
int history_heuristic[8][8][8][8] = {0};

MoveStack history;

char board[8][8] = {
    {'r', 'p', '.', '.', '.', '.', 'P', 'R'},  // File A
    {'n', 'p', '.', '.', '.', '.', 'P', 'N'},  // File B
    {'b', 'p', '.', '.', '.', '.', 'P', 'B'},  // File C
    {'q', 'p', '.', '.', '.', '.', 'P', 'Q'},  // File D
    {'k', 'p', '.', '.', '.', '.', 'P', 'K'},  // File E
    {'b', 'p', '.', '.', '.', '.', 'P', 'B'},  // File F
    {'n', 'p', '.', '.', '.', '.', 'P', 'N'},  // File G
    {'r', 'p', '.', '.', '.', '.', 'P', 'R'}   // File H
};

char fake_board[8][8] = {
    {'r', 'p', '.', '.', '.', '.', 'P', 'R'},  // File A
    {'n', 'p', '.', '.', '.', '.', 'P', 'N'},  // File B
    {'b', 'p', '.', '.', '.', '.', 'P', 'B'},  // File C
    {'q', 'p', '.', '.', '.', '.', 'P', 'Q'},  // File D
    {'k', 'p', '.', '.', '.', '.', 'P', 'K'},  // File E
    {'b', 'p', '.', '.', '.', '.', 'P', 'B'},  // File F
    {'n', 'p', '.', '.', '.', '.', 'P', 'N'},  // File G
    {'r', 'p', '.', '.', '.', '.', 'P', 'R'}   // File H
};
char dynamic_board[8][8] = {
    {'r', 'p', '.', '.', '.', '.', 'P', 'R'},  // File A
    {'n', 'p', '.', '.', '.', '.', 'P', 'N'},  // File B
    {'b', 'p', '.', '.', '.', '.', 'P', 'B'},  // File C
    {'q', 'p', '.', '.', '.', '.', 'P', 'Q'},  // File D
    {'k', 'p', '.', '.', '.', '.', 'P', 'K'},  // File E
    {'b', 'p', '.', '.', '.', '.', 'P', 'B'},  // File F
    {'n', 'p', '.', '.', '.', '.', 'P', 'N'},  // File G
    {'r', 'p', '.', '.', '.', '.', 'P', 'R'}   // File H
};
char bot_board[8][8] = {
    {'r', 'p', '.', '.', '.', '.', 'P', 'R'},  // File A
    {'n', 'p', '.', '.', '.', '.', 'P', 'N'},  // File B
    {'b', 'p', '.', '.', '.', '.', 'P', 'B'},  // File C
    {'q', 'p', '.', '.', '.', '.', 'P', 'Q'},  // File D
    {'k', 'p', '.', '.', '.', '.', 'P', 'K'},  // File E
    {'b', 'p', '.', '.', '.', '.', 'P', 'B'},  // File F
    {'n', 'p', '.', '.', '.', '.', 'P', 'N'},  // File G
    {'r', 'p', '.', '.', '.', '.', 'P', 'R'}   // File H
};

int pawn_pts[8][8]= {
    0, 50, 10, 5, 0, 5, 5, 0, 
    0, 50, 10, 5, 0, -5, 10, 0,
    0, 50, 20, 10, 0, -10, 10, 0,
    0, 50, 30, 25, 25, 0, -20, 0,
    0, 50, 30, 25, 25, 0, -20, 0,
    0, 50, 20, 10, 0, -10, 10, 0,
    0, 50, 10, 5, 0, -5, 10, 0,
    0, 50, 10, 5, 0, 5, 5, 0,
};
int knight_pts[8][8]= {
    -50, -40, -30, -30, -30, -30, -40, -50, 
    -40, -20, 0, 5, 0, 5, -20, -40,
    -30, 0, 10, 15, 15, 10, 0, -30,
    -30, 0, 15, 20, 20, 15, 5, -30,
    -30, 0, 15, 20, 20, 15, 5, -30,
    -30, 0, 10, 15, 15, 10, 0, -30,
    -40, -20, 0, 5, 0, 5, -20, -40,
    -50, -40, -30, -30, -30, -30, -40, -50,
};
int bishop_pts[8][8]= {
    -20, -10, -10, -10, -10, -10, -10, -20, 
    -10, 0, 0, 5, 0, 10, 5, -10,
    -10, 0, 5, 5, 10, 10, 0, -10,
    -10, 0, 10, 10, 10, 10, 0, -10,
    -10, 0, 10, 10, 10, 10, 0, -10,
    -10, 0, 5, 5, 10, 10, 0, -10,
    -10, 0, 0, 5, 0, 10, 5, -10,
    -20, -10, -10, -10, -10, -10, -10, -20,
};
int rook_pts[8][8]= {
    0, 5, -5, -5, -5, -5, -5, 0, 
    0, 10, 0, 0, 0, 0, 0, 0,
    0, 10, 0, 0, 0, 0, 0, 0,
    0, 10, 0, 0, 0, 0, 0, 5,
    0, 10, 0, 0, 0, 0, 0, 5,
    0, 10, 0, 0, 0, 0, 0, 0,
    0, 10, 0, 0, 0, 0, 0, 0,
    0, 5, -5, -5, -5, -5, -5, 0,
};
int queen_pts[8][8]= {
    -20, -10, -10, -5, 0, -10, -10, -20, 
    -10, 0, 0, 0, 0, 5, 0, -10,
    -10, 0, 5, 5, 5, 5, 5, -10,
    -5, 0, 5, 5, 5, 5, 0, -5,
    -5, 0, 5, 5, 5, 5, 0, -5,
    -10, 0, 5, 5, 5, 5, 0, -10,
    -10, 0, 0, 0, 0, 0, 0, -10,
    -20, -10, -10, -5, -5, -10, -10, -20,
};
int king_pts_mid[8][8]= {
    -30, -30, -30, -30, -20, -10, 20, 20, 
    -40, -40, -40, -40, -30, -20, 20, 30,
    -40, -40, -40, -40, -30, -20, 0, 10,
    -50, -50, -50, -50, -40, -20, 0, 0,
    -50, -50, -50, -50, -40, -20, 0, 0,
    -40, -40, -40, -40, -30, -20, 0, 10,
    -40, -40, -40, -40, -30, -20, 20, 30,
    -30, -30, -30, -30, -20, -10, 20, 20,
};
int king_pts_late[8][8]= {
    -50, -30, -30, -30, -30, -30, -30, -50, 
    -40, -20, -10, -10, -10, -10, -30, -30,
    -30, -10, 20, 30, 30, 20, 0, -30,
    -20, 0, 30, 40, 40, 30, 0, -30,
    -20, 0, 30, 40, 40, 30, 0, -30,
    -30, -10, 20, 30, 30, 20, 0, -30,
    -40, -20, -10, -10, -10, -10, -30, -30,
    -50, -30, -30, -30, -30, -30, -30, -50,
};
