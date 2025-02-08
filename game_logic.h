#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

// Include necessary headers
#include <stdio.h>

// Function prototypes
void highlight_legal_moves(int x, int y);
int check_piece_color(int x, int y);
int is_valid_attack(int x, int y, int color);

int pawn_logic(int x, int y, int color);
int rook_logic(int x, int y, int color);
int bishop_logic(int x, int y, int color);
int knight_logic(int x, int y, int color);
int queen_logic(int x, int y, int color);
int king_logic(int x, int y, int color);
int legal_moves(int x, int y);


#endif // GAME_LOGIC_H