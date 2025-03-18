#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

// Include necessary headers
#include <stdio.h>

// Function prototypes
int real(int coords);
void highlight_legal_moves(int x, int y);
int check_piece_color(int x, int y, int mode);
int is_valid_attack(int x, int y, int color);
void update_fake_board();

int pawn_logic(int x, int y, int color, int checkmate);
int rook_logic(int x, int y, int color, int checkmate);
int bishop_logic(int x, int y, int color, int checkmate);
int knight_logic(int x, int y, int color, int checkmate);
int queen_logic(int x, int y, int color, int checkmate);
int king_logic(int x, int y, int color, int checkmate);
int legal_moves(int x, int y);

int is_checkmate();
#endif // GAME_LOGIC_H