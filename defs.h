#ifndef DEFS_H
#define DEFS_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_image.h>

typedef struct {
    int board_start_x;
} X_Start_Coords;

typedef struct {
    SDL_Texture* texture;
    int width;
    int height;
} ChessPiece;
typedef struct {
    int from_x, from_y;   // Current position
    int to_x, to_y;       // Target position
    int count;
} Move;

extern Move move_list[256];
extern int* gFrameBuffer;
extern SDL_Window* gSDLWindow;
extern SDL_Renderer* gSDLRenderer;
extern SDL_Texture* gSDLTexture;
extern int gDone;

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

extern int SQUARE_SIZE;
extern int BOARD_SIZE;
extern float board_start_x;

extern int selected_x;
extern int selected_y;

extern int castling_rights_QS;
extern int castling_rights_KS;
extern int castling_rights_qs;
extern int castling_rights_ks;
extern int turn_color; //1=white, 0=black
extern int turn_count; //pair values

extern int en_passant_x;
extern int en_passant_y;

extern int flipped;
extern int is_selected_piece;
extern int bking_x;
extern int bking_y;

extern int wking_x;
extern int wking_y;

extern int piece_logic_for_moving;
extern int sim_counter;

extern int highlighted_squares_x[32];
extern int highlighted_squares_y[32];
extern int squares_under_attack_x[400];
extern int squares_under_attack_y[400];

extern char board[8][8];
extern char fake_board[8][8];
extern char dynamic_board[8][8];
extern char bot_board[8][8];
extern int pawn_pts[8][8];
extern int knight_pts[8][8];
extern int bishop_pts[8][8];
extern int rook_pts[8][8];
extern int queen_pts[8][8];
extern int king_pts_mid[8][8];
extern int king_pts_late[8][8];
#endif // DEFS_H